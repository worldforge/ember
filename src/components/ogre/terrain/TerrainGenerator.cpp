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
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "EmberOgre.h"

#include "EmberOgrePrerequisites.h"
#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
//#include "EmberTerrainRenderable.h"
#include "TerrainShader.h"
#include "EmberSceneManager/include/EmberTerrainPageSource.h"
#include "EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "environment/Foliage.h"


//#include "environment/GroundCover.h"

#include "TerrainGenerator.h"
#include "TerrainPage.h"
#include <Mercator/Area.h>
#include "model/ModelDefinition.h"
#include "model/ModelDefinitionManager.h"


using namespace Ogre;
namespace EmberOgre {


TerrainGenerator::TerrainGenerator()
: mGrassShader(0)
{
	//new Foliage(EmberOgre::getSingleton().getSceneManager());
	
    loadTerrainOptions();
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED); //, mOptions.pageSize - 1);

  
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();

	
	
    createShader(std::string(configSrv->getValue("shadertextures", "rock")), new Mercator::FillShader());
	
    createShader(std::string(configSrv->getValue("shadertextures", "sand")), new Mercator::BandShader(-2.f, 1.5f)); // Sandy beach
 
 	mGrassShader = createShader(std::string(configSrv->getValue("shadertextures", "grass")), new Mercator::GrassShader(1.f, 80.f, .5f, 1.f)); // Grass




//      createShader(std::string(configSrv->getValue("shadertextures", "snow")), new Mercator::HighShader(110.f)); // Snow
//      createShader(std::string(configSrv->getValue("shadertextures", "seabottom")), new Mercator::DepthShader(0.f, -10.f)); // Underwater


//    this->addShader(new TerrainShader(std::string(configSrv->getVariable("Shadertextures", "grass")), new Mercator::GrassShader(1.f, 80.f, .5f, 1.f))); // Grass
   
	
	mTerrainPageSource = new EmberTerrainPageSource(this);
    EmberOgre::getSingleton().getSceneManager()->registerPageSource(EmberTerrainPageSource::Name, mTerrainPageSource);



    EmberOgre::getSingleton().getSceneManager()->setWorldGeometry(mOptions);
	Ogre::Root::getSingleton().addFrameListener(this);

}

TerrainGenerator::~TerrainGenerator()
{
	delete mTerrain;
	delete mTerrainPageSource;
	
}

const Mercator::Terrain& TerrainGenerator::getTerrain() const
{
	return *mTerrain;
}

const Ogre::TerrainOptions& TerrainGenerator::getTerrainOptions() const
{
	return mOptions;
}

void TerrainGenerator::loadTerrainOptions()
{
// 	Ogre::TerrainOptions& options =  *(new Ogre::TerrainOptions());
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();
	
	if (configSrv->itemExists("terrain", "detailtile"))
		mOptions.detailTile = (int)configSrv->getValue("terrain", "detailtile");
	
	if (configSrv->itemExists("terrain", "maxmipmaplevel"))
		mOptions.maxGeoMipMapLevel = (int)configSrv->getValue("terrain", "maxmipmaplevel");
	
	if (configSrv->itemExists("terrain", "pagesize"))
		mOptions.pageSize = (int)configSrv->getValue("terrain", "pagesize");
	
 
	
	
	if (configSrv->itemExists("terrain", "tilesize"))
		mOptions.tileSize = (int)configSrv->getValue("terrain", "tilesize");
	
	if (configSrv->itemExists("terrain", "maxpixelerror"))
		mOptions.maxPixelError = (int)configSrv->getValue("terrain", "maxpixelerror");
	
	if (configSrv->itemExists("terrain", "vertexcolours"))
		mOptions.coloured = (bool)configSrv->getValue("terrain", "vertexcolours");
	
	if (configSrv->itemExists("terrain", "vertexnormals"))
		mOptions.lit = (bool)configSrv->getValue("terrain", "vertexnormals");
	
	if (configSrv->itemExists("terrain", "usetristrips"))
		mOptions.useTriStrips = (bool)configSrv->getValue("terrain", "usetristrips");
	
	if (configSrv->itemExists("terrain", "vertexprogrammorph"))
		mOptions.lodMorph = (bool)configSrv->getValue("terrain", "vertexprogrammorph");

	if (configSrv->itemExists("terrain", "lodmorphstart"))
		mOptions.lit = (bool)configSrv->getValue("terrain", "lodmorphstart");
	
	if (configSrv->itemExists("terrain", "usedebuglodcolors"))
		mOptions.debuglod = (bool)configSrv->getValue("terrain", "usedebuglodcolors");
	
	mOptions.scale.y = 100;
}

TerrainShader* TerrainGenerator::createShader(const std::string& textureName, Mercator::Shader* mercatorShader)
{
	int index = mShaderMap.size();
    TerrainShader* shader = new TerrainShader(mTerrain, index, textureName, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	return shader;
}

TerrainShader* TerrainGenerator::createShader(Ogre::MaterialPtr material, Mercator::Shader* mercatorShader)
{
	int index = mShaderMap.size();
    TerrainShader* shader = new TerrainShader(mTerrain, index, material, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	return shader;
}



void TerrainGenerator::addArea(Mercator::Area* area)
{
	mTerrain->addArea(area);
	if (!mAreaShaders.count(area->getLayer())) {
		TerrainShader* shader;
		const ModelDefinition::AreaDefinition* areaDef = ModelDefinitionManager::getSingleton().getAreaDefinition(area->getLayer());
		if (areaDef) {
			if (areaDef->MaterialName != "") {
				Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(areaDef->MaterialName));
				if (!material.isNull()) {
					material->load();
					shader = createShader(material, new Mercator::AreaShader(area->getLayer()));
				} else {
					return;
				}
			} else {
				shader = createShader(areaDef->TextureName, new Mercator::AreaShader(area->getLayer()));
			}
			mAreaShaders[area->getLayer()] = shader;
		}
	}
	if (mAreaShaders.count(area->getLayer())) {
		markShaderForUpdate(mAreaShaders[area->getLayer()]);
	}
}

void TerrainGenerator::markShaderForUpdate(TerrainShader* shader)
{
	mShadersToUpdate.insert(shader);
}

bool TerrainGenerator::frameStarted(const Ogre::FrameEvent & evt)
{
	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
		for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
			J->second->populateSurfaces();
		}	
		
		for (ShaderSet::iterator I = mShadersToUpdate.begin(); I != mShadersToUpdate.end(); ++I) {
			for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
				J->second->updateShaderTexture(*I);
			}
		}	
	}
	
	mShadersToUpdate.clear();
	return true;

}


void TerrainGenerator::prepareSegments(long segmentXStart, long segmentZStart, long numberOfSegments, bool alsoPushOntoTerrain)
{
//TODO: implement!


// 	int i,j;
// 	for (i = segmentXStart; i < segmentXStart + numberOfSegments; ++i) {
// 		for (j = segmentZStart; j < segmentZStart + numberOfSegments; ++j) {
// 			if (i >= mXmin && i <= mXmax && j >= mYmin && j <=mYmax) {
// 				mTerrain->getSegment(i, j)->populate();
// 				mTerrain->getSegment(i, j)->populateNormals();
// 				mTerrain->getSegment(i, j)->populateSurfaces();
// 				TerrainPosition pos(i,j);
// 				generateTerrainMaterials(mTerrain->getSegment(i, j), pos);
// 				if (alsoPushOntoTerrain) {
// 					TerrainPosition pos(i, j);
// 					mTerrainPageSource->pushPage(pos);
// 				}
// 			}
// 		}
// 	}
// //	generateUnderVegetation(0, 0, 1);
// //	generateUnderVegetation(segmentXStart, segmentZStart, numberOfSegments);
// 	mTerrainPageSource->setHasTerrain(true);
// 	if (alsoPushOntoTerrain) {
// 		mTerrainPageSource->resizeTerrain();
// 	}
	
}

int TerrainGenerator::getSegmentSize() const
{
	return mOptions.pageSize - 1;
}

void TerrainGenerator::prepareAllSegments(bool alsoPushOntoTerrain)
{
	
// 	mTerrain->getSegment(0, 0)->populate();
// 	TerrainPosition pos(0,0);
// 	generateTerrainMaterials(mTerrain->getSegment(0, 0), pos);
// 	if (alsoPushOntoTerrain) {
// 		mTerrainPageSource->pushPage(pos);
// 	}
// 	mTerrainPageSource->setHasTerrain(true);
// 	if (alsoPushOntoTerrain) {
// 		mTerrainPageSource->resizeTerrain();
// 	}
// 
// 	return;	
	bool showFoliage = false;
	
	if (Ember::EmberServices::getInstance()->getConfigService()->itemExists("graphics", "foliage") && GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1")) {
		showFoliage = Ember::EmberServices::getInstance()->getConfigService()->getValue("graphics", "foliage");
	}
	
	
	int i,j;
	for (i = mXmin; i < mXmax; ++i) {
		for (j = mYmin; j < mYmax; ++j) {
			Mercator::Segment* segment = mTerrain->getSegment(i, j);
			if (segment) {
				segment->populate();
				segment->populateNormals();
				segment->populateSurfaces();
			}
		}
	}

	int mercatorSegmentsPerPage =  getSegmentSize() / 64;
	int xNumberOfPages = (int)ceil((mXmax - mXmin) / (double)mercatorSegmentsPerPage);
	int yNumberOfPages = (int)ceil((mYmax - mYmin) / (double)mercatorSegmentsPerPage);
	int xStart = (int)floor(mXmin / (double)(mercatorSegmentsPerPage));
	int yStart = (int)floor(mYmin / (double)(mercatorSegmentsPerPage));
	for (i = 0; i < xNumberOfPages; ++i) {
		for (j = 0; j < yNumberOfPages; ++j) {
			TerrainPosition pos(xStart + i, yStart + j);
			std::stringstream ss;
			ss << pos;
			TerrainPage* page = new TerrainPage(pos, mShaderMap, this);
			mPages[ss.str()] = page;
			for (std::list<TerrainShader*>::iterator I = mBaseShaders.begin(); I != mBaseShaders.end(); ++I) {
				page->addShader(*I);
			}
			page->generateTerrainMaterials();
			if (showFoliage && mGrassShader) {
				page->createFoliage(mGrassShader);
			}
			if (alsoPushOntoTerrain) {
				mTerrainPageSource->addPage(page);
			}
		}
	}
	
//	generateUnderVegetation(-2, -2, 4);
	
	mTerrainPageSource->setHasTerrain(true);
	if (alsoPushOntoTerrain) {
		mTerrainPageSource->resizeTerrain();
	}
	
}



bool TerrainGenerator::isValidTerrainAt(TerrainPosition& position)
{
	int x = (int)position.x();
	int y = (int)position.y();
	Mercator::Segment* segment = mTerrain->getSegment(position.x(),position.y());
	return (segment &&	segment->isValid());
//	return (segment &&	segment->isValid() && getMaterialForSegment(position));
}









float TerrainGenerator::getHeight(const TerrainPosition& point) const
{
	
	float height = 0;
	WFMath::Vector<3> vector;
	
	mTerrain->getHeightAndNormal(point.x(), point.y(), height, vector);

	return height;
	
}





bool TerrainGenerator::initTerrain(Eris::Entity *we, Eris::View *world) 
{

   if (!we->hasAttr("terrain")) {
        S_LOG_FAILURE( "View entity has no terrain" )
        S_LOG_FAILURE( "View entity id " << we->getId() )
        return false;
    }
    const Atlas::Message::Element &terrain = we->valueOfAttr("terrain");
    if (!terrain.isMap()) {
        S_LOG_FAILURE( "Terrain is not a map" )
    }
    const Atlas::Message::MapType & tmap = terrain.asMap();
    Atlas::Message::MapType::const_iterator I = tmap.find("points");
    int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    if (I == tmap.end()) {
        S_LOG_FAILURE( "No terrain points" )
    }
	if (I->second.isList()) {
        // Legacy support for old list format.
        const Atlas::Message::ListType & plist = I->second.asList();
        Atlas::Message::ListType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->isList()) {
                S_LOG_INFO( "Non list in points" )
                continue;
            }
            const Atlas::Message::ListType & point = J->asList();
            if (point.size() != 3) {
                S_LOG_INFO( "point without 3 nums" )
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
    //        m_terrain.setBasePoint(x, y, point[2].asNum());
			mTerrain->setBasePoint(x,y,point[2].asNum());
        }
    } else if (I->second.isMap()) {

        const Atlas::Message::MapType & plist = I->second.asMap();
        Atlas::Message::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
                S_LOG_INFO( "Non list in points" )
                continue;
            }
            const Atlas::Message::ListType & point = J->second.asList();
            if (point.size() != 3) {
                S_LOG_INFO( "point without 3 nums" )
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            float z = point[2].asNum();
            Mercator::BasePoint bp;
            if (mTerrain->getBasePoint(x, y, bp) && (z == bp.height())) {
                S_LOG_INFO( "Point [" << x << "," << y << " unchanged")
                continue;
            }
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
            bp.height() = z;
            // FIXME Sort out roughness and falloff, and generally
            // verify this code is the same as that in Terrain layer
            mTerrain->setBasePoint(x, y, bp);
        }
	
	
/*        const Atlas::Message::MapType & plist = I->second.asMap();
        Atlas::Message::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
                std::cout << "Non list in points" << std::endl << std::flush;
                continue;
            }
            const Atlas::Message::ListType & point = J->second.asList();
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
			mTerrain->setBasePoint(x,y,point[2].asNum());
//System::instance()->getGraphics()->getTerrainRenderer()->m_terrain.setBasePoint(x,y,point[2].asNum());
//System::Instance()->getGraphics(x,y,point[2].asNum());
        }*/

      

    } else {
        S_LOG_FAILURE( "Terrain is the wrong type" )
        return false;
    }
    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;
    mSegments = &mTerrain->getTerrain();
    return true;
}

// Ogre::Material* TerrainGenerator::getMaterialForSegment(TerrainPosition& atPosition)
// {
// 	long x = (long)atPosition.x();
// 	long y = (long)atPosition.y();
// 	std::stringstream ss;
// 	ss << x <<":"<<y;
// 	return materialStore[ss.str()];
// }

const TerrainPosition TerrainGenerator::getMax( ) const
{
	return TerrainPosition(mXmax * 64, mYmax * 64);
}

const TerrainPosition TerrainGenerator::getMin( ) const
{
	return TerrainPosition(mXmin * 64, mYmin * 64);
}

}


