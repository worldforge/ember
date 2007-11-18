//
// C++ Implementation: TerrainPageSurfaceCompilerTechniqueShader
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

#include "TerrainPageSurfaceCompilerTechniqueShader.h"
#include "TerrainPageSurfaceLayer.h"

namespace EmberOgre {

namespace Terrain {

TerrainPageSurfaceCompilerShaderPassCoverageBatch::TerrainPageSurfaceCompilerShaderPassCoverageBatch(TerrainPageSurfaceCompilerShaderPass& shaderPass, Ogre::TexturePtr combinedCoverageTexture)
: mShaderPass(shaderPass)
, mCombinedCoverageTexture(combinedCoverageTexture)
, mCombinedCoverageImage(new Ogre::Image())
, mCombinedCoverageDataStream(new Ogre::MemoryDataStream(combinedCoverageTexture->getWidth() * combinedCoverageTexture->getWidth() * 4, false))
, mCombinedCoverageDataStreamPtr(mCombinedCoverageDataStream)
{
	///reset the coverage image
	memset(mCombinedCoverageDataStream->getPtr(), '\0', mCombinedCoverageDataStream->size());
}
TerrainPageSurfaceCompilerShaderPassCoverageBatch::~TerrainPageSurfaceCompilerShaderPassCoverageBatch()
{
}

void TerrainPageSurfaceCompilerShaderPassCoverageBatch::addLayer(TerrainPageSurfaceLayer* layer)
{
	addCoverage(layer->getCoverageImage(), mLayers.size(), 4);
	mLayers.push_back(layer);
}

void TerrainPageSurfaceCompilerShaderPassCoverageBatch::addCoverage(Ogre::Image* coverage, unsigned int channel, unsigned short numberOfChannels) {
	mCombinedCoverageDataStream->seek(0);
	unsigned char * coverageData = coverage->getData();
	unsigned char * combinedCoverageData = mCombinedCoverageDataStream->getPtr();
	combinedCoverageData += channel;
	for (unsigned int i = 0; i <= coverage->getSize(); ++i) {
		*combinedCoverageData = *coverageData;
		combinedCoverageData += numberOfChannels;
		coverageData++;
	}
}

std::vector<TerrainPageSurfaceLayer*>& TerrainPageSurfaceCompilerShaderPassCoverageBatch::getLayers()
{
	return mLayers;
}

void TerrainPageSurfaceCompilerShaderPassCoverageBatch::assignCombinedCoverageTexture() {
	mCombinedCoverageDataStreamPtr->seek(0);
	mCombinedCoverageImage->loadRawData(mCombinedCoverageDataStreamPtr, mShaderPass.getCoveragePixelWidth(), mShaderPass.getCoveragePixelWidth(), Ogre::PF_B8G8R8A8);
	mCombinedCoverageTexture->loadImage(*mCombinedCoverageImage);
	
	Ogre::HardwarePixelBufferSharedPtr hardwareBuffer = mCombinedCoverageTexture->getBuffer();
	///blit the whole image to the hardware buffer
	Ogre::PixelBox sourceBox = mCombinedCoverageImage->getPixelBox();
	hardwareBuffer->blitFromMemory(sourceBox);
	
}
	
Ogre::TexturePtr TerrainPageSurfaceCompilerShaderPassCoverageBatch::getCombinedCoverageTexture()
{
	return mCombinedCoverageTexture;
}
void TerrainPageSurfaceCompilerShaderPassCoverageBatch::finalize()
{
	///add our coverage textures first
	assignCombinedCoverageTexture();
	Ogre::TextureUnitState * textureUnitState = mShaderPass.mPass->createTextureUnitState();
	textureUnitState->setTextureScale(1, 1);
	textureUnitState->setTextureName(getCombinedCoverageTexture()->getName());
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	
	for (LayerStore::iterator I = mLayers.begin(); I != mLayers.end(); ++I) {
		TerrainPageSurfaceLayer* layer(*I);
		///add the layer textures
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName());	
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * textureUnitState = mShaderPass.mPass->createTextureUnitState();
		//textureUnitState->setTextureScale(0.025, 0.025);
		textureUnitState->setTextureName(layer->getDiffuseTextureName());
		textureUnitState->setTextureCoordSet(0);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);	
	}
}









TerrainPageSurfaceCompilerTechniqueShader::TerrainPageSurfaceCompilerTechniqueShader()
{
}


TerrainPageSurfaceCompilerTechniqueShader::~TerrainPageSurfaceCompilerTechniqueShader()
{
	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		delete *I;
	}
}


Ogre::TexturePtr TerrainPageSurfaceCompilerShaderPass::getCombinedCoverageTexture(size_t passIndex, size_t batchIndex)
{
	///we need an unique name for our alpha texture
	std::stringstream combinedCoverageTextureNameSS;
	combinedCoverageTextureNameSS << "terrain_" << mPage.getWFPosition().x() << "_" << mPage.getWFPosition().y() << "_combinedCoverage_"<< passIndex << "_" << batchIndex;
	const Ogre::String combinedCoverageName(combinedCoverageTextureNameSS.str());
	Ogre::TexturePtr combinedCoverageTexture;
	if (Ogre::Root::getSingletonPtr()->getTextureManager()->resourceExists(combinedCoverageName)) {
		S_LOG_VERBOSE("Using already created texture " << combinedCoverageName);
		combinedCoverageTexture = static_cast<Ogre::TexturePtr>(Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(combinedCoverageName));
	} else {
		S_LOG_VERBOSE("Creating new texture " << combinedCoverageName);
		combinedCoverageTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(combinedCoverageName, "General", Ogre::TEX_TYPE_2D, mPage.getAlphaTextureSize(), mPage.getAlphaTextureSize(), 4, Ogre::PF_B8G8R8A8);
	}
	return combinedCoverageTexture;
}

void TerrainPageSurfaceCompilerTechniqueShader::reset()
{
	for (PassStore::iterator I = mPasses.begin(); I != mPasses.end(); ++I) {
		delete *I;
	}
	mPasses.clear();	
}


void TerrainPageSurfaceCompilerTechniqueShader::compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow)
{
	reset();
	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();
	TerrainPageSurfaceCompilerShaderPass* shaderPass = addPass(technique);
	if (shaderPass) {
		shaderPass->addShadowLayer(terrainPageShadow);
		for (std::map<int, TerrainPageSurfaceLayer*>::iterator I = terrainPageSurfaces.begin(); I != terrainPageSurfaces.end(); ++I) {
			TerrainPageSurfaceLayer* surfaceLayer = I->second;
			if (I == terrainPageSurfaces.begin()) {
				shaderPass->setBaseLayer(surfaceLayer);
			} else {
				if (surfaceLayer->intersects()) {
					shaderPass->addLayer(surfaceLayer);
				}
			}
		}
		shaderPass->finalize();
	}
// 	if (terrainPageShadow) {
// 		addShadow(technique, terrainPageShadow);
// 	}
}


TerrainPageSurfaceCompilerShaderPass::TerrainPageSurfaceCompilerShaderPass(Ogre::Pass* pass, TerrainPage& page)
: 
mPass(pass)
// , mCurrentLayerIndex(0)
, mBaseLayer(0)
, mPage(page)
{
}

TerrainPageSurfaceCompilerShaderPass::~TerrainPageSurfaceCompilerShaderPass()
{
	for (CoverageBatchStore::iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		delete *I;
	}
}


void TerrainPageSurfaceCompilerShaderPass::setBaseLayer(TerrainPageSurfaceLayer* layer)
{
	mLayers.push_back(layer);
	mBaseLayer = layer;
	mScales[0] = layer->getScale();
}

TerrainPageSurfaceCompilerShaderPassCoverageBatch* TerrainPageSurfaceCompilerShaderPass::getCurrentBatch()
{
	CoverageBatchStore::reverse_iterator I = mCoverageBatches.rbegin();
	if (!mCoverageBatches.size() || (*I)->getLayers().size() == 4) {
		TerrainPageSurfaceCompilerShaderPassCoverageBatch* batch = new TerrainPageSurfaceCompilerShaderPassCoverageBatch(*this, getCombinedCoverageTexture(mPass->getIndex(), mCoverageBatches.size()));
		mCoverageBatches.push_back(batch);
		return batch;
	} else {
		return *I;
	}
}


void TerrainPageSurfaceCompilerShaderPass::addLayer(TerrainPageSurfaceLayer* layer)
{
// 	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
// // 	if (mCurrentLayerIndex < std::min<unsigned short>(numberOfTextureUnitsOnCard - 1, 4)) {
// 		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName() << " and scale " << layer->getScale() << " at index "<< (mCurrentLayerIndex + 1) <<" (" << numberOfTextureUnitsOnCard << " texture units supported)");
// 		Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
// // 		textureUnitState->setTextureScale(0.025, 0.025);
// 		textureUnitState->setTextureName(layer->getDiffuseTextureName());
// 		textureUnitState->setTextureCoordSet(0);
// 		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		
		getCurrentBatch()->addLayer(layer);
		
// 		mCurrentLayerIndex++;
		mScales[mLayers.size()] = layer->getScale();
		mLayers.push_back(layer);
// 	}
}




LayerStore& TerrainPageSurfaceCompilerShaderPass::getLayers()
{
	return mLayers;
}

void TerrainPageSurfaceCompilerShaderPass::finalize()
{
	//TODO: add shadow here
	
	///should we use a base pass?
	if (mBaseLayer) {
		Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
		S_LOG_VERBOSE("Adding new base layer with diffuse texture " << mBaseLayer->getDiffuseTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");	
		///add the first layer of the terrain, no alpha or anything
		Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
		textureUnitState->setTextureName(mBaseLayer->getDiffuseTextureName());
		textureUnitState->setTextureCoordSet(0);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}	
	
	///add our coverage textures first
	for (CoverageBatchStore::iterator I = mCoverageBatches.begin(); I != mCoverageBatches.end(); ++I) {
		TerrainPageSurfaceCompilerShaderPassCoverageBatch* batch = *I;
		batch->finalize();
	}
	


	
	std::stringstream ss;
	ss << "splatting_fragment_" << mLayers.size();
	std::string fragmentProgramName(ss.str());
	
	mPass->setLightingEnabled(false);
	mPass->setFog(true, Ogre::FOG_NONE);
	
			
	///add fragment shader for splatting
	mPass->setFragmentProgram("splatting_fragment_dynamic");
// 	mPass->setFragmentProgram(fragmentProgramName);
	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getFragmentProgramParameters();
		fpParams->setNamedAutoConstant("iFogColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
		fpParams->setNamedConstant("iNumberOfLayers", (float)mLayers.size()); //4*4=16
		///set how much the texture should tile
		fpParams->setNamedConstant("iScales", mScales, 4); //4*4=16
	} catch (const Ogre::Exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters. Message:\n" << ex.what());
	}
	
	///add vertex shader for fog	
	mPass->setVertexProgram("fog_linear_vp");
	try {
		Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getVertexProgramParameters();
		fpParams->setNamedAutoConstant("iFogParams", Ogre::GpuProgramParameters::ACT_FOG_PARAMS);
		fpParams->setNamedAutoConstant("iWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX );
	} catch (const Ogre::Exception& ex) {
		S_LOG_WARNING("Error when setting fragment program parameters. Message:\n" << ex.what());
	}
}

bool TerrainPageSurfaceCompilerShaderPass::hasRoomForLayer(TerrainPageSurfaceLayer* layer)
{
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	return (mPass->getNumTextureUnitStates() - numberOfTextureUnitsOnCard) > 1;

}



void TerrainPageSurfaceCompilerShaderPass::addShadowLayer(TerrainPageShadow* terrainPageShadow)
{
	S_LOG_VERBOSE("Adding shadow layer.");	
	Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
	//textureUnitState->setTextureScale(0.025, 0.025);
	textureUnitState->setTextureName(terrainPageShadow->getTexture()->getName());
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitState->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
	textureUnitState->setTextureAnisotropy(2);
	
}

unsigned int TerrainPageSurfaceCompilerShaderPass::getCoveragePixelWidth() const
{
	return 512;
}

void TerrainPageSurfaceCompilerTechniqueShader::setPage(TerrainPage* page)
{
	mPage = page;
}

TerrainPageSurfaceCompilerShaderPass* TerrainPageSurfaceCompilerTechniqueShader::addPass(Ogre::Technique* technique)
{
	Ogre::Pass* pass = technique->createPass();
	TerrainPageSurfaceCompilerShaderPass* shaderPass = new TerrainPageSurfaceCompilerShaderPass(pass, *mPage);
	return shaderPass;
}

}

}
