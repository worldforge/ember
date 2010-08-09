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
#include "SegmentReference.h"
#include "Segment.h"
#include "SegmentManager.h"

#include "TerrainPage.h"
#include "OgreImage.h"
#include "WFImage.h"
#include "Buffer.h"
#include "components/ogre/Convert.h"
#include <Mercator/Segment.h>
#include <wfmath/stream.h>

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

TerrainPageGeometry::TerrainPageGeometry(TerrainPage& page, SegmentManager& segmentManager, float defaultHeight) :
	mPage(page), mDefaultHeight(defaultHeight)
{

	SegmentManager::IndexMap indices;
	int segmentsPerAxis = mPage.getNumberOfSegmentsPerAxis();
	int segmentOffset = segmentsPerAxis;

	for (int y = 0; y < mPage.getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < mPage.getNumberOfSegmentsPerAxis(); ++x) {
			int segX = (int)((mPage.getWFPosition().x() * segmentsPerAxis) + x);
			int segY = (int)((mPage.getWFPosition().y() * segmentsPerAxis) + y) - segmentOffset;
			indices[x][y] = std::make_pair(segX, segY);

		}
	}
	size_t count = segmentManager.getSegmentReferences(indices, mLocalSegments);
	if (count == 0) {
		std::stringstream ss;
		ss << "Created TerrainPageGeometry for which there are no valid segments. Pos: " << page.getWFPosition();
		S_LOG_WARNING(ss.str());
	}
}

TerrainPageGeometry::~TerrainPageGeometry()
{
}

void TerrainPageGeometry::repopulate()
{
	for (SegmentRefStore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (SegmentRefColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment& segment = J->second->getSegment().getMercatorSegment();
			if (!segment.isValid()) {
				segment.populate();
				//segment->populateNormals();
			}
		}
	}
}

TerrainPage& TerrainPageGeometry::getPage()
{
	return mPage;
}

float TerrainPageGeometry::getMaxHeight() const
{
	float max = std::numeric_limits<float>::min();
	for (SegmentRefStore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (SegmentRefColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment& segment = J->second->getSegment().getMercatorSegment();
			if (segment.isValid()) {
				max = std::max<float>(max, segment.getMax());
			}
		}
	}
	return max;
}
void TerrainPageGeometry::updateOgreHeightData(float* heightData)
{

	float* heightDataPtr = heightData;
	unsigned int sizeOfBitmap = mPage.getVerticeCount();
	//Set the height of any uninitialized part to the default height. This might be optimized better though.
	for (unsigned int i = 0; i < sizeOfBitmap; ++i) {
		*(heightDataPtr++) = mDefaultHeight;
	}

	for (SegmentRefStore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (SegmentRefColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment& segment = J->second->getSegment().getMercatorSegment();
			if (segment.isValid()) {
				//Note that we add one to the x position here. That's to adjust for the slight mismatch between the WF Mercator::Segments and the Ogre space.
				blitSegmentToOgre(heightData, segment, (I->first * 64) + 1, ((mPage.getNumberOfSegmentsPerAxis() - J->first - 1) * 64));
			}
		}
	}
}

void TerrainPageGeometry::blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startY)
{
	int segmentWidth = segment.getSize();
	int i, j;
	int pageWidth = mPage.getPageSize();
	size_t ogreDataSize = pageWidth * pageWidth;

	const float* sourcePtr = segment.getPoints();
	float* destPtr = ogreHeightData;

	float* dataEnd = ogreHeightData + ogreDataSize;

	float* end = destPtr + (pageWidth * ((segmentWidth - 1) + startY)) + (((segmentWidth - 1) + startX));

	float* tempPtr = end;
	for (i = 0; i < segmentWidth; ++i) {
		tempPtr -= segmentWidth;
		for (j = 0; j < segmentWidth; ++j) {

			if (tempPtr >= ogreHeightData && tempPtr < dataEnd) {
				*(tempPtr) = *(sourcePtr + j);
			}
			tempPtr += 1;
		}
		tempPtr -= pageWidth;
		sourcePtr += segmentWidth;
	}
}

Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));

	SegmentRefStore::const_iterator I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return 0;
	}
	SegmentRefColumn::const_iterator J = I->second.find(iy);
	if (J == I->second.end()) {
		return 0;
	}
	return &J->second->getSegment().getMercatorSegment();
}

Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));

	localPositionInSegment.x() = pos.x() - (ix * 64);
	localPositionInSegment.y() = pos.y() - (iy * 64);

	SegmentRefStore::const_iterator I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return 0;
	}
	SegmentRefColumn::const_iterator J = I->second.find(iy);
	if (J == I->second.end()) {
		return 0;
	}
	return &J->second->getSegment().getMercatorSegment();
}

const SegmentVector TerrainPageGeometry::getValidSegments() const
{
	SegmentVector validSegments;
	for (SegmentRefStore::const_iterator I = mLocalSegments.begin(); I != mLocalSegments.end(); ++I) {
		for (SegmentRefColumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			Mercator::Segment& segment = J->second->getSegment().getMercatorSegment();
			PageSegment pageSegment;
			pageSegment.index = TerrainPosition(I->first, J->first);
			pageSegment.segment = &segment;

			validSegments.push_back(pageSegment);
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
