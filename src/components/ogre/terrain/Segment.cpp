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

#include "Segment.h"
#include <Mercator/Segment.h>
#include <sstream>
namespace EmberOgre
{

namespace Terrain
{

Segment::Segment(Mercator::Segment& segment) :
	mSegment(segment)
{
}

Segment::~Segment()
{
}

Mercator::Segment& Segment::getMercatorSegment()
{
	return mSegment;
}

int Segment::getXIndex() const
{
	return mSegment.getXRef() / mSegment.getResolution();

}
int Segment::getYIndex() const
{
	return mSegment.getYRef() / mSegment.getResolution();
}

std::string Segment::getKey() const
{
	std::stringstream ss;
	ss << getXIndex() << "_" << getYIndex();
	return ss.str();
}
}

}
