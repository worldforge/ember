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

#include "ShaderPass.h"
#include "ShaderPassBlendMapBatch.h"
#include "Shader.h"

#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"

#include <OgreRoot.h>
#include <OgreShadowCameraSetupPSSM.h>
#include <OgreTextureManager.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Terrain {

namespace Techniques {

Ogre::TexturePtr ShaderPass::getCombinedBlendMapTexture(size_t passIndex, size_t batchIndex, std::set<std::string>& managedTextures) const {
	// we need an unique name for our alpha texture
	std::stringstream combinedBlendMapTextureNameSS;

	combinedBlendMapTextureNameSS << "terrain_" << mPosition.first << "_" << mPosition.second << "_combinedBlendMap_" << passIndex << "_" << batchIndex << "_" << mBlendMapPixelWidth;
	const Ogre::String combinedBlendMapName(combinedBlendMapTextureNameSS.str());
	Ogre::TexturePtr combinedBlendMapTexture;
	Ogre::TextureManager* textureMgr = Ogre::Root::getSingletonPtr()->getTextureManager();
	if (textureMgr->resourceExists(combinedBlendMapName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
		S_LOG_VERBOSE("Using already created blendMap texture " << combinedBlendMapName);
		combinedBlendMapTexture = static_cast<Ogre::TexturePtr>(textureMgr->getByName(combinedBlendMapName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME));
		if (!combinedBlendMapTexture->isLoaded()) {
			combinedBlendMapTexture->createInternalResources();
		}
		return combinedBlendMapTexture;
	}
	S_LOG_VERBOSE("Creating new blendMap texture " << combinedBlendMapName << " with size " << mBlendMapPixelWidth);
	int flags = Ogre::TU_DYNAMIC_WRITE_ONLY;
	// automipmapping seems to cause some trouble on Windows, at least in OpenGL on Nvidia cards
	// Thus we'll disable it. The performance impact shouldn't be significant.
#ifndef _WIN32
	flags |= Ogre::TU_AUTOMIPMAP;
#endif // ifndef _WIN32
	combinedBlendMapTexture = textureMgr->createManual(combinedBlendMapName, "General", Ogre::TEX_TYPE_2D, mBlendMapPixelWidth, mBlendMapPixelWidth, (int) textureMgr->getDefaultNumMipmaps(), Ogre::PF_B8G8R8A8, flags);
	managedTextures.insert(combinedBlendMapName);
	combinedBlendMapTexture->createInternalResources();
	return combinedBlendMapTexture;
}

ShaderPass::ShaderPass(Ogre::SceneManager& sceneManager, int blendMapPixelWidth, TerrainIndex position, bool useNormalMapping) :
		mScales{},
		mBaseLayer(nullptr),
		mSceneManager(sceneManager),
		mBlendMapPixelWidth(blendMapPixelWidth),
		mPosition(std::move(position)),
		mShadowLayers(0),
		mUseNormalMapping(useNormalMapping) {
	for (float& scale: mScales) {
		scale = 0.0;
	}
}

ShaderPass::~ShaderPass() = default;

void ShaderPass::setBaseLayer(const TerrainPageSurfaceLayer* layer) {
	mLayers.push_back(layer);
	mBaseLayer = layer;
	mScales[0] = layer->getScale();
}

ShaderPassBlendMapBatch* ShaderPass::getCurrentBatch() {
	auto I = mBlendMapBatches.rbegin();
	if (mBlendMapBatches.empty() || (*I)->getLayers().size() >= 4) {
		mBlendMapBatches.emplace_back(createNewBatch());
		return mBlendMapBatches.back().get();
	} else {
		return I->get();
	}
}

std::unique_ptr<ShaderPassBlendMapBatch> ShaderPass::createNewBatch() {
	return std::make_unique<ShaderPassBlendMapBatch>(*this, getBlendMapPixelWidth());
}

void ShaderPass::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer) {
	getCurrentBatch()->addLayer(geometry, layer);

	mScales[mLayers.size()] = layer->getScale();
	mLayers.push_back(layer);
}

bool ShaderPass::finalize(Ogre::Pass& pass, std::set<std::string>& managedTextures, bool useShadows, const std::string& shaderSuffix) const {
	S_LOG_VERBOSE("Creating terrain material pass with: NormalMapping=" << mUseNormalMapping << " Shadows=" << useShadows << " Suffix=" << shaderSuffix);
	if (shaderSuffix != "/NoLighting") {
		S_LOG_VERBOSE("Adding normal map texture unit state.");
		Ogre::TextureUnitState* normalMapTextureUnitState = pass.createTextureUnitState();

		// Set up an alias for the normal texture. This way the terrain implementation can generate the normal texture at a later time and link it to this material.
		// With the Ogre Terrain Component, this is set up in OgreTerrainMaterialGeneratorEmber.cpp.
		normalMapTextureUnitState->setName(Techniques::Shader::NORMAL_TEXTURE_ALIAS);
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	}

	if (useShadows) {
		for (int i = 0; i < mShadowLayers; ++i) {
			Ogre::TextureUnitState* textureUnitState = pass.createTextureUnitState();

			textureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
			textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
			textureUnitState->setTextureBorderColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));
		}
		S_LOG_VERBOSE("Added " << mShadowLayers << " shadow layers.");
	}

	bool useBaseLayer = mBaseLayer && !mBaseLayer->getDiffuseTextureName().empty();

	// should we use a base pass?
	if (useBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");
		// add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState* textureUnitState = pass.createTextureUnitState();
		textureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

		if (mUseNormalMapping) {
			Ogre::TextureUnitState* normalMapTextureUnitState = pass.createTextureUnitState();
			std::string normalTextureName = mBaseLayer->getNormalTextureName();
			if (normalTextureName.empty()) {
				//Since the shader always expects a normal texture we need to supply a dummy one if no specific one exists.
				normalTextureName = "dynamic/onepixel";
			}
			normalMapTextureUnitState->setTextureName(normalTextureName);
			textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
	}

	size_t i = 0;
	// add our blendMap textures first
	for (auto& batch: mBlendMapBatches) {
		batch->finalize(pass, getCombinedBlendMapTexture(pass.getIndex(), i++, managedTextures), mUseNormalMapping);
	}

	//we provide different fragment programs for different amounts of textures used, so we need to determine which one to use.
	std::stringstream ss;
	ss << "SplattingFp/";
	if (mUseNormalMapping) {
		ss << "OffsetMapping/";
	}
	ss << mLayers.size();
	//If no base layer is used we need to use a variant of the shader adapted to this.
	//This is done by adding the "NoBaseLayer" segment.
	if (!useBaseLayer) {
		ss << "/NoBaseLayer";
		pass.setSceneBlending(Ogre::SBF_ONE, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);
	}
	ss << shaderSuffix;
	std::string fragmentProgramName(ss.str());

	if (shaderSuffix == "/NoLighting") {
		pass.setMaxSimultaneousLights(0);
	} else {
		pass.setMaxSimultaneousLights(3);
	}

	//If there are no valid layers we'll just use a simple white colour. This should normally not happen.
	if (mLayers.empty()) {
		fragmentProgramName = "SimpleWhiteFp";
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
			auto* pssmSetup = dynamic_cast<Ogre::PSSMShadowCameraSetup*>(mSceneManager.getShadowCameraSetup().get());
			if (pssmSetup) {
				Ogre::Vector4 splitPoints;
				Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();
				for (int splitPointIndex = 0; splitPointIndex < 3; splitPointIndex++) {
					splitPoints[splitPointIndex] = splitPointList[splitPointIndex];
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

bool ShaderPass::hasRoomForLayer(const TerrainPageSurfaceLayer* layer) {
	//TODO: calculate this once
	Ogre::ushort numberOfTextureUnitsOnCard = std::min(static_cast<Ogre::ushort>(OGRE_MAX_TEXTURE_LAYERS), Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits());

	//We'll project the number of taken units if we should add another pass.
	//Later on we'll compare this with the actual number it texture units available.
	size_t projectedTakenUnits = 1; // One unit is always used by the global normal texture
	projectedTakenUnits += mShadowLayers; // Shadow textures
	// A blend map texture for every 4 layers
	// Make sure to always have 1 for 1 layer, 2 for 5 layers etc.
	projectedTakenUnits += mBlendMapBatches.size();
	if (!mBlendMapBatches.empty() && mBlendMapBatches.back()->getLayers().size() == 4) {
		//If the last batch if full we should need to create a new one; thus we'll increase the number of units
		projectedTakenUnits++;
	}

	projectedTakenUnits += mLayers.size();
	if (mUseNormalMapping) {
		projectedTakenUnits += mLayers.size();
	}

	projectedTakenUnits++;
	if (mUseNormalMapping) {
		projectedTakenUnits++;
	}


	return numberOfTextureUnitsOnCard > projectedTakenUnits;
}

void ShaderPass::addShadowLayer() {
	mShadowLayers++;
}

unsigned int ShaderPass::getBlendMapPixelWidth() const {
	return mBlendMapPixelWidth;
}

}

}

}
}
