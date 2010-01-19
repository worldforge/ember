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
#include "components/ogre/terrain/Image.h"

#include "framework/TimedLog.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

ShaderPassCoverageBatch::ShaderPassCoverageBatch(ShaderPass& shaderPass, unsigned int imageSize) :
	mShaderPass(shaderPass), mCombinedCoverageImage(new Image::ImageBuffer(imageSize, 4))
{
	///reset the coverage image
	mCombinedCoverageImage.reset();
}

ShaderPassCoverageBatch::~ShaderPassCoverageBatch()
{
}

void ShaderPassCoverageBatch::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer)
{
	addCoverage(layer, mLayers.size());
	mLayers.push_back(layer);
}

void ShaderPassCoverageBatch::addCoverage(const TerrainPageSurfaceLayer* layer, unsigned int channel)
{
	layer->fillImage(mCombinedCoverageImage, channel);
	mSyncedTextures.clear();
}

std::vector<const TerrainPageSurfaceLayer*>& ShaderPassCoverageBatch::getLayers()
{
	return mLayers;
}

void ShaderPassCoverageBatch::assignCombinedCoverageTexture(Ogre::TexturePtr texture)
{
	if (std::find(mSyncedTextures.begin(), mSyncedTextures.end(), texture->getName()) == mSyncedTextures.end()) {
		Ember::TimedLog log("ShaderPassCoverageBatch::assignCombinedCoverageTexture");
		Ogre::Image image;

		image.loadDynamicImage(mCombinedCoverageImage.getData(), mCombinedCoverageImage.getResolution(), mCombinedCoverageImage.getResolution(), 1, Ogre::PF_B8G8R8A8);
		texture->loadImage(image);
		log.report("image loaded");
		///blit the whole image to the hardware buffer
		Ogre::PixelBox sourceBox(image.getPixelBox());
		//blit for each mipmap
		for (int i = 0; i <= texture->getNumMipmaps(); ++i) {
			Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, i));
			hardwareBuffer->blitFromMemory(sourceBox);
		}
		mSyncedTextures.push_back(texture->getName());
	}
}

void ShaderPassCoverageBatch::finalize(Ogre::Pass& pass, Ogre::TexturePtr texture)
{
	///add our coverage textures first
	assignCombinedCoverageTexture(texture);
	Ogre::TextureUnitState * coverageTUS = pass.createTextureUnitState();
	coverageTUS->setTextureScale(1, 1);
	coverageTUS->setTextureName(texture->getName());
	coverageTUS->setTextureCoordSet(0);
	coverageTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	for (LayerStore::iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		const TerrainPageSurfaceLayer* layer(*I);
		///add the layer textures
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName());
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * diffuseTUS = pass.createTextureUnitState();
		//textureUnitState->setTextureScale(0.025, 0.025);
		diffuseTUS->setTextureName(layer->getDiffuseTextureName());
		diffuseTUS->setTextureCoordSet(0);
		diffuseTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}
}

}

}

}
