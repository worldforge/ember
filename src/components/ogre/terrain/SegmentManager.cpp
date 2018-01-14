/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "SegmentManager.h"
#include "Segment.h"
#include "SegmentHolder.h"

#include "framework/LoggingInstance.h"

#include <Mercator/Shader.h>
#include <Mercator/Terrain.h>

#include <wfmath/MersenneTwister.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

SegmentManager::SegmentManager(Mercator::Terrain& terrain, unsigned int desiredSegmentBuffer) :
		mTerrain(terrain),
		mDesiredSegmentBuffer(desiredSegmentBuffer),
		mFakeSegmentHeight(-12.0f),
		mFakeSegmentHeightVariation(10.0f),
		mEndlessWorldEnabled(false)
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

	for (const auto& index : indices) {
		for (auto entry : index.second) {

			const std::pair<int, int>& worldIndex = entry.second;
			std::stringstream ss;
			ss << worldIndex.first << "_" << worldIndex.second;
			std::string key = ss.str();
			SegmentStore::const_iterator segI = mSegments.find(key);
			if (segI != mSegments.end()) {
				segments[index.first][entry.first] = segI->second->getReference();
				count++;
			} else if (mEndlessWorldEnabled) {
				segments[index.first][entry.first] = createFakeSegment(key, worldIndex.first, worldIndex.second);
				count++;
			}
		}
	}
	return count;
}

std::shared_ptr<Segment> SegmentManager::createFakeSegment(const std::string& key, int xIndex, int zIndex)
{

	std::function<void(Mercator::Segment*)> invalidate = [](Mercator::Segment* s)
	{
		if (s) {
			s->invalidate();
		}
	};
	std::function<Mercator::Segment*()> segmentProvider = [=]()
	{
		Mercator::Segment* segment = new Mercator::Segment(xIndex * mTerrain.getResolution(), zIndex * mTerrain.getResolution(), mTerrain.getResolution());

		WFMath::MTRand rand;
		auto setPoint = [&](unsigned int x, unsigned int z) {
				Mercator::BasePoint bp;
				if (mTerrain.getBasePoint(xIndex + x, zIndex + z, bp)) {
					segment->setCornerPoint(x,z, bp);
				} else {
					//Use a predictive way of generating a random height.
				rand.seed(xIndex + x + ((zIndex + z) * 10000.0f));
				segment->setCornerPoint(x,z, Mercator::BasePoint(mFakeSegmentHeight - (rand.rand<float>() * mFakeSegmentHeightVariation)));
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

	Segment* fakeSegment = new Segment(xIndex, zIndex, segmentProvider, invalidate);

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
	ss << (segment.getXRef() / segment.getResolution()) << "_" << (segment.getZRef() / segment.getResolution());
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
		mSegments.insert(SegmentStore::value_type(ss.str(), new SegmentHolder(new Segment(segment.getXRef() / segment.getResolution(), segment.getZRef() / segment.getResolution(), segmentProvider, invalidate), *this)));
	}
}

void SegmentManager::syncWithTerrain()
{
//There's currently no way to remove segments from the terrain, so we don't have to worry about that for now.

	Mercator::Terrain::Segmentstore segmentStore = mTerrain.getTerrain();
	for (auto& I : segmentStore) {
		const Mercator::Terrain::Segmentcolumn& segmentColumn = I.second;
		for (auto entry : segmentColumn) {
			Mercator::Segment* segment = entry.second;
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
	auto I = std::find(mUnusedAndDirtySegments.begin(), mUnusedAndDirtySegments.end(), holder);
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
