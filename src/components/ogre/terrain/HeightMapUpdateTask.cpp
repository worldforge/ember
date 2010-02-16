/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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
#include "HeightMapUpdateTask.h"
#include "HeightMapBufferProvider.h"
#include "HeightMapBuffer.h"
#include "HeightMapSegment.h"
#include "HeightMap.h"
#include "HeightMapFlatSegment.h"
#include "Buffer.h"

#include "framework/tasks/TaskExecutionContext.h"

#include <wfmath/point.h>

#include <Mercator/Segment.h>

#include <string.h>

namespace EmberOgre
{

namespace Terrain
{

HeightMapUpdateTask::HeightMapUpdateTask(HeightMapBufferProvider& provider, HeightMap& heightMap, const SegmentStore& segments) :
	mProvider(provider), mHeightMap(heightMap), mSegments(segments)
{

}

HeightMapUpdateTask::~HeightMapUpdateTask()
{
}

void HeightMapUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	createHeightMapSegments();
}

void HeightMapUpdateTask::executeTaskInMainThread()
{
	injectHeightMapSegmentsIntoHeightMap();
}

void HeightMapUpdateTask::createHeightMapSegments()
{
	for (SegmentStore::const_iterator I = mSegments.begin(); I != mSegments.end(); ++I) {
		Mercator::Segment* segment = *I;
		if (segment) {
			IHeightMapSegment* heightMapSegment = 0;
			Mercator::Matrix<2, 2, Mercator::BasePoint>& basePoints(segment->getControlPoints());
			if ((basePoints[0].height() == basePoints[1].height()) && (basePoints[1].height() == basePoints[2].height()) && (basePoints[2].height() == basePoints[3].height())) {
				heightMapSegment = new HeightMapFlatSegment(basePoints[0].height());
			} else {
				HeightMapBuffer* buffer = mProvider.checkout();
				if (buffer) {
					memcpy(buffer->getBuffer()->getData(), segment->getPoints(), sizeof(float) * segment->getSize() * segment->getSize());
					heightMapSegment = new HeightMapSegment(buffer);
				}
			}
			if (heightMapSegment) {
				mHeightMapSegments.push_back(std::pair<WFMath::Point<2>, IHeightMapSegment*>(WFMath::Point<2>(segment->getXRef() / segment->getResolution(), segment->getYRef() / segment->getResolution()), heightMapSegment));
			}
		}
	}

}
void HeightMapUpdateTask::injectHeightMapSegmentsIntoHeightMap()
{
	for (HeightMapSegmentStore::const_iterator I = mHeightMapSegments.begin(); I != mHeightMapSegments.end(); ++I) {
		WFMath::Point<2> position = I->first;
		IHeightMapSegment* heightMapSegment = I->second;
		mHeightMap.insert(position.x(), position.y(), heightMapSegment);
	}

}

}

}
