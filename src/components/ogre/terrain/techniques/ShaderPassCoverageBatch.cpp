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


#include "ShaderPassCoverageBatch.h"
#include "ShaderPass.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

ShaderPassCoverageBatch::ShaderPassCoverageBatch(ShaderPass& shaderPass, Ogre::TexturePtr combinedCoverageTexture) :
	mShaderPass(shaderPass), mCombinedCoverageTexture(combinedCoverageTexture), mCombinedCoverageImage(OGRE_NEW Ogre::Image()), mCombinedCoverageDataStream(OGRE_NEW Ogre::MemoryDataStream(combinedCoverageTexture->getWidth() * combinedCoverageTexture->getWidth() * 4, true))
{
	///reset the coverage image
	memset(mCombinedCoverageDataStream->getPtr(), '\0', mCombinedCoverageDataStream->size());
}

ShaderPassCoverageBatch::~ShaderPassCoverageBatch()
{
	OGRE_DELETE mCombinedCoverageImage;
	OGRE_DELETE mCombinedCoverageDataStream;
}

void ShaderPassCoverageBatch::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer)
{
	//	layer->createCoverageImage();
	//	layer->updateCoverageImage(geometry);
	addCoverage(layer->getCoverageImage(), mLayers.size(), 4);
	mLayers.push_back(layer);
	//	layer->destroyCoverageImage();
}

void ShaderPassCoverageBatch::addCoverage(Ogre::Image* coverage, unsigned int channel, unsigned short numberOfChannels)
{
	mCombinedCoverageDataStream->seek(0);
	unsigned char * coverageData = coverage->getData();
	unsigned char * combinedCoverageData = mCombinedCoverageDataStream->getPtr();
	combinedCoverageData += channel;
	for (unsigned int i = 0; i < coverage->getSize(); ++i) {
		*combinedCoverageData = *coverageData;
		combinedCoverageData += numberOfChannels;
		coverageData++;
	}
}

std::vector<const TerrainPageSurfaceLayer*>& ShaderPassCoverageBatch::getLayers()
{
	return mLayers;
}

void ShaderPassCoverageBatch::assignCombinedCoverageTexture()
{
	mCombinedCoverageDataStream->seek(0);
	mCombinedCoverageImage->loadDynamicImage(mCombinedCoverageDataStream->getPtr(), mShaderPass.getCoveragePixelWidth(), mShaderPass.getCoveragePixelWidth(), 1, Ogre::PF_B8G8R8A8);
	mCombinedCoverageTexture->loadImage(*mCombinedCoverageImage);

	Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(mCombinedCoverageTexture->getBuffer());
	///blit the whole image to the hardware buffer
	Ogre::PixelBox sourceBox(mCombinedCoverageImage->getPixelBox());
	hardwareBuffer->blitFromMemory(sourceBox);

}

Ogre::TexturePtr ShaderPassCoverageBatch::getCombinedCoverageTexture()
{
	return mCombinedCoverageTexture;
}
void ShaderPassCoverageBatch::finalize()
{
	///add our coverage textures first
	assignCombinedCoverageTexture();
	Ogre::TextureUnitState * coverageTUS = mShaderPass.mPass->createTextureUnitState();
	coverageTUS->setTextureScale(1, 1);
	coverageTUS->setTextureName(getCombinedCoverageTexture()->getName());
	coverageTUS->setTextureCoordSet(0);
	coverageTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	for (LayerStore::iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		const TerrainPageSurfaceLayer* layer(*I);
		///add the layer textures
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName());
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * diffuseTUS = mShaderPass.mPass->createTextureUnitState();
		//textureUnitState->setTextureScale(0.025, 0.025);
		diffuseTUS->setTextureName(layer->getDiffuseTextureName());
		diffuseTUS->setTextureCoordSet(0);
		diffuseTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}
}

}

}

}
