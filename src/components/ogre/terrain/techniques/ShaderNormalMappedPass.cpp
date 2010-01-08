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

ShaderNormalMappedPass::ShaderNormalMappedPass(const TerrainPage& page) :
	ShaderPass(page)
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
	int takenUnits = 0;
	if (mBaseLayer) {
		takenUnits += 2;
	}
	takenUnits += mLayers.size() * 2;
	takenUnits += 3; //shadow texture and two coverage textures
	return (numberOfTextureUnitsOnCard - takenUnits) >= 2;
}

bool ShaderNormalMappedPass::finalize(Ogre::Pass& pass, bool useShadows, const std::string shaderSuffix) const
{
	//TODO: add shadow here

	///should we use a base pass?
	if (mBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " and normal map texture "<< mBaseLayer->getNormalTextureName() <<" (" << numberOfTextureUnitsOnCard << " texture units supported)");
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * diffuseTextureUnitState = pass.createTextureUnitState();
		diffuseTextureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		diffuseTextureUnitState->setTextureCoordSet(0);
		diffuseTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		Ogre::TextureUnitState * normalMapTextureUnitState = pass.createTextureUnitState();
		normalMapTextureUnitState->setTextureName(mBaseLayer->getNormalTextureName());
		normalMapTextureUnitState->setTextureCoordSet(0);
		normalMapTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	}

	int i = 0;
	///add our coverage textures first
	for (CoverageBatchStore::const_iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		ShaderPassCoverageBatch* batch = *I;
		batch->finalize(pass, getCombinedCoverageTexture(pass.getIndex(), i++));
	}

	std::stringstream ss;
	ss << "splatting_fragment_normalmapped_" << mLayers.size();
	std::string fragmentProgramName(ss.str());

	pass.setLightingEnabled(false);
	// 	pass.setFog(true, Ogre::FOG_NONE);

	///add fragment shader for splatting
	pass.setFragmentProgram("splatting_fragment_normalmapped_dynamic");
	// 	pass.setFragmentProgram(fragmentProgramName);
	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = pass.getFragmentProgramParameters();
		fpParams->setNamedAutoConstant("iLightDiffuse", Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR);
		fpParams->setNamedAutoConstant("iLightAmbient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
		fpParams->setNamedAutoConstant("iFogColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		float theValues[4] = { 0.04, -0.02, 1, 0 };
		fpParams->setNamedConstant("scaleBias", theValues, 4); //4*4=16
		fpParams->setNamedConstant("iNumberOfLayers", (float)mLayers.size()); //4*4=16
		///set how much the texture should tile
		fpParams->setNamedConstant("iScales", mScales, 4); //4*4=16
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}

	///add vertex shader for fog
	if (EmberOgre::getSingleton().getSceneManager()->getFogMode() == Ogre::FOG_EXP2) {
		pass.setVertexProgram("splatting_vertex_normalmapped_exp2");
	} else {
		pass.setVertexProgram("splatting_vertex_normalmapped");
	}

	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = pass.getVertexProgramParameters();
		fpParams->setNamedAutoConstant("lightPosition", Ogre::GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
		fpParams->setNamedAutoConstant("eyePosition", Ogre::GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
		fpParams->setNamedAutoConstant("iFogParams", Ogre::GpuProgramParameters::ACT_FOG_PARAMS);
		fpParams->setNamedAutoConstant("worldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
		fpParams->setNamedAutoConstant("iLightAmbient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
		fpParams->setNamedAutoConstant("iLightDiffuse", Ogre::GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters." << ex);
		return false;
	}
	return true;
}


}

}

}
