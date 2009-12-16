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

#include "HeightMap.h"
#include "HeightMapSegment.h"
#include "framework/LoggingInstance.h"
#include <cmath>

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x))
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x))
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x))
#else
    #define I_ROUND(_x) ((int)(_x))
#endif

#ifdef HAVE_FABSF
    #define F_ABS(_x) (::fabsf(_x))
#else
    #define F_ABS(_x) (::fabs(_x))
#endif


namespace EmberOgre
{

namespace Terrain
{

HeightMap::HeightMap(float defaultLevel, unsigned int segmentResolution)
: mDefaultLevel(defaultLevel), mSegmentResolution(segmentResolution)
{

}

HeightMap::~HeightMap()
{
}

void HeightMap::insert(int x, int y, HeightMapSegment* segment)
{
	S_LOG_VERBOSE("Adding heightmap for segment " << x << ", " << y);
	mSegments[x][y] = boost::shared_ptr<HeightMapSegment>(segment);
}

void HeightMap::remove(int x, int y)
{
	mSegments[x][y].reset();
}

float HeightMap::getHeight(float x, float y) const
{
    int ix = I_ROUND(floor(x / mSegmentResolution));
    int iy = I_ROUND(floor(y / mSegmentResolution));

	boost::shared_ptr<HeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
        return mDefaultLevel;
    }
    return segment->getHeight(I_ROUND(x) - (ix * mSegmentResolution), I_ROUND(y) - (iy * mSegmentResolution));


}

bool HeightMap::getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const
{
    int ix = I_ROUND(floor(x / mSegmentResolution));
    int iy = I_ROUND(floor(y / mSegmentResolution));

	boost::shared_ptr<HeightMapSegment> segment(getSegment(ix, iy));
	if (!segment.get()) {
        return false;
    }
	segment->getHeightAndNormal(x - (ix * (int)mSegmentResolution), y - (iy * (int)mSegmentResolution), height, normal);
	return true;
}

boost::shared_ptr<HeightMapSegment> HeightMap::getSegment(int x, int y) const
{
    Segmentstore::const_iterator I = mSegments.find(x);
    if (I == mSegments.end()) {
        return boost::shared_ptr<HeightMapSegment>();
    }
    Segmentcolumn::const_iterator J = I->second.find(y);
    if (J == I->second.end()) {
        return boost::shared_ptr<HeightMapSegment>();
    }
    return J->second;
}

}

}
