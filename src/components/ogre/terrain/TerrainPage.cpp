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


#include "TerrainGenerator.h"

namespace EmberOgre {

TerrainPage::TerrainPage(TerrainPosition& position, const std::map<const Mercator::Shader*, TerrainShader*> shaderMap, TerrainGenerator* generator) 
: mPosition(position), mShaderMap(shaderMap), mGenerator(generator), mMaterial(0)
{
	int segmentsPerAxis = getNumberOfSegmentsPerAxis();
	
	for (int y_axis = 0; y_axis < segmentsPerAxis; ++y_axis) {
		for (int x_axis = 0; x_axis < segmentsPerAxis; ++x_axis) {
			mMercatorSegments.push_back(mGenerator->getTerrain().getSegment(mPosition.x() + x_axis, mPosition.y() + y_axis));
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
	float max = (*I)->getMax();
	I++;
	for (; I != I_end; ++I) {
		max = std::max(max, (*I)->getMax());
	}
	return max;
}
	
float TerrainPage::getMinHeight()
{
	SegmentVector::iterator I = mMercatorSegments.begin();
	SegmentVector::iterator I_end = mMercatorSegments.end();
	float min = (*I)->getMin();
	I++;
	for (; I != I_end; ++I) {
		min = std::min(min, (*I)->getMin());
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

const Ogre::Vector3& TerrainPage::getOgrePosition() const
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
    int width = getTerrainOptions().pageSize - 1;
    int bufferSize = width*width*4*factor*factor;
	Ogre::DataChunk chunk(dataStart, (getTerrainOptions().pageSize) * (getTerrainOptions().pageSize));
    Ogre::DataChunk* finalChunk = new Ogre::DataChunk();
    finalChunk->allocate(bufferSize);
    Ogre::uchar* finalPtr = finalChunk->getPtr();
    Ogre::uchar* chunkPtr = chunk.getPtr();
    long i,j; 
    long sizeOfOneChannel = width*width;

     Ogre::uchar* tempPtr = finalPtr + bufferSize;
    for (i = 0; i < width; ++i) {
    	for (int l = 0; l < factor; ++l) {
	    	tempPtr -= (width * 4 * factor);
	    	for (j = 0; j < width; ++j) {
	        	Ogre::uchar alpha = *(chunkPtr + j);
	        	for (int k = 0; k < factor; ++k) {
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = 0;
		        	*(tempPtr++) = alpha;
	        	}
	    		
	    	}
	    	tempPtr -= (width * 4 * factor);
    	}
    	chunkPtr += getTerrainOptions().pageSize;
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
    short factor = 1;


    //the format for our alpha texture
    //for some reason we can't use PF_A8
    Ogre::PixelFormat pixelFormat = Ogre::PF_B8G8R8A8;

	/*first we need to change the 8 bit mask into 32 bits
	 * because somehow Ogre doesn't like the 8 bit mask
	 * 
	 */
	Ogre::DataChunk* finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), factor);
	
	//printTextureToImage(finalChunk, name, pixelFormat);
	
	//create the new alpha texture
	Ogre::Texture* splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(name, *finalChunk, (getTerrainOptions().pageSize - 1) * factor, (getTerrainOptions().pageSize - 1) * factor, pixelFormat);
	
	return splatTexture;
	
}

};

Ogre::Material * EmberOgre::TerrainPage::generateTerrainMaterialComplex( )
{ 
return generateTerrainMaterialSimple();
//TODO: implement stub
assert(false);
return 0; }

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
