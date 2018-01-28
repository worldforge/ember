//
// C++ Implementation: TerrainPageGeometry
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageGeometry.h"
#include "Segment.h"
#include "SegmentManager.h"

#include "TerrainPage.h"
#include "components/ogre/Convert.h"
#include <Mercator/Segment.h>
#include <wfmath/stream.h>

//MSVC 11.0 doesn't support std::lround so we'll use boost. When MSVC gains support for std::lround this could be removed.
#ifdef _MSC_VER
#include <boost/math/special_functions/round.hpp>
#define I_ROUND(_x) (boost::math::lround(_x))
#else
#define I_ROUND(_x) (std::lround(_x))
#endif

namespace Ember {
namespace OgreView {
namespace Terrain {

TerrainPageGeometry::TerrainPageGeometry(TerrainPage& page, SegmentManager& segmentManager, float defaultHeight) :
		mPage(page), mDefaultHeight(defaultHeight) {

	SegmentManager::IndexMap indices;
	int segmentsPerAxis = mPage.getNumberOfSegmentsPerAxis();
	int segmentOffset = segmentsPerAxis;

	for (int z = 0; z < mPage.getNumberOfSegmentsPerAxis(); ++z) {
		for (int x = 0; x < mPage.getNumberOfSegmentsPerAxis(); ++x) {
			int segX = (int) ((mPage.getWFPosition().x() * segmentsPerAxis) + x);
			int segZ = (int) ((mPage.getWFPosition().y() * segmentsPerAxis) + z) - (segmentOffset - 1);
			indices[x][mPage.getNumberOfSegmentsPerAxis() - z - 1] = std::make_pair(segX, -segZ);

			S_LOG_INFO("segment: x: "<< x <<" z: " << z << " segX: " <<segX << " segZ: " << -segZ);
		}
	}
	size_t count = segmentManager.getSegmentReferences(indices, mLocalSegments);
	if (count == 0) {
		std::stringstream ss;
		ss << "Created TerrainPageGeometry for which there are no valid segments. Pos: " << page.getWFPosition();
		S_LOG_WARNING(ss.str());
	}
}

TerrainPageGeometry::~TerrainPageGeometry() {
}

void TerrainPageGeometry::repopulate(bool alsoNormals) {
	for (const auto& column : mLocalSegments) {
		for (const auto& entry : column.second) {
			Mercator::Segment& segment = entry.second->getMercatorSegment();
			if (!segment.isValid()) {
				segment.populate();
			}
			if (alsoNormals && !segment.getNormals()) {
				segment.populateNormals();
			}
		}
	}
}

TerrainPage& TerrainPageGeometry::getPage() {
	return mPage;
}

float TerrainPageGeometry::getMaxHeight() const {
	float max = std::numeric_limits<float>::min();
	for (const auto& column : mLocalSegments) {
		for (const auto& entry : column.second) {
			Mercator::Segment& segment = entry.second->getMercatorSegment();
			if (segment.isValid()) {
				max = std::max<float>(max, segment.getMax());
			}
		}
	}
	return max;
}

void TerrainPageGeometry::updateOgreHeightData(float* heightData) {
	float* heightDataPtr = heightData;

	int sizeOfBitmap = mPage.getVerticeCount();
	//Set the height of any uninitialized part to the default height. This might be optimized better though.
	for (int i = 0; i < sizeOfBitmap; ++i) {
		*(heightDataPtr++) = mDefaultHeight;
	}

	for (const auto& column : mLocalSegments) {
		for (const auto& entry : column.second) {
			Mercator::Segment& segment = entry.second->getMercatorSegment();
			if (segment.isValid()) {
				blitSegmentToOgre(heightData, segment, column.first * segment.getResolution(),
								  entry.first * segment.getResolution());
			}
		}
	}
}

void TerrainPageGeometry::blitSegmentToOgre(float* ogreHeightData, Mercator::Segment& segment, int startX, int startZ) {
	int segmentWidth = segment.getSize();
	int pageWidth = mPage.getPageSize();
	int ogreDataSize = pageWidth * pageWidth;

	const float* sourcePtr = segment.getPoints();

	float* dataEnd = ogreHeightData + ogreDataSize;

	// copy points line-by line
	float* destPtr = ogreHeightData + (pageWidth * ( pageWidth - startZ - 1)) + startX;


	for (int i = 0; i < segmentWidth; ++i) {
		for (int j = 0; j < segmentWidth; ++j) {
			if ((destPtr + j) >= ogreHeightData && (destPtr + j) < dataEnd) {
				*(destPtr + j) = *(sourcePtr + j);
			}
		}
		destPtr -= pageWidth;
		sourcePtr += segmentWidth;
	}
}

Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos) const {
	int ix = I_ROUND(floor(pos.x() / 64));
	int iz = I_ROUND(floor(pos.y() / 64));

	auto I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return nullptr;
	}
	auto J = I->second.find(iz);
	if (J == I->second.end()) {
		return nullptr;
	}
	return &J->second->getMercatorSegment();
}

Mercator::Segment* TerrainPageGeometry::getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const {
	int ix = I_ROUND(floor(pos.x() / 64));
	int iz = I_ROUND(floor(pos.y() / 64));

	localPositionInSegment.x() = pos.x() - (ix * 64);
	localPositionInSegment.y() = pos.y() - (iz * 64);

	auto I = mLocalSegments.find(ix);
	if (I == mLocalSegments.end()) {
		return nullptr;
	}
	auto J = I->second.find(iz);
	if (J == I->second.end()) {
		return nullptr;
	}
	return &J->second->getMercatorSegment();
}

const SegmentVector TerrainPageGeometry::getValidSegments() const {
	SegmentVector validSegments;
	for (const auto& column : mLocalSegments) {
		for (const auto& entry : column.second) {
			Mercator::Segment& segment = entry.second->getMercatorSegment();
			PageSegment pageSegment;
			pageSegment.index = TerrainPosition(column.first, entry.first);
			pageSegment.segment = &segment;

			validSegments.push_back(pageSegment);
		}
	}
	return validSegments;
}

bool TerrainPageGeometry::getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const {

	const Mercator::Segment* segment(getSegmentAtLocalPosition(localPosition));
	if (segment && segment->getNormals()) {
		int resolution = segment->getResolution();
		size_t xPos = localPosition.x() - (I_ROUND(floor(localPosition.x() / resolution)) * resolution);
		size_t zPos = localPosition.y() - (I_ROUND(floor(localPosition.y() / resolution)) * resolution);
		size_t normalPos = (zPos * segment->getSize() * 3) + (xPos * 3);
		normal = WFMath::Vector<3>(segment->getNormals()[normalPos], segment->getNormals()[normalPos + 1], segment->getNormals()[normalPos] + 2);
		return true;
	} else {
		return false;
	}
}
}
}
}
