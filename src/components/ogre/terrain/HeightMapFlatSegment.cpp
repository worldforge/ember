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

#include "HeightMapFlatSegment.h"
#include "wfmath/vector.h"

namespace EmberOgre
{

namespace Terrain
{

HeightMapFlatSegment::HeightMapFlatSegment(float height) :
	mHeight(height)
{
}

HeightMapFlatSegment::~HeightMapFlatSegment()
{
}

float HeightMapFlatSegment::getHeight(int x, int y) const
{
	return mHeight;
}

void HeightMapFlatSegment::getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const
{
	height = mHeight;
	normal.x() = 0;
	normal.y() = 0;
	normal.z() = 1;
}

}

}
