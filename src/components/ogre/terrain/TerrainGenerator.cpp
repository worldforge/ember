/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "DimeOgre.h"

#include "MathConverter.h"
#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "services/logging/LoggingService.h"
#include "DimeTerrainRenderable.h"
#include "TerrainShader.h"
#include "DimeTerrainPageSource.h"

#include "TerrainGenerator.h"


using namespace Ogre;
namespace DimeOgre {

/*
TerrainGenerator* TerrainGenerator::_instance = 0;


TerrainGenerator & TerrainGenerator::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new TerrainGenerator;
	return *_instance;
}
*/


TerrainGenerator::TerrainGenerator()
: mTerrain(Mercator::Terrain::SHADED)
, mNumberOfTilesInATerrainPage(65)
{
/*    mTerrain.addShader(new Mercator::FillShader());
    mTerrain.addShader(new Mercator::BandShader(-2.f, 1.5f)); // Sandy beach
    mTerrain.addShader(new Mercator::GrassShader(1.f, 80.f, .5f, 1.f)); // Grass
    mTerrain.addShader(new Mercator::DepthShader(0.f, -10.f)); // Underwater
    mTerrain.addShader(new Mercator::HighShader(110.f)); // Snow
  */
  
//    this->addShader(new TerrainShader("granite.png", new Mercator::FillShader()));
    this->addShader(new TerrainShader("terr_rock6.jpg", new Mercator::FillShader()));
    this->addShader(new TerrainShader("sand.png", new Mercator::BandShader(-2.f, 1.5f))); // Sandy beach
    this->addShader(new TerrainShader("rabbithill_grass_hh.png", new Mercator::GrassShader(1.f, 80.f, .5f, 1.f))); // Grass
    this->addShader(new TerrainShader("dark.png", new Mercator::DepthShader(0.f, -10.f))); // Underwater
    this->addShader(new TerrainShader("snow.png", new Mercator::HighShader(110.f))); // Snow
    
    mTerrainPageSource = new DimeTerrainPageSource(this);
    DimeOgre::getSingleton().getSceneManager()->registerPageSource("DimeTerrain", mTerrainPageSource);
    
    const Ogre::String config = Ogre::String("terrain.cfg");
    DimeOgre::getSingleton().getSceneManager()->setWorldGeometry(config);

}

TerrainGenerator::~TerrainGenerator()
{}

void TerrainGenerator::addShader(TerrainShader* shader)
{
	mTerrain.addShader(shader->getShader());
	mShaderMap[shader->getShader()] = shader;
}


void TerrainGenerator::prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain)
{
	int i,j;
	for (i = segmentXStart; i < segmentXStart + numberOfSegments; ++i) {
		for (j = segmentZStart; j < segmentZStart + numberOfSegments; ++j) {
			if (i >= mXmin && i <= mXmax && j >= mYmin && j <=mYmax) {
				mTerrain.getSegment(i, j)->populate();
				generateTerrainMaterials(mTerrain.getSegment(i, j), i,j);
				if (alsoPushOntoTerrain) {
					mTerrainPageSource->pushPage(i, j);
				}
			}
		}
	}
	//mTerrainPageSource->setHasTerrain(true);
	if (alsoPushOntoTerrain) {
		mTerrainPageSource->resizeTerrain();
	}
	
}

void TerrainGenerator::prepareAllSegments(bool alsoPushOntoTerrain)
{
	int i,j;
	for (i = mXmin; i < mXmax; ++i) {
		for (j = mYmin; j < mYmax; ++j) {
			mTerrain.getSegment(i, j)->populate();
			generateTerrainMaterials(mTerrain.getSegment(i, j), i,j);
			if (alsoPushOntoTerrain) {
				mTerrainPageSource->pushPage(i, j);
			}
		}
	}
	mTerrainPageSource->setHasTerrain(true);
	if (alsoPushOntoTerrain) {
		mTerrainPageSource->resizeTerrain();
	}
	
}


bool TerrainGenerator::isValidTerrainAt(int x, int y)
{
	Mercator::Segment* segment = mTerrain.getSegment(x,y);
	return (segment &&	segment->isValid() && getMaterialForSegment(x, y));
}



void TerrainGenerator::generateTerrainMaterials(Mercator::Segment* segment, long segmentX, long segmentY) {
	
 
	Ogre::ushort numberOfTextureUnitsOnCard = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNumTextureUnits();

			
	segment->populateNormals();
	segment->populateSurfaces();
	
	Ogre::TerrainSceneManager* sceneManager = DimeOgre::getSingleton().getSceneManager();
	std::stringstream materialNameSS;
	materialNameSS << "DimeTerrain_Segment";
	materialNameSS << "_" << segmentX << "_" << segmentY;
	const Ogre::String materialName = materialNameSS.str();
	Ogre::Material* material = sceneManager->createMaterial(materialName);
	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
	//pass->setLightingEnabled(false);
	//pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	
    
    const Mercator::Segment::Surfacestore & surfaces = segment->getSurfaces();
    Mercator::Segment::Surfacestore::const_iterator I = surfaces.begin();


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
		splatTextureNameSS << materialName << "_" << texNo;
		const Ogre::String splatTextureName = splatTextureNameSS.str();

		createAlphaTexture(splatTextureName, *I);
		

		
		{
			const Mercator::Shader* mercShader = &((*I)->m_shader);
			TerrainShader* shader = mShaderMap[mercShader];
			pass = shader->addPassToTechnique(material->getTechnique(0), splatTextureName);
/*
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
   
	//store our new material in the materialStore for later retrieval   
	material->load();
	std::stringstream ss;
	ss << segmentX <<":"<<segmentY;	
	materialStore[ss.str()] = material;
		// (LayerBlendOperationEx op, LayerBlendSource source1=LBS_TEXTURE, LayerBlendSource source2=LBS_CURRENT, const ColourValue &arg1=ColourValue::White, const ColourValue &arg2=ColourValue::White, Real manualBlend=0.0)
//		LayerBlendOperationEx op, LayerBlendSource source1=LBS_TEXTURE, LayerBlendSource source2=LBS_CURRENT, Real arg1=1.0, Real arg2=1.0, Real manualBlend=0.0)
        
        
        
        
        
        
    
	
	
}



Ogre::DataChunk* TerrainGenerator::convertWFAlphaTerrainToOgreFormat(Ogre::uchar* dataStart, short factor) {
    int width = mNumberOfTilesInATerrainPage - 1;
    int bufferSize = width*width*4*factor*factor;
	Ogre::DataChunk chunk(dataStart, (mNumberOfTilesInATerrainPage) * (mNumberOfTilesInATerrainPage));
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
    	chunkPtr += mNumberOfTilesInATerrainPage;
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



void TerrainGenerator::createAlphaTexture(Ogre::String name, Mercator::Surface* surface) {
    short factor = 1;


    //the format for our alpha texture
    //for some reason we can't use PF_A8
    Ogre::PixelFormat pixelFormat = PF_B8G8R8A8;

	/*first we need to change the 8 bit mask into 32 bits
	 * because somehow Ogre doesn't like the 8 bit mask
	 * 
	 */
	Ogre::DataChunk* finalChunk = convertWFAlphaTerrainToOgreFormat(surface->getData(), factor);
	
	//For debugging
/*	{
		const Mercator::Shader* mercShader = &((*I)->m_shader);
		TerrainShader* shader = mShaderMap[mercShader];
		printTextureToImage(finalChunk, shader->getTextureName() + Ogre::StringConverter::toString(segmentX)+ Ogre::StringConverter::toString(segmentY), pixelFormat);
	}
	*/
	//create the new alpha texture
	Ogre::Texture* splatTexture = Ogre::Root::getSingletonPtr()->getTextureManager()->loadRawData(name, *finalChunk, (mNumberOfTilesInATerrainPage - 1) * factor, (mNumberOfTilesInATerrainPage - 1) * factor, pixelFormat);
	
	
}
void TerrainGenerator::printTextureToImage(Ogre::DataChunk* dataChunk, const Ogre::String name, Ogre::PixelFormat pixelFormat) {
// DEBUG   
//prints the bitmap to a png-image
//TODO: remove when finished with debugging
	
	const Ogre::String extension = "png";
	
	Ogre::ImageCodec::ImageData imgData;
	imgData.width = mNumberOfTilesInATerrainPage;
	imgData.height = mNumberOfTilesInATerrainPage;
	//imgData.depth =  1;
	imgData.format = pixelFormat;	
	     		
	Ogre::Codec * pCodec = Ogre::Codec::getCodec(extension);
	
	// Write out
	pCodec->codeToFile(*dataChunk, Ogre::String("img/") + name + "." + extension, &imgData);
	
}

float TerrainGenerator::getHeight(float ogreX, float ogreZ) const
{
	
	//convert our ogre coordinates to atlas
	float atlasX = ogreX;
	float atlasY = -ogreZ;
	
	float height = mTerrain.get(atlasX, atlasY);
	return height;
	
}


float TerrainGenerator::getMaxHeightForSegment(int ogreX, int ogreZ) const
{
	
	//convert our ogre coordinates to atlas
	int atlasX = ogreX;
	int atlasY = -ogreZ;
	
	return mTerrain.getSegment(atlasX, atlasY)->getMax();
	
}

float TerrainGenerator::getMinHeightForSegment(int ogreX, int ogreZ) const
{
	
	//convert our ogre coordinates to atlas
	int atlasX = ogreX;
	int atlasY = -ogreZ;
	
	return mTerrain.getSegment(atlasX, atlasY)->getMin();
	
}


bool TerrainGenerator::initTerrain(Eris::Entity *we, Eris::World *world) 
{

   if (!we->hasProperty("terrain")) {
        std::cerr << "World entity has no terrain" << std::endl << std::flush;
        std::cerr << "World entity id " << we->getID() << std::endl
                  << std::flush;
        return false;
    }
    const Atlas::Message::Element &terrain = we->getProperty("terrain");
    if (!terrain.isMap()) {
        std::cerr << "Terrain is not a map" << std::endl << std::flush;
    }
    const Atlas::Message::Element::MapType & tmap = terrain.asMap();
    Atlas::Message::Element::MapType::const_iterator I = tmap.find("points");
    int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    if (I == tmap.end()) {
        std::cerr << "No terrain points" << std::endl << std::flush;
    }
	if (I->second.isList()) {
        // Legacy support for old list format.
        const Atlas::Message::Element::ListType & plist = I->second.asList();
        Atlas::Message::Element::ListType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::Element::ListType & point = J->asList();
            if (point.size() != 3) {
                std::cout << "point without 3 nums" << std::endl << std::flush;
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
    //        m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain.setBasePoint(x,y,point[2].asNum());
        }
    } else if (I->second.isMap()) {

        const Atlas::Message::Element::MapType & plist = I->second.asMap();
        Atlas::Message::Element::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::Element::ListType & point = J->second.asList();
            if (point.size() != 3) {
                std::cout << "point without 3 nums" << std::endl << std::flush;
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
          //  m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain.setBasePoint(x,y,point[2].asNum());
//System::instance()->getGraphics()->getTerrainRenderer()->m_terrain.setBasePoint(x,y,point[2].asNum());
//System::Instance()->getGraphics(x,y,point[2].asNum());
        }

      

    } else {
        std::cerr << "Terrain is the wrong type" << std::endl << std::flush;
        return false;
    }
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mSegments = &mTerrain.getTerrain();
    return true;
}

Ogre::Material* TerrainGenerator::getMaterialForSegment(long x, long y)
{
	std::stringstream ss;
	ss << x <<":"<<y;
	return materialStore[ss.str()];
}


}

