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

//MSVC 11.0 doesn't support std::lround so we'll use boost. When MSVC gains support for std::lround this could be removed.
#ifdef _MSC_VER
#include <boost/math/special_functions/round.hpp>
#define I_ROUND(_x) (boost::math::lround(_x))
#else
#define I_ROUND(_x) (std::lround(_x))
#endif

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

HeightMap::HeightMap(float defaultLevel, unsigned int segmentResolution) :
		mDefaultLevel(defaultLevel), mSegmentResolution(segmentResolution)
{

}

HeightMap::~HeightMap()
{
}

void HeightMap::insert(int xIndex, int yIndex, IHeightMapSegment* segment)
{
	mSegments[xIndex][yIndex] = std::shared_ptr < IHeightMapSegment > (segment);
}

bool HeightMap::remove(int xIndex, int yIndex)
{
	Segmentstore::iterator column = mSegments.find(xIndex);
	if (column != mSegments.end()) {
		Segmentcolumn::iterator row = column->second.find(yIndex);
		if (row != column->second.end()) {
			column->second.erase(row);
			return true;
		}
	}
	return false;
}

void HeightMap::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const
{

	int xSize = xMax - xMin;

	int segmentXMin = I_ROUND(floor(xMin / (double)mSegmentResolution));
	int segmentXMax = I_ROUND(floor(xMax / (double)mSegmentResolution));
	int segmentYMin = I_ROUND(floor(yMin / (double)mSegmentResolution));
	int segmentYMax = I_ROUND(floor(yMax / (double)mSegmentResolution));

	for (int segmentX = segmentXMin; segmentX <= segmentXMax; ++segmentX) {
		for (int segmentY = segmentYMin; segmentY <= segmentYMax; ++segmentY) {

			auto segmentPtr = getSegment(segmentX, segmentY);
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

float HeightMap::getHeight(float x, float y) const
{
	int ix = I_ROUND(floor(x / mSegmentResolution));
	int iy = I_ROUND(floor(y / mSegmentResolution));

	std::shared_ptr<IHeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
		return mDefaultLevel;
	}
	return segment->getHeight(I_ROUND(x) - (ix * mSegmentResolution), I_ROUND(y) - (iy * mSegmentResolution));

}

bool HeightMap::getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const
{
	int ix = I_ROUND(floor(x / mSegmentResolution));
	int iy = I_ROUND(floor(y / mSegmentResolution));

	std::shared_ptr<IHeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
		return false;
	}
	segment->getHeightAndNormal(x - (ix * (int)mSegmentResolution), y - (iy * (int)mSegmentResolution), height, normal);
	return true;
}

std::shared_ptr<IHeightMapSegment> HeightMap::getSegment(int xIndex, int yIndex) const
{
	Segmentstore::const_iterator I = mSegments.find(xIndex);
	if (I == mSegments.end()) {
		return std::shared_ptr<IHeightMapSegment>();
	}
	Segmentcolumn::const_iterator J = I->second.find(yIndex);
	if (J == I->second.end()) {
		return std::shared_ptr<IHeightMapSegment>();
	}
	return J->second;
}

}

}
}
