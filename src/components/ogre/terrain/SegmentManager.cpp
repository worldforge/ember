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
#include "SegmentReference.h"

#include "framework/LoggingInstance.h"

#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#include <boost/thread/locks.hpp>

namespace EmberOgre
{

namespace Terrain
{

SegmentManager::SegmentManager(Mercator::Terrain& terrain, unsigned int desiredSegmentBuffer) :
	mTerrain(terrain), mDesiredSegmentBuffer(desiredSegmentBuffer)
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
	boost::shared_lock<boost::shared_mutex> l(mSegmentsMutex);
	SegmentStore::const_iterator I = mSegments.find(ss.str());
	if (I != mSegments.end()) {
		return I->second->getReference();
	}
	return SegmentRefPtr();
}

size_t SegmentManager::getSegmentReferences(const SegmentManager::IndexMap& indices, SegmentRefStore& segments)
{
	size_t count = 0;
	boost::shared_lock<boost::shared_mutex> l(mSegmentsMutex);

	for (IndexMap::const_iterator I = indices.begin(); I != indices.end(); ++I) {
		for (IndexColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {

			const std::pair<int, int>& worldIndex(J->second);
			std::stringstream ss;
			ss << worldIndex.first << "_" << worldIndex.second;
			SegmentStore::const_iterator segI = mSegments.find(ss.str());
			if (segI != mSegments.end()) {
				segments[I->first][J->first] = segI->second->getReference();
				count++;
			}
		}
	}
	return count;
}

void SegmentManager::addSegment(Mercator::Segment& segment)
{
	std::stringstream ss;
	ss << (segment.getXRef() / segment.getResolution()) << "_" << (segment.getYRef() / segment.getResolution());
	boost::unique_lock<boost::shared_mutex> l(mSegmentsMutex);
	SegmentStore::const_iterator I = mSegments.find(ss.str());
	if (I == mSegments.end()) {
		mSegments.insert(SegmentStore::value_type(ss.str(), new SegmentHolder(new Segment(segment), *this)));
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
	boost::shared_lock<boost::shared_mutex> l(mSegmentsMutex);
	boost::unique_lock<boost::shared_mutex> l1(mUnusedAndDirtySegmentsMutex);
	while (mUnusedAndDirtySegments.size() > mDesiredSegmentBuffer) {
		SegmentHolder* holder = mUnusedAndDirtySegments.front();
		mUnusedAndDirtySegments.pop_front();
		holder->getSegment().getMercatorSegment().invalidate(true);
	}
}

void SegmentManager::markHolderAsDirtyAndUnused(SegmentHolder* holder)
{
	boost::unique_lock<boost::shared_mutex> l(mUnusedAndDirtySegmentsMutex);

	mUnusedAndDirtySegments.push_back(holder);
}

void SegmentManager::unmarkHolder(SegmentHolder* holder)
{
	boost::unique_lock<boost::shared_mutex> l(mUnusedAndDirtySegmentsMutex);
	SegmentList::iterator I = std::find(mUnusedAndDirtySegments.begin(), mUnusedAndDirtySegments.end(), holder);
	if (I != mUnusedAndDirtySegments.end()) {
		mUnusedAndDirtySegments.erase(I);
	}
}

}

}
