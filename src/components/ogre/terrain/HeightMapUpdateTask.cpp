/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

#include <utility>
#include <cstring>

namespace Ember {
namespace OgreView {

namespace Terrain {

HeightMapUpdateTask::HeightMapUpdateTask(HeightMapBufferProvider& provider, HeightMap& heightMap, SegmentStore segments) :
		mProvider(provider),
		mHeightMap(heightMap),
		mSegments(std::move(segments)) {
}

HeightMapUpdateTask::~HeightMapUpdateTask() = default;

void HeightMapUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	createHeightMapSegments();
}

bool HeightMapUpdateTask::executeTaskInMainThread() {
	injectHeightMapSegmentsIntoHeightMap();
	return true;
}

void HeightMapUpdateTask::createHeightMapSegments() {
	for (auto segment: mSegments) {
		if (segment) {
			std::unique_ptr<IHeightMapSegment> heightMapSegment;
			Mercator::Matrix<2, 2, Mercator::BasePoint>& basePoints(segment->getControlPoints());
			//If all of the base points are on the same level, and there are no mods, we know that the segment is completely flat, and we can save some memory by using a HeightMapFlatSegment instance.
			if (WFMath::Equal(basePoints[0].height(), basePoints[1].height()) &&
				WFMath::Equal(basePoints[1].height(), basePoints[2].height()) &&
				WFMath::Equal(basePoints[2].height(), basePoints[3].height()) &&
				(segment->getMods().empty())) {
				heightMapSegment = std::make_unique<HeightMapFlatSegment>(basePoints[0].height());
			} else {
				auto buffer = mProvider.checkout();
				if (buffer) {
					std::memcpy(buffer->getBuffer().getData(), segment->getPoints(), sizeof(float) * segment->getSize() * segment->getSize());
					heightMapSegment = std::make_unique<HeightMapSegment>(std::move(buffer));
				}
			}
			if (heightMapSegment) {
				mHeightMapSegments.emplace_back(WFMath::Point<2>((WFMath::CoordType) segment->getXRef() / segment->getResolution(),
																 (WFMath::CoordType) segment->getZRef() / segment->getResolution()),
												std::move(heightMapSegment));
			}
		}
	}

}

void HeightMapUpdateTask::injectHeightMapSegmentsIntoHeightMap() {
	for (auto& entry: mHeightMapSegments) {
		const WFMath::Point<2>& position = entry.first;
		mHeightMap.insert((int) position.x(), (int) position.y(), std::move(entry.second));
	}

}

}

}
}
