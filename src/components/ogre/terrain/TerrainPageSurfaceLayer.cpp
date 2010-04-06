//
// C++ Implementation: TerrainPageSurfaceLayer
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

#include "TerrainPage.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageGeometry.h"
#include "WFImage.h"
#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Shader.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>

namespace EmberOgre
{
namespace Terrain
{

TerrainPageSurfaceLayer::TerrainPageSurfaceLayer(TerrainPageSurface& terrainPageSurface, const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader) :
	mTerrainPageSurface(terrainPageSurface), mShader(shader), mSurfaceIndex(surfaceIndex), mDefinition(definition)
{

}

TerrainPageSurfaceLayer::~TerrainPageSurfaceLayer()
{

}

bool TerrainPageSurfaceLayer::intersects(const TerrainPageGeometry& geometry) const
{
	const SegmentVector validSegments = geometry.getValidSegments();
	///check if at least one surface intersects
	for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
		if (mShader.checkIntersect(*I->segment)) {
			return true;
		}
	}
	return false;
}

void TerrainPageSurfaceLayer::fillImage(const TerrainPageGeometry& geometry, Image& image, unsigned int channel) const
{
	SegmentVector validSegments = geometry.getValidSegments();
	for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
		if (mShader.checkIntersect(*I->segment)) {
			Mercator::Surface* surface = getSurfaceForSegment(I->segment);
			if (surface && surface->isValid()) {
				int alphaChannel = 0;
				WFImage sourceImage(new Image::ImageBuffer(65, 1, surface->getData()));
				//We need to adjust the position of the x index by one because there's a one pixel offset when converting between the Mercator Segments and the Ogre page.
				image.blit(sourceImage, channel, ((int)I->index.x() * 64) + 1, ((mTerrainPageSurface.getNumberOfSegmentsPerAxis() - (int)I->index.y() - 1) * 64));
			}
		}
	}
}

unsigned int TerrainPageSurfaceLayer::getPixelWidth() const
{
	return mTerrainPageSurface.getPixelWidth();
}

float TerrainPageSurfaceLayer::getScale() const
{
	return mScale;
}

void TerrainPageSurfaceLayer::setScale(float scale)
{
	mScale = scale;
}


Mercator::Surface* TerrainPageSurfaceLayer::getSurfaceForSegment(const Mercator::Segment* segment) const
{
	Mercator::Segment::Surfacestore::const_iterator I = segment->getSurfaces().find(mSurfaceIndex);
	if (I == segment->getSurfaces().end()) {
		return 0;
	}
	return I->second;
}


const std::string& TerrainPageSurfaceLayer::getDiffuseTextureName() const
{
	return mDiffuseTextureName;
}
void TerrainPageSurfaceLayer::setDiffuseTextureName(const std::string& textureName)
{
	mDiffuseTextureName = textureName;
}
const std::string& TerrainPageSurfaceLayer::getSpecularTextureName() const
{
	return mSpecularTextureName;
}
void TerrainPageSurfaceLayer::setSpecularTextureName(const std::string& textureName)
{
	mSpecularTextureName = textureName;
}
const std::string& TerrainPageSurfaceLayer::getNormalTextureName() const
{
	return mNormalTextureName;
}
void TerrainPageSurfaceLayer::setNormalTextureName(const std::string& textureName)
{
	mNormalTextureName = textureName;
}

int TerrainPageSurfaceLayer::getSurfaceIndex() const
{
	return mSurfaceIndex;
}

const TerrainLayerDefinition& TerrainPageSurfaceLayer::getDefinition() const
{
	return mDefinition;
}

void TerrainPageSurfaceLayer::populate(const TerrainPageGeometry& geometry)
{
	const SegmentVector validSegments = geometry.getValidSegments();
	for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
#if 0
		//the current Mercator code works such that whenever an Area is added to Terrain, _all_ surfaces for the affected segments are invalidated, thus requiering a total repopulation of the segment
		//If however that code was changed to only invalidate the affected surface the code below would be very much handy
		Mercator::Surface* surface(getSurfaceForSegment(I->segment));
		if (surface) {
			surface->populate();
		}
#else

		Mercator::Segment* segment(I->segment);
		if (!segment->isValid()) {
			segment->populate();
		}

		Mercator::Segment::Surfacestore::iterator I(segment->getSurfaces().find(mSurfaceIndex));
		if (I == segment->getSurfaces().end()) {
			///the segment doesn't contain this surface yet, lets add it
			if (mShader.checkIntersect(*segment)) {
				S_LOG_VERBOSE("Adding new surface with id " << mSurfaceIndex << " to segment at x: " << segment->getXRef() << " y: " << segment->getYRef());
				Mercator::Segment::Surfacestore & sss = segment->getSurfaces();
				sss[mSurfaceIndex] = mShader.newSurface(*segment);
			}
		}
		///NOTE: we have to repopulate all surfaces mainly to get the foliage to work.
		segment->populateSurfaces();
#endif
	}
}

}

}
