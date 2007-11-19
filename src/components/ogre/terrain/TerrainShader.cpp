/*
    Copyright (C) 2004  Erik Hjortsberg

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

#include "TerrainShader.h"
#include <OgreIteratorWrappers.h>
#include "TerrainLayerDefinition.h"
namespace EmberOgre {
namespace Terrain {

TerrainShader::TerrainShader(Mercator::Terrain* terrain, int terrainIndex, const TerrainLayerDefinition* layerDef, Mercator::Shader* shader)
: mLayerDef(layerDef)
, mShader(shader)
, mTerrain(terrain)
, mTerrainIndex(terrainIndex)
// , mMaterial(0)
// , mScale(32)
{

	mTerrain->addShader(shader, mTerrainIndex);

}

/*
TerrainShader::TerrainShader(Mercator::Terrain* terrain, int terrainIndex,  Ogre::MaterialPtr material, Mercator::Shader* shader)
: mTextureName("")
, mShader(shader)
, mTerrain(terrain)
, mTerrainIndex(terrainIndex)
, mMaterial(material)
{

	mTerrain->addShader(shader, mTerrainIndex);
}*/

TerrainShader::~TerrainShader()
{
	///not available yet
	//mTerrain->removeShader(mShader)
}

Ogre::ushort TerrainShader::getMaxTextureUnits() const 
{
 	return std::min<unsigned short>(4, Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits());
}

Mercator::Shader* TerrainShader::getShader() const
{
	return mShader;
}

// const Ogre::String& TerrainShader::getTextureName() const
// {
// 	return mTextureName;
// }

const TerrainLayerDefinition* TerrainShader::getLayerDefinition() const
{
	return mLayerDef;
}


void TerrainShader::addTextureUnitsToPass(Ogre::Pass* pass, const Ogre::String& splatTextureName) {
	
/*	if (getMaxTextureUnits() - pass->getNumTextureUnitStates() < 2 || pass->getParent()->getNumPasses() > 1)  {
		addPassToTechnique(pass->getParent(), splatTextureName);
// 		S_LOG_WARNING("Trying to add texture units to pass with too few available texture unit states.");
		return;
	}
		
	S_LOG_VERBOSE("Adding new texture unit (detailtexture: " << mTextureName << " alphatexture: " << splatTextureName << ") to pass nr " << pass->getIndex() << " in technique for material " << pass->getParent()->getParent()->getName());*/
	
/*	pass->setSelfIllumination(Ogre::ColourValue(1,1,1));
	pass->setAmbient(Ogre::ColourValue(1,1,1));
	pass->setDiffuse(Ogre::ColourValue(1,1,1));
	pass->setLightingEnabled(true);*/
/*	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
    textureUnitStateSplat->setTextureName(splatTextureName);
     
    textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
    textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);*/
// 	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);
//	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_TEXTURE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);

/*	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
    textureUnitState->setTextureName(mTextureName);
    textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);*/
/*	textureUnitState->setTextureCoordSet(0);*/
/*	textureUnitState->setTextureScale(0.025, 0.025);
	textureUnitState->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);*/
	
/*	Ogre::TextureUnitState * alphaTextureState= pass->createTextureUnitState();
    alphaTextureState->setTextureName(mTextureName);
//     alphaTextureState->setTextureName(splatTextureName);
    alphaTextureState->setTextureCoordSet(0);
	alphaTextureState->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
    alphaTextureState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
 	alphaTextureState->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);



// 	detailTextureState->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
// 	detailTextureState->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);

	Ogre::TextureUnitState * detailTextureState  = pass->createTextureUnitState();
    detailTextureState ->setTextureName(splatTextureName);
//     detailTextureState ->setTextureName(mTextureName);
    detailTextureState ->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	detailTextureState ->setTextureCoordSet(0);
	detailTextureState ->setTextureScale(0.01, 0.01);
	//detailTextureState ->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);*/
	
}

Ogre::Pass* TerrainShader::addPassToTechnique(Ogre::Technique* technique, const Ogre::String& splatTextureName) {
	return 0;
	///check if we instead can reuse the existing pass
// 	if (technique->getNumPasses() != 0) {
// 		Ogre::Pass* pass = technique->getPass(technique->getNumPasses() - 1);
// 		if (4 - pass->getNumTextureUnitStates() >= 2) {
// 			///there's more than two texture units available, use those instead of creating a new pass
// 			S_LOG_VERBOSE("Reusing existing pass. ("<< pass->getNumTextureUnitStates() << " of "<< mNumberOfTextureUnitsOnCard << " texture unit used)");
// 			addTextureUnitsToPass(pass, splatTextureName);
// 			return pass;
// 		}
// 		
// 	}
	
	
// 	Ogre::Pass* pass = technique->createPass();
// 	
// 	S_LOG_VERBOSE("Adding new pass (" << mTextureName << ") to technique for material " << technique->getParent()->getName() << ". Number of passes in this technique: " << technique->getNumPasses());
// 	
//  	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
// 	pass->setLightingEnabled(false);
// 	
// 	
// 	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
//     textureUnitStateSplat->setTextureName(splatTextureName);
//      
//     textureUnitStateSplat->setTextureCoordSet(0);
// //	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//     textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
// 	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
// //	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
// 	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);
// 
// 	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
//     textureUnitState->setTextureName(mTextureName);
//     textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
//     textureUnitState->setTextureCoordSet(0);
// 	textureUnitState->setTextureScale(0.025, 0.025);
// 
//     
//     return pass; 
//	textureUnitState->setColourOperationEx(LBX_BLEND_CURRENT_ALPHA, LBS_TEXTURE, LBS_CURRENT);
	
}

void TerrainShader::addMaterialToTechnique(Ogre::Technique*  technique, const Ogre::String& splatTextureName) {

// 	if (!mMaterial->getNumSupportedTechniques()) {
// 		return;
// 	}
// 	
// 	Ogre::Technique* sourceTech = mMaterial->getSupportedTechnique(0);
// 	Ogre::Technique::PassIterator I = sourceTech->getPassIterator();
// 	bool isFirstPass = true;
// 	while (I.hasMoreElements()) {
// 		Ogre::Pass* sourcePass = I.getNext();
// 		
// 		
// 		///if there's only one pass, check if we can fit that into the existing pass
// 		if (isFirstPass && technique->getNumPasses() == 1) {
// 			isFirstPass = false;
// 			if (technique->getNumPasses() != 0) {
// 				Ogre::Pass* existingPass = technique->getPass(technique->getNumPasses() - 1);
// 				if (getMaxTextureUnits() - existingPass->getNumTextureUnitStates() >= sourcePass->getNumTextureUnitStates()) {
// 					///there's more than two texture units available, use those instead of creating a new pass
// 					///now iterate through all textures, copy the texture units to the existing splat and look for the splat
// 					Ogre::Pass::TextureUnitStateIterator J = sourcePass->getTextureUnitStateIterator();
// 					while (J.hasMoreElements()) {
// 						Ogre::TextureUnitState* newTexUnitState = new Ogre::TextureUnitState(existingPass, *J.getNext());
// 						//Ogre::TextureUnitState* texUnitState = J.getNext();
// 						///clone the texture unit state
// 						//*newTexUnitState = *texUnitState;
// 						if (newTexUnitState->getTextureName() == "splat") {
// 							newTexUnitState->setTextureName(splatTextureName);
// 						}
// 						
// 					}
// 					continue;
// 				}
// 			}
// 		}
// 		
// 		Ogre::Pass* destPass = technique->createPass();
// 		
// 		//just make a copy of the source pass
// 		*destPass = *sourcePass;
// 		destPass->setName("");
// 		
// 		S_LOG_VERBOSE("Added new pass from template material(" << mMaterial->getName()<< ") to technique for material " << technique->getParent()->getName() << ". Number of passes in this technique: " << technique->getNumPasses());
// 		
// 		//now iterate through all textures and look for the splat
// 		Ogre::Pass::TextureUnitStateIterator J = destPass->getTextureUnitStateIterator();
// 		while (J.hasMoreElements()) {
// 			Ogre::TextureUnitState* texUnitState = J.getNext();
// 			texUnitState->setName("");
// 			if (texUnitState->getTextureName() == "splat") {
// 				texUnitState->setTextureName(splatTextureName);
// 			}
// 		}
// 	}
}

void TerrainShader::addSplatToTechnique(Ogre::Technique*  technique, const Ogre::String& splatTextureName) {
/*	if (!mMaterial.isNull()) {
		addMaterialToTechnique(technique, splatTextureName);
	} else {
		addTextureUnitsToPass(technique->getPass(technique->getNumPasses() - 1) , splatTextureName);
// 		Ogre::Pass* pass = addPassToTechnique(technique, splatTextureName);
		//pass->setLightingEnabled(false);
		//pass->setSelfIllumination(Ogre::ColourValue(1,1,1));
	}*/
}


Mercator::Surface* TerrainShader::getSurfaceForSegment(Mercator::Segment* segment) const
{

	Mercator::Surface* surface = 0;
	if (segment->getSurfaces().find(getTerrainIndex()) != segment->getSurfaces().end()) {
		surface = segment->getSurfaces().find(getTerrainIndex())->second;
	}
	return surface;
}

}
}
