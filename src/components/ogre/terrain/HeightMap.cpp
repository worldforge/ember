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

#include "HeightMap.h"
#include "IHeightMapSegment.h"
#include "framework/LoggingInstance.h"
#include <wfmath/vector.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

HeightMap::HeightMap(float defaultLevel, int segmentResolution) :
		mDefaultLevel(defaultLevel),
		mSegmentResolution(segmentResolution) {

}

HeightMap::~HeightMap() = default;

void HeightMap::insert(int xIndex, int yIndex, std::unique_ptr<IHeightMapSegment> segment) {
	mSegments[xIndex][yIndex] = std::shared_ptr<IHeightMapSegment>(std::move(segment));
}

bool HeightMap::remove(int xIndex, int yIndex) {
	auto column = mSegments.find(xIndex);
	if (column != mSegments.end()) {
		auto row = column->second.find(yIndex);
		if (row != column->second.end()) {
			column->second.erase(row);
			return true;
		}
	}
	return false;
}

void HeightMap::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const {

	int xSize = xMax - xMin;

	int segmentXMin = (int) std::lround(std::floor(xMin / (double) mSegmentResolution));
	int segmentXMax = (int) std::lround(std::floor(xMax / (double) mSegmentResolution));
	int segmentYMin = (int) std::lround(std::floor(yMin / (double) mSegmentResolution));
	int segmentYMax = (int) std::lround(std::floor(yMax / (double) mSegmentResolution));

	for (int segmentX = segmentXMin; segmentX <= segmentXMax; ++segmentX) {
		for (int segmentY = segmentYMin; segmentY <= segmentYMax; ++segmentY) {

			auto segmentPtr = getSegment(segmentX, segmentY);
			if (segmentPtr) {
				auto segment = segmentPtr.get();

				int segmentXStart = segmentX * mSegmentResolution;
				int segmentYStart = segmentY * mSegmentResolution;
				int dataXOffset = segmentXStart - xMin;
				int dataYOffset = segmentYStart - yMin;

				int xStart = std::max(xMin - segmentXStart, 0);
				int yStart = std::max(yMin - segmentYStart, 0);
				int xEnd = std::min<int>(xMax - segmentXStart, mSegmentResolution);
				int yEnd = std::min<int>(yMax - segmentYStart, mSegmentResolution);

				for (int x = xStart; x < xEnd; ++x) {
					for (int y = yStart; y < yEnd; ++y) {
						heights[((dataYOffset + y) * xSize) + (dataXOffset + x)] = segment->getHeight(x, y);
					}
				}
			}
		}
	}
}

float HeightMap::getHeight(float x, float y) const {
	int ix = (int) std::lround(std::floor(x / (float) mSegmentResolution));
	int iy = (int) std::lround(std::floor(y / (float) mSegmentResolution));

	std::shared_ptr<IHeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
		return mDefaultLevel;
	}
	return segment->getHeight((int) std::lround(x) - (ix * mSegmentResolution), (int) std::lround(y) - (iy * mSegmentResolution));

}

bool HeightMap::getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const {
	int ix = (int) std::lround(std::floor(x / (float) mSegmentResolution));
	int iy = (int) std::lround(std::floor(y / (float) mSegmentResolution));

	std::shared_ptr<IHeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
		return false;
	}
	segment->getHeightAndNormal(x - (float) (ix * mSegmentResolution), y - (float) (iy * mSegmentResolution), height, normal);
	return true;
}

std::shared_ptr<IHeightMapSegment> HeightMap::getSegment(int xIndex, int yIndex) const {
	auto I = mSegments.find(xIndex);
	if (I == mSegments.end()) {
		return std::shared_ptr<IHeightMapSegment>();
	}
	auto J = I->second.find(yIndex);
	if (J == I->second.end()) {
		return std::shared_ptr<IHeightMapSegment>();
	}
	return J->second;
}

}

}
}
