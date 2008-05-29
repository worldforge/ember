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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainGenerator.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "../EmberOgre.h"
#include "../EmberEntity.h"
#include "../WorldEmberEntity.h"
#include "../EmberEntityFactory.h"

#include <Eris/Entity.h>
#include <Eris/View.h>

#include <OgreCodec.h>
#include <OgreImage.h>
#include <OgreImageCodec.h>
#include <OgreTextureManager.h>
#include <OgreStringConverter.h>
#include <OgreRenderSystemCapabilities.h>
#include "TerrainShader.h"
#include "../environment/Foliage.h"
#include "../environment/Forest.h"
#include "ISceneManagerAdapter.h"


#include "TerrainPage.h"
#include "TerrainArea.h"
#include <Mercator/Area.h>
#include <Mercator/Segment.h>
#include <Mercator/FillShader.h>
#include <Mercator/ThresholdShader.h>
#include <Mercator/DepthShader.h>
#include <Mercator/GrassShader.h>
#include <Mercator/Surface.h>
#include <Mercator/Matrix.h>

#include <wfmath/intersect.h>
#include <wfmath/axisbox.h>
#include <wfmath/ball.h>
#include <Mercator/TerrainMod.h>

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageSurfaceLayer.h"
// #include "../model/ModelDefinition.h"
// #include "../model/ModelDefinitionManager.h"

#include "../AvatarCamera.h"

#include "../environment/Environment.h"

#include <sigc++/object_slot.h>

#ifdef WIN32
	#include <tchar.h>
	#define snprintf _snprintf
    #include <io.h> // for _access, Win32 version of stat()
    #include <direct.h> // for _mkdir
#endif
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"


using namespace Ogre;
namespace EmberOgre {
namespace Terrain {


TerrainGenerator::TerrainGenerator(ISceneManagerAdapter* adapter)
: 
UpdateShadows("update_shadows", this, "Updates shadows in the terrain."),
mHasTerrainInfo(false),
mFoliageBatchSize(32)
{

	registerSceneManagerAdapter( adapter);
	//new Foliage(EmberOgre::getSingleton().getSceneManager());
	
	
    loadTerrainOptions();
	mTerrainInfo.setPageIndicesSize(adapter->getPageSize());
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED); //, mOptions.pageSize - 1);
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

  
   
	
//	mTerrainPageSource = new EmberTerrainPageSource(this);
//    EmberOgre::getSingleton().getSceneManager()->registerPageSource(EmberTerrainPageSource::Name, mTerrainPageSource);



  //  EmberOgre::getSingleton().getSceneManager()->setWorldGeometry(mOptions);
	Ogre::Root::getSingleton().addFrameListener(this);
	
	configSrv->EventChangedConfigItem.connect(sigc::mem_fun(*this, &TerrainGenerator::ConfigService_EventChangedConfigItem));
	
}

TerrainGenerator::~TerrainGenerator()
{

	for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
		delete J->second;
	}
	
	for (ShaderStore::iterator J = mShaderMap.begin(); J != mShaderMap.end(); ++J) {
		delete J->second;
	}
	
			
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

	getAdapter()->loadOptions(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "terrain.cfg");
	
	getAdapter()->setCamera( EmberOgre::getSingleton().getMainCamera()->getCamera());
	
	
	//Ogre::PagingLandScapeOptions::getSingleton().data2DFormat = "EmberHeightField";

}

ISceneManagerAdapter* TerrainGenerator::getAdapter() const
{
	return mSceneManagerAdapter;
}

TerrainShader* TerrainGenerator::createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader)
{
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->getShaderName() <<" with index " << index);
    TerrainShader* shader = new TerrainShader(mTerrain, index, layerDef, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	EventShaderCreated.emit(shader);
	return shader;
}

// TerrainShader* TerrainGenerator::createShader(Ogre::MaterialPtr material, Mercator::Shader* mercatorShader)
// {
// 	size_t index = mShaderMap.size();
// 	S_LOG_VERBOSE("Creating new shader for material " << material->getName() <<" with index " << index);
//     TerrainShader* shader = new TerrainShader(mTerrain, index, material, mercatorShader);
// 
// 	mBaseShaders.push_back(shader);
// 	mShaderMap[shader->getShader()] = shader;
// 	return shader;
// }



void TerrainGenerator::addArea(TerrainArea* terrainArea)
{

	terrainArea->EventAreaChanged.connect(sigc::mem_fun(*this, &TerrainGenerator::TerrainArea_Changed));
	Mercator::Area* area = terrainArea->getArea();
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
		const TerrainLayerDefinition* layerDef = TerrainLayerDefinitionManager::getSingleton().getDefinitionForArea(area->getLayer());
		if (layerDef) {
			shader = createShader(layerDef, new Mercator::AreaShader(area->getLayer()));
			mAreaShaders[area->getLayer()] = shader;
		}
	}
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], terrainArea);
	}
}

void TerrainGenerator::TerrainArea_Changed(TerrainArea* terrainArea)
{
	Mercator::Area* area = terrainArea->getArea();
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		markShaderForUpdate(mAreaShaders[area->getLayer()], terrainArea);
	}
}

void TerrainGenerator::markShaderForUpdate(TerrainShader* shader, TerrainArea* terrainArea)
{
	mShadersToUpdate.insert(shader);
	if (terrainArea) {
		mChangedTerrainAreas[shader].push_back(terrainArea);
	}
}
bool TerrainGenerator::frameStarted(const Ogre::FrameEvent & evt)
{
	return true;
}

bool TerrainGenerator::frameEnded(const Ogre::FrameEvent & evt)
{
	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
// 		for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
// 			J->second->populateSurfaces();
// 		}	
		
		///use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (ShaderSet::reverse_iterator I = mShadersToUpdate.rbegin(); I != mShadersToUpdate.rend(); ++I) {
			for (PageStore::iterator J = mPages.begin(); J != mPages.end(); ++J) {
				///repopulate the layer
				J->second->updateShaderTexture(*I, true);
			}
			TerrainAreaMap::iterator areaI = mChangedTerrainAreas.find(*I);
			std::vector<TerrainArea*>* areas(0);
			if (areaI != mChangedTerrainAreas.end()) {
				///only send areas if the update actually affects only areas
				areas = &(areaI->second);
			}
			EventLayerUpdated.emit(*I, areas);
		}	
	}
	
	mShadersToUpdate.clear();
	mChangedTerrainAreas.clear();
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

int TerrainGenerator::getPageIndexSize() 
{
	return mSceneManagerAdapter->getPageSize();
}

int TerrainGenerator::getPageMetersSize() 
{
	return getPageIndexSize() - 1;
}

void TerrainGenerator::buildHeightmap()
{
	mHeightMax = 0, mHeightMin = 0;							//tb  used for the call to addMod() below
	const WFMath::Ball<3> myMathBall;						//tb
	Mercator::CraterTerrainMod *myCrater = new Mercator::CraterTerrainMod(myMathBall); //tb

	///initialize all terrain here, since we need to do that in order to get the correct height for placement even though the terrain might not show up in the SceneManager yet
	
	///note that we want to use int's here, since a call to getSegment(float, float) is very much different from a call to getSegment(int, int)
	int i,j;
	const WFMath::AxisBox<2>& segmentBbox = mTerrainInfo.getWorldSizeInSegments();
	for (i = static_cast<int>(segmentBbox.lowCorner().x()); i < segmentBbox.highCorner().x(); ++i) {
		for (j = static_cast<int>(segmentBbox.lowCorner().y()); j < segmentBbox.highCorner().y(); ++j) {
			Mercator::Segment* segment = mTerrain->getSegment(i, j);
			if (segment) {
				//S_LOG_VERBOSE("Preparing segment at position: x=" << i << " y=" << j );
				segment->populate();
				segment->populateNormals();
				segment->populateSurfaces();
				mHeightMax = std::max(mHeightMax, segment->getMax());
				mHeightMin = std::min(mHeightMin, segment->getMin());
				
				
    				
			}
		}
	}
	mTerrain->getSegment(2,2)->addMod(myCrater); //tb
}

// void TerrainGenerator::createShaders(WorldEmberEntity* worldEntity)
// {
// 	if (worldEntity) {
// 		if (worldEntity->hasAttr("surfaces")) {
// 			
// 		}
// 	}
// }


void TerrainGenerator::registerSceneManagerAdapter(ISceneManagerAdapter* adapter)
{
	mSceneManagerAdapter = adapter;
}


void TerrainGenerator::prepareAllSegments()
{
	
 //   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);

	
	getAdapter()->setWorldPagesDimensions(mTerrainInfo.getTotalNumberOfPagesX(), mTerrainInfo.getTotalNumberOfPagesY(), mTerrainInfo.getPageOffsetX(), mTerrainInfo.getPageOffsetY());
	

		
	getAdapter()->loadScene();
	const WFMath::AxisBox<2>& worldSize = mTerrainInfo.getWorldSizeInIndices();
	Ogre::AxisAlignedBox worldBox(worldSize.lowCorner().x(), mHeightMin, -worldSize.highCorner().y(), worldSize.highCorner().x(), mHeightMax, -worldSize.lowCorner().y());
	
	std::stringstream ss;
	ss << worldBox;
	S_LOG_INFO("New size of the world: " << ss.str());
	
	getAdapter()->resize(worldBox ,16);
	
	S_LOG_INFO("Pages: X: " << mTerrainInfo.getTotalNumberOfPagesX() << " Y: " << mTerrainInfo.getTotalNumberOfPagesY() << " Total: " <<  mTerrainInfo.getTotalNumberOfPagesX() *  mTerrainInfo.getTotalNumberOfPagesY());
	S_LOG_INFO("Page offset: X" << mTerrainInfo.getPageOffsetX() << " Y: " << mTerrainInfo.getPageOffsetY());
	
	///load the first page, thus bypassing the normal paging system. This is to prevent the user from floating in the thin air while the paging system waits for a suitable time to load the first page.
	getAdapter()->loadFirstPage();

	EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getForest()->initialize();	
	
}


bool TerrainGenerator::isValidTerrainAt(const TerrainPosition& position)
{
// /*	int x = (int)position.x();
// 	int y = (int)position.y();*/
	Mercator::Segment* segment = mTerrain->getSegment(position.x(),position.y());
	return (segment &&	segment->isValid());
//	return (segment &&	segment->isValid() && getMaterialForSegment(position));
}


TerrainPage* TerrainGenerator::getTerrainPage(const TerrainPosition& worldPosition)
{

	
	int xRemainder = static_cast<int>(getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int>(getMin().y()) % (getPageMetersSize());
	
 	int xIndex = static_cast<int>(floor((worldPosition.x() + xRemainder)/ (getPageMetersSize())));
 	int yIndex = static_cast<int>(ceil((worldPosition.y() + yRemainder) / (getPageMetersSize())));
 	
	
	return mTerrainPages[xIndex][yIndex];
}

TerrainPage* TerrainGenerator::getTerrainPage(const Ogre::Vector2& ogreIndexPosition, bool createIfMissing)
{
	//_fpreset();
	//S_LOG_INFO("Requesting page at ogre position x: " << ogreIndexPosition.x << " y: " << ogreIndexPosition.y);
	
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo.getPageOffsetY(), ogreIndexPosition.y - mTerrainInfo.getPageOffsetX());
	
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
	
	
	///since we initialized all terrain in initTerrain we can count on all terrain segments being created and populated already
	
	
	TerrainPage* page = new TerrainPage(TerrainPosition(pos), mShaderMap, this);
	//mPages[ss.str()] = page;
	
	std::stringstream ss;
	ss << pos.x() << "x" << pos.y();
	mPages[ss.str()] = page;
	
	//add the base shaders, this should probably be refactored into a server side thing in the future
	for (std::list<TerrainShader*>::iterator I = mBaseShaders.begin(); I != mBaseShaders.end(); ++I) {
		page->addShader(*I);
	}
	
	page->createShadow(EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection());
	page->generateTerrainMaterials();
	if (showFoliage) {
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
			I->second->hideFoliage();
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

///make undefined ground be one meter below the water
	return -1;
	
}

void TerrainGenerator::updateShadows()
{
	Ogre::Vector3 sunDirection = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection();

	PageStore::iterator I = mPages.begin();
	for(;I != mPages.end(); ++I) {
		I->second->updateShadow(sunDirection);
	}
}


bool TerrainGenerator::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
	std::vector<TerrainPosition> updatedPositions;
	bool wasUpdate = false;
	for (TerrainDefPointStore::const_iterator I = terrainPoints.begin(); I != terrainPoints.end(); ++I) {
		
        Mercator::BasePoint bp;
        if (mTerrain->getBasePoint(static_cast<int>(I->position.x()), static_cast<int>(I->position.y()), bp) && (I->height == bp.height())) {
			S_LOG_VERBOSE( "Point [" << I->position.x() << "," << I->position.y() << " unchanged");
            continue;
        } else {
        	wasUpdate = true;
			S_LOG_VERBOSE( "Setting base point [" << I->position.x() << "," << I->position.y() << " to height " << I->height);
        }
        bp.height() = I->height;
        /// FIXME Sort out roughness and falloff, and generally
        /// verify this code is the same as that in Terrain layer
        mTerrain->setBasePoint(static_cast<int>(I->position.x()), static_cast<int>(I->position.y()), bp);
        mTerrainInfo.setBasePoint(I->position, bp);
		updatedPositions.push_back(I->position);
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
	
	if (!mHasTerrainInfo) {
		mHasTerrainInfo = true;
	} else {
		if (wasUpdate) {
			///if it's an update, we need to reload all pages and adjust all entity positions
			reloadTerrain(updatedPositions);
		}
	}
	
	
	return true;
}

void TerrainGenerator::reloadTerrain(std::vector<TerrainPosition>& positions)
{
	std::set<TerrainPage*> pagesToUpdate;
	for (std::vector<TerrainPosition>::iterator I(positions.begin()); I != positions.end(); ++I) {
		TerrainPosition worldPosition(I->x() * 64, I->y() * 64);
		TerrainPage* page;
		///make sure we sample pages from all four points around the base point, in case the base point is on a page border
		for (int i = -65; i < 66; i += 64) {
			for (int j = -65; j < 66; j += 64) {
				TerrainPosition position(worldPosition.x() + i, worldPosition.y() + j);
				page = getTerrainPage(position);
				if (page) {
					pagesToUpdate.insert(page);
				}
			}
		}
	}

	updateHeightMapAndShaders(pagesToUpdate);
	updateEntityPositions(pagesToUpdate);
	
}
void TerrainGenerator::updateHeightMapAndShaders(const std::set<TerrainPage*>& pagesToUpdate)
{
	const Ogre::Vector3& sunDirection = EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection();

	///reload all shader textures of the affected pages
	for (std::set<TerrainPage*>::const_iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
		(*I)->updateAllShaderTextures();
		(*I)->updateShadow(sunDirection);
		(*I)->update();
	}
}

void TerrainGenerator::updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate)
{
	EmberEntity* entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
	if (entity) {
		updateEntityPosition(entity, pagesToUpdate);
	}
}

void TerrainGenerator::updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate)
{
	entity->adjustPosition();
	for (unsigned int i = 0; i < entity->numContained(); ++i) {
		EmberEntity* containedEntity = static_cast<EmberEntity*>(entity->getContained(i));
		updateEntityPosition(containedEntity, pagesToUpdate);
	}
}


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

void TerrainGenerator::runCommand(const std::string &command, const std::string &args)
{
	if (UpdateShadows == command) {
		updateShadows();
	}
}
const TerrainInfo & TerrainGenerator::getTerrainInfo( ) const
{
	return mTerrainInfo;
}

void TerrainGenerator::getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour)
{
	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
	TerrainPosition wfPos(Ogre2Atlas(position));
	TerrainPage* terrainPage = getTerrainPage(wfPos);
	TerrainPageShadow& terrainShadow = terrainPage->getPageShadow();
	
	Ogre::TRect<float> ogrePageExtent = Atlas2Ogre(terrainPage->getExtent());
	terrainShadow.getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);

}

void TerrainGenerator::getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour)
{
	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
	TerrainPosition wfPos(Ogre2Atlas(position));
	TerrainPage* terrainPage = getTerrainPage(wfPos);
	TerrainPageShadow& terrainShadow = terrainPage->getPageShadow();
	Ogre::TRect<float> ogrePageExtent = Atlas2Ogre(terrainPage->getExtent());
	terrainShadow.getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);
}

}
}
