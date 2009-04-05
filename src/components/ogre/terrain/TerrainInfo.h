//
// C++ Interface: TerrainInfo
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGRETERRAININFO_H
#define EMBEROGRETERRAININFO_H

#include "../EmberOgrePrerequisites.h"
// #include <wfmath/wfmath.h>
#include <wfmath/point.h>
#include <wfmath/axisbox.h>

namespace Mercator
{
class BasePoint;
}

namespace EmberOgre {
namespace Terrain {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	Class holding information about the terrain.
*/
class TerrainInfo
{
public:
	TerrainInfo();
	double getWorldSizeX() const;
	double getWorldSizeY() const;
	int getTotalNumberOfPagesX()  const;
	int getTotalNumberOfPagesY() const;
	int getPageOffsetX() const;
	int getPageOffsetY() const;
	void setPageIndicesSize(int size);
	
/*	void setNewXMax(int xMax);
	void setNewXMin(int xMin);
	void setNewYMax(int yMax);
	void setNewYMin(int yMin);*/
	
	void setBasePoint(const WFMath::Point<2>& position, const Mercator::BasePoint& basepoint);
	const WFMath::AxisBox<2>& getWorldSizeInIndices() const;
	const WFMath::AxisBox<2>& getWorldSizeInSegments() const;
	const WFMath::AxisBox<2>& getWorldSizeInPages() const;
	
private:
	
	void adjustBasePointPositionDown(WFMath::CoordType& basePointPositionAdjusted, WFMath::CoordType newBasePointPosition);
	
	void adjustBasePointPositionUp(WFMath::CoordType& basePointPositionAdjusted, WFMath::CoordType newBasePointPosition);
	
	void recalculateSize();
	
	/**
	* 
	*/
	WFMath::AxisBox<2> mCalculatedSize, mCalculatedSegmentSize, mCalculatedPageSize;
//	int mCalculatedXMin, mCalculatedXmax, mCalculatedYmin, mCalculatedYmax;
	
	/**
	 * the min and max indices for segments
	 */
// 	int mXmin, mXmax, mYmin, mYmax;
	WFMath::CoordType mXminBasePoint, mXmaxBasePoint, mYminBasePoint, mYmaxBasePoint;
	WFMath::CoordType mXminBasePointAdjusted, mXmaxBasePointAdjusted, mYminBasePointAdjusted, mYmaxBasePointAdjusted;

	int mPageIndicesSize;

};
}
}

#endif
