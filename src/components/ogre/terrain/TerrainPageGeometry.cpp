//
// C++ Implementation: TerrainPageGeometry
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageGeometry.h"

#include "TerrainPage.h"
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x))
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x))
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x))
#else
    #define I_ROUND(_x) ((int)(_x))
#endif

namespace EmberOgre
{
namespace Terrain
{

TerrainPageGeometry::TerrainPageGeometry(TerrainPage& page) :
	mPage(page)
{
}

TerrainPageGeometry::~TerrainPageGeometry()
{
}

void TerrainPageGeometry::init(const Mercator::Terrain& terrain)
{
	mLocalSegments.clear();
	for (int y = 0; y < mPage.getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < mPage.getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegmentAtLocalIndex(terrain, x,y);
			if (segment) {
				if (!segment->isValid()) {
					segment->populate();
					segment->populateNormals();
				}
			}
			mLocalSegments[x][y] = segment;
		}
	}
}

void TerrainPageGeometry::repopulate()
{
	for (Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment* segment = J->second;
			if (segment) {
				if (!segment->isValid()) {
					segment->populate();
					segment->populateNormals();
				}
			}
		}
	}
}


float TerrainPageGeometry::getMaxHeight() const
{
	SegmentVector validSegments = getValidSegments();
	float max = std::numeric_limits<float>::min();
	for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
		max = std::max<float>(max, I->segment->getMax());
	}
	return max;
}
void TerrainPageGeometry::updateOgreHeightData(float* heightData)
{
	int i = 0;
	for (Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		int j = 0;
		for (Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment* segment = J->second;
			if (segment) {
				blitSegmentToOgre(heightData, *segment, (i * 64), ((mPage.getNumberOfSegmentsPerAxis() - j - 1) * 64));
			}
			++j;
		}
		++i;
	}
}

void TerrainPageGeometry::blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startY)
{

	int width = 64;
	int finalBitmapWidth = mPage.getPageSize();

	float* segmentHeightPtr = segment.getPoints();

	float* start = ogreHeightData + (finalBitmapWidth * (startY)) + ((startX));
	float* end = start + (width * finalBitmapWidth) + width;
	///we need to do this to get the alignment correct
	//segmentHeightPtr += (width + 1);

	float* tempPtr = end + 1 ;
	for (unsigned int i = 0; i < width + 1; ++i)
	{
		tempPtr -= width + 1;
		for (unsigned int j = 0; j < width + 1; ++j)
		{
			*(tempPtr) = *(segmentHeightPtr + j);
			tempPtr += 1;
		}
		tempPtr -= finalBitmapWidth;
		segmentHeightPtr += (width + 1);
	}
}

Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalIndex(const Mercator::Terrain& terrain, int indexX, int indexY) const
{
	int segmentsPerAxis = mPage.getNumberOfSegmentsPerAxis();
	int segmentOffset = segmentsPerAxis;
	int segX = (int)((mPage.getWFPosition().x() * segmentsPerAxis) + indexX);
	int segY = (int)((mPage.getWFPosition().y() * segmentsPerAxis) + indexY) - segmentOffset;


	return terrain.getSegment(segX, segY);
}

const Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));

	Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return 0;
	}
	Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
	if (J == I->second.end()) {
		return 0;
	}
	return J->second;
}

const Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));

	localPositionInSegment.x() = pos.x() - (ix * 64);
	localPositionInSegment.y() = pos.y() - (iy * 64);

	Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return 0;
	}
	Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
	if (J == I->second.end()) {
		return 0;
	}
	return J->second;
}

const SegmentVector TerrainPageGeometry::getValidSegments() const
{
	SegmentVector validSegments;
	for (Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment* segment = J->second;
			if (segment) {
				if (segment->isValid()) {
					PageSegment pageSegment;
					pageSegment.index = TerrainPosition(I->first, J->first);
					pageSegment.segment = segment;

					validSegments.push_back(pageSegment);
				}
			}
		}
	}
	return validSegments;
}

bool TerrainPageGeometry::getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const
{

	const Mercator::Segment* segment(getSegmentAtLocalPosition(localPosition));
	if (segment && segment->getNormals()) {
		int resolution = segment->getResolution();
		size_t xPos = localPosition.x() - (I_ROUND(floor(localPosition.x() / resolution)) * resolution);
		size_t yPos = localPosition.y() - (I_ROUND(floor(localPosition.y() / resolution)) * resolution);
		size_t normalPos = (yPos * segment->getSize() * 3) + (xPos * 3);
		normal = WFMath::Vector<3>(segment->getNormals()[normalPos], segment->getNormals()[normalPos + 1], segment->getNormals()[normalPos] + 2);
		return true;
	} else {
		return false;
	}
}
}
}
