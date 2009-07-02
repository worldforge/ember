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
#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Shader.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>

namespace EmberOgre {
namespace Terrain {

TerrainPageSurfaceLayer::TerrainPageSurfaceLayer(TerrainPageSurface& terrainPageSurface, const TerrainLayerDefinition& definition, int surfaceIndex, Mercator::Shader* shader)
: mTerrainPageSurface(terrainPageSurface)
, mShader(shader)
, mCoverageImage(0)
, mCoverageDataStream(0)
, mSurfaceIndex(surfaceIndex)
, mDefinition(definition)
{

}


TerrainPageSurfaceLayer::~TerrainPageSurfaceLayer()
{
	OGRE_DELETE mCoverageImage;
	OGRE_DELETE mCoverageDataStream;
}

Ogre::TexturePtr TerrainPageSurfaceLayer::createTexture()
{
	if (mTexture.isNull()) {
		///we need an unique name for our alpha texture
		std::stringstream splatTextureNameSS;
		splatTextureNameSS << "terrain_" << mTerrainPageSurface.getWFPosition().x() << "_" << mTerrainPageSurface.getWFPosition().y() << "_" << mSurfaceIndex;
		const Ogre::String splatTextureName(splatTextureNameSS.str());

//		mTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(splatTextureName, "General", Ogre::TEX_TYPE_2D, getPixelWidth(), getPixelWidth(), 1, Ogre::PF_A8);
		///use no mipmaps since we had problems on nvidia cards with updating it
 		mTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadImage(splatTextureName, "General", *mCoverageImage, Ogre::TEX_TYPE_2D, 0);


	}
	return mTexture;
}

bool TerrainPageSurfaceLayer::unloadTexture()
{
	if (!mTexture.isNull()) {
		mTexture->unload();
		return true;
	}
	return false;
}


bool TerrainPageSurfaceLayer::intersects(const TerrainPageGeometry& geometry) const
{
	const SegmentVector validSegments = geometry.getValidSegments();
	///check if at least one surface intersects
	for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
		if (mShader->checkIntersect(*I->segment)) {
			return true;
		}
	}
	return false;
}

void TerrainPageSurfaceLayer::fillAlphaLayer(unsigned char* finalImagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY, unsigned short numberOfChannels) {

	int width = 64;
	int finalImageWidth = getPixelWidth();
	long i,j;

	Ogre::uchar* start = finalImagePtr + (numberOfChannels * finalImageWidth * (startY - 1)) + ((startX - 1) * numberOfChannels);
	Ogre::uchar* end = start + (width * finalImageWidth * numberOfChannels) + (width * numberOfChannels);
	///we need to do this to get the alignment correct
	wfImagePtr += 65;

	Ogre::uchar* tempPtr = end + channel + numberOfChannels;
	for (i = 0; i < width; ++i) {
		tempPtr -= (width * numberOfChannels);
		for (j = 0; j < width; ++j) {
			Ogre::uchar alpha = *(wfImagePtr + j);
			*(tempPtr) = alpha;
			///advance the number of channels
			tempPtr += numberOfChannels;

		}
		tempPtr -= (finalImageWidth * numberOfChannels);
		wfImagePtr += 65;
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

bool TerrainPageSurfaceLayer::createCoverageImage()
{
	if (mCoverageImage) {
		return false;
	}
	mCoverageDataStream = OGRE_NEW Ogre::MemoryDataStream(mTerrainPageSurface.getPixelWidth() * mTerrainPageSurface.getPixelWidth() * 1, true);
	mCoverageImage = OGRE_NEW Ogre::Image();
	mCoverageImage->loadDynamicImage(mCoverageDataStream->getPtr(), getPixelWidth(), getPixelWidth(), 1, Ogre::PF_A8);
	return true;

}


bool TerrainPageSurfaceLayer::destroyCoverageImage()
{
	if (mCoverageImage) {
		OGRE_DELETE mCoverageImage;
		mCoverageImage = 0;
		OGRE_DELETE mCoverageDataStream;
		mCoverageDataStream = 0;
	}
	return false;
}
void TerrainPageSurfaceLayer::updateCoverageImage(const TerrainPageGeometry& geometry)
{
	///only update if we have a coverage image, else we'll wait until later when we're need to call updateCoverageImage anyway
	if (mCoverageImage) {
/*	mCoverageDataStream = new Ogre::MemoryDataStream(mTerrainPageSurface.getPixelWidth() * mTerrainPageSurface.getPixelWidth() * 1, false);
	mCoverageDataStreamPtr = Ogre::DataStreamPtr(mCoverageDataStream);*/
		mCoverageDataStream->seek(0);

		///reset the coverage image
		memset(mCoverageDataStream->getPtr(), '\0', mCoverageDataStream->size());

		SegmentVector validSegments = geometry.getValidSegments();
		for (SegmentVector::const_iterator I = validSegments.begin(); I != validSegments.end(); ++I) {
			if (mShader->checkIntersect(*I->segment)) {
				Mercator::Surface* surface = getSurfaceForSegment(I->segment);
				if (surface && surface->isValid()) {

					int alphaChannel = 0;
					///use only one channel
					fillAlphaLayer(mCoverageDataStream->getPtr(), surface->getData(), alphaChannel, (int)I->index.x() * 64, (mTerrainPageSurface.getNumberOfSegmentsPerAxis() - (int)I->index.y() - 1) * 64, 1);

				}
			}
		}

		if (!mTexture.isNull()) {
			///if it's alreay loaded we need to blit directly to the hardware buffer to make sure it's updated
		 	if (mTexture->isLoaded()) {
				Ogre::HardwarePixelBufferSharedPtr hardwareBuffer = mTexture->getBuffer();

				///blit the whole image to the hardware buffer
				Ogre::PixelBox sourceBox = mCoverageImage->getPixelBox();
				//Ogre::Box targetBox(0,0, texture->getWidth(), texture->getHeight());
				hardwareBuffer->blitFromMemory(sourceBox);
		 	} else {
				mTexture->loadImage(*mCoverageImage);
		 	}
		}
	}
}


Mercator::Surface* TerrainPageSurfaceLayer::getSurfaceForSegment(const Mercator::Segment* segment) const
{
	Mercator::Segment::Surfacestore::const_iterator I = segment->getSurfaces().find(mSurfaceIndex);
	if (I == segment->getSurfaces().end()) {
		return 0;
	}
	return I->second;
}

const std::string& TerrainPageSurfaceLayer::getCoverageTextureName() const
{
	if (!mTexture.isNull()) {
		return mTexture->getName();
	}
	static std::string empty("");
	return empty;
}

Ogre::Image* TerrainPageSurfaceLayer::getCoverageImage() const
{
	return mCoverageImage;
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

Mercator::Shader* TerrainPageSurfaceLayer::getShader() const
{
	return mShader;
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

		Mercator::Segment::Surfacestore::iterator I(segment->getSurfaces().find(mSurfaceIndex));
		if (I == segment->getSurfaces().end()) {
			///the segment doesn't contain this surface yet, lets add it
			if (mShader->checkIntersect(*segment)) {
				S_LOG_VERBOSE("Adding new surface with id " << mSurfaceIndex << " to segment at x: " << segment->getXRef() << " y: " << segment->getYRef());
				Mercator::Segment::Surfacestore & sss = segment->getSurfaces();
				sss[mSurfaceIndex] = mShader->newSurface(*segment);
			}
		}
		///NOTE: we have to repopulate all surfaces mainly to get the foliage to work.
		segment->populateSurfaces();
		#endif
	}
}

}

}
