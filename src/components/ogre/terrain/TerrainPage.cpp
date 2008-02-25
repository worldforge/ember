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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPage.h"

#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "TerrainShader.h"
#include "TerrainPageSurfaceLayer.h"

#include "../EmberOgre.h"

#include <OgreRenderSystemCapabilities.h>

#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>
#include <OgreCommon.h>

// #include <IL/il.h>
// #include <IL/ilu.h>

#include "../environment/Foliage.h"
#include "../environment/FoliageArea.h"
#include "TerrainGenerator.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurface.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageFoliage.h"
#include "ISceneManagerAdapter.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

#include <OgreHardwarePixelBuffer.h>

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#ifdef HAVE_LRINTF
    #define I_ROUND(_x) (::lrintf(_x)) 
#elif defined(HAVE_RINTF)
    #define I_ROUND(_x) ((int)::rintf(_x)) 
#elif defined(HAVE_RINT)
    #define I_ROUND(_x) ((int)::rint(_x)) 
#else
    #define I_ROUND(_x) ((int)(_x)) 
#endif

#ifdef HAVE_FABSF
    #define F_ABS(_x) (::fabsf(_x))
#else
    #define F_ABS(_x) (::fabs(_x))
#endif

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
, mExtent(WFMath::Point<2>(mPosition.x() * (getPageSize() - 1), (mPosition.y() - 1) * (getPageSize() - 1)), WFMath::Point<2>((mPosition.x() + 1) * (getPageSize() - 1), (mPosition.y()) * (getPageSize() - 1))
)
, mPageFoliage(new TerrainPageFoliage(*mGenerator, *this))
{

	S_LOG_VERBOSE("Creating TerrainPage at position " << position.x() << ":" << position.y());
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegmentAtLocalIndex(x,y);
			if (segment && segment->isValid()) {
 				//S_LOG_VERBOSE("Segment is valid.");
				PageSegment pageSegment;
				pageSegment.pos = TerrainPosition(x,y);
				pageSegment.segment = segment;
				mValidSegments.push_back(pageSegment);
			}
			mLocalSegments[x][y] = segment;
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

Mercator::Segment* TerrainPage::getSegmentAtLocalIndex(int indexX, int indexY) const
{
// 	const TerrainInfo& info = mGenerator->getTerrainInfo();
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	//the mPosition is in the middle of the page, so we have to use an offset to get the real segment position
	//int segmentOffset = static_cast<int>(info.getWorldSizeInSegments(). x() * 0.5);
	int segmentOffset = segmentsPerAxis;
	int segX = (int)((mPosition.x() * segmentsPerAxis) + indexX);
	int segY = (int)((mPosition.y() * segmentsPerAxis) + indexY) - segmentOffset;
	
	//S_LOG_VERBOSE("Added segment with position " << segX << ":" << segY);

	return mGenerator->getTerrain().getSegment(segX, segY);
}

Mercator::Segment* TerrainPage::getSegmentAtLocalPosition(const TerrainPosition& pos) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));
    
    Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
    if (I == mLocalSegments.end()) {
        return 0;
    }
    Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
    if (J == I->second.end()) {
        return 0;
    }
    return J->second;
}

Mercator::Segment* TerrainPage::getSegmentAtLocalPosition(const TerrainPosition& pos, TerrainPosition& localPositionInSegment) const
{
	int ix = I_ROUND(floor(pos.x() / 64));
	int iy = I_ROUND(floor(pos.y() / 64));
	
	localPositionInSegment.x() = pos.x() - (ix * 64);
	localPositionInSegment.y() = pos.y() - (iy * 64);
    
    Mercator::Terrain::Segmentstore::const_iterator I = mLocalSegments.find(ix);
    if (I == mLocalSegments.end()) {
        return 0;
    }
    Mercator::Terrain::Segmentcolumn::const_iterator J = I->second.find(iy);
    if (J == I->second.end()) {
        return 0;
    }
    return J->second;	
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
	if (!mFoliageArea) {
		prepareFoliage();
	}
	if (mFoliageArea) {
		mFoliageArea->setVisible(true);
	}
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
	mPageFoliage->generatePlantPositions();
	mPageFoliage->generateCoverageMap();

}

const WFMath::AxisBox<2>& TerrainPage::getExtent() const
{
	return mExtent;
}

// const TerrainPage::PlantsStore& TerrainPage::getPlants() const
// {
// 	return mPlants;
// }

TerrainPageSurface* TerrainPage::getSurface() const
{
	return mTerrainSurface.get();
}

TerrainPageFoliage* TerrainPage::getPageFoliage() const
{
	return mPageFoliage.get();
}

TerrainPageShadow& TerrainPage::getPageShadow()
{
	return mShadow;
}



TerrainPageSurfaceLayer* TerrainPage::addShader(TerrainShader* shader)
{
	TerrainPageSurfaceLayer* layer = mTerrainSurface->createSurfaceLayer(*shader->getLayerDefinition(), shader->getTerrainIndex(), shader->getShader());
	layer->populate();
	layer->setDiffuseTextureName(shader->getLayerDefinition()->getDiffuseTextureName());
	layer->setNormalTextureName(shader->getLayerDefinition()->getNormalMapTextureName());
	///get the scale by dividing the total size of the page with the size of each tile
	float scale = getAlphaTextureSize() / shader->getLayerDefinition()->getTileSize();
	layer->setScale(scale);
	layer->updateCoverageImage();
	return layer;

/*	mUsedShaders.push_back(shader);
	///if the material already has been created, add the shader instantly, else wait until the generateTerrainMaterials method is called
	
	if (!mMaterial.isNull()) {
//		populateSurfaces();
		addShaderToSimpleTechnique(mMaterial->getTechnique(0), shader);
	}*/
}



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

// void TerrainPage::populateSurface(TerrainShader* shader)
// {
// 	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
// 		for (Mercator::Segment::Surfacestore J = I->segment->getSurfaces()
// 		I->segment->populateSurfaces();
// 	}
// 	
// 	I->segment->getSurface
// }


void TerrainPage::updateAllShaderTextures(bool repopulate)
{
	TerrainPageSurface::TerrainPageSurfaceLayerStore::const_iterator I = mTerrainSurface->getLayers().begin();

	///skip the first texture, since it's the ground, and doesn't have an alpha texture
	++I;
	for (; I != mTerrainSurface->getLayers().end(); ++I) {
		mTerrainSurface->updateLayer(I->first, repopulate);
	}
	mPageFoliage->generateCoverageMap();
}

TerrainPageSurfaceLayer* TerrainPage::updateShaderTexture(TerrainShader* shader, bool repopulate)
{
	TerrainPageSurfaceLayer* layer;
	if (mTerrainSurface->getLayers().find(shader->getTerrainIndex()) == mTerrainSurface->getLayers().end()) {
		layer = addShader(shader);
		mTerrainSurface->recompileMaterial();
	} else {
		layer = mTerrainSurface->updateLayer(shader->getTerrainIndex(), repopulate);
		mTerrainSurface->recompileMaterial();
	}
	
	mPageFoliage->generateCoverageMap();
	return layer;
	
}
}
}
