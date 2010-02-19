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

#include "TerrainManager.h"

#include "ISceneManagerAdapter.h"
#include "ITerrainPageBridge.h"
#include "TerrainDefPoint.h"
#include "TerrainShader.h"
#include "TerrainPage.h"
#include "TerrainPageShadow.h"
#include "TerrainPageGeometry.h"
#include "TerrainArea.h"
#include "TerrainMod.h"
#include "TerrainInfo.h"
#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"
#include "TerrainPageCreationTask.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainAreaUpdateTask.h"
#include "TerrainAreaAddTask.h"
#include "TerrainAreaRemoveTask.h"
#include "TerrainModAddTask.h"
#include "TerrainModChangeTask.h"
#include "TerrainModRemoveTask.h"
#include "TerrainUpdateTask.h"
#include "TerrainMaterialCompilationTask.h"
#include "GeometryUpdateTask.h"
#include "ShadowUpdateTask.h"
#include "PlantQueryTask.h"
#include "HeightMap.h"
#include "HeightMapBufferProvider.h"
#include "PlantAreaQuery.h"
#include "SegmentManager.h"

#include "framework/LoggingInstance.h"
#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/SerialTask.h"
#include "framework/tasks/TemplateNamedTask.h"
#include "services/config/ConfigService.h"

#include "../Convert.h"
#include "../EmberOgre.h"
#include "../EmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../ShaderManager.h"
#include "../WorldEmberEntity.h"
#include "../ILightning.h"

#include "main/Application.h"

#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TerrainMod.h>

#include <Mercator/Area.h>
#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>
#include <Mercator/TerrainMod.h>

#include <sigc++/object_slot.h>
#include <sigc++/bind.h>

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>

#ifdef WIN32
#include <tchar.h>
#define snprintf _snprintf
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
#endif

#include <limits>

#include <boost/shared_ptr.hpp>


using namespace Ogre;
namespace EmberOgre
{
namespace Terrain
{

class BasePointRetrieveTask: public Ember::Tasks::TemplateNamedTask<BasePointRetrieveTask>
{

private:
	Mercator::Terrain& mTerrain;
	sigc::slot<void, Mercator::Terrain::Pointstore&> mAsyncCallback;
	Mercator::Terrain::Pointstore mPoints;

public:
	BasePointRetrieveTask(Mercator::Terrain& terrain, sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback) :
		mTerrain(terrain), mAsyncCallback(asyncCallback)
	{
	}
	void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
	{
		mPoints = mTerrain.getPoints();
	}

	void executeTaskInMainThread()
	{
		mAsyncCallback(mPoints);
	}

};

class FoliagePreparationTask: public Ember::Tasks::TemplateNamedTask<FoliagePreparationTask>
{
private:

	PageVector mPages;

public:
	FoliagePreparationTask(const PageVector& pages) :
		mPages(pages)
	{
	}
	virtual ~FoliagePreparationTask()
	{
	}

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
	{
		for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
			(*I)->prepareFoliage();
		}
	}

};

TerrainManager::TerrainManager(ISceneManagerAdapter* adapter) :
	UpdateShadows("update_shadows", this, "Updates shadows in the terrain."), mTerrainInfo(new TerrainInfo(adapter->getPageSize())), mTerrain(0), mHeightMax(std::numeric_limits<Ogre::Real>::min()), mHeightMin(std::numeric_limits<Ogre::Real>::max()), mHasTerrainInfo(false), mSceneManagerAdapter(0), mIsFoliageShown(false), mHeightMap(0), mHeightMapBufferProvider(0), mFoliageBatchSize(32), mTaskQueue(new Ember::Tasks::TaskQueue(1)), mLightning(0), mSegmentManager(0)
{
	mSceneManagerAdapter = adapter;

	loadTerrainOptions();
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED);

	mSegmentManager = new SegmentManager(*mTerrain, 64);
	//The mercator buffers are one size larger than the resolution
	mHeightMapBufferProvider = new HeightMapBufferProvider(mTerrain->getResolution() + 1);
	mHeightMap = new HeightMap(Mercator::Terrain::defaultLevel, mTerrain->getResolution());

	Ogre::Root::getSingleton().addFrameListener(this);

	registerConfigListener("graphics", "foliage", sigc::mem_fun(*this, &TerrainManager::config_Foliage));

	EmberOgre::getSingleton().getShaderManager()->EventLevelChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::shaderManager_LevelChanged), EmberOgre::getSingleton().getShaderManager()));

	Ember::Application::getSingleton().EventEndErisPoll.connect(sigc::mem_fun(*this, &TerrainManager::application_EndErisPoll));
}

TerrainManager::~TerrainManager()
{

	//Deleting the task queue will purge it, making sure that all jobs are processed first.
	delete mTaskQueue;

	for (PageVector::iterator J = mPages.begin(); J != mPages.end(); ++J) {
		delete (*J);
	}

	for (ShaderStore::iterator J = mShaderMap.begin(); J != mShaderMap.end(); ++J) {
		delete J->second;
	}

	delete mHeightMap;
	delete mHeightMapBufferProvider;

	delete mSegmentManager;

	delete mSceneManagerAdapter;
	delete mTerrain;
	//There should be no areas left
	assert(!mAreas.size());
	//There should be no mods left
	assert(!mTerrainMods.size());
}

void TerrainManager::getBasePoints(sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback)
{
	mTaskQueue->enqueueTask(new BasePointRetrieveTask(*mTerrain, asyncCallback));
}

void TerrainManager::loadTerrainOptions()
{
	chdir(Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory().c_str());

	getAdapter()->setResourceGroupName("General");

	getAdapter()->loadOptions(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "terrain.cfg");

	getAdapter()->setCamera(EmberOgre::getSingleton().getMainOgreCamera());

	getAdapter()->setUninitializedHeight(getDefaultHeight());

}

ISceneManagerAdapter* TerrainManager::getAdapter() const
{
	return mSceneManagerAdapter;
}

TerrainShader* TerrainManager::createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader)
{
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->getShaderName() <<" with index " << index);
	TerrainShader* shader = new TerrainShader(mTerrain, index, layerDef, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[shader->getShader()] = shader;
	EventShaderCreated.emit(shader);
	return shader;
}

void TerrainManager::addTerrainMod(TerrainMod* terrainMod)
{
	Eris::TerrainMod* erisTerrainMod(terrainMod->getErisMod());
	/// Listen for changes to the modifier
	terrainMod->EventModChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::TerrainMod_Changed), terrainMod));
	/// Listen for deletion of the modifier
	terrainMod->EventModDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::TerrainMod_Deleted), terrainMod));

	mTaskQueue->enqueueTask(new TerrainModAddTask(*mTerrain, *terrainMod->getMercatorMod(), terrainMod->getEntityId(), *this, mTerrainMods));
}

void TerrainManager::TerrainMod_Changed(TerrainMod* terrainMod)
{
	Mercator::TerrainMod* existingMod(0);
	TerrainModMap::iterator I = mTerrainMods.find(terrainMod->getEntityId());
	if (I != mTerrainMods.end()) {
		existingMod = I->second;
		mTerrainMods.erase(I);
		mTaskQueue->enqueueTask(new TerrainModChangeTask(*mTerrain, *terrainMod->getMercatorMod(), terrainMod->getEntityId(), *this, mTerrainMods, existingMod));
	} else {
		S_LOG_WARNING("Got a change signal for a terrain mod which isn't registered with the terrain manager. This shouldn't happen.");
	}
}

void TerrainManager::TerrainMod_Deleted(TerrainMod* terrainMod)
{
	TerrainModMap::iterator I = mTerrainMods.find(terrainMod->getEntityId());
	if (I != mTerrainMods.end()) {
		Mercator::TerrainMod* existingMod = I->second;
		mTerrainMods.erase(I);
		mTaskQueue->enqueueTask(new TerrainModRemoveTask(*mTerrain, existingMod, terrainMod->getEntityId(), *this, mTerrainMods));
	}
}

void TerrainManager::addArea(TerrainArea& terrainArea)
{
	terrainArea.EventAreaChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::TerrainArea_Changed), &terrainArea));
	terrainArea.EventAreaRemoved.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::TerrainArea_Removed), &terrainArea));
	terrainArea.EventAreaSwapped.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::TerrainArea_Swapped), &terrainArea));

	Mercator::Area* area = new Mercator::Area(*terrainArea.getArea());

	mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainManager::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders, mAreas, terrainArea.getEntityId()));
}

void TerrainManager::TerrainArea_Changed(TerrainArea* terrainArea)
{
	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		area->setShape(terrainArea->getArea()->shape());
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(area->getLayer())) {
			shader = mAreaShaders[area->getLayer()];
		}
		mTaskQueue->enqueueTask(new TerrainAreaUpdateTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainManager::markShaderForUpdate), shader));

	}
}

void TerrainManager::TerrainArea_Removed(TerrainArea* terrainArea)
{
	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(area->getLayer())) {
			shader = mAreaShaders[area->getLayer()];
		}
		mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainManager::markShaderForUpdate), shader, mAreas, terrainArea->getEntityId()));
	}
}

void TerrainManager::TerrainArea_Swapped(Mercator::Area& oldArea, TerrainArea* terrainArea)
{

	AreaMap::const_iterator I = mAreas.find(terrainArea->getEntityId());
	if (I != mAreas.end()) {
		Mercator::Area* area = I->second;
		const TerrainShader* shader = 0;
		if (mAreaShaders.count(oldArea.getLayer())) {
			shader = mAreaShaders[oldArea.getLayer()];
		}

		mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, area, sigc::mem_fun(*this, &TerrainManager::markShaderForUpdate), shader, mAreas, terrainArea->getEntityId()));

		Mercator::Area* newArea = new Mercator::Area(*terrainArea->getArea());
		mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, newArea, sigc::mem_fun(*this, &TerrainManager::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders, mAreas, terrainArea->getEntityId()));
	}
}
void TerrainManager::markShaderForUpdate(const TerrainShader* shader, Mercator::Area* terrainArea)
{
	if (shader) {
		ShaderUpdateRequest& updateRequest = mShadersToUpdate[shader];
		if (terrainArea) {
			updateRequest.Areas.push_back(terrainArea->bbox());
		} else {
			updateRequest.UpdateAll = true;
		}
	}
}

const ShaderStore& TerrainManager::getAllShaders() const
{
	return mShaderMap;
}

bool TerrainManager::frameEnded(const Ogre::FrameEvent & evt)
{

	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
		GeometryPtrVector geometry;
		for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
			geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
		}
		///use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (ShaderUpdateSet::reverse_iterator I = mShadersToUpdate.rbegin(); I != mShadersToUpdate.rend(); ++I) {
			mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->first, I->second.Areas, I->second.UpdateAll, EventLayerUpdated), 0);
		}
		mShadersToUpdate.clear();
	}


	return true;
}

void TerrainManager::addPage(TerrainPage* page)
{
	const TerrainPosition& pos = page->getWFPosition();
	//TODO: check that the page doesn't already exist
	mTerrainPages[pos.x()][pos.y()] = page;
	mPages.push_back(page);

	//Since the height data for the page probably wasn't correctly set up before the page was created, we should adjust the positions for the entities that are placed on the page.
	std::set<TerrainPage*> pagesToUpdate;
	pagesToUpdate.insert(page);
	updateEntityPositions(pagesToUpdate);
}

int TerrainManager::getPageIndexSize() const
{
	return mSceneManagerAdapter->getPageSize();
}

int TerrainManager::getPageMetersSize() const
{
	return getPageIndexSize() - 1;
}

void TerrainManager::getPlantsForArea(PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback)
{
	TerrainPosition wfPos(Convert::toWF(query.getCenter()));
	TerrainPage* terrainPage = getTerrainPageAtPosition(wfPos);
	if (terrainPage) {
		mTaskQueue->enqueueTask(new PlantQueryTask(new TerrainPageGeometry(*terrainPage, *mSegmentManager, getDefaultHeight()), query, asyncCallback));
	}
}

TerrainPage* TerrainManager::getTerrainPageAtPosition(const TerrainPosition& worldPosition) const
{

	int xRemainder = static_cast<int> (getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int> (getMin().y()) % (getPageMetersSize());

	int xIndex = static_cast<int> (floor((worldPosition.x() + xRemainder) / (getPageMetersSize())));
	int yIndex = static_cast<int> (ceil((worldPosition.y() + yRemainder) / (getPageMetersSize())));

	TerrainPagestore::const_iterator I = mTerrainPages.find(xIndex);
	if (I != mTerrainPages.end()) {
		TerrainPagecolumn::const_iterator J = I->second.find(yIndex);
		if (J != I->second.end()) {
			return J->second;
		}
	}
	return 0;
}

void TerrainManager::setUpTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition, ITerrainPageBridge& bridge)
{
	//_fpreset();

	///TerrainInfo deals with WF space, so we need to flip the x and y offsets here (as it's in Ogre space)
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo->getPageOffsetY(), ogreIndexPosition.y - mTerrainInfo->getPageOffsetX());

	TerrainPosition pos(Convert::toWF(adjustedOgrePos));

	int x = static_cast<int> (pos.x());
	int y = static_cast<int> (pos.y());

	S_LOG_INFO("Setting up TerrainPage at index [" << x << "," << y << "]");
	if (mTerrainPages[x][y] == 0) {
		WFMath::Vector<3> sunDirection = WFMath::Vector<3>(0, 0, -1);
		if (mLightning) {
			sunDirection = mLightning->getMainLightDirection();
		}
		mTaskQueue->enqueueTask(new TerrainPageCreationTask(*this, pos, &bridge, *mHeightMapBufferProvider, *mHeightMap, sunDirection));
	} else {
		S_LOG_WARNING("Trying to set up TerrainPage at index [" << x << "," << y << "], but it was already created");
	}
}

TerrainPage* TerrainManager::getTerrainPageAtIndex(const Ogre::Vector2& ogreIndexPosition)
{
	//_fpreset();

	///TerrainInfo deals with WF space, so we need to flip the x and y offsets here (as it's in Ogre space)
	Ogre::Vector2 adjustedOgrePos(ogreIndexPosition.x - mTerrainInfo->getPageOffsetY(), ogreIndexPosition.y - mTerrainInfo->getPageOffsetX());

	TerrainPosition pos(Convert::toWF(adjustedOgrePos));

	int x = static_cast<int> (pos.x());
	int y = static_cast<int> (pos.y());

	return mTerrainPages[x][y];
}

void TerrainManager::updateFoliageVisibility()
{
	bool showFoliage = isFoliageShown();

	PageVector::iterator I = mPages.begin();
	for (; I != mPages.end(); ++I) {
		if (showFoliage) {
			PageVector pages;
			pages.push_back(*I);
			mTaskQueue->enqueueTask(new FoliagePreparationTask(pages));
		} else {
			//TODO: implement destruction of the foliage data
			(*I)->hideFoliage();
		}
	}
}

void TerrainManager::config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1") && variable.is_bool()) {
		mIsFoliageShown = static_cast<bool> (variable);
	} else {
		mIsFoliageShown = false;
	}
	updateFoliageVisibility();
}

bool TerrainManager::isFoliageShown() const
{
	return mIsFoliageShown;
}

bool TerrainManager::getHeight(const TerrainPosition& point, float& height) const
{
	WFMath::Vector<3> vector;

	return mHeightMap->getHeightAndNormal(point.x(), point.y(), height, vector);
}

void TerrainManager::setLightning(ILightning* lightning)
{
	mLightning = lightning;
}

void TerrainManager::updateShadows()
{
	//	if (mLightning) {
	//		WFMath::Vector<3> sunDirection = mLightning->getMainLightDirection();
	//		mTaskQueue->enqueueTask(new Ember::Tasks::SerialTask(new ShadowUpdateTask(mPages, sunDirection), new TerrainMaterialCompilationTask(mPages)));
	//	} else {
	//		S_LOG_WARNING("Tried to update shadows without having any ILightning instance set.");
	//	}
}

bool TerrainManager::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
	mTaskQueue->enqueueTask(new TerrainUpdateTask(*mTerrain, terrainPoints, *this, *mTerrainInfo, mHasTerrainInfo, *mSegmentManager));
	return true;
}

void TerrainManager::reloadTerrain(const std::vector<TerrainPosition>& positions)
{
	GeometryPtrVector geometryToUpdate;
	std::set<TerrainPage*> pagesToUpdate;
	for (std::vector<TerrainPosition>::const_iterator I(positions.begin()); I != positions.end(); ++I) {
		const TerrainPosition& worldPosition(*I);
		TerrainPage* page;
		///make sure we sample pages from all four points around the base point, in case the base point is on a page border
		for (int i = -65; i < 66; i += 64) {
			for (int j = -65; j < 66; j += 64) {
				TerrainPosition position(worldPosition.x() + i, worldPosition.y() + j);
				page = getTerrainPageAtPosition(position);
				if (page) {
					geometryToUpdate.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(*page, *mSegmentManager, getDefaultHeight())));
					pagesToUpdate.insert(page);
				}
			}
		}
	}

	EventBeforeTerrainUpdate(positions, pagesToUpdate);
	mTaskQueue->enqueueTask(new GeometryUpdateTask(geometryToUpdate, positions, *this, mShaderMap, *mHeightMapBufferProvider, *mHeightMap));

}

void TerrainManager::updateEntityPositions(const std::set<TerrainPage*>& pagesToUpdate)
{
	EmberEntity* entity = EmberOgre::getSingleton().getEntityFactory()->getWorld();
	if (entity) {
		updateEntityPosition(entity, pagesToUpdate);
	}
}

void TerrainManager::updateEntityPosition(EmberEntity* entity, const std::set<TerrainPage*>& pagesToUpdate)
{
	entity->adjustPosition();
	for (unsigned int i = 0; i < entity->numContained(); ++i) {
		EmberEntity* containedEntity = static_cast<EmberEntity*> (entity->getContained(i));
		updateEntityPosition(containedEntity, pagesToUpdate);
	}
}

const TerrainPosition TerrainManager::getMax() const
{
	return mTerrainInfo->getWorldSizeInIndices().highCorner();
}

const TerrainPosition TerrainManager::getMin() const
{
	return mTerrainInfo->getWorldSizeInIndices().lowCorner();
}

void TerrainManager::runCommand(const std::string& command, const std::string& args)
{
	if (UpdateShadows == command) {
		updateShadows();
	}
}
const TerrainInfo& TerrainManager::getTerrainInfo() const
{
	return *mTerrainInfo;
}

float TerrainManager::getDefaultHeight() const
{
	return -15;
}

SegmentManager& TerrainManager::getSegmentManager()
{
	return *mSegmentManager;
}

//void TerrainManager::getShadowColourAt(const Ogre::Vector2& position, Ogre::uint32& colour) const
//{
//	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
//	TerrainPosition wfPos(Convert::toWF(position));
//	TerrainPage* terrainPage = getTerrainPageAtPosition(wfPos);
//	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(terrainPage->getExtent());
//	terrainPage->getPageShadow().getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);
//}
//
//void TerrainManager::getShadowColourAt(const Ogre::Vector2& position, Ogre::ColourValue& colour) const
//{
//	//TODO: add caching of the last fetched terrain page and first check if the position isn't at that page, since we'll in most cass will call this method with positions that are close to eachother
//	TerrainPosition wfPos(Convert::toWF(position));
//	TerrainPage* terrainPage = getTerrainPageAtPosition(wfPos);
//	Ogre::TRect<float> ogrePageExtent = Convert::toOgre(terrainPage->getExtent());
//	terrainPage->getPageShadow().getShadowColourAt(Ogre::Vector2(position.x - ogrePageExtent.left, position.y - ogrePageExtent.top), colour);
//}

void TerrainManager::shaderManager_LevelChanged(ShaderManager* shaderManager)
{
	GeometryPtrVector geometry;
	for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
	}
	//Update all shaders on all pages
	for (ShaderStore::const_iterator I = mShaderMap.begin(); I != mShaderMap.end(); ++I) {
		mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->second, AreaStore(), true, EventLayerUpdated), 0);
	}
}

void TerrainManager::application_EndErisPoll(float)
{
	//10 milliseconds seems like a suitable time to limit each frame to. This could be extended to something more sophisticated though to better allow for a smooth frame rate.
	mTaskQueue->pollProcessedTasks(10L);
}

}
}
