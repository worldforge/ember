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
: mPosition(position), mShaderMap(shaderMap), mGenerator(generator), mMaterial(0), mBytesPerPixel(4), mAlphaMapScale(2)
{
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	
	for (int y_axis = 0; y_axis < segmentsPerAxis; ++y_axis) {
		for (int x_axis = 0; x_axis < segmentsPerAxis; ++x_axis) {
			Mercator::Segment* segment = mGenerator->getTerrain().getSegment((int)((mPosition.x() * segmentsPerAxis) + x_axis), (int)((mPosition.y() * segmentsPerAxis) + y_axis));
//			Mercator::Segment* segment = mGenerator->getTerrain().getSegment((int)((mPosition.x() ) + x_axis), (int)((mPosition.y() ) + y_axis));
			//assert(segment);
			mMercatorSegments.push_back(segment);
		}
	}
}


TerrainPage::~TerrainPage()
{
}

int TerrainPage::getNumberOfSegmentsPerAxis() const
{
	return (getTerrainOptions().pageSize -1) / 64;
}
	
float TerrainPage::getMaxHeight()
{
	SegmentVector::iterator I = mMercatorSegments.begin();
	SegmentVector::iterator I_end = mMercatorSegments.end();
	float max = 0;
	for (; I != I_end; ++I) {
		if (*I) {
			max = std::max(max, (*I)->getMax());
		}
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	SegmentVector::iterator I = mMercatorSegments.begin();
	SegmentVector::iterator I_end = mMercatorSegments.end();
	float min = 0;
	for (; I != I_end; ++I) {
		if (*I) {
			min = std::min(min, (*I)->getMin());
		}
	}
	return min;
}


Ogre::Material* TerrainPage::generateTerrainMaterials() {

	if (getNumberOfSegmentsPerAxis() == 1) {
		mMaterial = generateTerrainMaterialSimple();
	} else {
		mMaterial = generateTerrainMaterialComplex();
/*		std::vector<const Mercator::Segment::Surfacestore&> surfaceStores;
		
		for (mMercatorSegments::const_iterator I = mMercatorSegments.begin(); I != mMercatorSegments.end(); ++I) {
			surfaceStores.push_back(*I);
		}*/
		
	}
	//store our new material in the materialStore for later retrieval   
	mMaterial->load();
/*	std::stringstream ss;
	ss << mPosition.getX() <<":"<< mPosition.getY();	
/*	materialStore[ss.str()] = static_cast<Ogre::Material*>(Ogre::MaterialManager::getSingleton().getByName("Malebuilder/Body"));
	return;*/
//	materialStore[ss.str()] = material;*/
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


Ogre::Material* TerrainPage::getMaterial() const
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

Ogre::DataChunk* TerrainPage::convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor) {
    //int width = getTerrainOptions().pageSize - 1;
    int width = 64;
    int bufferSize = width*width*mBytesPerPixel*factor*factor;
	Ogre::DataChunk chunk(dataStart, 65*65);
    Ogre::DataChunk* finalChunk = new Ogre::DataChunk();
    finalChunk->allocate(bufferSize);
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
    
	return finalChunk;
}

Ogre::Texture* TerrainPage::createAlphaTexture(Ogre::String name, Mercator::Surface* surface) {


    //the format for our alpha texture
    //for some reason we can't use PF_A8
    Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;

	/*first we need to change the 8 bit mask into 32 bits
	 * because somehow Ogre doesn't like the 8 bit mask
	 * 
	 */
	Ogre::DataChunk* finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), 1);
	
	//printTextureToImage(finalChunk, name, pixelFormat);
	
	//create the new alpha texture
	Ogre::Texture* splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(name, *finalChunk, getAlphaTextureSize(), getAlphaTextureSize(), pixelFormat);
	finalChunk->clear();
	delete finalChunk;
	
	return splatTexture;
	
}

void TerrainPage::printTextureToImage(Ogre::DataChunk* dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat, int height, int width) {
// DEBUG   
//prints the bitmap to a png-image
//TODO: remove when finished with debugging
	
	const Ogre::String extension = "png";
	
	Ogre::ImageCodec::ImageData imgData;
	imgData.width = width;
	imgData.height = height;
	
	//imgData.depth =  1;
	imgData.format = pixelFormat;	
	     		
	Ogre::Codec * pCodec = Ogre::Codec::getCodec(extension);
	
	// Write out
	
	//MAKE SURE THAT THE DIRECTORY EXISTS!!!
	//ELSE YOY'LL GET EVIL RESOURCE ERRORS!!
	pCodec->codeToFile(*dataChunk, Ogre::String("img/") + name + "." + extension, &imgData);
	
}

};

inline int  EmberOgre::TerrainPage::getAlphaTextureSize( ) const
{
	return (getTerrainOptions().pageSize - 1);

}

Ogre::Material * EmberOgre::TerrainPage::generateTerrainMaterialComplex( )
{ 
 
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();

				
	EmberTerrainSceneManager* sceneManager = EmberOgre::getSingleton().getSceneManager();
	std::stringstream materialNameSS;
	materialNameSS << "EmberTerrain_Segment";
	materialNameSS << "_" << mPosition.x() << "_" << mPosition.y();
	mMaterialName = materialNameSS.str();
		

	Ogre::Material* material = sceneManager->createMaterial(mMaterialName);
	
	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
	pass->setLightingEnabled(false);
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	
	//for convenience we declare these here to be used every time we want to iterate over the segments later on
	SegmentVector::iterator segmentI_begin = mMercatorSegments.begin();
	SegmentVector::iterator segmentI_end = mMercatorSegments.end();
	
	//we'll start by getting a the first surface of the first segment, since we're only interested in the name of the texture used for the backdrop (we won't be needing any alpha texture)    
    //const Mercator::Segment::Surfacestore & surfaces
	Mercator::Segment* aValidSegment;
	typedef std::list<Mercator::Surface*>::iterator vectorOfSurfaceListIterator;
//	std::vector<std::list<Mercator::Surface*>::iterator> surfaceListIterators;
	std::map<Mercator::Segment*, vectorOfSurfaceListIterator> segmentSurfaceListIteratorMapping;
	for(SegmentVector::iterator I = segmentI_begin; I != segmentI_end; ++I) {
		if (*I) {
			aValidSegment = *I;
			std::list<Mercator::Surface*>::iterator tempI = (*I)->getSurfaces().begin();
			//tempI++;
//			surfaceListIterators.push_back(tempI);
			segmentSurfaceListIteratorMapping[*I] = tempI;
		} else {
/*			segmentSurfaceListIteratorMapping[*I] = surfaceListIterators;
			std::list<Mercator::Surface*>::iterator tempI;
			*tempI = 0;
			surfaceListIterators.push_back(tempI);*/
		}
	}
	
	Mercator::Surface* surface = *aValidSegment->getSurfaces().begin();
    //Mercator::Segment::Surfacestore::const_iterator I = surfaces.begin();

//	pass->setShininess(20);
//	pass->setSpecular(1,1,1,1);

//granite layer
	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
	{
		const Mercator::Shader* mercShader = &(surface->m_shader);
		TerrainShader* shader = mShaderMap[mercShader];
	    textureUnitState->setTextureName(shader->getTextureName());
	    textureUnitState->setTextureCoordSet(1);
	}
  
	//now loop over all the remaining surfaces, and for each surface create a common alpha image from the different segments
	int i = 1;
	for (int i = 1; i < aValidSegment->getSurfaces().size(); ++i) {

		TerrainShader* shader;
		
		//check if at least one surface instersects, else continue
		bool intersects = false;
	//	for(std::vector<std::list<Mercator::Surface*>::iterator>::iterator I = surfaceListIterators.begin(); I != surfaceListIterators.end(); ++I) {
		for(SegmentVector::iterator I = segmentI_begin; I != segmentI_end; ++I) {
			if (*I) {
				std::list<Mercator::Surface*>::iterator* surfaceListI = &(segmentSurfaceListIteratorMapping[*I]);
				surface = *(++(*surfaceListI));
			
				const Mercator::Shader* mercShader = &(surface->m_shader);
				shader = mShaderMap[mercShader];
				if (surface->m_shader.checkIntersect(*surface)) {
					intersects = true;
				}
			
			}

		}
/*		if (!intersects) {
			continue;
		}*/
		
        //we need an unique name for our alpha texture
		std::stringstream splatTextureNameSS;
		splatTextureNameSS << mMaterialName << "_" << i;
		const Ogre::String splatTextureName = splatTextureNameSS.str();
		
		//the format for our alpha texture
		//for some reason we can't use PF_A8
		Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;

		ILuint ImageName;
		ilGenImages( 1, &ImageName );
		ilBindImage( ImageName );
		ilTexImage(getAlphaTextureSize() , getAlphaTextureSize(), 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, 0 );
		unsigned char * imagePointer = ilGetData();
		
		//clear the image
		memset (imagePointer, 0, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel);
		
		SegmentVector::iterator I = segmentI_begin;
		
		for (int y = 0; y < getNumberOfSegmentsPerAxis(); ++y) {
			for (int x = 0; x < getNumberOfSegmentsPerAxis(); ++x) {
				if (*I) {
					std::list<Mercator::Surface*>::iterator surfaceListI = segmentSurfaceListIteratorMapping[*(I)];
					surface = *surfaceListI;
					std::stringstream splatTextureNameSS_;
					splatTextureNameSS_ << splatTextureName << "_" << x << "_" << y;
					Ogre::DataChunk* tempChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), 1);
	/*				printTextureToImage(tempChunk, splatTextureNameSS_.str(), pixelFormat, 64, 64);*/
					
					ILuint tempImageName;
					ilGenImages( 1, &tempImageName );
					ilBindImage( tempImageName );
					ilTexImage(64 , 64, 1, mBytesPerPixel, IL_BGRA, IL_UNSIGNED_BYTE, tempChunk->getPtr());
					ilBindImage(ImageName);
					ilOverlayImage(tempImageName, x * 64, y * 64, 0);
					
	// 				char name[100];
	// 				strcpy(name, (std::string("/home/erik/opt/worldforge/share/ember/data/temp/") + splatTextureNameSS_.str() + std::string(".png")).c_str());
	// 				ilSaveImage(name);
	
					ilDeleteImages(1, &tempImageName);
				}
				++I;
			}
		}
		ilBindImage(ImageName);
		
		if (mAlphaMapScale > 1) {
			//double the size of the image
			
			//use filter to smooth everything out
			iluImageParameter(ILU_FILTER, ILU_SCALE_BSPLINE);
			iluScale(getAlphaTextureSize() * mAlphaMapScale, getAlphaTextureSize() * mAlphaMapScale, 1);
		}
		
		imagePointer = ilGetData();
		//wrap the image data in a Datachunk
		Ogre::DataChunk finalChunk(imagePointer, getAlphaTextureSize() * getAlphaTextureSize() * mBytesPerPixel * (mAlphaMapScale * 2) );
//  		printTextureToImage(&finalChunk, splatTextureName, pixelFormat, (getTerrainOptions().pageSize - 1) * 2, (getTerrainOptions().pageSize - 1)*2);

		Ogre::Texture* splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(splatTextureName, finalChunk, getAlphaTextureSize() * mAlphaMapScale , getAlphaTextureSize() * mAlphaMapScale, pixelFormat);
		ilDeleteImages(1, &ImageName);
		
// 		SegmentVector::iterator I = segmentI_begin;
// 		std::vector<Ogre::DataChunk*> datachunks;
// 		for(SegmentVector::iterator I = segmentI_begin; I != segmentI_end; ++I) {
// 			Ogre::DataChunk* finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), factor);
// 			
// 		}
// 		createAlphaTexture(splatTextureName, *I);

//     int textureUnits = 0;
//     for (int texNo = 1; I != surfaces.end(); ++I, ++texNo) {
//         if (!(*I)->m_shader.checkIntersect(**I)) {
//             continue;
//         }
		

/*			std::vector<std::list<Mercator::Surface*>::iterator>::iterator Itemp = surfaceListIterators.begin();
			surface = *(*Itemp);*/
			//surface = *(mMercatorSegments[0]->getSurfaces().begin());
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

//		++textureUnits;
    
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

Ogre::Material * EmberOgre::TerrainPage::generateTerrainMaterialSimple( )
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
		

	Ogre::Material* material = sceneManager->createMaterial(mMaterialName);
	
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
