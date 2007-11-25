//
// C++ Implementation: TerrainPage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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


#include "TerrainPage.h"

#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "TerrainShader.h"

#include "../EmberOgre.h"

#include <OgreRenderSystemCapabilities.h>

#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>
#include <OgreCommon.h>

#include <IL/il.h>
#include <IL/ilu.h>

#include "../environment/Foliage.h"
#include "../environment/FoliageArea.h"
#include "TerrainGenerator.h"
#include "ISceneManagerAdapter.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

#include <OgreHardwarePixelBuffer.h>

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"

//#include <fenv.h>

using namespace EmberOgre::Environment;

namespace EmberOgre {
namespace Terrain {

TerrainPage::TerrainPage(TerrainPosition position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator) 
: mFoliageArea(0)
, mGenerator(generator)
, mPosition(position)
// , mShaderMap(shaderMap)
// , mBytesPerPixel(4)
, mTerrainSurface(new TerrainPageSurface(*this))
, mShadow(*this)
, mShadowTechnique(0)
{

	S_LOG_VERBOSE("Creating TerrainPage at position " << position.x() << ":" << position.y());
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegment(x,y);
			if (segment && segment->isValid()) {
 				//S_LOG_VERBOSE("Segment is valid.");
				PageSegment pageSegment;
				pageSegment.pos = TerrainPosition(x,y);
				pageSegment.segment = segment;
				mValidSegments.push_back(pageSegment);
			}
		}
	}
	S_LOG_VERBOSE("Number of valid segments: " << mValidSegments.size());
	setupShadowTechnique();
	mTerrainSurface->setShadow(&mShadow);
}

TerrainPage::~TerrainPage()
{
	delete mShadowTechnique;
}

Mercator::Segment* TerrainPage::getSegment(int x, int y) const
{
// 	const TerrainInfo& info = mGenerator->getTerrainInfo();
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	//the mPosition is in the middle of the page, so we have to use an offset to get the real segment position
	//int segmentOffset = static_cast<int>(info.getWorldSizeInSegments(). x() * 0.5);
	int segmentOffset = segmentsPerAxis;
	int segX = (int)((mPosition.x() * segmentsPerAxis) + x);
	int segY = (int)((mPosition.y() * segmentsPerAxis) + y) - segmentOffset;
	
	//S_LOG_VERBOSE("Added segment with position " << segX << ":" << segY);

	return mGenerator->getTerrain().getSegment(segX, segY);
}

int TerrainPage::getPageSize() const 
{
	return mGenerator->getPageSize();
}
		

int TerrainPage::getNumberOfSegmentsPerAxis() const
{
	return (getPageSize() -1) / 64;
}
	
float TerrainPage::getMaxHeight()
{
	float max = 0;
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		max = std::max<float>(max, I->segment->getMax());
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	float min = 0;
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		min = std::min<float>(min, I->segment->getMin());
	}
	return min;
}

void TerrainPage::update()
{
	Ogre::Vector2 targetPage = Atlas2Ogre_Vector2(mPosition);
	
	Ogre::Vector2 adjustedOgrePos(targetPage.x + mGenerator->getTerrainInfo().getPageOffsetX(), targetPage.y + mGenerator->getTerrainInfo().getPageOffsetY());
	
	S_LOG_VERBOSE("Updating terrain page at position x: " << adjustedOgrePos.x << " y: " << adjustedOgrePos.y);
	mGenerator->getAdapter()->reloadPage(static_cast<unsigned int>(adjustedOgrePos.x), static_cast<unsigned int>(adjustedOgrePos.y)); 

}

void TerrainPage::setupShadowTechnique()
{
	if (mShadowTechnique) {
		delete mShadowTechnique;
	}
	mShadowTechnique = new SimpleTerrainPageShadowTechnique();
	mShadow.setShadowTechnique(mShadowTechnique);
}

void TerrainPage::createShadow(const Ogre::Vector3& lightDirection)
{
	mShadow.setLightDirection(lightDirection);
	mShadow.createImage();
/*	Ogre::Technique* technique = mMaterial->getTechnique(0);
	Ogre::Pass* shadowPass = technique->createPass();
	
 	shadowPass->setSceneBlending(Ogre::SBT_MODULATE);
	shadowPass->setLightingEnabled(false);
	shadowPass->setFog(false);
	
	
	Ogre::TextureUnitState * textureUnitStateSplat = shadowPass->createTextureUnitState();
    textureUnitStateSplat->setTextureName(mShadow.getTexture()->getName());
     
    textureUnitStateSplat->setTextureCoordSet(0);
//	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
    textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
// 	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);*/
	
}

void TerrainPage::updateShadow(const Ogre::Vector3& lightDirection)
{
	mShadow.setLightDirection(lightDirection);
	mShadow.updateShadow();
}


Ogre::MaterialPtr TerrainPage::generateTerrainMaterials() {

	mTerrainSurface->recompileMaterial();
	mTerrainSurface->getMaterial()->load();
	return mTerrainSurface->getMaterial();
// 	//Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
// 
// 	if (mMaterial.isNull()) {
// 	
// 		//create a name for out material
// 		S_LOG_INFO("Creating a material for the terrain.");
// 		std::stringstream materialNameSS;
// 		materialNameSS << "EmberTerrain_Segment";
// 		materialNameSS << "_" << mPosition.x() << "_" << mPosition.y();
// 		mMaterialName = materialNameSS.str();
// 	
// 		//create the actual material
// 		mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(mMaterialName, "General"));
// 	} else {
// 		mMaterial->removeAllTechniques();
// 		mMaterial->createTechnique();
// 	}
// 
// 	//we'll use at least two different techniques
// 	//for modern GPU's we'll use a technique which uses fragment shaders
// 	if ((Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0")) &&
// 		(Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1") || Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0"))
// 	) {
// 		//generateTerrainTechniqueComplexAtlas(mMaterial->getTechnique(0));
// 		S_LOG_INFO("Try to create a complex material.");
// //		generateTerrainTechniqueComplex(mMaterial->getTechnique(0));
// 		generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
// 	} else {
// 	//and as a fallback for older gfx cards we'll supply a technique which doesn't
// 		S_LOG_INFO("Try to create a simple material.");
// 		generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
// 	}
// // 	generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
// 	
// 	//and if the user so wants, we'll add some debug materials
// /*	if (getTerrainOptions().debuglod) {
// 		generateTerrainTechniqueDebug();
// 	}*/
// 		
// 	mMaterial->load();
// 	
// 	return mMaterial;

}
	
SegmentVector& TerrainPage::getValidSegments()
{
	return mValidSegments;
}

// inline const Ogre::TerrainOptions& TerrainPage::getTerrainOptions() const
// {
// 	return mGenerator->getTerrainOptions();
// }

long TerrainPage::getVerticeCount() const
{
	return (getPageSize()) *(getPageSize());
}

const TerrainPosition& TerrainPage::getWFPosition() const
{
	return mPosition;
}


Ogre::MaterialPtr TerrainPage::getMaterial()
{
//	generateTerrainMaterials();
	return mTerrainSurface->getMaterial();
}

unsigned int TerrainPage::getAlphaMapScale() const
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	if (configSrv->itemExists("terrain", "scalealphamap")) {
		int value = (int)configSrv->getValue("terrain", "scalealphamap");
		//make sure it can't go below 1
		return std::max<int>(1, value);
	} else {
		return 1;
	}
}



void TerrainPage::createHeightData(Ogre::Real* heightData)
{
	int pageSizeInVertices = getPageSize();
	int pageSizeInMeters = pageSizeInVertices - 1;
	
	///since Ogre uses a different coord system than WF, we have to do some conversions here
	TerrainPosition origPosition(mPosition);
	///start in one of the corners...
	origPosition[0] = (mPosition[0] * (pageSizeInMeters));
	origPosition[1] = (mPosition[1] * (pageSizeInMeters));
	
	S_LOG_INFO("Page x:" << mPosition.x() << " y:" << mPosition.y() << " starts at x:" << origPosition.x() << " y:" << origPosition.y());
	 
	TerrainPosition position(origPosition);
	
	for (int i = 0; i < pageSizeInVertices; ++i) {
		position = origPosition;
		position[1] = position[1] - i;
		for (int j = 0; j < pageSizeInVertices; ++j) {
			Ogre::Real height = mGenerator->getHeight(position);
			*(heightData++) = height;
			position[0] = position[0] + 1;
		}
	}
	
}



// Ogre::MemoryDataStreamPtr TerrainPage::convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor) {
//     //int width = getTerrainOptions().pageSize - 1;
//     int width = 64;
//     int bufferSize = width*width*mBytesPerPixel*factor*factor;
// 	Ogre::MemoryDataStream chunk(dataStart, 65*65, false);
//     Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(bufferSize);
//     //finalChunk->allocate(bufferSize);
//     Ogre::uchar* finalPtr = finalChunk->getPtr();
//     Ogre::uchar* chunkPtr = chunk.getPtr();
//     long i,j; 
//     long sizeOfOneChannel = width*width;
// 
//      Ogre::uchar* tempPtr = finalPtr + bufferSize;
//     for (i = 0; i < width; ++i) {
//     	for (int l = 0; l < factor; ++l) {
// 	    	tempPtr -= (width * mBytesPerPixel * factor);
// 	    	for (j = 0; j < width; ++j) {
// 	        	Ogre::uchar alpha = *(chunkPtr + j);
// 	        	for (int k = 0; k < factor; ++k) {
// 		        	*(tempPtr++) = 0;
// 		        	*(tempPtr++) = 0;
// 		        	*(tempPtr++) = 0;
// 		        	*(tempPtr++) = alpha;
// 	        	}
// 	    		
// 	    	}
// 	    	tempPtr -= (width * mBytesPerPixel * factor);
//     	}
//     	chunkPtr += 65;
//     	//++chunkPtr;
//     }
//    /*
//     Ogre::uchar* tempPtr = finalPtr;
//     for (i = 0; i < width; ++i) {
//     	for (j = 0; j < width; ++j) {
//         	Ogre::uchar alpha = *(chunkPtr++);
//         	*tempPtr++ = 0;
//         	*tempPtr++ = 0;
//         	*tempPtr++ = 0;
//         	*tempPtr++ = alpha;
//     		
//     	}
//     	++chunkPtr;
//     }   
//     */
//     
// 	return Ogre::MemoryDataStreamPtr(finalChunk);
// }

// Ogre::TexturePtr TerrainPage::createAlphaTexture(Ogre::String name, Mercator::Surface* surface) {
// 
// 
//     //the format for our alpha texture
//     //for some reason we can't use PF_A8
//     Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;
// 
// 	/*first we need to change the 8 bit mask into 32 bits
// 	 * because somehow Ogre doesn't like the 8 bit mask
// 	 * 
// 	 */
// 	Ogre::MemoryDataStreamPtr finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), 1);
// 	
// 	//printTextureToImage(finalChunk, name, pixelFormat);
// 	Ogre::DataStreamPtr temp(finalChunk.get());
// 	//create the new alpha texture
// 	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(name, "General", temp, getAlphaTextureSize(), getAlphaTextureSize(), pixelFormat);
// //	finalChunk->clear();
// //	delete finalChunk;
// 	
// 	return splatTexture;
// 	
// }

// void TerrainPage::printTextureToImage(Ogre::MemoryDataStreamPtr dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat, int height, int width) {
// // DEBUG   
// //prints the bitmap to a png-image
// //TODO: remove when finished with debugging
// 	
// 	std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory();
// 	if (!oslink::directory(dir).isExisting()) {
// 	
// 		const Ogre::String extension = "png";
// 		
// 		Ogre::ImageCodec::ImageData* imgData = new Ogre::ImageCodec::ImageData();
// 		imgData->width = width;
// 		imgData->height = height;
// 		
// 		imgData->depth =  1;
// 		imgData->format = pixelFormat;	
// 					
// 		Ogre::Codec * pCodec = Ogre::Codec::getCodec(extension);
// 		// Write out
// 		Ogre::SharedPtr<Ogre::Codec::CodecData> temp(imgData);
// 		
// 		pCodec->codeToFile(dataChunk, dir + "/" + name + "." + extension, temp);
// 	}
// 	
// }

// void TerrainPage::createShadow()
// {
// 	Mercator::Surface* surface;
// 	
// 	Ogre::MemoryDataStream* shadowChunk = new Ogre::MemoryDataStream(getAlphaTextureSize() * getAlphaTextureSize() * 1, false);
// 	
// 	memset( shadowChunk->getPtr(), '\0', shadowChunk->size());
// 	
// 	///we need an unique name for our alpha texture
// 	std::stringstream shadowTextureNameSS;
// 	shadowTextureNameSS << mMaterialName << "_shadow";
// 	const Ogre::String shadowTextureName(shadowTextureNameSS.str());
// 	
// 	mShadow.createShadowData(shadowChunk->getPtr());
// 	
// 	Ogre::DataStreamPtr dataStreamPtr(splatChunk);
// 		
// 	Ogre::Image* image = new Ogre::Image();
// 	image->loadRawData(dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_A8);
// 
// 	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(shadowTextureName, "General", Ogre::TEX_TYPE_2D, getAlphaTextureSize(), getAlphaTextureSize(), 1, Ogre::PF_A8);
// 	splatTexture->loadImage(*image);
// 		
// 
// 
// }





// void TerrainPage::fillAlphaLayer(unsigned char* finalImagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY, unsigned short numberOfChannels) {
// 
//     int width = 64;
//  	int finalImageWidth = getAlphaTextureSize( );
//     long i,j; 
// 	
//     Ogre::uchar* start = finalImagePtr + (numberOfChannels * finalImageWidth * (startY - 1)) + ((startX - 1) * numberOfChannels);
//     Ogre::uchar* end = start + (width * finalImageWidth * numberOfChannels) + (width * numberOfChannels);
// 	///we need to do this to get the alignment correct
// 	wfImagePtr += 65;
// 	
//     Ogre::uchar* tempPtr = end + channel + numberOfChannels;
//     for (i = 0; i < width; ++i) {
// 	    tempPtr -= (width * numberOfChannels);
// 		for (j = 0; j < width; ++j) {
// 			Ogre::uchar alpha = *(wfImagePtr + j);
// 			*(tempPtr) = alpha;
// 			///advance the number of channels
// 			tempPtr += numberOfChannels;
// 			
// 		}
// 		tempPtr -= (finalImageWidth * numberOfChannels);
// 		wfImagePtr += 65;
//     }
// }

void TerrainPage::showFoliage()
{
//  	if (mPosition.x() == 0 && mPosition.y() == 0) {
	if (!mFoliageArea) {
		prepareFoliage();
	}
	if (mFoliageArea) {
		mFoliageArea->setVisible(true);
	}
//  	}
	
}

void TerrainPage::hideFoliage()
{
	if (!mFoliageArea) {
		return;
	}
	mFoliageArea->setVisible(false);
}

void TerrainPage::destroyFoliage()
{
	if (!mFoliageArea) {
		return;
	}
	Foliage::getSingleton().destroyArea(mFoliageArea);
	mFoliageArea = 0;
}



void TerrainPage::prepareFoliage()
{
	TerrainShader* grassShader = mGenerator->getFoliageShader();
	
	if (!grassShader) {
		S_LOG_FAILURE("Could not create foliage since there's no grass shader registered.");
		return;
	}
	if (!mFoliageArea) {
		mFoliageArea = Foliage::getSingleton().createArea();
	} else {
		return;
	}
	double grassSpacing = Foliage::getSingleton().getGrassSpacing();
	
	//for each 1 m^2, how big chance is there of grass? [0,1.0]
	Ogre::Real grassChanceThreshold = 1.0 / grassSpacing; 
	
	int pageSizeInVertices = getPageSize();
	int pageSizeInMeters = pageSizeInVertices - 1;
	
	///since Ogre uses a different coord system than WF, we have to do some conversions here
	TerrainPosition worldUnitsStartOfPage(mPosition);
	///start in one of the corners...
	worldUnitsStartOfPage[0] = (mPosition[0] * (pageSizeInMeters));
	worldUnitsStartOfPage[1] = (mPosition[1] * (pageSizeInMeters));
	
	
	//for now, just check with the grass shader
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		Mercator::Segment* segment = I->segment;
		Mercator::Surface* surface = grassShader->getSurfaceForSegment(segment);
		if (!surface || !surface->isValid()) {
			//we could not find any grass surface
			continue;
		}
		int segmentStartX = static_cast<int>(worldUnitsStartOfPage.x() + (I->pos.x() * 64));
		int segmentStartY = static_cast<int>(worldUnitsStartOfPage.y() + (I->pos.y() * 64) - pageSizeInMeters);
		std::stringstream ss;
/*		ss << "Adding grass to the area between x: " << segmentStartX << " to  " <<  (segmentStartX + 64) << " and y: " << segmentStartY << " to " << (segmentStartY + 64);
		S_LOG_VERBOSE(ss.str());*/
		for (int x = 0; x < 64; ++x) {
			for (int y = 0; y < 64; ++y) {
				//some random offset
				Ogre::Real x_offset = Ogre::Math::RangeRandom(-0.5, 0.5);
				Ogre::Real y_offset = Ogre::Math::RangeRandom(-0.5, 0.5);
				TerrainPosition position(segmentStartX + x + x_offset, segmentStartY + y + y_offset);
				float height = mGenerator->getHeight(position);
				
				if (height > 0) {
					bool blocked = false;
					for (TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator J = mTerrainSurface->getLayers().begin(); J != mTerrainSurface->getLayers().end(); ++J) {
						if (J->second->getShader() != grassShader->getShader()  && J->first) {
							Mercator::Surface* blockingSurface = J->second->getSurfaceForSegment(segment);
							if (blockingSurface && blockingSurface->isValid()) {
								unsigned char blockingAmount = ((*blockingSurface)(x, y, 0));
								if (blockingAmount > 10) {
									blocked = true;
									break;
								}
							}
						}
					}
					//blocked = false;
					if (!blocked) {
				
						//first check if this is a good spot for grass (the more "green", the greater chance of grass
						//then check with grassChanceThreshold if we should do grass
						unsigned char cover = (*surface)(x, y, 0);
						unsigned char coverChance = static_cast<unsigned char>( Ogre::Math::RangeRandom(0, 260));
 						if (cover > coverChance) {
							Ogre::Real grassChance = Ogre::Math::UnitRandom();
							if (grassChanceThreshold > grassChance) {
 								const Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.5, 0.8), 1);

//								std::string type("placeholder");
								std::string type("bittergrass");
								Ogre::Vector3 ogrePosition = Atlas2Ogre(position);
								ogrePosition.y = height;
								if (Ogre::Math::UnitRandom() > 0.95) {
									type = "heartblood";
								}
								if (height > 1000 || height < -1000) {
									S_LOG_WARNING("Incorrect height (" << height << ") set for foliage position x: " << position.x() << " y: " << position.y());
								} else {
									mFoliageArea->placeGrass(type, ogrePosition, scale);
								}
							}
						}
					}
				}
			}
		}
	}
	mFoliageArea->build();
	mFoliageArea->setVisible(false);
	return;
	
}

void TerrainPage::addShader(TerrainShader* shader)
{
	TerrainPageSurfaceLayer* layer = mTerrainSurface->createSurfaceLayer(shader->getTerrainIndex(), shader->getShader());
	layer->setDiffuseTextureName(shader->getLayerDefinition()->getDiffuseTextureName());
	layer->setNormalTextureName(shader->getLayerDefinition()->getNormalMapTextureName());
	///get the scale by dividing the total size of the page with the size of each tile
	float scale = getAlphaTextureSize() / shader->getLayerDefinition()->getTileSize();
	layer->setScale(scale);
	layer->updateCoverageImage();

/*	mUsedShaders.push_back(shader);
	///if the material already has been created, add the shader instantly, else wait until the generateTerrainMaterials method is called
	
	if (!mMaterial.isNull()) {
//		populateSurfaces();
		addShaderToSimpleTechnique(mMaterial->getTechnique(0), shader);
	}*/
}



// void TerrainPage::generateTerrainTechniqueComplexAtlas( Ogre::Technique* technique)
// { 
//  
// 	Ogre::Pass* pass = technique->getPass(0);
// 	//pass->setLightingEnabled(false);
// 	//pass->setFragmentProgram("splat3arb");
// 	pass->setFragmentProgram("splatatlas_cg");
// 	
// 	
// 	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
// 
// 	Ogre::TextureUnitState * alphaTextureUnitState = pass->createTextureUnitState();
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
// 		  
// 	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
// 	
// /*	int numberOfSegmentsToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
// 	for (int i = 0; i < numberOfSegmentsToDo; ++i) {*/
// 	
// // 	size_t numberOfbaseTextureUnits = pass->getNumTextureUnitStates();
// 	int splattextureChannel = 0;
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
// 			if (segment && segment->isValid() && splattextureChannel < 4) {
// 				
// 				surface = (*shadersIterator)->getSurfaceForSegment(segment);
// 				if (surface && surface->isValid()) {
// 					
// 					fillAlphaLayer(finalChunk->getPtr(), surface->getData(), splattextureChannel, (int)I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - (int)I->pos.y() - 1) * 64, 4);
// 				}
// 			}
// 		}
// 		++splattextureChannel;
// 	}
// 	Ogre::TextureUnitState * splatTextureUnitState = pass->createTextureUnitState();
// 	splatTextureUnitState->setTextureName("atlassplat.png");
// 	splatTextureUnitState->setTextureCoordSet(1);
// 	splatTextureUnitState->setTextureScale(0.025, 0.025);
//     splatTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
// 
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
//  }
// 


// void TerrainPage::generateTerrainTechniqueComplex( Ogre::Technique* technique)
// { 
//  
// 	S_LOG_VERBOSE("Entered generateTerrainTechniqueComplex.");
// 	Ogre::Pass* pass = technique->getPass(0);
// 	//pass->setLightingEnabled(false);
// 	
// 	try {
// 		//add fragment shader for splatting
// 		pass->setFragmentProgram("splat_cg");
// 		
// 		Ogre::GpuProgramParametersSharedPtr fpParams = Ogre::GpuProgramManager::getSingleton().createParameters();
// // 		fpParams->setAutoAddParamName(true);
// 		//set how much the texture should tile, perhaps this shouldn't be placed here...
// 		fpParams->setNamedConstant("tile",  50);
// 		pass->setFragmentProgramParameters(fpParams);
// 		
// 		
// 		//add vertex shader for fog	
// 		pass->setVertexProgram("fog_linear_vp");
// 	} catch (const Ogre::Exception&) {
// 		//if there was some kind of error, go with the simple technique
// 		S_LOG_INFO("Falling back to simple technique.");
// 		generateTerrainTechniqueSimple(technique);
// 		return;
// 	}
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
//  }
//  
// void TerrainPage::generateTerrainTechniqueDebug()
// {
// 	for (int i = 1; i < 5; ++i) {
// 		Ogre::Technique *tech = mMaterial->createTechnique();
// 		tech->setLodIndex(i);
// 		
// 		Ogre::TextureUnitState *textureUnitState = tech->createPass()->createTextureUnitState();
// 		std::stringstream ss;
// 		ss << "lod" << i << ".png";
// 		textureUnitState->setTextureName(ss.str());
// 		textureUnitState->setTextureCoordSet(0);
// 	
// 	}
// }
// 
// void TerrainPage::generateTerrainTechniqueSimple( Ogre::Technique* technique)
// {
// 	S_LOG_VERBOSE("Entered generateTerrainTechniqueSimple.");
// 
// 	Mercator::Segment* segment = mGenerator->getTerrain().getSegment((int)mPosition.x(), (int)mPosition.y());
//  
// // 	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
// 	
// 	
// 	std::list<TerrainShader*>::iterator shadersIterator = mUsedShaders.begin();
// 			
// 	
// 	
// 	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
// 	Mercator::Segment* aValidSegment = 0;
// 	
// 	if (mValidSegments.begin() == mValidSegments.end()) {
// 		return;
// 	}
// 	aValidSegment = mValidSegments.begin()->segment;
// 	
// // 	Mercator::Surface* surface = (*shadersIterator)->getSurfaceForSegment(aValidSegment);
// // 	Mercator::Surface* surface = *aValidSegment->getSurfaces().begin();
//  
// 	Ogre::Pass* pass = technique->getPass(0);
// 	//pass->setSelfIllumination(Ogre::ColourValue(0.5f,0.5f,0.5f));	
// 	//pass->setLightingEnabled(false);
// 	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
// 	
// 	   
//     const Mercator::Segment::Surfacestore & surfaces = segment->getSurfaces();
//     Mercator::Segment::Surfacestore::const_iterator I = surfaces.begin();
// 
// //	pass->setShininess(20);
// //	pass->setSpecular(1,1,1,1);
// 	///add the first layer of the terrain, no alpha or anything
// 	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
// 	textureUnitState->setTextureScale(0.025, 0.025);
// 	textureUnitState->setTextureName((*shadersIterator)->getTextureName());
//     textureUnitState->setTextureCoordSet(0);
// 	
// 	
// 	
// 	
// 	
// 	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
// 	
// // 	int numberOfSurfacesToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
// // 	for (int i = 0; i < numberOfSurfacesToDo; ++i) {
// 	++shadersIterator;
// 	for (; shadersIterator != mUsedShaders.end() ; ++shadersIterator) {
// 		addShaderToSimpleTechnique(technique, *shadersIterator);
// 	}
// 	
// 	
// 
// }


void TerrainPage::populateSurfaces()
{
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);


	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		I->segment->populateSurfaces();
	}
//	fesetround(FE_DOWNWARD);
}

void TerrainPage::updateAllShaderTextures()
{
	TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainSurface->getLayers().begin();

	///skip the first texture, since it's the ground, and doesn't have an alpha texture
	++I;
	for (; I != mTerrainSurface->getLayers().end(); ++I) {
		mTerrainSurface->updateLayer(I->first);
	}
}

void TerrainPage::updateShaderTexture(TerrainShader* shader)
{

	if (mTerrainSurface->getLayers().find(shader->getTerrainIndex()) == mTerrainSurface->getLayers().end()) {
		addShader(shader);
		mTerrainSurface->recompileMaterial();
	} else {
		mTerrainSurface->updateLayer(shader->getTerrainIndex());
		mTerrainSurface->recompileMaterial();
	}
	
	///check if at least one surface intersects, else continue
// 	bool intersects = false;
// 	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 		if (shader->getShader()->checkIntersect(*I->segment)) {
// 			intersects = true;
// 			break;
// 		}
// 	}
// 	if (!intersects) {
// 		return;
// 	}	
// 	
// 	bool found = false;
// 	for (std::list<TerrainShader*>::iterator I = mUsedShaders.begin(); I != mUsedShaders.end(); ++I) {
// 		if (*I == shader) {
// 			found = true;
// 			break;
// 		}
// 	}
// 	if (!found) {
// 		addShader(shader);
// 	} else if (mShaderTextures.find(shader) == mShaderTextures.end()) {
// 		addShaderToSimpleTechnique(mMaterial->getTechnique(0), shader);
// 	} else {
// 	
// 	
// 		//update the alpha texture
// 		//we do this by first creating a new, empty data chunk
// 		//we fill this chunk with the correct alpha values through the fillAlphaLayer method
// 		//from this chunk we'll then create a temporary image
// 		//we'll then blit from the image directly to the hardware buffer
// 		//and when we go out of scope the temporary image is deleted along with the data chunk
// 	
// 		Ogre::TexturePtr texture = mShaderTextures.find(shader)->second;
// 		
// 		Mercator::Surface* surface;
// 		
// 		///Create a new image. This image is temporary (since it will be blitted into the hardware memory) and destruction of it will be taken care of by the enveloping DataStreamPtr.
// 		Ogre::MemoryDataStream* splatChunk = new Ogre::MemoryDataStream(getAlphaTextureSize() * getAlphaTextureSize() * 1, true);
// 		Ogre::DataStreamPtr dataStreamPtr(splatChunk);
// 		Ogre::Image image; ///the image to hold the data
// 
// 		///make sure we clear the image
// 		memset( splatChunk->getPtr(), '\0', splatChunk->size());
// 		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 			Mercator::Segment* segment = I->segment;
// 			if (shader->getShader()->checkIntersect(*segment)) {
// 				surface = shader->getSurfaceForSegment(segment);
// 				if (surface && surface->isValid()) {
// 					
// 					int alphaChannel = 0;
// 					//use only one channel
// 					fillAlphaLayer(splatChunk->getPtr(), surface->getData(), alphaChannel, (int)I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - (int)I->pos.y() - 1) * 64, 1);
// 				}
// 			}
// 		}
// 		
// 		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer = texture->getBuffer();
// 		
// 		image.loadRawData(dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_A8);
// 		//image->save(std::string("/home/erik/tempimages/") + texture->getName()  + "_temp" + std::string(".png"));
// 		
// 		
// 		///blit the whole image to the hardware buffer
// 		Ogre::PixelBox sourceBox = image.getPixelBox();
// 		//Ogre::Box targetBox(0,0, texture->getWidth(), texture->getHeight());
// 		hardwareBuffer->blitFromMemory(sourceBox);
// 		
// 	}
	
}
	



// void TerrainPage::addShaderToSimpleTechnique(Ogre::Technique* technique, TerrainShader* shader)
// {
// 	Mercator::Surface* surface;
// 	
// 	///check if at least one surface intersects, else continue
// 	bool intersects = false;
// //	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
// 	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 		if (shader->getShader()->checkIntersect(*I->segment)) {
// 			intersects = true;
// 		}
// 	}
// 	if (!intersects) {
// 		return;
// 	}
// 	
// 	Ogre::MemoryDataStream* splatChunk = new Ogre::MemoryDataStream(getAlphaTextureSize() * getAlphaTextureSize() * 1, false);
// 	
// 	memset( splatChunk->getPtr(), '\0', splatChunk->size());
// 	
// 	///we need an unique name for our alpha texture
// 	std::stringstream splatTextureNameSS;
// 	splatTextureNameSS << mMaterialName << "_" << shader->getTerrainIndex();
// 	const Ogre::String splatTextureName(splatTextureNameSS.str());
// 	
// 			
// /*		SegmentVector::iterator I = segmentI_begin;*/
// 	
// 	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 		if (shader->getShader()->checkIntersect(*I->segment)) {
// 			surface = shader->getSurfaceForSegment(I->segment);
// 			if (surface && surface->isValid()) {
// 			
// 				int alphaChannel = 0;
// 				///use only one channel
// 				fillAlphaLayer(splatChunk->getPtr(), surface->getData(), alphaChannel, (int)I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - (int)I->pos.y() - 1) * 64, 1);
// 	
// 			}
// 		}
// 	}
// 		
// 	Ogre::DataStreamPtr dataStreamPtr(splatChunk);
// 		
// 	Ogre::Image* image = new Ogre::Image();
// 	image->loadRawData(dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_A8);
// 	//image->save(std::string("~/tempimages/") + splatTextureName + "_temp" + std::string(".png"));
// 	
// //	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_B8G8R8A8);
// //	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_A8);
// 	
// //		Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_B8G8R8A8);
// 	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->createManual(splatTextureName, "General", Ogre::TEX_TYPE_2D, getAlphaTextureSize(), getAlphaTextureSize(), 1, Ogre::PF_A8);
// 	splatTexture->loadImage(*image);
// 		
// /*	char name[100];
// 	strcpy(name, (std::string("/home/erik/tempimages/") + splatTextureName + "_temp" + std::string(".png")).c_str());
// 	ilSaveImage(name);			*/
// 		
// 	mShaderTextures[shader] = splatTexture;
// 
// 	shader->addSplatToTechnique(technique, splatTextureName);
/*
* 	TODO: implement this in a more efficient manner
			if (pass->getNumTextureUnitStates() < numberOfTextureUnitsOnCard - 1) {
				//there's room for two more texture unit states
				shader->addTextureUnitsToPass(pass, splatTextureName);
			} else {
				//we need to use a new pass, else we would run out of texture units
				pass = shader->addPassToTechnique(material->getTechnique(0), splatTextureName);
			}
*/			


		
		

// }
}
}
