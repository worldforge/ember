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


#include "ShaderNormalMappedPassCoverageBatch.h"
#include "ShaderNormalMappedPass.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "framework/LoggingInstance.h"
#include <OgrePass.h>
#include <OgreTexture.h>
#include <OgreTextureUnitState.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

ShaderNormalMappedPassCoverageBatch::ShaderNormalMappedPassCoverageBatch(ShaderPass& shaderPass, Ogre::TexturePtr combinedCoverageTexture) :
	ShaderPassCoverageBatch(shaderPass, combinedCoverageTexture)
{
}

void ShaderNormalMappedPassCoverageBatch::finalize()
{
	///add our coverage textures first
	assignCombinedCoverageTexture();
	Ogre::TextureUnitState * coverageTUS = mShaderPass.getPass()->createTextureUnitState();
	coverageTUS->setTextureScale(1, 1);
	coverageTUS->setTextureName(getCombinedCoverageTexture()->getName());
	coverageTUS->setTextureCoordSet(0);
	coverageTUS->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	for (LayerStore::iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		const TerrainPageSurfaceLayer* layer(*I);
		///add the layer textures
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName() << " and normal map texture "<< layer->getNormalTextureName() );
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * diffuseTextureUnitState = mShaderPass.getPass()->createTextureUnitState();
		//textureUnitState->setTextureScale(0.025, 0.025);
		diffuseTextureUnitState->setTextureName(layer->getDiffuseTextureName());
		diffuseTextureUnitState->setTextureCoordSet(0);
		diffuseTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		Ogre::TextureUnitState * normalMapTextureUnitState = mShaderPass.getPass()->createTextureUnitState();
		normalMapTextureUnitState->setTextureName(layer->getNormalTextureName());
		normalMapTextureUnitState->setTextureCoordSet(0);
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}
}

}

}

}
