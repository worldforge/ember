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

#include "ShaderPass.h"
#include "ShaderPassCoverageBatch.h"
#include "Shader.h"

#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "framework/LoggingInstance.h"

#include <OgreTexture.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>
#include <OgreRenderSystem.h>
#include <OgreShadowCameraSetupPSSM.h>

#include <algorithm>
#include <cmath>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

Ogre::TexturePtr ShaderPass::getCombinedCoverageTexture(size_t passIndex, size_t batchIndex) const
{
	// we need an unique name for our alpha texture
	std::stringstream combinedCoverageTextureNameSS;

	combinedCoverageTextureNameSS << "terrain_" << mPosition.x() << "_" << mPosition.y() << "_combinedCoverage_" << passIndex << "_" << batchIndex << "_" << mCoveragePixelWidth;
	const Ogre::String combinedCoverageName(combinedCoverageTextureNameSS.str());
	Ogre::TexturePtr combinedCoverageTexture;
	Ogre::TextureManager* textureMgr = Ogre::Root::getSingletonPtr()->getTextureManager();
	if (textureMgr->resourceExists(combinedCoverageName)) {
		S_LOG_VERBOSE("Using already created coverage texture " << combinedCoverageName);
		combinedCoverageTexture = static_cast<Ogre::TexturePtr>(textureMgr->getByName(combinedCoverageName));
		if(!combinedCoverageTexture->isLoaded()) {
			combinedCoverageTexture->createInternalResources();
		}
		return combinedCoverageTexture;
	}
	S_LOG_VERBOSE("Creating new coverage texture " << combinedCoverageName << " with size " << mCoveragePixelWidth);
	int flags = Ogre::TU_DYNAMIC_WRITE_ONLY;
	// automipmapping seems to cause some trouble on Windows, at least in OpenGL on Nvidia cards
	// Thus we'll disable it. The performance impact shouldn't be significant.
#ifndef _WIN32
	flags |= Ogre::TU_AUTOMIPMAP;
#endif // ifndef _WIN32
	combinedCoverageTexture = textureMgr->createManual(combinedCoverageName, "General", Ogre::TEX_TYPE_2D, mCoveragePixelWidth, mCoveragePixelWidth, textureMgr->getDefaultNumMipmaps(), Ogre::PF_B8G8R8A8, flags);
	combinedCoverageTexture->createInternalResources();
	return combinedCoverageTexture;
}

ShaderPass::ShaderPass(Ogre::SceneManager& sceneManager, int coveragePixelWidth, const WFMath::Point<2>& position, bool useNormalMapping) :
		mBaseLayer(0), mSceneManager(sceneManager), mCoveragePixelWidth(coveragePixelWidth), mPosition(position), mShadowLayers(0), mUseNormalMapping(useNormalMapping)
{
	for (int i = 0; i < 16; i++) {
		mScales[i] = 0.0;
	}
}

ShaderPass::~ShaderPass()
{
	for (CoverageBatchStore::iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		delete *I;
	}
}

void ShaderPass::setBaseLayer(const TerrainPageSurfaceLayer* layer)
{
	mLayers.push_back(layer);
	mBaseLayer = layer;
	mScales[0] = layer->getScale();
}

ShaderPassCoverageBatch* ShaderPass::getCurrentBatch()
{
	CoverageBatchStore::reverse_iterator I = mCoverageBatches.rbegin();
	if (!mCoverageBatches.size() || (*I)->getLayers().size() >= 4) {
		ShaderPassCoverageBatch* batch = createNewBatch();
		mCoverageBatches.push_back(batch);
		return batch;
	} else {
		return *I;
	}
}

ShaderPassCoverageBatch* ShaderPass::createNewBatch()
{
	ShaderPassCoverageBatch* batch = new ShaderPassCoverageBatch(*this, getCoveragePixelWidth(), mUseNormalMapping);
	return batch;
}

void ShaderPass::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer)
{
	getCurrentBatch()->addLayer(geometry, layer);

	mScales[mLayers.size()] = layer->getScale();
	mLayers.push_back(layer);
}

LayerStore& ShaderPass::getLayers()
{
	return mLayers;
}

bool ShaderPass::finalize(Ogre::Pass& pass, bool useShadows, const std::string shaderSuffix) const
{
	S_LOG_VERBOSE("Creating terrain material pass with: NormalMapping=" << mUseNormalMapping << " Shadows=" << useShadows << " Suffix=" << shaderSuffix);
	if (shaderSuffix != "/NoLighting") {
		S_LOG_VERBOSE("Adding normal map texture unit state.");
		Ogre::TextureUnitState* normalMapTextureUnitState = pass.createTextureUnitState();

		// Set up an alias for the normal texture. This way the terrain implementation can generate the normal texture at a later time and link it to this material.
		// With the Ogre Terrain Component, this is set up in OgreTerrainMaterialGeneratorEmber.cpp.
		normalMapTextureUnitState->setTextureNameAlias(Techniques::Shader::NORMAL_TEXTURE_ALIAS);
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	}

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
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");
		// add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState* textureUnitState = pass.createTextureUnitState();
		textureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

		if (mUseNormalMapping) {
			Ogre::TextureUnitState * normalMapTextureUnitState = pass.createTextureUnitState();
			std::string normalTextureName = mBaseLayer->getNormalTextureName();
			if (normalTextureName.empty()) {
				normalTextureName = "3d_objects/primitives/textures/onepixel/N.png";
			}
			normalMapTextureUnitState->setTextureName(normalTextureName);
			textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
	}

	int i = 0;
	// add our coverage textures first
	for (CoverageBatchStore::const_iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		ShaderPassCoverageBatch* batch = *I;
		batch->finalize(pass, getCombinedCoverageTexture(pass.getIndex(), i++));
	}

	//we provide different fragment programs for different amounts of textures used, so we need to determine which one to use.
	std::stringstream ss;
	ss << "SplattingFp/";
	if (mUseNormalMapping) {
		ss << "OffsetMapping/";
	}
	ss << mLayers.size() << shaderSuffix;
	std::string fragmentProgramName(ss.str());

	if (shaderSuffix == "NoLighting") {
		pass.setMaxSimultaneousLights(0);
	} else {
		pass.setMaxSimultaneousLights(3);
	}

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

bool ShaderPass::hasRoomForLayer(const TerrainPageSurfaceLayer* layer)
{
	Ogre::ushort numberOfTextureUnitsOnCard = std::min(static_cast<Ogre::ushort>(OGRE_MAX_TEXTURE_LAYERS), Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits());

	// Include the layer to be added
	int numLayers = mLayers.size() + 1;

	int takenUnits = 1;			 // One unit is always used by the global normal texture
	takenUnits += mShadowLayers; // Shadow textures
	// A coverage texture for every 4 layers
	// Make sure to always have 1 for 1 layer, 2 for 5 layers etc.
	takenUnits += static_cast<int>(std::ceil(numLayers / 4.0f));

	// The base layer is not included in the layer list
	if (mBaseLayer) {
		takenUnits += 1;
		if (mUseNormalMapping) {
			takenUnits += 1;
		}
	}
	takenUnits += mLayers.size();
	if (mUseNormalMapping) {
		takenUnits += mLayers.size();
	}

	return (numberOfTextureUnitsOnCard - takenUnits) >= 0;
}

void ShaderPass::addShadowLayer(const TerrainPageShadow* terrainPageShadow)
{
	mShadowLayers++;
}

unsigned int ShaderPass::getCoveragePixelWidth() const
{
	return mCoveragePixelWidth;
}

}

}

}
}
