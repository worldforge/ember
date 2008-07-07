//
// C++ Implementation: TerrainPageSurfaceCompilerTechniqueSimple
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageSurfaceCompilerTechniqueSimple.h"
#include "TerrainPageSurfaceLayer.h"

namespace EmberOgre {

namespace Terrain {

bool TerrainPageSurfaceCompilerTechniqueSimple::compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow)
{
	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();
	for (std::map<int, TerrainPageSurfaceLayer*>::iterator I = terrainPageSurfaces.begin(); I != terrainPageSurfaces.end(); ++I) {
		TerrainPageSurfaceLayer* surfaceLayer = I->second;
		if (I == terrainPageSurfaces.begin()) {
			Ogre::Pass* pass = technique->createPass();
			pass->setLightingEnabled(false);
			///add the first layer of the terrain, no alpha or anything
			Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
			textureUnitState->setTextureScale(1.0f / surfaceLayer->getScale(), 1.0f / surfaceLayer->getScale());
			textureUnitState->setTextureName(surfaceLayer->getDiffuseTextureName());
			textureUnitState->setTextureCoordSet(0);
		} else {
			if (surfaceLayer->intersects()) {
				addPassToTechnique(technique, surfaceLayer);
			}
		}
	}
	if (terrainPageShadow) {
		addShadow(technique, terrainPageShadow);
	}
	return true;
}

void TerrainPageSurfaceCompilerTechniqueSimple::addShadow(Ogre::Technique* technique, TerrainPageShadow* terrainPageShadow)
{
	Ogre::Pass* shadowPass = technique->createPass();
	
	shadowPass->setSceneBlending(Ogre::SBT_MODULATE);
	shadowPass->setLightingEnabled(false);
	shadowPass->setFog(false);
	
	
	Ogre::TextureUnitState * textureUnitStateSplat = shadowPass->createTextureUnitState();
	textureUnitStateSplat->setTextureName(terrainPageShadow->getTexture()->getName());

	textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
}

void TerrainPageSurfaceCompilerTechniqueSimple::setPage(TerrainPage* page)
{
	mPage = page;
}


// void TerrainPageSurfaceCompiler::addTextureUnitsToPass(Ogre::Pass* pass, const Ogre::String& splatTextureName) {
// 	
// 	if (getMaxTextureUnits() - pass->getNumTextureUnitStates() < 2 || pass->getParent()->getNumPasses() > 1)  {
// 		addPassToTechnique(pass->getParent(), splatTextureName);
// // 		S_LOG_WARNING("Trying to add texture units to pass with too few available texture unit states.");
// 		return;
// 	}
// 		
// 	S_LOG_VERBOSE("Adding new texture unit (detailtexture: " << mTextureName << " alphatexture: " << splatTextureName << ") to pass nr " << pass->getIndex() << " in technique for material " << pass->getParent()->getParent()->getName());
// 	
// /*	pass->setSelfIllumination(Ogre::ColourValue(1,1,1));
// 	pass->setAmbient(Ogre::ColourValue(1,1,1));
// 	pass->setDiffuse(Ogre::ColourValue(1,1,1));
// 	pass->setLightingEnabled(true);*/
// 	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
//     textureUnitStateSplat->setTextureName(splatTextureName);
//      
//     textureUnitStateSplat->setTextureCoordSet(0);
// 	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
// 	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
// 	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
//     textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
// // 	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);
// //	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_TEXTURE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);
// 
// 	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
//     textureUnitState->setTextureName(mTextureName);
//     textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
// /*	textureUnitState->setTextureCoordSet(0);*/
// 	textureUnitState->setTextureScale(0.025, 0.025);
// 	textureUnitState->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
// 	
// /*	Ogre::TextureUnitState * alphaTextureState= pass->createTextureUnitState();
//     alphaTextureState->setTextureName(mTextureName);
// //     alphaTextureState->setTextureName(splatTextureName);
//     alphaTextureState->setTextureCoordSet(0);
// 	alphaTextureState->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//     alphaTextureState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
//  	alphaTextureState->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);
// 
// 
// 
// // 	detailTextureState->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
// // 	detailTextureState->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
// 
// 	Ogre::TextureUnitState * detailTextureState  = pass->createTextureUnitState();
//     detailTextureState ->setTextureName(splatTextureName);
// //     detailTextureState ->setTextureName(mTextureName);
//     detailTextureState ->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
// 	detailTextureState ->setTextureCoordSet(0);
// 	detailTextureState ->setTextureScale(0.01, 0.01);
// 	//detailTextureState ->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);*/
// 	
// }
// 
Ogre::Pass* TerrainPageSurfaceCompilerTechniqueSimple::addPassToTechnique(Ogre::Technique* technique, TerrainPageSurfaceLayer* layer) {
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
	
	///we need to create the image, update it and then destroy it again (to keep the memory usage down)
	if (layer->getCoverageTextureName() == "") {
		///no texture yet; let's create one
		layer->createCoverageImage();
		layer->updateCoverageImage();
		layer->createTexture();
// 		layer->destroyCoverageImage();
	} else {
		///a texture exists, so we just need to update the image
// 		layer->createCoverageImage();
		layer->updateCoverageImage(); ///calling this will also update the texture since the method will blit the image onto it
// 		layer->destroyCoverageImage();
	}
	
	Ogre::Pass* pass = technique->createPass();
	

	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	pass->setLightingEnabled(false);


	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	textureUnitState->setTextureName(layer->getDiffuseTextureName());
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureScale(1.0f / layer->getScale(), 1.0f / layer->getScale());		

	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
	textureUnitStateSplat->setTextureName(layer->getCoverageTextureName());

	textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_BLEND_DIFFUSE_COLOUR, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
	return pass; 
	
}


}

}
