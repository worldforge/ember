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

#include "SegmentHolder.h"
#include "SegmentReference.h"
#include "Segment.h"
#include "SegmentManager.h"

#include <cassert>

namespace EmberOgre
{

namespace Terrain
{

SegmentHolder::SegmentHolder(Segment* segment, SegmentManager& segmentManager) :
	mSegment(segment), mSegmentManager(segmentManager), mRefCount(0)
{

}

SegmentHolder::~SegmentHolder()
{
	delete mSegment;
}

boost::shared_ptr<SegmentReference> SegmentHolder::getReference()
{
	boost::mutex::scoped_lock l(mRefCountMutex);
	mRefCount++;
	//If mRefCount is 1 we're guaranteed to be the only one interacting with the segment, so it's thread safe to call Mercator::Segment::isValid
	if (mRefCount == 1 && mSegment->getMercatorSegment().isValid()) {
		mSegmentManager.unmarkHolder(this);
	}
	return boost::shared_ptr<SegmentReference>(new SegmentReference(*this));
}

void SegmentHolder::returnReference()
{
	boost::mutex::scoped_lock l(mRefCountMutex);
	assert(mRefCount > 0);
	mRefCount--;
	//If mRefCount is 0 we're guaranteed to be the only one interacting with the segment, so it's thread safe to call Mercator::Segment::isValid
	if (mRefCount == 0 && mSegment->getMercatorSegment().isValid()) {
		mSegmentManager.markHolderAsDirtyAndUnused(this);
		mSegmentManager.pruneUnusedSegments();
	}
}

bool SegmentHolder::isUnused()
{
	boost::mutex::scoped_lock l(mRefCountMutex);
	return mRefCount == 0;
}

Segment& SegmentHolder::getSegment()
{
	return *mSegment;
}

}

}
