//
// C++ Implementation: TerrainInfo
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "TerrainInfo.h"

namespace EmberOgre {


TerrainInfo::TerrainInfo()
:  mXminBasePoint(0), mXmaxBasePoint(0), mYminBasePoint(0), mYmaxBasePoint(0),
	 mXminBasePointAdjusted(0), mXmaxBasePointAdjusted(0), mYminBasePointAdjusted(0), mYmaxBasePointAdjusted(0)
{
	recalculateSize();
}


void TerrainInfo::setBasePoint(const WFMath::Point<2>& position, const Mercator::BasePoint& basepoint)
{
	mXminBasePoint = std::min<WFMath::CoordType>(position.x(), mXminBasePoint);
	mXmaxBasePoint = std::max<WFMath::CoordType>(position.x(), mXmaxBasePoint);
	mYminBasePoint = std::min<WFMath::CoordType>(position.y(), mYminBasePoint);
	mYmaxBasePoint = std::max<WFMath::CoordType>(position.y(), mYmaxBasePoint);
	
	adjustBasePointPositionDown(mXminBasePointAdjusted, mXminBasePoint);
	adjustBasePointPositionUp(mXmaxBasePointAdjusted, mXmaxBasePoint);
	adjustBasePointPositionDown(mYminBasePointAdjusted, mYminBasePoint);
	adjustBasePointPositionUp(mYmaxBasePointAdjusted, mYmaxBasePoint);
	
	recalculateSize();
}

void TerrainInfo::recalculateSize()
{
	mCalculatedSize = WFMath::AxisBox<2>(WFMath::Point<2>(mXminBasePointAdjusted * 64, mYminBasePointAdjusted * 64), WFMath::Point<2>(mXmaxBasePointAdjusted * 64, mYmaxBasePointAdjusted * 64));
	mCalculatedSegmentSize = WFMath::AxisBox<2>(WFMath::Point<2>(mXminBasePointAdjusted, mYminBasePointAdjusted ), WFMath::Point<2>(mXmaxBasePointAdjusted , mYmaxBasePointAdjusted ));
	
	int mercatorSegmentsPerOgrePage = mPageIndicesSize / 64;
	mCalculatedPageSize = WFMath::AxisBox<2>(WFMath::Point<2>(mXminBasePointAdjusted / mercatorSegmentsPerOgrePage, mYminBasePointAdjusted / mercatorSegmentsPerOgrePage), WFMath::Point<2>(mXmaxBasePointAdjusted / mercatorSegmentsPerOgrePage, mYmaxBasePointAdjusted / mercatorSegmentsPerOgrePage));
}


void TerrainInfo::adjustBasePointPositionUp(WFMath::CoordType& basePointPositionAdjusted, WFMath::CoordType newBasePointPosition)
{
	int mercatorSegmentsPerOgrePage = mPageIndicesSize / 64;
	int remainder = static_cast<int>(newBasePointPosition) % mercatorSegmentsPerOgrePage;
	if (remainder != 0) {
		///adjust the position
		basePointPositionAdjusted = newBasePointPosition + (mercatorSegmentsPerOgrePage - remainder);
	}
}

void TerrainInfo::adjustBasePointPositionDown(WFMath::CoordType& basePointPositionAdjusted, WFMath::CoordType newBasePointPosition)
{
	int mercatorSegmentsPerOgrePage = mPageIndicesSize / 64;
	int remainder = std::abs(static_cast<int>(newBasePointPosition) % mercatorSegmentsPerOgrePage);
	if (remainder != 0) {
		///adjust the position
		basePointPositionAdjusted = newBasePointPosition - (mercatorSegmentsPerOgrePage - remainder);
	}
}

const WFMath::AxisBox<2>& TerrainInfo::getWorldSizeInSegments() const
{
	return mCalculatedSegmentSize;
}


const WFMath::AxisBox<2>& TerrainInfo::getWorldSizeInIndices() const
{
	return mCalculatedSize;
}

const WFMath::AxisBox<2>& TerrainInfo::getWorldSizeInPages() const
{
	return mCalculatedPageSize;
}


double TerrainInfo::getWorldSizeX() const
{
	return mCalculatedSize.highCorner().x() - mCalculatedSize.lowCorner().x();
}

double TerrainInfo::getWorldSizeY() const
{
	return mCalculatedSize.highCorner().y() - mCalculatedSize.lowCorner().y();
}

int TerrainInfo::getTotalNumberOfPagesX() const
{
	return static_cast<int>(mCalculatedPageSize.highCorner().x() - mCalculatedPageSize.lowCorner().x());
}
int TerrainInfo::getTotalNumberOfPagesY() const
{
	return static_cast<int>(mCalculatedPageSize.highCorner().y() - mCalculatedPageSize.lowCorner().y());
}

int TerrainInfo::getPageOffsetX() const
{
	return static_cast<int>(-mCalculatedPageSize.lowCorner().x());
}
int TerrainInfo::getPageOffsetY() const
{
	return static_cast<int>(-mCalculatedPageSize.lowCorner().y());
}

void TerrainInfo::setPageIndicesSize(int size)
{
	mPageIndicesSize = size;
}


}
