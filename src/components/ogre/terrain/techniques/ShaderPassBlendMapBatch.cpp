/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ShaderPassBlendMapBatch.h"
#include "ShaderPass.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"

#include "framework/TimedLog.h"

#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

ShaderPassBlendMapBatch::ShaderPassBlendMapBatch(ShaderPass& shaderPass, unsigned int imageSize) :
	mShaderPass(shaderPass), mCombinedBlendMapImage(new Image::ImageBuffer(imageSize, 4))
{
	//reset the blendMap image
	mCombinedBlendMapImage.reset();
}

ShaderPassBlendMapBatch::~ShaderPassBlendMapBatch()
{
}

void ShaderPassBlendMapBatch::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer)
{
	addBlendMap(geometry, layer, mLayers.size());
	mLayers.push_back(layer);
}

void ShaderPassBlendMapBatch::addBlendMap(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer, unsigned int channel)
{
	layer->fillImage(geometry, mCombinedBlendMapImage, channel);
	mSyncedTextures.clear();
}

std::vector<const TerrainPageSurfaceLayer*>& ShaderPassBlendMapBatch::getLayers()
{
	return mLayers;
}

void ShaderPassBlendMapBatch::assignCombinedBlendMapTexture(Ogre::TexturePtr texture)
{
	if (std::find(mSyncedTextures.begin(), mSyncedTextures.end(), texture->getName()) == mSyncedTextures.end()) {
		TimedLog log("ShaderPassBlendMapBatch::assignCombinedBlendMapTexture", true);

		//blit the whole image to the hardware buffer
		Ogre::PixelBox sourceBox(mCombinedBlendMapImage.getResolution(), mCombinedBlendMapImage.getResolution(), 1, Ogre::PF_B8G8R8A8, mCombinedBlendMapImage.getData());

		if ((texture->getUsage() & Ogre::TU_AUTOMIPMAP) && texture->getMipmapsHardwareGenerated()) {
			//No need to blit for all mipmaps as they will be generated.
			Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, 0));
			hardwareBuffer->blitFromMemory(sourceBox);
		} else {
			for (size_t i = 0; i <= texture->getNumMipmaps(); ++i)
			{
				Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, i));
				hardwareBuffer->blitFromMemory(sourceBox);
			}
		}

		mSyncedTextures.push_back(texture->getName());
	}
}

void ShaderPassBlendMapBatch::finalize(Ogre::Pass& pass, Ogre::TexturePtr texture, bool useNormalMapping)
{
	//add our blend map textures first
	assignCombinedBlendMapTexture(texture);
	Ogre::TextureUnitState * blendMapTUS = pass.createTextureUnitState();
	blendMapTUS->setTextureScale(1, 1);
	blendMapTUS->setTextureName(texture->getName());
	blendMapTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	for (LayerStore::iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		const TerrainPageSurfaceLayer* layer(*I);
		//add the layer textures
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName());
		Ogre::TextureUnitState * diffuseTUS = pass.createTextureUnitState();
		//textureUnitState->setTextureScale(0.025, 0.025);
		diffuseTUS->setTextureName(layer->getDiffuseTextureName());
		diffuseTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

		if (useNormalMapping) {
			Ogre::TextureUnitState * normalMapTextureUnitState = pass.createTextureUnitState();
			std::string normalTextureName = layer->getNormalTextureName();
			if (normalTextureName.empty()) {
				normalTextureName = "dynamic/onepixel";
			}
			normalMapTextureUnitState->setTextureName(normalTextureName);
			normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
	}
}

}

}

}
}
