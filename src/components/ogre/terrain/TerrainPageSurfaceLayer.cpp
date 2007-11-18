//
// C++ Implementation: TerrainPageSurfaceLayer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#include <Mercator/Surface.h>
#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

namespace EmberOgre {
namespace Terrain {

TerrainPageSurfaceLayer::TerrainPageSurfaceLayer(TerrainPageSurface& terrainPageSurface, int surfaceIndex, Mercator::Shader* shader)
: mTerrainPageSurface(terrainPageSurface)
, mShader(shader)
, mCoverageImage(new Ogre::Image())
, mCoverageDataStream(new Ogre::MemoryDataStream(terrainPageSurface.getPixelWidth() * terrainPageSurface.getPixelWidth() * 1, false))
, mCoverageDataStreamPtr(mCoverageDataStream)
, mSurfaceIndex(surfaceIndex)
{
	///we need an unique name for our alpha texture
	std::stringstream splatTextureNameSS;
	splatTextureNameSS << "terrain_" << terrainPageSurface.getWFPosition().x() << "_" << terrainPageSurface.getWFPosition().y() << "_" << surfaceIndex;
	const Ogre::String splatTextureName(splatTextureNameSS.str());
	
	mTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(splatTextureName, "General", Ogre::TEX_TYPE_2D, getPixelWidth(), getPixelWidth(), 1, Ogre::PF_A8);

}


TerrainPageSurfaceLayer::~TerrainPageSurfaceLayer()
{
}

bool TerrainPageSurfaceLayer::intersects()
{
	///check if at least one surface intersects
	for (SegmentVector::iterator I = mTerrainPageSurface.getValidSegments().begin(); I != mTerrainPageSurface.getValidSegments().end(); ++I) {
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



void TerrainPageSurfaceLayer::updateCoverageImage()
{
/*	mCoverageDataStream = new Ogre::MemoryDataStream(mTerrainPageSurface.getPixelWidth() * mTerrainPageSurface.getPixelWidth() * 1, false);
	mCoverageDataStreamPtr = Ogre::DataStreamPtr(mCoverageDataStream);*/
	mCoverageDataStream->seek(0);
	
	///reset the coverage image
	memset(mCoverageDataStream->getPtr(), '\0', mCoverageDataStream->size());

	for (SegmentVector::iterator I = mTerrainPageSurface.getValidSegments().begin(); I != mTerrainPageSurface.getValidSegments().end(); ++I) {
		if (mShader->checkIntersect(*I->segment)) {
			Mercator::Surface* surface = getSurfaceForSegment(I->segment);
			if (surface && surface->isValid()) {
			
				int alphaChannel = 0;
				///use only one channel
				fillAlphaLayer(mCoverageDataStream->getPtr(), surface->getData(), alphaChannel, (int)I->pos.x() * 64, (mTerrainPageSurface.getNumberOfSegmentsPerAxis() - (int)I->pos.y() - 1) * 64, 1);
	
			}
		}
	}
		
	mCoverageImage->loadRawData(mCoverageDataStreamPtr, getPixelWidth(), getPixelWidth(), Ogre::PF_A8);
	mTexture->loadImage(*mCoverageImage);
	
	///if it's alreay loaded we need to blit directly to the hardware buffer to make sure it's updated
// 	if (mTexture->isLoaded()) {
		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer = mTexture->getBuffer();
		
		///blit the whole image to the hardware buffer
		Ogre::PixelBox sourceBox = mCoverageImage->getPixelBox();
		//Ogre::Box targetBox(0,0, texture->getWidth(), texture->getHeight());
		hardwareBuffer->blitFromMemory(sourceBox);	
// 	}
	
	
	
}

Mercator::Surface* TerrainPageSurfaceLayer::getSurfaceForSegment(Mercator::Segment* segment) const
{
	Mercator::Segment::Surfacestore::const_iterator I = segment->getSurfaces().find(mSurfaceIndex);
	if (I == segment->getSurfaces().end()) {
		return 0;
	}
	return I->second;
}

const std::string& TerrainPageSurfaceLayer::getCoverageTextureName() const
{
	return mTexture->getName();
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

Mercator::Shader* TerrainPageSurfaceLayer::getShader()
{
	return mShader;
}
int TerrainPageSurfaceLayer::getSurfaceIndex()
{
	return mSurfaceIndex;
}
}

}
