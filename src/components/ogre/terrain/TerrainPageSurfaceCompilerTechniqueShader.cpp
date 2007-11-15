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

TerrainPageSurfaceCompilerTechniqueShader::TerrainPageSurfaceCompilerTechniqueShader()
{
}


TerrainPageSurfaceCompilerTechniqueShader::~TerrainPageSurfaceCompilerTechniqueShader()
{
}


void TerrainPageSurfaceCompilerTechniqueShader::compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, TerrainPage& page)
{

	///we need an unique name for our alpha texture
	std::stringstream combinedCoverageTextureNameSS;
	combinedCoverageTextureNameSS << page.getWFPosition().x() << "_" << page.getWFPosition().y() << "_combinedCoverage";
	const Ogre::String combinedCoverageName(combinedCoverageTextureNameSS.str());
	Ogre::TexturePtr combinedCoverageTexture;
	if (Ogre::Root::getSingletonPtr()->getTextureManager()->resourceExists(combinedCoverageName)) {
		S_LOG_VERBOSE("Using already created texture " << combinedCoverageName);
		return;
		combinedCoverageTexture = static_cast<Ogre::TexturePtr>(Ogre::Root::getSingletonPtr()->getTextureManager()->getByName(combinedCoverageName));
	} else {
		S_LOG_VERBOSE("Creating new texture " << combinedCoverageName);
		combinedCoverageTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(combinedCoverageName, "General", Ogre::TEX_TYPE_2D, page.getAlphaTextureSize(), page.getAlphaTextureSize(), 4, Ogre::PF_B8G8R8A8);
	}
	
	

	material->removeAllTechniques();
	Ogre::Technique* technique = material->createTechnique();
	Ogre::Pass* pass = createPass(technique);
	if (pass) {
		TerrainPageSurfaceCompilerShaderPass shaderPass(pass, combinedCoverageTexture);
		shaderPass.addShadowLayer(terrainPageShadow);
		for (std::map<int, TerrainPageSurfaceLayer*>::iterator I = terrainPageSurfaces.begin(); I != terrainPageSurfaces.end(); ++I) {
			TerrainPageSurfaceLayer* surfaceLayer = I->second;
			if (I == terrainPageSurfaces.begin()) {
				shaderPass.addBaseLayer(surfaceLayer);
			} else {
				if (surfaceLayer->intersects()) {
					shaderPass.addLayer(surfaceLayer);
				}
			}
		}
		shaderPass.finalize();
	}
// 	if (terrainPageShadow) {
// 		addShadow(technique, terrainPageShadow);
// 	}
}

TerrainPageSurfaceCompilerShaderPass::TerrainPageSurfaceCompilerShaderPass(Ogre::Pass* pass, Ogre::TexturePtr combinedCoverageTexture)
: 
mCombinedCoverageImage(new Ogre::Image())
, mCombinedCoverageDataStream(new Ogre::MemoryDataStream(combinedCoverageTexture->getWidth() * combinedCoverageTexture->getWidth() * 4, false))
, mCombinedCoverageDataStreamPtr(mCombinedCoverageDataStream)
, mPass(pass)
, mCombinedCoverageTexture(combinedCoverageTexture)
, mCurrentLayerIndex(0)
{
	///reset the coverage image
	memset(mCombinedCoverageDataStream->getPtr(), '\0', mCombinedCoverageDataStream->size());
	
	///add our coverage texture first
	Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
	textureUnitState->setTextureScale(1, 1);
	textureUnitState->setTextureName(mCombinedCoverageTexture->getName());
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

}

void TerrainPageSurfaceCompilerShaderPass::addBaseLayer(TerrainPageSurfaceLayer* layer)
{
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	S_LOG_VERBOSE("Adding new base layer with diffuse texture " << layer->getDiffuseTextureName() << " (" << numberOfTextureUnitsOnCard << " texture units supported)");	
	///add the first layer of the terrain, no alpha or anything
	Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
	//textureUnitState->setTextureScale(0.025, 0.025);
	textureUnitState->setTextureName(layer->getDiffuseTextureName());
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	mScales[0] = layer->getScale();
}

void TerrainPageSurfaceCompilerShaderPass::addLayer(TerrainPageSurfaceLayer* layer)
{
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	if (mCurrentLayerIndex < std::min<unsigned short>(numberOfTextureUnitsOnCard - 1, 4)) {
		S_LOG_VERBOSE("Adding new layer with diffuse texture " << layer->getDiffuseTextureName() << " and scale " << layer->getScale() << " at index "<< (mCurrentLayerIndex + 1) <<" (" << numberOfTextureUnitsOnCard << " texture units supported)");
		Ogre::TextureUnitState * textureUnitState = mPass->createTextureUnitState();
// 		textureUnitState->setTextureScale(0.025, 0.025);
		textureUnitState->setTextureName(layer->getDiffuseTextureName());
		textureUnitState->setTextureCoordSet(0);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		
		addCoverage(layer->getCoverageImage(), mCurrentLayerIndex, 4);
		
		mCurrentLayerIndex++;
		mScales[mCurrentLayerIndex] = layer->getScale();
	}
}

void TerrainPageSurfaceCompilerShaderPass::addCoverage(Ogre::Image* coverage, unsigned int channel, unsigned short numberOfChannels) {
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

void TerrainPageSurfaceCompilerShaderPass::assignCombinedCoverageTexture() {
	mCombinedCoverageDataStreamPtr->seek(0);
	mCombinedCoverageImage->loadRawData(mCombinedCoverageDataStreamPtr, getCoveragePixelWidth(), getCoveragePixelWidth(), Ogre::PF_B8G8R8A8);
	mCombinedCoverageTexture->loadImage(*mCombinedCoverageImage);
}

void TerrainPageSurfaceCompilerShaderPass::finalize()
{
	assignCombinedCoverageTexture();
	try {
		mPass->setLightingEnabled(false);
		mPass->setFog(true, Ogre::FOG_NONE);
		
		//add vertex shader for fog	
		mPass->setVertexProgram("fog_linear_vp");
		
		try {
			Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getVertexProgramParameters();
	// 		fpParams->setAutoAddParamName(true);
			//set how much the texture should tile, perhaps this shouldn't be placed here...
	// 		fpParams->setNamedConstant("tile",  50);
			fpParams->setNamedAutoConstant("iWorldViewProj", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX );
			
		} catch (const Ogre::Exception& ex) {
			S_LOG_WARNING("Error when setting fragment program parameters. Message:\n" << ex.what());
			
		}
				
		//add fragment shader for splatting
		mPass->setFragmentProgram("splat_cg");
		try {
			Ogre::GpuProgramParametersSharedPtr fpParams = mPass->getFragmentProgramParameters();
	// 		fpParams->setAutoAddParamName(true);
			//set how much the texture should tile, perhaps this shouldn't be placed here...
	// 		fpParams->setNamedConstant("tile",  50);
			fpParams->setNamedAutoConstant("iFogParams", Ogre::GpuProgramParameters::ACT_FOG_PARAMS);
			fpParams->setNamedAutoConstant("iFogColour", Ogre::GpuProgramParameters::ACT_FOG_COLOUR);
			fpParams->setNamedConstant("iNumberOfLayers", (float)mCurrentLayerIndex); //4*4=16
			fpParams->setNamedConstant("iScales", mScales, 4); //4*4=16
			
		} catch (const Ogre::Exception& ex) {
			S_LOG_WARNING("Error when setting fragment program parameters. Message:\n" << ex.what());
			
		}
		
	} catch (const Ogre::Exception&) {
		///if there was some kind of error, go with the simple technique
		S_LOG_INFO("Falling back to simple technique.");
// 		throw;
	}	
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

Ogre::Pass* TerrainPageSurfaceCompilerTechniqueShader::createPass(Ogre::Technique* technique)
{
	Ogre::Pass* pass = technique->createPass();
}


// void TerrainPage::generateTerrainTechniqueComplex(Ogre::Technique* technique)
// { 
//  
// 	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
// 
// 	Ogre::TextureUnitState * alphaTextureUnitState = pass->createTextureUnitState( );
// 	
// 	
// 	//use this iterator to iterate over all shaders we will use. The first shader will be used as a base (i.e. with no alpha map), so make sure that it's something like rock or dirt (upon which one would want to put some grass etc.)
// 	std::list<TerrainShader*>::iterator shadersIterator = mUsedShaders.begin();
// 
// 	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
// 	Mercator::Segment* aValidSegment = 0;
// 	
// 	if (mValidSegments.begin() == mValidSegments.end()) {
// 		return;
// 	}
// 	aValidSegment = mValidSegments.begin()->segment;
// 	
// 	Mercator::Surface* surface = (*shadersIterator)->getSurfaceForSegment(aValidSegment);
//  
// //	pass->setShininess(20);
// //	pass->setSpecular(1,1,1,1);
// 
// //granite layer
// 	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
// 	{
// 	    textureUnitState->setTextureName((*shadersIterator)->getTextureName());
// //	    textureUnitState->setTextureName("splat3d.dds");
// 		//textureUnitState->setTextureScale(0.01, 0.01);
// 		textureUnitState->setTextureCoordSet(0);
// 		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
// 	}
// 
// 	
// 	//we need an unique name for our alpha texture
// 	std::stringstream splatTextureNameSS;
// 	splatTextureNameSS << mMaterialName;
// 	Ogre::String splatTextureName(splatTextureNameSS.str());
// 	
// 	//the format for our alpha texture
// 	Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;
// 	
// // 	//let's create our map image
// // 	ILuint mapImageName;
// // 	ilGenImages( 1, &mapImageName );
// // 	ilBindImage( mapImageName );
// // 	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, 0 );
// // 	unsigned char * mapImagePointer = ilGetData();
// 
// 	
// 	
// 
// 	ILuint ImageName;
// 	ilGenImages( 1, &ImageName );
// 	ilBindImage( ImageName );
// 	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, 0 );
// 	unsigned char * imagePointer = ilGetData();
// 	
// 	//clear the image
// 	memset (imagePointer, 0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel);
// 	
// 	Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, false);
// 	
// 	
// 	
// 		  
// 	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
// 	
// /*	int numberOfSegmentsToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
// 	for (int i = 0; i < numberOfSegmentsToDo; ++i) {*/
// 	
// 	size_t numberOfbaseTextureUnits = pass->getNumTextureUnitStates();
// 	++shadersIterator;
// 	for (; shadersIterator != mUsedShaders.end() ; ++shadersIterator) {
// 
// //		TerrainShader* shader;
// 		
// 		//check if at least one surface instersects, else continue
// 		bool intersects = false;
// 	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
// 		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 			if ((*shadersIterator)->getShader()->checkIntersect(*I->segment)) {
// 				intersects = true;
// 			}
// 		}
// 
// /*		if (!intersects) {
// 			continue;
// 		}*/
// 		
// 		
// 		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 			Mercator::Segment* segment = I->segment;
// 			if (segment && segment->isValid() && pass->getNumTextureUnitStates()  < 4) {
// 				
// 				surface = (*shadersIterator)->getSurfaceForSegment(segment);
// 				if (surface && surface->isValid()) {
// 					//use four channels
// 					fillAlphaLayer(finalChunk->getPtr(), surface->getData(), pass->getNumTextureUnitStates() - numberOfbaseTextureUnits, ((int)I->pos.x() * 64), (getNumberOfSegmentsPerAxis() - (int)I->pos.y() - 1) * 64, 4);
// 				}
// 			}
// 		}
// 		//TODO: make it not hard coded to 4
// 		if (pass->getNumTextureUnitStates()  < 4) {
// 			Ogre::TextureUnitState * splatTextureUnitState = pass->createTextureUnitState();
// 			splatTextureUnitState->setTextureName((*shadersIterator)->getTextureName());
// 			splatTextureUnitState->setTextureCoordSet(0);
// 			//splatTextureUnitState->setTextureScale(0.01, 0.01);
// 			splatTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
// 		}
// 		
// 		
// 	}
// 	
// 	
// 	//printTextureToImage(Ogre::MemoryDataStreamPtr(new Ogre::MemoryDataStream(*finalChunk, true)), splatTextureName, pixelFormat, getAlphaTextureSize(), getAlphaTextureSize());
// 	
// 	ilBindImage(ImageName);
// /*	char name[100];
// 	strcpy(name, (std::string("/home/erik/") + splatTextureName + std::string(".png")).c_str());
// 	ilSaveImage(name);*/
// 		
// 	if (getAlphaMapScale() > 1) {
// 		
// 		//smooth out the terrain
// 		//do this by enlarging the image and apply a blur filter to it
// 		
// 		//use filter to smooth everything out
// 		iluImageParameter(ILU_FILTER, ILU_SCALE_BSPLINE);
// 		iluScale(getAlphaTextureSize() * getAlphaMapScale(), getAlphaTextureSize() * getAlphaMapScale(), 1);
// 		imagePointer = ilGetData();
// 		//wrap the image data in a Datachunk
// 		delete finalChunk;
// 		finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel * (getAlphaMapScale() * getAlphaMapScale()) , false);
// 	}
// 		
// 	Ogre::DataStreamPtr dataStreamPtr(finalChunk);
// 	
// 
// 	
// 	
// 	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize() * getAlphaMapScale(), getAlphaTextureSize() * getAlphaMapScale(), pixelFormat);
// /*	temp.setNull();
// 	splatTexture.setNull();*/
// 	if (getAlphaMapScale() > 1) {
// 		ilDeleteImages(1, &ImageName);
// 	}
// 	alphaTextureUnitState->setTextureName(splatTextureName);
//     alphaTextureUnitState->setTextureCoordSet(0);
// 	alphaTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
// 
//  
// 
//  
// }




}

}
