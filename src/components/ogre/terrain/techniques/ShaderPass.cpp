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

#include "components/ogre/terrain/TerrainPage.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "components/ogre/EmberOgre.h"
#include "framework/LoggingInstance.h"

#include <OgreTexture.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>
#include <OgrePass.h>
#include <OgreRenderSystem.h>
#include <OgreShadowCameraSetupPSSM.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

Ogre::TexturePtr ShaderPass::getCombinedCoverageTexture(size_t passIndex, size_t batchIndex) const
{
	///we need an unique name for our alpha texture
	std::stringstream combinedCoverageTextureNameSS;
	combinedCoverageTextureNameSS << "terrain_" << mPage.getWFPosition().x() << "_" << mPage.getWFPosition().y() << "_combinedCoverage_" << passIndex << "_" << batchIndex;
	const Ogre::String combinedCoverageName(combinedCoverageTextureNameSS.str());
	Ogre::TexturePtr combinedCoverageTexture;
	if (Ogre::Root::getSingletonPtr()->getTextureManager()->resourceExists(combinedCoverageName)) {
		S_LOG_VERBOSE("Using already created texture " << combinedCoverageName);
		combinedCoverageTexture = static_cast<Ogre::TexturePtr> (Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(combinedCoverageName));
	} else {
		S_LOG_VERBOSE("Creating new texture " << combinedCoverageName);
		combinedCoverageTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(combinedCoverageName, "General", Ogre::TEX_TYPE_2D, mPage.getAlphaTextureSize(), mPage.getAlphaTextureSize(), 1, Ogre::PF_B8G8R8A8, Ogre::TU_DYNAMIC_WRITE_ONLY);
	}
	return combinedCoverageTexture;
}

ShaderPass::ShaderPass(const TerrainPage& page) :
	mBaseLayer(0), mPage(page), mShadowLayers(0)
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
	ShaderPassCoverageBatch* batch = new ShaderPassCoverageBatch(*this, getCoveragePixelWidth());
	return batch;
}

void ShaderPass::addLayer(const TerrainPageGeometry& geometry, const TerrainPageSurfaceLayer* layer)
{
	// 	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	// // 	if (mCurrentLayerIndex < std::min<unsigned short>(numberOfTextureUnitsOnCard - 1, 4)) {
	// 		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName() << " and scale " << layer->getScale() << " at index "<< (mCurrentLayerIndex + 1) <<" (" << numberOfTextureUnitsOnCard << " texture units supported)");
	// 		Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
	// // 		textureUnitState->setTextureScale(0.025, 0.025);
	// 		textureUnitState->setTextureName(layer->getDiffuseTextureName());
	// 		textureUnitState->setTextureCoordSet(0);
	// 		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

	getCurrentBatch()->addLayer(geometry, layer);

	// 		mCurrentLayerIndex++;
	mScales[mLayers.size()] = layer->getScale();
	mLayers.push_back(layer);
	// 	}
}

LayerStore& ShaderPass::getLayers()
{
	return mLayers;
}

bool ShaderPass::finalize(Ogre::Pass& pass, bool useShadows, const std::string shaderSuffix) const
{
	for (int i = 0; i < mShadowLayers; ++i) {
		S_LOG_VERBOSE("Adding shadow layer.");
		Ogre::TextureUnitState * textureUnitState = pass.createTextureUnitState();

		textureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
		textureUnitState->setTextureBorderColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));
	}

	///should we use a base pass?
	if (mBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * textureUnitState = pass.createTextureUnitState();
		textureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		textureUnitState->setTextureCoordSet(0);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}

	int i = 0;
	///add our coverage textures first
	for (CoverageBatchStore::const_iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		ShaderPassCoverageBatch* batch = *I;
		batch->finalize(pass, getCombinedCoverageTexture(pass.getIndex(), i++));
	}

	///we provide different fragment programs for different amounts of textures used, so we need to determine which one to use. They all have the form of "splatting_fragment_*"
	std::stringstream ss;
	ss << "SplattingFp/" << mLayers.size() << shaderSuffix;

	std::string fragmentProgramName(ss.str());

	//Disable lightning here since we're forcing lights to be set for the shaders in PagingLandScapeRenderable::getLights
	pass.setLightingEnabled(false);

	pass.setMaxSimultaneousLights(3);
	// 	pass.setFog(true, Ogre::FOG_NONE);


	///add fragment shader for splatting
	// 	pass.setFragmentProgram("splatting_fragment_dynamic");
	try {
		S_LOG_VERBOSE("Using fragment program " << fragmentProgramName << " for terrain page.");
		pass.setFragmentProgram(fragmentProgramName);
		// 		pass.setFragmentProgram("splatting_fragment_dynamic");
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
		/*
		 fpParams->setNamedAutoConstant("iFogColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		 fpParams->setNamedConstant("iNumberOfLayers", (float)mLayers.size()); ///this will only apply to the splatting_fragment_dynamic material
		 */
		///set how much the texture should tile
		//fpParams->setNamedConstant("scales", mScales, 3); //4*4=16
		fpParams->setNamedConstant("scales", mScales, (mLayers.size() - 1) / 4 + 1);

		if (useShadows) {
			Ogre::PSSMShadowCameraSetup* pssmSetup = static_cast<Ogre::PSSMShadowCameraSetup*> (EmberOgre::getSingleton().getSceneManager()->getShadowCameraSetup().get());
			if (pssmSetup) {
				Ogre::Vector4 splitPoints;
				Ogre::PSSMShadowCameraSetup::SplitPointList splitPointList = pssmSetup->getSplitPoints();
				for (int i = 0; i < 3; i++) {
					splitPoints[i] = splitPointList[i];
				}

				fpParams->setNamedConstant("pssmSplitPoints", splitPoints);
			}

			//		fpParams->setNamedConstant("shadowMap0", 0);
			//		fpParams->setNamedConstant("shadowMap1", 1);
			//		fpParams->setNamedConstant("shadowMap2", 2);
		}
		/*
		 fpParams->setNamedAutoConstant("iLightAmbient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
		 fpParams->setNamedAutoConstant("iLightDiffuse", Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR_ARRAY, 3);
		 fpParams->setNamedAutoConstant("iLightAttenuation", Ogre::GpuProgramParameters::ACT_LIGHT_ATTENUATION_ARRAY, 3);
		 fpParams->setNamedAutoConstant("iLightPosition", Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE_ARRAY, 3);
		 */
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}

	///add vertex shader for fog
	std::string lightningVpProgram;
	if (EmberOgre::getSingleton().getSceneManager()->getFogMode() == Ogre::FOG_EXP2) {
		if (useShadows) {
			lightningVpProgram = "Lighting/ShadowVp";
		} else {
			lightningVpProgram = "Lighting/SimpleVp";
		}
		S_LOG_VERBOSE("Using vertex program " << "Lighting/ShadowVp" << " for terrain page.");
	} else {
		if (useShadows) {
			lightningVpProgram = "Lighting/ShadowVp";
		} else {
			lightningVpProgram = "Lighting/SimpleVp";
		}
		S_LOG_FAILURE("Fog mode is different, but using vertex program " << lightningVpProgram << " for terrain page.");
	}
	pass.setVertexProgram(lightningVpProgram);

	try {
		if (!pass.hasVertexProgram()) {
			return false;
		}
		//Not available in Ogre 1.7
		//pass.getVertexProgram()->setSurfaceAndPassLightStates(true);
		Ogre::GpuProgramParametersSharedPtr fpParams = pass.getVertexProgramParameters();
		fpParams->setIgnoreMissingParams(true);
		fpParams->setNamedAutoConstant("iFogParams", Ogre::GpuProgramParameters::ACT_FOG_PARAMS);
		fpParams->setNamedAutoConstant("iWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}
	return true;
}

bool ShaderPass::hasRoomForLayer(const TerrainPageSurfaceLayer* layer)
{

	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	int takenUnits = 0;
	if (mBaseLayer) {
		takenUnits += 1;
	}
	takenUnits += mLayers.size() * 1;
	takenUnits += 3; //shadow texture and two coverage textures
	return (numberOfTextureUnitsOnCard - takenUnits) >= 1;
}

void ShaderPass::addShadowLayer(const TerrainPageShadow* terrainPageShadow)
{
	mShadowLayers++;
	//	S_LOG_VERBOSE("Adding shadow layer.");
	//	Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
	//
	//	//textureUnitState->setTextureScale(0.025, 0.025);
	//	/*
	//	 textureUnitState->setTextureName(terrainPageShadow->getTexture()->getName());
	//	 textureUnitState->setTextureCoordSet(0);
	//	 textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	//	 textureUnitState->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
	//	 textureUnitState->setTextureAnisotropy(2);
	//	 */
	//
	//	textureUnitState->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
	//	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
	//	textureUnitState->setTextureBorderColour(Ogre::ColourValue(1.0, 1.0, 1.0, 1.0));
}

unsigned int ShaderPass::getCoveragePixelWidth() const
{
	return mPage.getAlphaTextureSize();
}

}

}

}
