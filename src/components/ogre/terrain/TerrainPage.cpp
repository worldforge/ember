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
//#include "EmberTerrainRenderable.h"
#include "TerrainShader.h"
#include "EmberSceneManager/include/EmberTerrainPageSource.h"
#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "EmberSceneManager/include/OgreTerrainRenderable.h"

#include "EmberOgre.h"

#include <OgreRenderSystemCapabilities.h>

#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>
#include <OgreCommon.h>

#include <IL/il.h>
#include <IL/ilu.h>

#include "environment/Foliage.h"
#include "environment/FoliageArea.h"
#include "TerrainGenerator.h"

#include <Mercator/Segment.h>
#include <Mercator/Shader.h>

#include <OgreHardwarePixelBuffer.h>
namespace EmberOgre {

TerrainPage::TerrainPage(TerrainPosition& position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator) 
: mPosition(position), mShaderMap(shaderMap), mGenerator(generator), mBytesPerPixel(4), mAlphaMapScale(2), mFoliageArea(0)
{
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegment(x,y);
			if (segment && segment->isValid()) {
				PageSegment pageSegment;
				pageSegment.pos = TerrainPosition(x,y);
				pageSegment.segment = segment;
				mValidSegments.push_back(pageSegment);
			}
		}
	}

}

TerrainPage::~TerrainPage()
{
}

Mercator::Segment* TerrainPage::getSegment(int x, int y) const
{
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	return mGenerator->getTerrain().getSegment((int)((mPosition.x() * segmentsPerAxis) + x), (int)((mPosition.y() * segmentsPerAxis) + y));
}

int TerrainPage::getNumberOfSegmentsPerAxis() const
{
	return (getTerrainOptions().pageSize -1) / 64;
}
	
float TerrainPage::getMaxHeight()
{
	float max = 0;
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		max = std::max(max, I->segment->getMax());
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	float min = 0;
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		min = std::min(min, I->segment->getMin());
	}
	return min;
}



Ogre::MaterialPtr TerrainPage::generateTerrainMaterials() {

	//Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();

		
	//create a name for out material
	EmberTerrainSceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << mPosition.x() << "_" << mPosition.y();
	mMaterialName = materialNameSS.str();
		

	//create the actual material
	mMaterial = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(mMaterialName, "General"));

	//we'll use at least two different techniques
	//for modern GPU's we'll use a technique which uses fragment shaders
	if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("arbfp1") && !Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0")) {
		//generateTerrainTechniqueComplexAtlas(mMaterial->getTechnique(0));
		generateTerrainTechniqueComplex(mMaterial->getTechnique(0));
//		generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
	} else {
	//and as a fallback for older gfx cards we'll supply a technique which doesn't
		generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
	}
// 	generateTerrainTechniqueSimple(mMaterial->getTechnique(0));
	
	//and if the user so wants, we'll add some debug materials
	if (getTerrainOptions().debuglod) {
		generateTerrainTechniqueDebug();
	}

		
	mMaterial->load();
	
	return mMaterial;

}
	
inline const Ogre::TerrainOptions& TerrainPage::getTerrainOptions() const
{
	return mGenerator->getTerrainOptions();
}

long TerrainPage::getVerticeCount() const
{
	return (getTerrainOptions().pageSize) *(getTerrainOptions().pageSize);
}

const TerrainPosition& TerrainPage::getWFPosition() const
{
	return mPosition;
}

const Ogre::Vector3 TerrainPage::getOgrePosition() const
{
	Ogre::Vector3 vector = Atlas2Ogre(mPosition);
	//we have to do this in order to align the pages correctly
	vector.z = vector.z - 1;
	return vector;

}


Ogre::MaterialPtr TerrainPage::getMaterial() const
{
	return mMaterial;
}



void TerrainPage::createHeightData(Ogre::Real* heightData)
{
//TODO: make this work with complex pages
		Ogre::Vector3 vector = getOgrePosition();
		long xStart = (long)vector.x * (getTerrainOptions().pageSize - 1);
		long zStart = (long)vector.z * (getTerrainOptions().pageSize - 1);
		
		for (int i = 0; i < getTerrainOptions().pageSize; ++i) {
			for (int j = 0; j < getTerrainOptions().pageSize; ++j) {
				Ogre::Vector3 _pos(xStart + j, 0, zStart + i);
				TerrainPosition pos = Ogre2Atlas_TerrainPosition(_pos);
				Ogre::Real height = mGenerator->getHeight(pos);
				*(heightData++) = height / 100.0;
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

void TerrainPage::printTextureToImage(Ogre::MemoryDataStreamPtr dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat, int height, int width) {
// DEBUG   
//prints the bitmap to a png-image
//TODO: remove when finished with debugging
	
	const Ogre::String extension = "png";
	
	Ogre::ImageCodec::ImageData* imgData = new Ogre::ImageCodec::ImageData();
	imgData->width = width;
	imgData->height = height;
	
	imgData->depth =  1;
	imgData->format = pixelFormat;	
	     		
	Ogre::Codec * pCodec = Ogre::Codec::getCodec(extension);
	// Write out
	Ogre::SharedPtr<Ogre::Codec::CodecData> temp(imgData);
	
	//MAKE SURE THAT THE DIRECTORY EXISTS!!!
	//ELSE YOY'LL GET EVIL RESOURCE ERRORS!!
	pCodec->codeToFile(dataChunk, Ogre::String("img/") + name + "." + extension, temp);
	
}

};

inline int  EmberOgre::TerrainPage::getAlphaTextureSize( ) const
{
	return (getTerrainOptions().pageSize - 1);

}

void EmberOgre::TerrainPage::fillAlphaLayer(unsigned char* finalImagePtr, unsigned char* wfImagePtr, unsigned int channel, int startX, int startY) {

// 	if (startX != 64 || startY != 64) {
// 		return;
// 	
// 	}

	//int width = getTerrainOptions().pageSize - 1;
    int width = 64;
 	int finalImageWidth = getAlphaTextureSize( );
	

    long i,j; 

	
	
    Ogre::uchar* start = finalImagePtr + (4 * finalImageWidth * (startY - 1)) + (startX * 4);
    Ogre::uchar* end = start + (width * finalImageWidth * 4) + (width * 4);
	
	
//     Ogre::uchar* start = finalImagePtr + ((startX * 4) + ((startY) * finalImageSize * 4));
//     Ogre::uchar* end = start + (finalImageSize * 4 * 64);//  - (finalImageSize * 4) ;

	
	
	
	
	
//	wfImagePtr += 65;
	
    Ogre::uchar* tempPtr = end + channel;
    for (i = 0; i < width; ++i) {
	    tempPtr -= (width * 4);
		for (j = 0; j < width; ++j) {
			//Ogre::uchar alpha = 255 - j;
			Ogre::uchar alpha = *(wfImagePtr + j);
			*(tempPtr) = alpha;
			//skip four channels
			tempPtr += 4;

			
		}
		tempPtr -= (finalImageWidth * 4);
		wfImagePtr += 65;
    }

	return;	
	
	
		
//     Ogre::uchar* tempPtr = finalImagePtr + ((startX * 4) + (startY * finalImageSize * 4)) + channel;
//     for (i = 0; i < width; ++i) {
//     	for (j = 0; j < width; ++j) {
//         	Ogre::uchar alpha = *(wfImagePtr++);
//         	*tempPtr++ = alpha;
// 			tempPtr++;
//         	tempPtr++;
//         	tempPtr++;
//     		
//     	}
//     	
// 		tempPtr += (finalImageSize - width) * 4;
//     }   

    	//wfImagePtr += 65;
//position the pointer at the end of data, and starting at the correct channel
    
	
	
	
	
	
	
// 	Ogre::uchar* tempPtr = end - (3 - channel);
//     for (i = 0; i < width; ++i) {
// 	    tempPtr -= (width * mBytesPerPixel);
// 		for (j = 0; j < width; ++j) {
// 			Ogre::uchar alpha = *(wfImagePtr + j);
// 			*(tempPtr) = alpha;
// 			//skip four channels
// 			tempPtr += 4;
// 
// 			
// 		}
// 		tempPtr -= (finalImageSize * 4);
//     	wfImagePtr += 65;
//     	//++chunkPtr;
//     }

}


void EmberOgre::TerrainPage::createFoliage(TerrainShader* grassShader)
{
	if (!mFoliageArea) {
		mFoliageArea = Foliage::getSingleton().createArea();
	}
	double grassSpacing = Foliage::getSingleton().getGrassSpacing();
	
	//for each 1 m^2, how big chance is there of grass? [0,1.0]
	Ogre::Real grassChanceThreshold = 1 / grassSpacing; 
	TerrainPosition worldUnitsStartOfPage((getTerrainOptions().pageSize - 1) * mPosition.x(), (getTerrainOptions().pageSize - 1) * mPosition.y());
	
	//for now, just check with the grass shader
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		Mercator::Segment* segment = I->segment;
		Mercator::Surface* surface = grassShader->getSurfaceForSegment(segment);
		if (!surface) {
			//we could not find any grass surface
			return;
		}
		for (unsigned int x = 0; x < 64; ++x) {
			for (unsigned int y = 0; y < 64; ++y) {
				
				//first check if this is a good spot for grass (the more "green", the greater chance of grass
				//then check with grassChanceThreshold if we should do grass
				unsigned char cover = (*surface)(x, y, 0);
				unsigned char coverChance = (unsigned char) Ogre::Math::RangeRandom(0, 255);
				if (cover > coverChance) {
					Ogre::Real grassChance = Ogre::Math::UnitRandom();
					if (grassChanceThreshold > grassChance) {
						//some random offset
						Ogre::Real x_offset = Ogre::Math::RangeRandom(-1, 1);
						Ogre::Real y_offset = Ogre::Math::RangeRandom(-1, 1);
						TerrainPosition position(worldUnitsStartOfPage.x() + (I->pos.x() * 64) + x + x_offset, worldUnitsStartOfPage.y() + (I->pos.y() * 64) + y + y_offset);
						if (Ogre::Math::UnitRandom() > 0.95) {
							const Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.5, 0.8), 1);

							mFoliageArea->placeGrass("heartblood", position, scale);
						} else {
							mFoliageArea->placeGrass("bittergrass", position);
						}
					}
				}
			}
		}
	}
	mFoliageArea->build();
	
	
}

void EmberOgre::TerrainPage::addShader(TerrainShader* shader)
{
	mUsedShaders.push_back(shader);
	//if the material already has been created, add the shader instantly, else wait until the generateTerrainMaterials method is called
	if (!mMaterial.isNull()) {
//		populateSurfaces();
		addShaderToSimpleTechnique(mMaterial->getTechnique(0), shader);
	}
}



void EmberOgre::TerrainPage::generateTerrainTechniqueComplexAtlas( Ogre::Technique* technique)
{ 
 
	Ogre::Pass* pass = technique->getPass(0);
	pass->setLightingEnabled(false);
	//pass->setFragmentProgram("splat3arb");
	pass->setFragmentProgram("splatatlas_cg");
	
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	Ogre::TextureUnitState * alphaTextureUnitState = pass->createTextureUnitState();
	
	
	//use this iterator to iterate over all shaders we will use. The first shader will be used as a base (i.e. with no alpha map), so make sure that it's something like rock or dirt (upon which one would want to put some grass etc.)
	std::list<TerrainShader*>::iterator shadersIterator = mUsedShaders.begin();

	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
	Mercator::Segment* aValidSegment = 0;
	
	if (mValidSegments.begin() == mValidSegments.end()) {
		return;
	}
	aValidSegment = mValidSegments.begin()->segment;
	
	Mercator::Surface* surface = (*shadersIterator)->getSurfaceForSegment(aValidSegment);
 
	
	//we need an unique name for our alpha texture
	std::stringstream splatTextureNameSS;
	splatTextureNameSS << mMaterialName;
	Ogre::String splatTextureName(splatTextureNameSS.str());
	
	//the format for our alpha texture
	Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;
	
// 	//let's create our map image
// 	ILuint mapImageName;
// 	ilGenImages( 1, &mapImageName );
// 	ilBindImage( mapImageName );
// 	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, 0 );
// 	unsigned char * mapImagePointer = ilGetData();

	
	

	ILuint ImageName;
	ilGenImages( 1, &ImageName );
	ilBindImage( ImageName );
	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, 0 );
	unsigned char * imagePointer = ilGetData();
	
	//clear the image
	memset (imagePointer, 0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel);
	
	Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, false);
	

	
	
		  
	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
	
/*	int numberOfSegmentsToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
	for (int i = 0; i < numberOfSegmentsToDo; ++i) {*/
	
	int numberOfbaseTextureUnits = pass->getNumTextureUnitStates();
	int splattextureChannel = 0;
	++shadersIterator;
	for (; shadersIterator != mUsedShaders.end() ; ++shadersIterator) {

//		TerrainShader* shader;
		
		//check if at least one surface instersects, else continue
		bool intersects = false;
	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			if ((*shadersIterator)->getShader()->checkIntersect(*I->segment)) {
				intersects = true;
			}
		}

/*		if (!intersects) {
			continue;
		}*/
		
		
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			Mercator::Segment* segment = I->segment;
			if (segment && segment->isValid() && splattextureChannel < 4) {
				
				surface = (*shadersIterator)->getSurfaceForSegment(segment);
				if (surface && surface->isValid()) {
					
					fillAlphaLayer(finalChunk->getPtr(), surface->getData(), splattextureChannel, I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - I->pos.y() - 1) * 64);
				}
			}
		}
		++splattextureChannel;
	}
	Ogre::TextureUnitState * splatTextureUnitState = pass->createTextureUnitState();
	splatTextureUnitState->setTextureName("atlassplat.png");
	splatTextureUnitState->setTextureCoordSet(1);
    splatTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);

	
	ilBindImage(ImageName);
/*	char name[100];
	strcpy(name, (std::string("/home/erik/") + splatTextureName + std::string(".png")).c_str());
	ilSaveImage(name);*/
		
	if (mAlphaMapScale > 1) {
		
		//smooth out the terrain
		//do this by enlarging the image and apply a blur filter to it
		
		//use filter to smooth everything out
		iluImageParameter(ILU_FILTER, ILU_SCALE_BSPLINE);
		iluScale(getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, 1);
		imagePointer = ilGetData();
		//wrap the image data in a Datachunk
		delete finalChunk;
		finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel * (mAlphaMapScale * mAlphaMapScale) , false);
	}
		
	Ogre::DataStreamPtr dataStreamPtr(finalChunk);

	
	
	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, pixelFormat);
/*	temp.setNull();
	splatTexture.setNull();*/
	if (mAlphaMapScale > 1) {
		ilDeleteImages(1, &ImageName);
	}
	alphaTextureUnitState->setTextureName(splatTextureName);
    alphaTextureUnitState->setTextureCoordSet(0);
	alphaTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

 

 
 }



void EmberOgre::TerrainPage::generateTerrainTechniqueComplex( Ogre::Technique* technique)
{ 
 
	Ogre::Pass* pass = technique->getPass(0);
	pass->setLightingEnabled(false);
	//pass->setFragmentProgram("splat3arb");
	pass->setFragmentProgram("splat_cg");
	
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	Ogre::TextureUnitState * alphaTextureUnitState = pass->createTextureUnitState();
	
	
	//use this iterator to iterate over all shaders we will use. The first shader will be used as a base (i.e. with no alpha map), so make sure that it's something like rock or dirt (upon which one would want to put some grass etc.)
	std::list<TerrainShader*>::iterator shadersIterator = mUsedShaders.begin();

	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
	Mercator::Segment* aValidSegment = 0;
	
	if (mValidSegments.begin() == mValidSegments.end()) {
		return;
	}
	aValidSegment = mValidSegments.begin()->segment;
	
	Mercator::Surface* surface = (*shadersIterator)->getSurfaceForSegment(aValidSegment);
 
//	pass->setShininess(20);
//	pass->setSpecular(1,1,1,1);

//granite layer
	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	{
	    textureUnitState->setTextureName((*shadersIterator)->getTextureName());
//	    textureUnitState->setTextureName("splat3d.dds");
	    textureUnitState->setTextureCoordSet(1);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}

	
	//we need an unique name for our alpha texture
	std::stringstream splatTextureNameSS;
	splatTextureNameSS << mMaterialName;
	Ogre::String splatTextureName(splatTextureNameSS.str());
	
	//the format for our alpha texture
	Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;
	
// 	//let's create our map image
// 	ILuint mapImageName;
// 	ilGenImages( 1, &mapImageName );
// 	ilBindImage( mapImageName );
// 	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, 3, IL_RGB, IL_UNSIGNED_BYTE, 0 );
// 	unsigned char * mapImagePointer = ilGetData();

	
	

	ILuint ImageName;
	ilGenImages( 1, &ImageName );
	ilBindImage( ImageName );
	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, 0 );
	unsigned char * imagePointer = ilGetData();
	
	//clear the image
	memset (imagePointer, 0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel);
	
	Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, false);
	

	
	
		  
	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
	
/*	int numberOfSegmentsToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
	for (int i = 0; i < numberOfSegmentsToDo; ++i) {*/
	
	int numberOfbaseTextureUnits = pass->getNumTextureUnitStates();
	++shadersIterator;
	for (; shadersIterator != mUsedShaders.end() ; ++shadersIterator) {

//		TerrainShader* shader;
		
		//check if at least one surface instersects, else continue
		bool intersects = false;
	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			if ((*shadersIterator)->getShader()->checkIntersect(*I->segment)) {
				intersects = true;
			}
		}

/*		if (!intersects) {
			continue;
		}*/
		
		
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			Mercator::Segment* segment = I->segment;
			if (segment && segment->isValid() && pass->getNumTextureUnitStates()  < 4) {
				
				surface = (*shadersIterator)->getSurfaceForSegment(segment);
				if (surface && surface->isValid()) {
					
					fillAlphaLayer(finalChunk->getPtr(), surface->getData(), pass->getNumTextureUnitStates() - numberOfbaseTextureUnits, I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - I->pos.y() - 1) * 64);
				}
			}
		}
		//TODO: make it not hard coded to 4
		if (pass->getNumTextureUnitStates()  < 4) {
			Ogre::TextureUnitState * splatTextureUnitState = pass->createTextureUnitState();
			splatTextureUnitState->setTextureName((*shadersIterator)->getTextureName());
			splatTextureUnitState->setTextureCoordSet(1);
    		splatTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
		
		
	}
	
	ilBindImage(ImageName);
/*	char name[100];
	strcpy(name, (std::string("/home/erik/") + splatTextureName + std::string(".png")).c_str());
	ilSaveImage(name);*/
		
	if (mAlphaMapScale > 1) {
		
		//smooth out the terrain
		//do this by enlarging the image and apply a blur filter to it
		
		//use filter to smooth everything out
		iluImageParameter(ILU_FILTER, ILU_SCALE_BSPLINE);
		iluScale(getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, 1);
		imagePointer = ilGetData();
		//wrap the image data in a Datachunk
		delete finalChunk;
		finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel * (mAlphaMapScale * mAlphaMapScale) , false);
	}
		
	Ogre::DataStreamPtr dataStreamPtr(finalChunk);

	
	
	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, pixelFormat);
/*	temp.setNull();
	splatTexture.setNull();*/
	if (mAlphaMapScale > 1) {
		ilDeleteImages(1, &ImageName);
	}
	alphaTextureUnitState->setTextureName(splatTextureName);
    alphaTextureUnitState->setTextureCoordSet(0);
	alphaTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

 

 
 }
 
void EmberOgre::TerrainPage::generateTerrainTechniqueDebug()
{
	for (int i = 1; i < getTerrainOptions().maxGeoMipMapLevel; ++i) {
		Ogre::Technique *tech = mMaterial->createTechnique();
		tech->setLodIndex(i);
		
		Ogre::TextureUnitState *textureUnitState = tech->createPass()->createTextureUnitState();
		std::stringstream ss;
		ss << "lod" << i << ".png";
		textureUnitState->setTextureName(ss.str());
		textureUnitState->setTextureCoordSet(0);
	
	}
}

void EmberOgre::TerrainPage::generateTerrainTechniqueSimple( Ogre::Technique* technique)
{

	Mercator::Segment* segment = mGenerator->getTerrain().getSegment((int)mPosition.x(), (int)mPosition.y());
 
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();
	
	
	std::list<TerrainShader*>::iterator shadersIterator = mUsedShaders.begin();
			
	
	
	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
	Mercator::Segment* aValidSegment = 0;
	
	if (mValidSegments.begin() == mValidSegments.end()) {
		return;
	}
	aValidSegment = mValidSegments.begin()->segment;
	
	Mercator::Surface* surface = (*shadersIterator)->getSurfaceForSegment(aValidSegment);
// 	Mercator::Surface* surface = *aValidSegment->getSurfaces().begin();
 
	Ogre::Pass* pass = technique->getPass(0);
	pass->setLightingEnabled(false);
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	
	   
    const Mercator::Segment::Surfacestore & surfaces = segment->getSurfaces();
    Mercator::Segment::Surfacestore::const_iterator I = surfaces.begin();

//	pass->setShininess(20);
//	pass->setSpecular(1,1,1,1);
//granite layer
	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	{
/*		const Mercator::Shader* mercShader = &(surface->m_shader);
		TerrainShader* shader = mShaderMap[mercShader];*/
	    textureUnitState->setTextureName((*shadersIterator)->getTextureName());
	}
    textureUnitState->setTextureCoordSet(1);
	
	
	
	
	
	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
	
// 	int numberOfSurfacesToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
// 	for (int i = 0; i < numberOfSurfacesToDo; ++i) {
	++shadersIterator;
	for (; shadersIterator != mUsedShaders.end() ; ++shadersIterator) {
		addShaderToSimpleTechnique(technique, *shadersIterator);
	}
}


void EmberOgre::TerrainPage::populateSurfaces()
{
	for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
		I->segment->populateSurfaces();
	}
}


void EmberOgre::TerrainPage::updateShaderTexture(TerrainShader* shader)
{

	bool found = false;
	for (std::list<TerrainShader*>::iterator I = mUsedShaders.begin(); I != mUsedShaders.end(); ++I) {
		if (*I == shader) {
			found = true;
			break;
		}
	}
	if (!found) {
		addShader(shader);
	} else if (mShaderTextures.find(shader) == mShaderTextures.end()) {
		addShaderToSimpleTechnique(mMaterial->getTechnique(0), shader);
	} else {
		Ogre::TexturePtr texture = mShaderTextures.find(shader)->second;
		
		Mercator::Surface* surface;
		
		Ogre::MemoryDataStream* splatChunk = new Ogre::MemoryDataStream(getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, true);
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			Mercator::Segment* segment = I->segment;
			surface = shader->getSurfaceForSegment(segment);
			if (surface && surface->isValid()) {
				
				int alphaChannel = 0;
				fillAlphaLayer(splatChunk->getPtr(), surface->getData(), alphaChannel, I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - I->pos.y() - 1) * 64);
			}
		}
		
		Ogre::HardwarePixelBufferSharedPtr hardwareBuffer = texture->getBuffer();
//		hardwareBuffer->writeData(0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, splatChunk->getPtr());

		
		Ogre::DataStreamPtr dataStreamPtr(splatChunk);
		
		Ogre::Image image;
		image.loadRawData(dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_B8G8R8A8);
		//image->save(std::string("/home/erik/tempimages/") + texture->getName()  + "_temp" + std::string(".png"));
		
		
		
		Ogre::PixelBox sourceBox = image.getPixelBox();
		Ogre::Box targetBox(0,0, texture->getWidth(), texture->getHeight());
		hardwareBuffer->blitFromMemory(sourceBox);
		
	}
	
}


void EmberOgre::TerrainPage::addShaderToSimpleTechnique(Ogre::Technique* technique, TerrainShader* shader)
{
		Mercator::Surface* surface;
		
		//check if at least one surface instersects, else continue
		bool intersects = false;
	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			if (shader->getShader()->checkIntersect(*I->segment)) {
				intersects = true;
			}
		}
 		if (!intersects) {
 			return;
 		}
		
		Ogre::MemoryDataStream* splatChunk = new Ogre::MemoryDataStream(getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, false);

		
		//we need an unique name for our alpha texture
		std::stringstream splatTextureNameSS;
		splatTextureNameSS << mMaterialName << "_" << shader->getTerrainIndex();
		const Ogre::String splatTextureName(splatTextureNameSS.str());
		
				
/*		SegmentVector::iterator I = segmentI_begin;*/
		
		for (SegmentVector::iterator I = mValidSegments.begin(); I != mValidSegments.end(); ++I) {
			surface = shader->getSurfaceForSegment(I->segment);
			if (surface && surface->isValid()) {
			
				int alphaChannel = 0;
				fillAlphaLayer(splatChunk->getPtr(), surface->getData(), alphaChannel, I->pos.x() * 64, (getNumberOfSegmentsPerAxis() - I->pos.y() - 1) * 64);

			}
		}
			
		Ogre::DataStreamPtr dataStreamPtr(splatChunk);
		
/*	Ogre::Image* image = new Ogre::Image();
	image->loadRawData(dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_B8G8R8A8);
	image->save(std::string("/home/erik/tempimages/") + splatTextureName + "_temp" + std::string(".png"));*/
	
	
		Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize(), getAlphaTextureSize(), Ogre::PF_B8G8R8A8);
	
		
/*	char name[100];
	strcpy(name, (std::string("/home/erik/tempimages/") + splatTextureName + "_temp" + std::string(".png")).c_str());
	ilSaveImage(name);			*/
		
	mShaderTextures[shader] = splatTexture;

	shader->addSplatToTechnique(technique, splatTextureName);
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


		
		

}

