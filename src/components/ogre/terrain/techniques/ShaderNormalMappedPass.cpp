/*
 * ShaderNormalMappedPass.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#include "ShaderNormalMappedPass.h"
#include "ShaderNormalMappedPassCoverageBatch.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "components/ogre/EmberOgre.h"
#include "framework/LoggingInstance.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreTextureUnitState.h>
#include <OgreGpuProgram.h>

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

ShaderNormalMappedPass::ShaderNormalMappedPass(Ogre::Pass* pass, const TerrainPage& page) :
	ShaderPass(pass, page)
{
}

ShaderPassCoverageBatch* ShaderNormalMappedPass::createNewBatch()
{
	ShaderPassCoverageBatch* batch = new ShaderNormalMappedPassCoverageBatch(*this, getCombinedCoverageTexture(mPass->getIndex(), mCoverageBatches.size()));
	return batch;
}

bool ShaderNormalMappedPass::hasRoomForLayer(const TerrainPageSurfaceLayer* layer)
{

	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	int takenUnits = 0;
	if (mBaseLayer) {
		takenUnits += 2;
	}
	takenUnits += mLayers.size() * 2;
	takenUnits += 3; //shadow texture and two coverage textures
	return (numberOfTextureUnitsOnCard - takenUnits) >= 2;
}

bool ShaderNormalMappedPass::finalize()
{
	//TODO: add shadow here

	///should we use a base pass?
	if (mBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " and normal map texture "<< mBaseLayer->getNormalTextureName() <<" (" << numberOfTextureUnitsOnCard << " texture units supported)");
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * diffuseTextureUnitState = mPass->createTextureUnitState();
		diffuseTextureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		diffuseTextureUnitState->setTextureCoordSet(0);
		diffuseTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		Ogre::TextureUnitState * normalMapTextureUnitState = mPass->createTextureUnitState();
		normalMapTextureUnitState->setTextureName(mBaseLayer->getNormalTextureName());
		normalMapTextureUnitState->setTextureCoordSet(0);
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	}

	///add our coverage textures first
	for (CoverageBatchStore::iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		ShaderPassCoverageBatch* batch = *I;
		batch->finalize();
	}

	std::stringstream ss;
	ss << "splatting_fragment_normalmapped_" << mLayers.size();
	std::string fragmentProgramName(ss.str());

	mPass->setLightingEnabled(false);
	// 	mPass->setFog(true, Ogre::FOG_NONE);

	///add fragment shader for splatting
	mPass->setFragmentProgram("splatting_fragment_normalmapped_dynamic");
	// 	mPass->setFragmentProgram(fragmentProgramName);
	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getFragmentProgramParameters();
		fpParams->setNamedAutoConstant("iLightDiffuse", Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR);
		fpParams->setNamedAutoConstant("iLightAmbient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
		fpParams->setNamedAutoConstant("iFogColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		float theValues[4] = { 0.04, -0.02, 1, 0 };
		fpParams->setNamedConstant("scaleBias", theValues, 4); //4*4=16
		fpParams->setNamedConstant("iNumberOfLayers", (float)mLayers.size()); //4*4=16
		///set how much the texture should tile
		fpParams->setNamedConstant("iScales", mScales, 4); //4*4=16
	} catch (const Ogre::Exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}

	///add vertex shader for fog
	if (EmberOgre::getSingleton().getSceneManager()->getFogMode() == Ogre::FOG_EXP2) {
		mPass->setVertexProgram("splatting_vertex_normalmapped_exp2");
	} else {
		mPass->setVertexProgram("splatting_vertex_normalmapped");
	}

	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getVertexProgramParameters();
		fpParams->setNamedAutoConstant("lightPosition", Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
		fpParams->setNamedAutoConstant("eyePosition", Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
		fpParams->setNamedAutoConstant("iFogParams", Ogre::GpuProgramParameters::ACT_FOG_PARAMS);
		fpParams->setNamedAutoConstant("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		fpParams->setNamedAutoConstant("iLightAmbient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
		fpParams->setNamedAutoConstant("iLightDiffuse", Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR);
	} catch (const Ogre::Exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}
	return true;
}


}

}

}
