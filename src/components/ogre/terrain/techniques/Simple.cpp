//
// C++ Implementation: Simple
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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

#include "Simple.h"
#include "components/ogre/terrain/TerrainPageSurfaceLayer.h"
#include "components/ogre/terrain/TerrainPageGeometry.h"
#include "components/ogre/terrain/TerrainPageShadow.h"
#include "components/ogre/terrain/OgreImage.h"

#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreTextureUnitState.h>
#include <OgreRoot.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

Simple::Simple(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow) :
		Base(geometry, terrainPageSurfaces, terrainPageShadow), mLightingImage(0)
{

}

Simple::~Simple()
{
	for (auto& layer : mLayers) {
		delete layer.blendMap;
	}
	delete mLightingImage;
}

bool Simple::prepareMaterial()
{
	for (auto entry : mTerrainPageSurfaces) {
		auto surfaceLayer = entry.second;
		if (surfaceLayer != mTerrainPageSurfaces.begin()->second) {
			if (surfaceLayer->intersects(*mGeometry)) {
				mLayers.emplace_back(Layer { *surfaceLayer, new OgreImage(new Image::ImageBuffer(mPage.getBlendMapSize(), 1)) });
				OgreImage* image = mLayers.back().blendMap;
				image->reset();
				surfaceLayer->fillImage(*mGeometry, *image, 0);
			}
		}
	}
	return true;
}

bool Simple::compileMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const
{
	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();
	if (!mTerrainPageSurfaces.empty()) {
		//First add a base pass
		auto surfaceLayer = mTerrainPageSurfaces.begin()->second;
		Ogre::Pass* pass = technique->createPass();
		pass->setLightingEnabled(false);
		Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
		textureUnitState->setTextureScale(1.0f / surfaceLayer->getScale(), 1.0f / surfaceLayer->getScale());
		textureUnitState->setTextureName(surfaceLayer->getDiffuseTextureName());
		textureUnitState->setTextureCoordSet(0);

		for (auto& layer : mLayers) {
			addPassToTechnique(*mGeometry, technique, layer, managedTextures);
		}
	}
	if (mTerrainPageShadow) {
		addShadow(technique, mTerrainPageShadow, material, managedTextures);
	}

//	addLightingPass(technique, managedTextures);

	material->load();
	if (material->getNumSupportedTechniques() == 0) {
		S_LOG_WARNING("The material '" << material->getName() << "' has no supported techniques. The reason for this is: \n" << material->getUnsupportedTechniquesExplanation());
		return false;
	}
	return true;
}

void Simple::addLightingPass(Ogre::Technique* technique, std::set<std::string>& managedTextures) const
{
	Ogre::Pass* lightingPass = technique->createPass();

	lightingPass->setSceneBlending(Ogre::SBT_MODULATE);
	lightingPass->setLightingEnabled(false);

	Ogre::TextureUnitState * textureUnitStateSplat = lightingPass->createTextureUnitState();

	//we need an unique name for our alpha texture
	std::stringstream lightingTextureNameSS;
	lightingTextureNameSS << technique->getParent()->getName() << "_lighting";
	const Ogre::String lightingTextureName(lightingTextureNameSS.str());

	Ogre::TexturePtr texture = static_cast<Ogre::TexturePtr>(Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(lightingTextureName));
	if (texture.isNull()) {
		texture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(lightingTextureName, "General", Ogre::TEX_TYPE_2D, mPage.getBlendMapSize(), mPage.getBlendMapSize(), 1, Ogre::PF_L8, Ogre::TU_DYNAMIC_WRITE_ONLY);
		managedTextures.insert(texture->getName());
	}

	Ogre::Image ogreImage;
	ogreImage.loadDynamicImage(const_cast<unsigned char*>(mLightingImage->getData()), mLightingImage->getResolution(), mLightingImage->getResolution(), 1, Ogre::PF_L8);

	texture->loadImage(ogreImage);

	//blit the whole image to the hardware buffer
	Ogre::PixelBox sourceBox(ogreImage.getPixelBox());
	//blit for each mipmap
	for (unsigned int i = 0; i <= texture->getNumMipmaps(); ++i) {
		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, i));
		hardwareBuffer->blitFromMemory(sourceBox);
	}

	textureUnitStateSplat->setTextureName(texture->getName());

	textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);

}

void Simple::addShadow(Ogre::Technique* technique, const TerrainPageShadow* terrainPageShadow, Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const
{
	Ogre::Pass* shadowPass = technique->createPass();

	shadowPass->setSceneBlending(Ogre::SBT_MODULATE);
	shadowPass->setLightingEnabled(false);
//	shadowPass->setFog(true, Ogre::FOG_NONE);

	Ogre::TextureUnitState * textureUnitStateSplat = shadowPass->createTextureUnitState();
	Ogre::TexturePtr texture = updateShadowTexture(material, terrainPageShadow, managedTextures);
	textureUnitStateSplat->setTextureName(texture->getName());

	textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
}

Ogre::TexturePtr Simple::updateShadowTexture(Ogre::MaterialPtr material, const TerrainPageShadow* terrainPageShadow, std::set<std::string>& managedTextures) const
{
	auto shadowTextureName = getShadowTextureName(material);

	Ogre::TexturePtr texture = static_cast<Ogre::TexturePtr>(Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(shadowTextureName));
	if (texture.isNull()) {
		texture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(shadowTextureName, "General", Ogre::TEX_TYPE_2D, mPage.getBlendMapSize(), mPage.getBlendMapSize(), 1, Ogre::PF_L8, Ogre::TU_DYNAMIC_WRITE_ONLY);
		managedTextures.insert(texture->getName());
	}

	Ogre::Image ogreImage;
	terrainPageShadow->loadIntoImage(ogreImage);

	texture->loadImage(ogreImage);

	//blit the whole image to the hardware buffer
	Ogre::PixelBox sourceBox(ogreImage.getPixelBox());
	//blit for each mipmap
	for (unsigned int i = 0; i <= texture->getNumMipmaps(); ++i) {
		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(texture->getBuffer(0, i));
		hardwareBuffer->blitFromMemory(sourceBox);
	}

	return texture;
}

std::string Simple::getShadowTextureName(const Ogre::MaterialPtr& material) const
{
	std::stringstream shadowTextureNameSS;
	shadowTextureNameSS << material->getName() << "_shadow";
	return shadowTextureNameSS.str();
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
Ogre::Pass* Simple::addPassToTechnique(const TerrainPageGeometry& geometry, Ogre::Technique* technique, const Layer& layer, std::set<std::string>& managedTextures) const
{
	//check if we instead can reuse the existing pass
	// 	if (technique->getNumPasses() != 0) {
	// 		Ogre::Pass* pass = technique->getPass(technique->getNumPasses() - 1);
	// 		if (4 - pass->getNumTextureUnitStates() >= 2) {
	// 			//there's more than two texture units available, use those instead of creating a new pass
	// 			S_LOG_VERBOSE("Reusing existing pass. ("<< pass->getNumTextureUnitStates() << " of "<< mNumberOfTextureUnitsOnCard << " texture unit used)");
	// 			addTextureUnitsToPass(pass, splatTextureName);
	// 			return pass;
	// 		}
	//
	// 	}

	const OgreImage& ogreImage = *layer.blendMap;
	Ogre::Image image;

	image.loadDynamicImage(const_cast<unsigned char*>(ogreImage.getData()), ogreImage.getResolution(), ogreImage.getResolution(), 1, Ogre::PF_A8);

	std::stringstream splatTextureNameSS;
	splatTextureNameSS << "terrain_" << mPage.getWFPosition().x() << "_" << mPage.getWFPosition().y() << "_" << technique->getNumPasses();
	const Ogre::String splatTextureName(splatTextureNameSS.str());
	Ogre::TexturePtr blendMapTexture;
	if (Ogre::Root::getSingletonPtr()->getTextureManager()->resourceExists(splatTextureName)) {
		blendMapTexture = static_cast<Ogre::TexturePtr>(Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(splatTextureName));
		blendMapTexture->loadImage(image);

		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer(blendMapTexture->getBuffer());
		//blit the whole image to the hardware buffer
		Ogre::PixelBox sourceBox(image.getPixelBox());
		hardwareBuffer->blitFromMemory(sourceBox);
	} else {
		blendMapTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadImage(splatTextureName, "General", image, Ogre::TEX_TYPE_2D, 0);
		managedTextures.insert(blendMapTexture->getName());
	}

	//we need to create the image, update it and then destroy it again (to keep the memory usage down)
	//	if (layer->getBlendMapTextureName() == "") {
	//		//no texture yet; let's create one
	//		layer->createBlendMapImage();
	//		layer->updateBlendMapImage(geometry);
	//		layer->createTexture();
	//	} else {
	//		//a texture exists, so we just need to update the image
	//		layer->updateBlendMapImage(geometry); //calling this will also update the texture since the method will blit the image onto it
	//	}

	Ogre::Pass* pass = technique->createPass();

	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	pass->setAmbient(1, 1, 1);
	pass->setDiffuse(1, 1, 1, 1);
	pass->setLightingEnabled(false);

	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	textureUnitState->setTextureName(layer.surfaceLayer.getDiffuseTextureName());
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureScale(1.0f / layer.surfaceLayer.getScale(), 1.0f / layer.surfaceLayer.getScale());

	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
	textureUnitStateSplat->setTextureName(blendMapTexture->getName());

	textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
	//	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_BLEND_DIFFUSE_COLOUR, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);
	return pass;

}

bool Simple::compileCompositeMapMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const
{
	// Does not support composite maps
	return false;
}

}

}

}
}
