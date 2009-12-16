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

#ifndef EMBEROGRETERRAINHEIGHTMAP_H_
#define EMBEROGRETERRAINHEIGHTMAP_H_

#include "Types.h"
#include <boost/smart_ptr.hpp>
#include <map>


namespace EmberOgre
{

namespace Terrain
{

class HeightMapSegment;

class HeightMap
{
public:

    /// \brief STL map to store sparse array of Segment pointers.
    typedef std::map<int, boost::shared_ptr<HeightMapSegment> > Segmentcolumn;

    /// \brief STL map to store sparse array of Segment pointer columns.
    typedef std::map<int, Segmentcolumn > Segmentstore;

	HeightMap(float defaultLevel, unsigned int segmentResolution = 64);
	virtual ~HeightMap();

	void insert(int x, int y, HeightMapSegment* segment);

	void remove(int x, int y);

    float getHeight(float x, float y) const;

    bool getHeightAndNormal(float x, float y, float& height, WFMath::Vector<3>& normal) const;


private:

	Segmentstore mSegments;

	float mDefaultLevel;

	unsigned int mSegmentResolution;

	boost::shared_ptr<HeightMapSegment> getSegment(int x, int y) const;
};

}

}

#endif /* EMBEROGRETERRAINHEIGHTMAP_H_ */
