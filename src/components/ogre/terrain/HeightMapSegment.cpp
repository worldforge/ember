/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>
 Copyright (C) 2003 Alistair Riddoch, Damien McGinnes (portions taken from Mercator::Segment)

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

#include "HeightMapSegment.h"
#include "HeightMapBuffer.h"
#include "Buffer.h"
#include <wfmath/vector.h>
#include <cmath>
#include <cassert>

namespace EmberOgre
{

namespace Terrain
{

HeightMapSegment::HeightMapSegment(HeightMapBuffer* buffer) :
	mBuffer(buffer)
{

}

HeightMapSegment::~HeightMapSegment()
{
	delete mBuffer;
}

float HeightMapSegment::getHeight(int x, int y) const
{
	return mBuffer->getBuffer()->getData()[y * (mBuffer->getResolution()) + x];
}

/// \brief Get an accurate height and normal vector at a given coordinate
/// relative to this segment.
///
/// The height and surface normal are determined by finding the four adjacent
/// height points nearest to the coordinate, and interpolating between
/// those height values. The square area defined by the 4 height points is
/// considered as two triangles for the purposes of interpolation to ensure
/// that the calculated height falls on the surface rendered by a 3D
/// graphics engine from the same heightfield data. The line used to
/// divide the area is defined by the gradient y = x, so the first
/// triangle has relative vertex coordinates (0,0) (1,0) (1,1) and
/// the second triangle has vertex coordinates (0,0) (0,1) (1,1).
void HeightMapSegment::getHeightAndNormal(float x, float y, float& h, WFMath::Vector<3> &normal) const
{
	// FIXME this ignores edges and corners
	assert(x <= mBuffer->getResolution());
	assert(x >= 0.0f);
	assert(y <= mBuffer->getResolution());
	assert(y >= 0.0f);

	// get index of the actual tile in the segment
	int tile_x = (int)floor(x);
	int tile_y = (int)floor(y);

	// work out the offset into that tile
	float off_x = x - tile_x;
	float off_y = y - tile_y;

	float h1 = getHeight(tile_x, tile_y);
	float h2 = getHeight(tile_x, tile_y + 1);
	float h3 = getHeight(tile_x + 1, tile_y + 1);
	float h4 = getHeight(tile_x + 1, tile_y);

	// square is broken into two triangles
	// top triangle |/
	if ((off_x - off_y) <= 0.f) {
		normal = WFMath::Vector<3>(h2 - h3, h1 - h2, 1.0f);

		//normal for intersection of both triangles
		if (off_x == off_y) {
			normal += WFMath::Vector<3>(h1 - h4, h4 - h3, 1.0f);
		}
		normal.normalize();
		h = h1 + (h3 - h2) * off_x + (h2 - h1) * off_y;
	}
	// bottom triangle /|
	else {
		normal = WFMath::Vector<3>(h1 - h4, h4 - h3, 1.0f);
		normal.normalize();
		h = h1 + (h4 - h1) * off_x + (h3 - h4) * off_y;
	}
}

}

}
