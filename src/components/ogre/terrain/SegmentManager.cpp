/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "SegmentManager.h"
#include "Segment.h"
#include "SegmentHolder.h"

#include "framework/LoggingInstance.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>
#include <Mercator/Terrain.h>

#include <wfmath/MersenneTwister.h>

#include <sstream>
#include <algorithm>
#include <cassert>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

SegmentManager::SegmentManager(Mercator::Terrain& terrain, unsigned int desiredSegmentBuffer) :
		mTerrain(terrain), mDesiredSegmentBuffer(desiredSegmentBuffer), mFakeSegmentHeight(-12.0f), mFakeSegmentHeightVariation(10.0f), mEndlessWorldEnabled(false)
{

}

SegmentManager::~SegmentManager()
{
	for (SegmentStore::const_iterator I = mSegments.begin(); I != mSegments.end(); ++I) {
		delete I->second;
	}
}

SegmentRefPtr SegmentManager::getSegmentReference(int xIndex, int yIndex)
{
	std::stringstream ss;
	ss << xIndex << "_" << yIndex;
	std::string key = ss.str();
	std::unique_lock < std::mutex > l(mSegmentsMutex);
	SegmentStore::const_iterator I = mSegments.find(key);
	if (I != mSegments.end()) {
		return I->second->getReference();
	} else if (mEndlessWorldEnabled) {
		return createFakeSegment(key, xIndex, yIndex);
	} else {
		return SegmentRefPtr();
	}
}

size_t SegmentManager::getSegmentReferences(const SegmentManager::IndexMap& indices, SegmentRefStore& segments)
{
	size_t count = 0;
	std::unique_lock < std::mutex > l(mSegmentsMutex);

	for (IndexMap::const_iterator I = indices.begin(); I != indices.end(); ++I) {
		for (IndexColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {

			const std::pair<int, int>& worldIndex(J->second);
			std::stringstream ss;
			ss << worldIndex.first << "_" << worldIndex.second;
			std::string key = ss.str();
			SegmentStore::const_iterator segI = mSegments.find(key);
			if (segI != mSegments.end()) {
				segments[I->first][J->first] = segI->second->getReference();
				count++;
			} else if (mEndlessWorldEnabled) {
				segments[I->first][J->first] = createFakeSegment(key, worldIndex.first, worldIndex.second);
				count++;
			}
		}
	}
	return count;
}

std::shared_ptr<Segment> SegmentManager::createFakeSegment(const std::string& key, int xIndex, int yIndex)
{

	std::function<void(Mercator::Segment*)> invalidate = [](Mercator::Segment* s)
	{
		if (s) {
			s->invalidate();
		}
	};
	std::function<Mercator::Segment*()> segmentProvider = [=]()
	{
		Mercator::Segment* segment = new Mercator::Segment(xIndex * mTerrain.getResolution(), yIndex * mTerrain.getResolution(), mTerrain.getResolution());

		WFMath::MTRand rand;
		auto setPoint = [&](int x, int y) {
				Mercator::BasePoint bp;
				if (mTerrain.getBasePoint(xIndex + x, yIndex + y, bp)) {
					segment->setCornerPoint(x,y, bp);
				} else {
					//Use a predictive way of generating a random height.
				rand.seed(xIndex + x + ((yIndex + y) * 10000.0f));
				segment->setCornerPoint(x,y, Mercator::BasePoint(mFakeSegmentHeight - (rand.rand<float>() * mFakeSegmentHeightVariation)));
			}
		};

		setPoint(0, 0);
		setPoint(0, 1);
		setPoint(1, 0);
		setPoint(1, 1);

		auto& surfaces = segment->getSurfaces();
		for (auto shader : mTerrain.getShaders()) {
			auto surface = shader.second->newSurface(*segment);
			surfaces[shader.first] = surface;
		}
		return segment;
	};

	Segment* fakeSegment = new Segment(xIndex, yIndex, segmentProvider, invalidate);

	//When using fake segments we must delete the mercator segment when we delete the Segment instance.
	auto deleter = [](Segment* p) {
		if (p->hasSegment()) {
			delete &p->getMercatorSegment();
		}
		delete p;
	};

	//In contrast to regular Segments which refer to actual instances of Mercator::Segment we'll
	//just delete the fake segments once they are of no use no more.
	return std::shared_ptr<Segment>(fakeSegment, deleter);
}

void SegmentManager::addSegment(Mercator::Segment& segment)
{
	std::stringstream ss;
	ss << (segment.getXRef() / segment.getResolution()) << "_" << (segment.getYRef() / segment.getResolution());
	std::unique_lock < std::mutex > l(mSegmentsMutex);
	SegmentStore::const_iterator I = mSegments.find(ss.str());
	if (I == mSegments.end()) {
		std::function<void(Mercator::Segment*)> invalidate = [](Mercator::Segment* s)
		{
			if (s) {
				s->invalidate();
			}
		};
		std::function<Mercator::Segment*()> segmentProvider = [&]() {return &segment;};
		mSegments.insert(SegmentStore::value_type(ss.str(), new SegmentHolder(new Segment(segment.getXRef() / segment.getResolution(), segment.getYRef() / segment.getResolution(), segmentProvider, invalidate), *this)));
	}
}

void SegmentManager::syncWithTerrain()
{
//There's currently no way to remove segments from the terrain, so we don't have to worry about that for now.

	Mercator::Terrain::Segmentstore segmentStore = mTerrain.getTerrain();
	for (Mercator::Terrain::Segmentstore::const_iterator I = segmentStore.begin(); I != segmentStore.end(); ++I) {
		const Mercator::Terrain::Segmentcolumn& segmentColumn = I->second;
		for (Mercator::Terrain::Segmentcolumn::const_iterator J = segmentColumn.begin(); J != segmentColumn.end(); ++J) {
			Mercator::Segment* segment = J->second;
			addSegment(*segment);
		}
	}
}

void SegmentManager::pruneUnusedSegments()
{
	std::unique_lock < std::mutex > l(mSegmentsMutex);
	std::unique_lock < std::mutex > l1(mUnusedAndDirtySegmentsMutex);
	while (mUnusedAndDirtySegments.size() > mDesiredSegmentBuffer) {
		SegmentHolder* holder = mUnusedAndDirtySegments.front();
		mUnusedAndDirtySegments.pop_front();
		holder->getSegment().invalidate();
	}
}

void SegmentManager::markHolderAsDirtyAndUnused(SegmentHolder* holder)
{
	std::unique_lock < std::mutex > l(mUnusedAndDirtySegmentsMutex);

	mUnusedAndDirtySegments.push_back(holder);
}

void SegmentManager::unmarkHolder(SegmentHolder* holder)
{
	std::unique_lock < std::mutex > l(mUnusedAndDirtySegmentsMutex);
	SegmentList::iterator I = std::find(mUnusedAndDirtySegments.begin(), mUnusedAndDirtySegments.end(), holder);
	if (I != mUnusedAndDirtySegments.end()) {
		mUnusedAndDirtySegments.erase(I);
	}
}

void SegmentManager::setEndlessWorldEnabled(bool enabled)
{
	mEndlessWorldEnabled = enabled;
}

bool SegmentManager::getEndlessWorldEnabled() const
{
	return mEndlessWorldEnabled;
}

void SegmentManager::setDefaultHeight(float height)
{
	mFakeSegmentHeight = height;
}

float SegmentManager::getDefaultHeight() const
{
	return mFakeSegmentHeight;
}

void SegmentManager::setDefaultHeightVariation(float height)
{
	mFakeSegmentHeightVariation = height;
}

float SegmentManager::getDefaultHeightVariation() const
{
	return mFakeSegmentHeightVariation;
}

}
}
}
