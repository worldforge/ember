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


#include "TerrainGenerator.h"

namespace EmberOgre {

TerrainPage::TerrainPage(TerrainPosition& position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator) 
: mPosition(position), mShaderMap(shaderMap), mGenerator(generator), mBytesPerPixel(4), mAlphaMapScale(2)
{
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
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegment(x,y);
			if (segment && segment->isValid()) {
				max = std::max(max, segment->getMax());
			}
		}
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	float min = 0;
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegment(x,y);
			if (segment && segment->isValid()) {
				min = std::min(min, segment->getMin());
			}
		}
	}
	return min;
}


Ogre::MaterialPtr TerrainPage::generateTerrainMaterials() {

	if (getNumberOfSegmentsPerAxis() == 1) {
		mMaterial = generateTerrainMaterialComplex();
		//generateTerrainMaterialSimple();
	} else {
		mMaterial = generateTerrainMaterialComplex();
/*		std::vector<const Mercator::Segment::Surfacestore&> surfaceStores;
		
		for (mMercatorSegments::const_iterator I = mMercatorSegments.begin(); I != mMercatorSegments.end(); ++I) {
			surfaceStores.push_back(*I);
		}*/
		
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



Ogre::MemoryDataStreamPtr TerrainPage::convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor) {
    //int width = getTerrainOptions().pageSize - 1;
    int width = 64;
    int bufferSize = width*width*mBytesPerPixel*factor*factor;
	Ogre::MemoryDataStream chunk(dataStart, 65*65, false);
    Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(bufferSize);
    //finalChunk->allocate(bufferSize);
    Ogre::uchar* finalPtr = finalChunk->getPtr();
    Ogre::uchar* chunkPtr = chunk.getPtr();
    long i,j; 
    long sizeOfOneChannel = width*width;

     Ogre::uchar* tempPtr = finalPtr + bufferSize;
    for (i = 0; i < width; ++i) {
    	for (int l = 0; l < factor; ++l) {
	    	tempPtr -= (width * mBytesPerPixel * factor);
	    	for (j = 0; j < width; ++j) {
	        	Ogre::uchar alpha = *(chunkPtr + j);
	        	for (int k = 0; k < factor; ++k) {
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = alpha;
	        	}
	    		
	    	}
	    	tempPtr -= (width * mBytesPerPixel * factor);
    	}
    	chunkPtr += 65;
    	//++chunkPtr;
    }
   /*
    Ogre::uchar* tempPtr = finalPtr;
    for (i = 0; i < width; ++i) {
    	for (j = 0; j < width; ++j) {
        	Ogre::uchar alpha = *(chunkPtr++);
        	*tempPtr++ = 0;
        	*tempPtr++ = 0;
        	*tempPtr++ = 0;
        	*tempPtr++ = alpha;
    		
    	}
    	++chunkPtr;
    }   
    */
    
	return Ogre::MemoryDataStreamPtr(finalChunk);
}

Ogre::TexturePtr TerrainPage::createAlphaTexture(Ogre::String name, Mercator::Surface* surface) {


    //the format for our alpha texture
    //for some reason we can't use PF_A8
    Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;

	/*first we need to change the 8 bit mask into 32 bits
	 * because somehow Ogre doesn't like the 8 bit mask
	 * 
	 */
	Ogre::MemoryDataStreamPtr finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), 1);
	
	//printTextureToImage(finalChunk, name, pixelFormat);
	Ogre::DataStreamPtr temp(finalChunk.get());
	//create the new alpha texture
	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(name, "General", temp, getAlphaTextureSize(), getAlphaTextureSize(), pixelFormat);
//	finalChunk->clear();
//	delete finalChunk;
	
	return splatTexture;
	
}

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

	//int width = getTerrainOptions().pageSize - 1;
	int bytesPerPixel = 1;
    int width = 64;
    int bufferSize = width*width*bytesPerPixel;
	int finalImageSize = getAlphaTextureSize( );

    long i,j; 
    long sizeOfOneChannel = width*width;

    Ogre::uchar* start = finalImagePtr + ((startX * 4) + ((startY) * finalImageSize * 4));
    Ogre::uchar* end = start + (finalImageSize * 4 * 64);//  - (finalImageSize * 4) ;
	
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
//     	++wfImagePtr;
// 		tempPtr += (finalImageSize - width) * 4;
//     }   

//position the pointer at the end of data, and starting at the correct channel
    Ogre::uchar* tempPtr = end + channel + 1;
    for (i = 0; i < width; ++i) {
	    tempPtr -= (width * mBytesPerPixel);
		for (j = 0; j < width; ++j) {
			Ogre::uchar alpha = *(wfImagePtr + j);
			*(tempPtr) = alpha;
			//skip four channels
			tempPtr += 4;

			
		}
		tempPtr -= (finalImageSize * 4);
    	wfImagePtr += 65;
    	//++chunkPtr;
    }

}

Ogre::MaterialPtr EmberOgre::TerrainPage::generateTerrainMaterialComplex( )
{ 
 
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();

				
	EmberTerrainSceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << mPosition.x() << "_" << mPosition.y();
	mMaterialName = materialNameSS.str();
		

	Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create(mMaterialName, "General"));
	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
	pass->setLightingEnabled(false);
	pass->setFragmentProgram("splat3arb");
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	Ogre::TextureUnitState * alphaTextureUnitState = pass->createTextureUnitState();
	
	
		

	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
	Mercator::Segment* aValidSegment = 0;
	typedef std::list<Mercator::Surface*>::iterator vectorOfSurfaceListIterator;
	std::map<Mercator::Segment*, vectorOfSurfaceListIterator> segmentSurfaceListIteratorMapping;
	for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
		for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
			Mercator::Segment* segment = getSegment(x,y);
			if (segment && segment->isValid()) {
				aValidSegment = segment;
				std::list<Mercator::Surface*>::iterator tempI = segment->getSurfaces().begin();
				segmentSurfaceListIteratorMapping[segment] = tempI;
			} else {
	/*			segmentSurfaceListIteratorMapping[*I] = surfaceListIterators;
				std::list<Mercator::Surface*>::iterator tempI;
				*tempI = 0;
				surfaceListIterators.push_back(tempI);*/
			}
		}
	}
	if (!aValidSegment) {
		return material;
	}
	
	Mercator::Surface* surface = *aValidSegment->getSurfaces().begin();
 
//	pass->setShininess(20);
//	pass->setSpecular(1,1,1,1);

//granite layer
	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	{
		const Mercator::Shader* mercShader = &(surface->m_shader);
		TerrainShader* shader = mShaderMap[mercShader];
	    textureUnitState->setTextureName(shader->getTextureName());
//	    textureUnitState->setTextureName("splat3d.dds");
	    textureUnitState->setTextureCoordSet(1);
		textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	}

	
	//we need an unique name for our alpha texture
	std::stringstream splatTextureNameSS;
	splatTextureNameSS << mMaterialName;
	const Ogre::String splatTextureName = splatTextureNameSS.str();
	
	//the format for our alpha texture
	Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;

	ILuint ImageName;
	ilGenImages( 1, &ImageName );
	ilBindImage( ImageName );
	ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, 0 );
	unsigned char * imagePointer = ilGetData();
	
	//clear the image
	memset (imagePointer, 0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel);
	
	Ogre::MemoryDataStream* finalChunk = new Ogre::MemoryDataStream(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel, false);
	

	
	
		  
	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
	
	int numberOfSegmentsToDo = aValidSegment->getSurfaces().size() - 1; //we've already done the base
	for (int i = 0; i < numberOfSegmentsToDo; ++i) {

		TerrainShader* shader;
		
		//check if at least one surface instersects, else continue
		bool intersects = false;
	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
		for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
			for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
				Mercator::Segment* segment = getSegment(x,y);
				if (segment && segment->isValid()) {
					std::list<Mercator::Surface*>::iterator* surfaceListI = &(segmentSurfaceListIteratorMapping[segment]);
					surface = *(++(*surfaceListI));
				
					const Mercator::Shader* mercShader = &(surface->m_shader);
					shader = mShaderMap[mercShader];
					if (surface->m_shader.checkIntersect(*surface)) {
						intersects = true;
					}
				}
			}
		}
/*		if (!intersects) {
			continue;
		}*/
		
/*		SegmentVector::iterator I = segmentI_begin;*/
		
		for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
			for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
				
				Mercator::Segment* segment = getSegment(x,y);
				if (segment && segment->isValid() && i < 4) {
					
					std::list<Mercator::Surface*>::iterator surfaceListI = segmentSurfaceListIteratorMapping[segment];
					surface = *surfaceListI;
					if (surface->isValid()) {
						std::stringstream splatTextureNameSS_;
						splatTextureNameSS_ << splatTextureName << "_" << x << "_" << y;
						
						//we have to do this strange convertion because the ogre-wf coord convertion is a bit of a mess with the 
						//fillAlphaLayer(...) method. I don't have time to fix this now.
						int x_ = (x == (getNumberOfSegmentsPerAxis() - 1)) ? 0 : x + 1;
						fillAlphaLayer(finalChunk->getPtr(), surface->getData(), i, x_ * 64, (getNumberOfSegmentsPerAxis() - y - 1) * 64);
	
	
	
																			

					}
				}
			}
		}
		if (i < 2) {
			Ogre::TextureUnitState * splatTextureUnitState = pass->createTextureUnitState();
			splatTextureUnitState->setTextureName(shader->getTextureName());
			splatTextureUnitState->setTextureCoordSet(1);
    		splatTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
		}
		
		
	}
	
	ilBindImage(ImageName);
		
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

/*	char name[100];
	strcpy(name, (std::string("/home/erik/opt/worldforge/share/ember/data/temp/") + splatTextureName + std::string(".png")).c_str());
	ilSaveImage(name);*/
	
	
	Ogre::TexturePtr splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, "General", dataStreamPtr, getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, pixelFormat);
/*	temp.setNull();
	splatTexture.setNull();*/
	ilDeleteImages(1, &ImageName);
	alphaTextureUnitState->setTextureName(splatTextureName);
    alphaTextureUnitState->setTextureCoordSet(0);
	alphaTextureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_MIRROR);

    
   
	//create all lod levels
	if (getTerrainOptions().debuglod) {
		for (int i = 1; i < getTerrainOptions().maxGeoMipMapLevel; ++i) {
			Ogre::Technique *tech = material->createTechnique();
			tech->setLodIndex(i);
			
			Ogre::TextureUnitState *textureUnitState = tech->createPass()->createTextureUnitState();
			std::stringstream ss;
			ss << "lod" << i << ".png";
			textureUnitState->setTextureName(ss.str());
			textureUnitState->setTextureCoordSet(0);
		
		}
	} else {
		for (int i = 1; i < getTerrainOptions().maxGeoMipMapLevel; ++i) {
		
		}
	
	}
	
	
	return material;

 
 }

Ogre::MaterialPtr EmberOgre::TerrainPage::generateTerrainMaterialSimple( )
{

	Mercator::Segment* segment = mGenerator->getTerrain().getSegment((int)mPosition.x(), (int)mPosition.y());
 
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();

			
/*	segment->populateNormals();
	segment->populateSurfaces();*/
	
	EmberTerrainSceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << mPosition.x() << "_" << mPosition.y();
	mMaterialName = materialNameSS.str();
		

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(mMaterialName, "General");
	
	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
	pass->setLightingEnabled(false);
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	
    
    const Mercator::Segment::Surfacestore & surfaces = segment->getSurfaces();
    Mercator::Segment::Surfacestore::const_iterator I = surfaces.begin();

//	pass->setShininess(20);
//	pass->setSpecular(1,1,1,1);
//granite layer
	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	{
		const Mercator::Shader* mercShader = &((*I)->m_shader);
		TerrainShader* shader = mShaderMap[mercShader];
	    textureUnitState->setTextureName(shader->getTextureName());
	}
    textureUnitState->setTextureCoordSet(1);
  
	++I;
    


    int textureUnits = 0;
    for (int texNo = 1; I != surfaces.end(); ++I, ++texNo) {
        if (!(*I)->m_shader.checkIntersect(**I)) {
            continue;
        }

        //we need an unique name for our alpha texture
		std::stringstream splatTextureNameSS;
		splatTextureNameSS << mMaterialName << "_" << texNo;
		const Ogre::String splatTextureName = splatTextureNameSS.str();

		createAlphaTexture(splatTextureName, *I);
		

		
		{
			const Mercator::Shader* mercShader = &((*I)->m_shader);
			TerrainShader* shader = mShaderMap[mercShader];
			pass = shader->addPassToTechnique(material->getTechnique(0), splatTextureName);
			pass->setLightingEnabled(false);
			pass->setSelfIllumination(Ogre::ColourValue(1,1,1));
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
		++textureUnits;
    
    }
   
	//create all lod levels
	if (getTerrainOptions().debuglod) {
		for (int i = 1; i < getTerrainOptions().maxGeoMipMapLevel; ++i) {
			Ogre::Technique *tech = material->createTechnique();
			tech->setLodIndex(i);
			
			Ogre::TextureUnitState *textureUnitState = tech->createPass()->createTextureUnitState();
			std::stringstream ss;
			ss << "lod" << i << ".png";
			textureUnitState->setTextureName(ss.str());
			textureUnitState->setTextureCoordSet(0);
		
		}
	} else {
		for (int i = 1; i < getTerrainOptions().maxGeoMipMapLevel; ++i) {
		
		}
	
	}
	
	
		

	
	
	
			
		// (LayerBlendOperationEx op, LayerBlendSource source1=LBS_TEXTURE, LayerBlendSource source2=LBS_CURRENT, const ColourValue &arg1=ColourValue::White, const ColourValue &arg2=ColourValue::White, Real manualBlend=0.0)
//		LayerBlendOperationEx op, LayerBlendSource source1=LBS_TEXTURE, LayerBlendSource source2=LBS_CURRENT, Real arg1=1.0, Real arg2=1.0, Real manualBlend=0.0)
        
        
	return material;
        
        
        
    
	
	

}
