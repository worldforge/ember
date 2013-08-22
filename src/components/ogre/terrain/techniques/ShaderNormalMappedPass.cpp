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


#include "ShaderNormalMappedPass.h"
#include "ShaderNormalMappedPassCoverageBatch.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "framework/LoggingInstance.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreTextureUnitState.h>
#include <OgreGpuProgram.h>
#include <OgreShadowCameraSetupPSSM.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

ShaderNormalMappedPass::ShaderNormalMappedPass(Ogre::SceneManager& sceneManager, int coveragePixelWidth, const WFMath::Point<2>& position) :
	ShaderPass(sceneManager, coveragePixelWidth, position)
{
}



ShaderPassCoverageBatch* ShaderNormalMappedPass::createNewBatch()
{
	ShaderPassCoverageBatch* batch = new ShaderNormalMappedPassCoverageBatch(*this, getCoveragePixelWidth());
	return batch;
}

bool ShaderNormalMappedPass::hasRoomForLayer(const TerrainPageSurfaceLayer* layer)
{
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	int takenUnits = 1; // One is always used by normal texture
	if (mBaseLayer) {
		takenUnits += 2;
	}
	takenUnits += mLayers.size() * 2;
	takenUnits += 3; //shadow texture and two coverage textures
	return (numberOfTextureUnitsOnCard - takenUnits) >= 2;
}

bool ShaderNormalMappedPass::finalize(Ogre::Pass& pass, bool useShadows, const std::string shaderSuffix) const
{
	S_LOG_VERBOSE("Creating terrain material pass with: NormalMapping=" << true << " Shadows=" << useShadows << " Suffix=" << shaderSuffix);

	S_LOG_VERBOSE("Adding normal map texture unit state.");
	Ogre::TextureUnitState* normalMapTextureUnitState = pass.createTextureUnitState();

	// TODO SK: constant for name

	// Set up an alias for the normal texture. This way the terrain implementation can generate the normal texture at a later time and link it to this material.
	// With the Ogre Terrain Component, this is set up in OgreTerrainMaterialGeneratorEmber.cpp.
	normalMapTextureUnitState->setTextureNameAlias("EmberTerrain/normalMap");
	normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	if (useShadows) {
		for (unsigned int i = 0; i < mShadowLayers; ++i) {
			S_LOG_VERBOSE("Adding shadow layer.");
			Ogre::TextureUnitState* textureUnitState = pass.createTextureUnitState();

			textureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
			textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
			textureUnitState->setTextureBorderColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));
		}
	}

	// should we use a base pass?
	if (mBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " and normal map texture "<< mBaseLayer->getNormalTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");

		// add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState* textureUnitState = pass.createTextureUnitState();
		textureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

		Ogre::TextureUnitState * normalMapTextureUnitState = pass.createTextureUnitState();
		normalMapTextureUnitState->setTextureName(mBaseLayer->getNormalTextureName());
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	}

	int i = 0;
	// add our coverage textures first
	for (CoverageBatchStore::const_iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		ShaderPassCoverageBatch* batch = *I;
		batch->finalize(pass, getCombinedCoverageTexture(pass.getIndex(), i++));
	}

	//we provide different fragment programs for different amounts of textures used, so we need to determine which one to use.
	std::stringstream ss;
	ss << "SplattingFp/OffsetMapping/" << mLayers.size() << shaderSuffix;

	std::string fragmentProgramName(ss.str());

	pass.setMaxSimultaneousLights(3);
	try {
		S_LOG_VERBOSE("Using fragment program " << fragmentProgramName << " for terrain page.");
		pass.setFragmentProgram(fragmentProgramName);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program '" << fragmentProgramName << "'." << ex);
		return false;
	}
	if (!pass.hasFragmentProgram()) {
		return false;
	}
	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = pass.getFragmentProgramParameters();
		fpParams->setIgnoreMissingParams(true);
		fpParams->setNamedConstant("scales", mScales, (mLayers.size() - 1) / 4 + 1);

		if (useShadows) {
			Ogre::PSSMShadowCameraSetup* pssmSetup = static_cast<Ogre::PSSMShadowCameraSetup*>(mSceneManager.getShadowCameraSetup().get());
			if (pssmSetup) {
				Ogre::Vector4 splitPoints;
				Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();
				for (int i = 0; i < 3; i++) {
					splitPoints[i] = splitPointList[i];
				}

				fpParams->setNamedConstant("pssmSplitPoints", splitPoints);
			}

		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}

	// add vertex shader for fog
	std::string lightningVpProgram = "Lighting/NormalTexture/";
	if (useShadows) {
		lightningVpProgram += "ShadowVp";
	} else {
		lightningVpProgram += "SimpleVp";
	}

	if (mSceneManager.getFogMode() != Ogre::FOG_EXP2) {
		S_LOG_FAILURE("Fog mode is different, but using vertex program " << lightningVpProgram << " for terrain material pass.");
	}

	S_LOG_VERBOSE("Using vertex program " << lightningVpProgram << " for terrain material pass.");
	pass.setVertexProgram(lightningVpProgram);

	return true;
}


}

}

}
}
