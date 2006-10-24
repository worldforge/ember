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

#include "TerrainGenerator.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "../EmberOgre.h"

#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "TerrainShader.h"
#include "../environment/Foliage.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeSceneManager.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeData2DManager.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeTextureManager.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeOptions.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/EmberPagingLandScapeData2D_HeightField.h"
// #include "../SceneManagers/EmberPagingSceneManager/include/EmberPagingLandScapeTexture.h"
//#include "../SceneManagers/EmberPagingSceneManager/include/OgrePagingLandScapeCamera.h"
#include "ISceneManagerAdapter.h"




//#include "environment/GroundCover.h"

#include "TerrainPage.h"
#include <Mercator/Area.h>
#include "../model/ModelDefinition.h"
#include "../model/ModelDefinitionManager.h"

#include "../AvatarCamera.h"

#include <sigc++/object_slot.h>

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
#endif
#include "framework/ConsoleBackend.h"


using namespace Ogre;
namespace EmberOgre {


TerrainGenerator::TerrainGenerator(ISceneManagerAdapter* adapter)
: mGrassShader(0), hasTerrainInfo(false)
{

	registerSceneManagerAdapter( adapter);
	//new Foliage(EmberOgre::getSingleton().getSceneManager());
	
	mTerrainInfo.pageIndicesSize = adapter->getPageSize();
	
    loadTerrainOptions();
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED); //, mOptions.pageSize - 1);

  
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	
	
    createShader(std::string(configSrv->getValue("shadertextures", "rock")), new Mercator::FillShader());
	
    createShader(std::string(configSrv->getValue("shadertextures", "sand")), new Mercator::BandShader(-2.f, 1.5f)); // Sandy beach
 
 	mGrassShader = createShader(std::string(configSrv->getValue("shadertextures", "grass")), new Mercator::GrassShader(1.f, 80.f, .8f, 2.f)); // Grass




//      createShader(std::string(configSrv->getValue("shadertextures", "snow")), new Mercator::HighShader(110.f)); // Snow
//      createShader(std::string(configSrv->getValue("shadertextures", "seabottom")), new Mercator::DepthShader(0.f, -10.f)); // Underwater


//    this->addShader(new TerrainShader(std::string(configSrv->getVariable("Shadertextures", "grass")), new Mercator::GrassShader(1.f, 80.f, .5f, 1.f))); // Grass
   
	
//	mTerrainPageSource = new EmberTerrainPageSource(this);
//    EmberOgre::getSingleton().getSceneManager()->registerPageSource(EmberTerrainPageSource::Name, mTerrainPageSource);



  //  EmberOgre::getSingleton().getSceneManager()->setWorldGeometry(mOptions);
	Ogre::Root::getSingleton().addFrameListener(this);
	
	configSrv->EventChangedConfigItem.connect(sigc::mem_fun(*this, &TerrainGenerator::ConfigService_EventChangedConfigItem));
	
	Ember::ConsoleBackend::getMainConsole()->registerCommand("exportterrainmaterial",this);

}

TerrainGenerator::~TerrainGenerator()
{
	delete mTerrain;
	//delete mTerrainPageSource;
	
}

Mercator::Terrain& TerrainGenerator::getTerrain()
{
	return *mTerrain;
}


void TerrainGenerator::loadTerrainOptions()
{
 	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());
	
	//Ogre::ConfigFile cf;
 //   cf.load("terrain.cfg");

    
	// Go through all sections & settings in the file
	//Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	//
	//std::string first;
	//std::string second;
	//std::string pair;
	//Ogre::String secName, typeName, archName;
	//while (seci.hasMoreElements())
	//{
	//	secName = seci.peekNextKey();
	//	Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
	//	Ogre::ConfigFile::SettingsMultiMap::iterator i;
	//	for (i = settings->begin(); i != settings->end(); ++i)
	//	{
	//		first = i->first;
	//		second = i->second;
	//		pair = first + "=" + second;
	//	}
	//}
	
// 	Ogre::PagingLandScapeSceneManager* sceneManager = getEmberSceneManager();

	


	getAdapter()->setResourceGroupName("General" );

	getAdapter()->loadOptions(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "/terrain.cfg");
	
	getAdapter()->setCamera( EmberOgre::getSingleton().getMainCamera()->getCamera());
	
	
	//Ogre::PagingLandScapeOptions::getSingleton().data2DFormat = "EmberHeightField";

}

ISceneManagerAdapter* TerrainGenerator::getAdapter() const
{
	return mSceneManagerAdapter;
}

// // EmberPagingSceneManager* TerrainGenerator::getEmberSceneManager()
// // {
// // 	return static_cast<EmberPagingSceneManager*>(EmberOgre::getSingleton().getSceneManager());
// // }
// // 
// // const EmberPagingSceneManager* TerrainGenerator::getEmberSceneManager() const
// // {
// // 	return static_cast<const EmberPagingSceneManager*>(EmberOgre::getSingleton().getSceneManager());
// // }

TerrainShader* TerrainGenerator::createShader(const std::string& textureName, Mercator::Shader* mercatorShader)
{
	int index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for texture " << textureName <<" with index " << index);
    TerrainShader* shader = new TerrainShader(mTerrain, index, textureName, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	return shader;
}

TerrainShader* TerrainGenerator::createShader(Ogre::MaterialPtr material, Mercator::Shader* mercatorShader)
{
	int index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for material " << material->getName() <<" with index " << index);
    TerrainShader* shader = new TerrainShader(mTerrain, index, material, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	return shader;
}



void TerrainGenerator::addArea(Mercator::Area* area)
{
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);
	std::stringstream ss;
	ss << area->bbox();
	S_LOG_VERBOSE("Adding area to terrain with dimensions: " << ss.str());
	mTerrain->addArea(area);
	if (!mAreaShaders.count(area->getLayer())) {
		S_LOG_VERBOSE("Shader does not exists, creating new.");
		TerrainShader* shader;
		///try to get the materialdefinition for this kind of area
		const Model::ModelDefinition::AreaDefinition* areaDef = Model::ModelDefinitionManager::getSingleton().getAreaDefinition(area->getLayer());
		if (areaDef) {
			///is there a material or should we use a texture only?
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
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()]);
	}
}

void TerrainGenerator::markShaderForUpdate(TerrainShader* shader)
{
	mShadersToUpdate.insert(shader);
}

bool TerrainGenerator::frameStarted(const Ogre::FrameEvent & evt)
{
	//PagingLandScapeCamera * cam = static_cast<PagingLandScapeCamera *>(EmberOgre::getSingleton().getMainCamera()->getCamera());
	
	
/*	std::stringstream ss;
	ss << "X: " << cam->mCurrentCameraPageX << " Z: " << cam->mCurrentCameraPageZ;
	GUIManager::getSingleton().setDebugText(ss.str());*/
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

int TerrainGenerator::getPageSize() 
{
	return mSceneManagerAdapter->getPageSize();
}

int TerrainGenerator::getSegmentSize() 
{
	return getPageSize() - 1;
}

void TerrainGenerator::buildHeightmap()
{
	mHeightMax = 0, mHeightMin = 0;
	
	///initialize all terrain here, since we need to do that in order to get the correct height for placement even though the terrain might not show up in the SceneManager yet
	
	///note that we want to use int's here, since a call to getSegment(float, float) is very much different from a call to getSegment(int, int)
	int i,j;
	const WFMath::AxisBox<2>& segmentBbox = mTerrainInfo.getWorldSizeInSegments();
	for (i = static_cast<int>(segmentBbox.lowCorner().x()); i < segmentBbox.highCorner().x(); ++i) {
		for (j = static_cast<int>(segmentBbox.lowCorner().y()); j < segmentBbox.highCorner().y(); ++j) {
			Mercator::Segment* segment = mTerrain->getSegment(i, j);
			if (segment) {
				S_LOG_VERBOSE("Preparing segment at position: x=" << i << " y=" << j );
				segment->populate();
				segment->populateNormals();
				segment->populateSurfaces();
				mHeightMax = std::max(mHeightMax, segment->getMax());
				mHeightMin = std::min(mHeightMin, segment->getMin());
			}
		}
	}
}

void TerrainGenerator::registerSceneManagerAdapter(ISceneManagerAdapter* adapter)
{
	mSceneManagerAdapter = adapter;
}


void TerrainGenerator::prepareAllSegments()
{
	
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);

	//buildHeightmap();
	
// 	int mercatorSegmentsPerPage =  getSegmentSize() / 64;
// 	int xNumberOfPages = (int)ceil((mXmax - mXmin) / (double)mercatorSegmentsPerPage);
// 	int yNumberOfPages = (int)ceil((mYmax - mYmin) / (double)mercatorSegmentsPerPage);
// 	int xStart = (int)ceil(mXmin / (double)(mercatorSegmentsPerPage));
// 	int yStart = (int)ceil(mYmin / (double)(mercatorSegmentsPerPage));
// 	for (i = 0; i < xNumberOfPages; ++i) {
// 		for (j = 0; j < yNumberOfPages; ++j) {
// 			TerrainPosition pos(xStart + i, yStart + j);
// 			std::stringstream ss;
// 			ss << pos;
// 			TerrainPage* page = new TerrainPage(pos, mShaderMap, this);
// 			mPages[ss.str()] = page;
// 			mTerrainPages[i][j] = page;
// 			for (std::list<TerrainShader*>::iterator I = mBaseShaders.begin(); I != mBaseShaders.end(); ++I) {
// 				page->addShader(*I);
// 			}
// 			page->generateTerrainMaterials();
// 			if (showFoliage && mGrassShader) {
// 				page->createFoliage(mGrassShader);
// 			}
// 			if (alsoPushOntoTerrain) {
// //				mTerrainPageSource->addPage(page);
// 			}
// 		}
// 	}
	
	
// // 	Ogre::PagingLandScapeSceneManager * sceneManager = getEmberSceneManager();
// 	Ogre::PagingLandScapeOptions* options = getEmberSceneManager()->getOptions();
	
	///update the terrain info
/*	mTerrainInfo.worldSizeX = getMax().x() - getMin().x();
	mTerrainInfo.totalNumberOfPagesX = static_cast<int>( ceil(mTerrainInfo.worldSizeX / (getPageSize() - 1)));*/
//	mTerrainInfo.totalNumberOfPagesX = static_cast<int>( mTerrainInfo.worldSizeX / (getPageSize() - 1));
// 	mTerrainInfo.pageOffsetX = mTerrainInfo.totalNumberOfPagesX / 2;
/*	mTerrainInfo.worldSizeY = getMax().y() - getMin().y();
	mTerrainInfo.totalNumberOfPagesY = static_cast<int>( ceil(mTerrainInfo.worldSizeY / (getPageSize() - 1)));*/
//	mTerrainInfo.totalNumberOfPagesY = static_cast<int>( mTerrainInfo.worldSizeY / (getPageSize() - 1));
// 	mTerrainInfo.pageOffsetY = mTerrainInfo.totalNumberOfPagesY / 2;
	
	//if the world is in effect non-existant, quit here
// 	if (mTerrainInfo.totalNumberOfPagesX == 0 && mTerrainInfo.totalNumberOfPagesY == 0) {
// 		return;
// 	}
	
	getAdapter()->setWorldPagesDimensions( mTerrainInfo.getTotalNumberOfPagesX(), mTerrainInfo.getTotalNumberOfPagesY());
	

		
	getAdapter()->loadScene();
	const WFMath::AxisBox<2>& worldSize = mTerrainInfo.getWorldSizeInIndices();
	Ogre::AxisAlignedBox worldBox(worldSize.lowCorner().x(), mHeightMin, worldSize.lowCorner().y(), worldSize.highCorner().x(), mHeightMax, worldSize.highCorner().y());
	std::stringstream ss;
	ss << worldBox;
	S_LOG_INFO("New size of the world: " << ss.str());
	
	getAdapter()->resize(worldBox ,16);

	
}

// void TerrainInfo::setNewXMax(int xMax)
// {
// }
// 
// void TerrainInfo::setNewXMin(int xMin)
// {
// }
// 
// void TerrainInfo::setNewYMax(int yMax)
// {
// }
// 
// void TerrainInfo::setNewYMin(int yMin)
// {
// }





bool TerrainGenerator::isValidTerrainAt(const TerrainPosition& position)
{
	int x = (int)position.x();
	int y = (int)position.y();
	Mercator::Segment* segment = mTerrain->getSegment(position.x(),position.y());
	return (segment &&	segment->isValid());
//	return (segment &&	segment->isValid() && getMaterialForSegment(position));
}


TerrainPage* TerrainGenerator::getTerrainPage(const TerrainPosition& worldPosition)
{

	
	int xRemainder = static_cast<int>(getMin().x()) % (getPageSize() - 1);
	int yRemainder = static_cast<int>(getMin().y()) % (getPageSize() - 1);
	
// 	int xRelativePosition = worldPosition.x() - getMin().x();
// 	int xUnadjustedIndex = 0;
// 	if (xRelativePosition != 0) {
// 		xUnadjustedIndex /=  (getPageSize() - 1);
// 	} 
// 	
// 	int xIndex = static_cast<int>((worldPosition.x() - getMin().x()) / (getPageSize() - 1));
// 	int yIndex = static_cast<int>((worldPosition.y() - getMin().y()) / (getPageSize() - 1));
	
	TerrainPosition pageIndexPos(static_cast<int>((worldPosition.x() + xRemainder)/ (getPageSize() - 1)), static_cast<int>((worldPosition.y() + yRemainder) / (getPageSize() - 1)));
	
	return mTerrainPages[static_cast<int>(pageIndexPos.x())][static_cast<int>(pageIndexPos.y())];
	/*	double worldSizeX = getMax().x() - getMin().x();
	int totalNumberOfPagesX = static_cast<int>( worldSizeX / (getPageSize() - 1));
	int pageOffsetX = totalNumberOfPagesX / 2;
	double worldSizeY = getMax().y() - getMin().y();
	int totalNumberOfPagesY = static_cast<int>( worldSizeY / (getPageSize() - 1));
	int pageOffsetY = totalNumberOfPagesY / 2;*/
// 
}

TerrainPage* TerrainGenerator::getTerrainPage(const Ogre::Vector2& ogreIndexPosition, bool createIfMissing)
{
	//_fpreset();
	S_LOG_INFO("Requesting page at ogre position x: " << ogreIndexPosition.x << " y: " << ogreIndexPosition.y);
	
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo.getPageOffsetX(), ogreIndexPosition.y - mTerrainInfo.getPageOffsetY());
	
	TerrainPosition pos(Ogre2Atlas(adjustedOgrePos));
	
	int x = static_cast<int>(pos.x());
	int y = static_cast<int>(pos.y());
	
	if (mTerrainPages[x][y] == 0) {
		if (createIfMissing) {
	
			//TerrainPosition adjustedPos(pos.x() - pageOffsetX, pos.y() - pageOffsetY);
	
			mTerrainPages[x][y] = createPage(pos);
			assert(mTerrainPages[x][y]);
		} else {
			return 0;
		}
	}
	return mTerrainPages[x][y];
}

TerrainPage* TerrainGenerator::createPage(const TerrainPosition& pos)
{
	
	bool showFoliage = isFoliageShown();
	
	
	//since we initialized all terrain in initTerrain we can count on all terrain segments being created and populated already
	
	
	TerrainPage* page = new TerrainPage(TerrainPosition(pos), mShaderMap, this);
	//mPages[ss.str()] = page;
	
	std::stringstream ss;
	ss << pos;
	mPages[ss.str()] = page;
	
	//add the base shaders, this should probably be refactored into a server side thing in the future
	for (std::list<TerrainShader*>::iterator I = mBaseShaders.begin(); I != mBaseShaders.end(); ++I) {
		page->addShader(*I);
	}
	
	page->generateTerrainMaterials();
	if (showFoliage && mGrassShader) {
		page->showFoliage();
	}

		
	return page;
	
	
}

void TerrainGenerator::updateFoliageVisibilty()
{
	bool showFoliage = isFoliageShown();
	
	PageStore::iterator I = mPages.begin();
	for(;I != mPages.end(); ++I) {
		if (showFoliage) {
			I->second->showFoliage();
		} else {
			I->second->destroyFoliage();
		}
	}
}

void TerrainGenerator::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "graphics") {
		if (key == "foliage") {
			updateFoliageVisibilty();
		}
	}
}

bool TerrainGenerator::isFoliageShown() const
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("graphics", "foliage") && GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1")) {
		return static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("graphics", "foliage"));
	}	
	return false;
}



float TerrainGenerator::getHeight(const TerrainPosition& point) const
{
	
	float height = 0;
	WFMath::Vector<3> vector;
	
	bool success = mTerrain->getHeightAndNormal(point.x(), point.y(), height, vector);
	if (success) {
		return height;
	} 

	return 0;
	
}





bool TerrainGenerator::initTerrain(Eris::Entity *we, Eris::View *world) 
{

	//_fpreset();
   if (!we->hasAttr("terrain")) {
	   S_LOG_FAILURE( "View entity has no terrain" );
	   S_LOG_FAILURE( "View entity id " << we->getId() );
        return false;
    }
    const Atlas::Message::Element &terrain = we->valueOfAttr("terrain");
    if (!terrain.isMap()) {
		S_LOG_FAILURE( "Terrain is not a map" );
    }
    const Atlas::Message::MapType & tmap = terrain.asMap();
    Atlas::Message::MapType::const_iterator I = tmap.find("points");
    int xmin = 0, xmax = 0, ymin = 0, ymax = 0;
    if (I == tmap.end()) {
		S_LOG_FAILURE( "No terrain points" );
    }

	TerrainDefPointStore pointStore;
	if (I->second.isList()) {
        // Legacy support for old list format.
        const Atlas::Message::ListType & plist = I->second.asList();
        Atlas::Message::ListType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->isList()) {
				S_LOG_INFO( "Non list in points" );
                continue;
            }
            const Atlas::Message::ListType & point = J->asList();
            if (point.size() != 3) {
				S_LOG_INFO( "point without 3 nums" );
                continue;
            }
            //int x = (int)point[0].asNum();
            //int y = (int)point[1].asNum();

			TerrainDefPoint defPoint((int)point[0].asNum(),(int)point[1].asNum(),(int)point[3].asNum());
			pointStore.push_back(defPoint);
			//mTerrain->setBasePoint(x,y,point[2].asNum());
        }
    } else if (I->second.isMap()) {

        const Atlas::Message::MapType & plist = I->second.asMap();
        Atlas::Message::MapType::const_iterator J = plist.begin();
        for(; J != plist.end(); ++J) {
            if (!J->second.isList()) {
				S_LOG_INFO( "Non list in points" );
                continue;
            }
            const Atlas::Message::ListType & point = J->second.asList();
            if (point.size() != 3) {
				S_LOG_INFO( "point without 3 nums" );
                continue;
            }
            int x = (int)point[0].asNum();
            int y = (int)point[1].asNum();
            float z = point[2].asNum();
			TerrainDefPoint defPoint(x,y,z);
			pointStore.push_back(defPoint);

			
/*	        Mercator::BasePoint bp;
            if (mTerrain->getBasePoint(x, y, bp) && (z == bp.height())) {
				S_LOG_INFO( "Point [" << x << "," << y << " unchanged");
                continue;
            }
            xmin = std::min(xmin, x);
            xmax = std::max(xmax, x);
            ymin = std::min(ymin, y);
            ymax = std::max(ymax, y);
            bp.height() = z;
            // FIXME Sort out roughness and falloff, and generally
            // verify this code is the same as that in Terrain layer
            mTerrain->setBasePoint(x, y, bp);*/
            
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
		S_LOG_FAILURE( "Terrain is the wrong type" );
        return false;
    }
	updateTerrain(pointStore);
/*    mXmin = xmin;
    mXmax = xmax;
    mYmin = ymin;
    mYmax = ymax;*/
//     mSegments = &mTerrain->getTerrain();
	


	
	
    return true;
}



bool TerrainGenerator::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
/*	int Xmin = mXmin;
	int Xmax = mXmax;
	int Ymin = mYmin;
	int Ymax = mYmax;*/
	for (TerrainDefPointStore::const_iterator I = terrainPoints.begin(); I != terrainPoints.end(); ++I) {
		///if we don't have any info from before, initialize the min and max values now
		if (!hasTerrainInfo) {
/*			mXmin = static_cast<int>(I->position.x());
			mXmax = static_cast<int>(I->position.x());
			mYmin = static_cast<int>(I->position.y());
			mYmax = static_cast<int>(I->position.y());*/
			hasTerrainInfo = true;
		}
		
        Mercator::BasePoint bp;
        if (mTerrain->getBasePoint(static_cast<int>(I->position.x()), static_cast<int>(I->position.y()), bp) && (I->height == bp.height())) {
			S_LOG_VERBOSE( "Point [" << I->position.x() << "," << I->position.y() << " unchanged");
            continue;
        } else {
			S_LOG_VERBOSE( "Setting base point [" << I->position.x() << "," << I->position.y() << " to height " << I->height);
        }
/*        mXmin = std::min<int>(mXmin, static_cast<int>(I->position.x()));
        mXmax = std::max<int>(mXmax, static_cast<int>(I->position.x()));
        mYmin = std::min<int>(mYmin, static_cast<int>(I->position.y()));
        mYmax = std::max<int>(mYmax, static_cast<int>(I->position.y()));*/
        bp.height() = I->height;
        // FIXME Sort out roughness and falloff, and generally
        // verify this code is the same as that in Terrain layer
        mTerrain->setBasePoint(static_cast<int>(I->position.x()), static_cast<int>(I->position.y()), bp);
        mTerrainInfo.setBasePoint(I->position, bp);

	}
    mSegments = &mTerrain->getTerrain();
	
	buildHeightmap();
	
	///for some yet undetermined reason we'll get blank segments seeminly at random in the terrain if we'll load it dynamically when requested by the scene manager, so avoid that we'll initialize everything now
	///HACK: this is of course just a temporary fix
	for (int i = 0; i < mTerrainInfo.getTotalNumberOfPagesX(); ++i) {
		for (int j = 0; j < mTerrainInfo.getTotalNumberOfPagesY(); ++j) {
			getTerrainPage(Ogre::Vector2(i, j), true);
		}
	}
	
	///check if the size of the world has been changed
// 	if (Xmin != mXmin || Xmax != mXmax || Ymin != mYmin || Ymax != mYmax) {
		EventWorldSizeChanged.emit();
// 	}
	
	
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
	return mTerrainInfo.getWorldSizeInIndices().highCorner();
// 	return TerrainPosition(mXmax * 64, mYmax * 64);
}

const TerrainPosition TerrainGenerator::getMin( ) const
{
	return mTerrainInfo.getWorldSizeInIndices().lowCorner();
// 	return TerrainPosition(mXmin * 64, mYmin * 64);
}

TerrainShader* TerrainGenerator::getFoliageShader() const
{
	return mGrassShader;
}



void TerrainGenerator::runCommand(const std::string &command, const std::string &args)
{
	if(command == "exportterrainmaterial") {
	//disable for now
		try {
			Ogre::MaterialPtr materialPtr = mTerrainPages.begin()->second.begin()->second->getMaterial();
			Ogre::MaterialSerializer serializer;
			
			serializer.exportMaterial(materialPtr, Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/terrain.material");
		} catch (Ogre::Exception& ex) 
		{
			S_LOG_FAILURE(ex.getFullDescription());
		}
	} 
}
}

const EmberOgre::TerrainInfo & EmberOgre::TerrainGenerator::getTerrainInfo( ) const
{
	return mTerrainInfo;
}


