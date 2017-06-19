/*
 Copyright (C) 2010 erik

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

#include "TerrainHandler.h"

#include "ITerrainPageBridge.h"
#include "TerrainDefPoint.h"
#include "TerrainShader.h"
#include "TerrainPage.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurface.h"
#include "TerrainPageGeometry.h"
#include "TerrainArea.h"
#include "TerrainMod.h"
#include "TerrainInfo.h"
#include "TerrainPageCreationTask.h"
#include "TerrainPageDeletionTask.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainAreaUpdateTask.h"
#include "TerrainAreaAddTask.h"
#include "TerrainAreaRemoveTask.h"
#include <components/ogre/terrain/TerrainModUpdateTask.h>
#include "TerrainUpdateTask.h"

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"

#include "GeometryUpdateTask.h"
#include "ShadowUpdateTask.h"
#include "PlantQueryTask.h"
#include "HeightMap.h"
#include "HeightMapBufferProvider.h"
#include "PlantAreaQuery.h"
#include "SegmentManager.h"

#include "../Convert.h"
#include "../ILightning.h"

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/SerialTask.h"
#include "framework/tasks/TaskExecutionContext.h"
#include "framework/MainLoopController.h"

#include <Eris/EventService.h>

#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>

#include <sigc++/bind.h>

namespace Ember
{

namespace OgreView
{

namespace Terrain
{

class BasePointRetrieveTask: public Tasks::TemplateNamedTask<BasePointRetrieveTask>
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
	virtual ~BasePointRetrieveTask()
	{
	}

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
		mPoints = mTerrain.getPoints();
	}

	bool executeTaskInMainThread()
	{
		mAsyncCallback(mPoints);
		return true;
	}

};

class TerrainPageReloadTask: public Tasks::TemplateNamedTask<TerrainPageReloadTask>
{
private:
	TerrainHandler& mHandler;
	ITerrainPageBridgePtr mBridge;
	TerrainPageGeometryPtr mGeometry;
	const ShaderStore mShaders;
	const WFMath::AxisBox<2> mArea;
	const WFMath::Vector<3> mMainLightDirection;

public:
	TerrainPageReloadTask(TerrainHandler& handler, ITerrainPageBridgePtr bridge, TerrainPageGeometryPtr geometry, const ShaderStore& shaders, const WFMath::AxisBox<2>& area, const WFMath::Vector<3>& mainLightDirection) :
			mHandler(handler), mBridge(bridge), mGeometry(geometry), mShaders(shaders), mArea(area), mMainLightDirection(mainLightDirection)
	{
	}

	virtual ~TerrainPageReloadTask()
	{
	}

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
	{
		mGeometry->repopulate();
		std::vector<const TerrainShader*> shaders;
		for (ShaderStore::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
			shaders.push_back(I->second);
		}
		AreaStore areas;
		areas.push_back(mArea);
		GeometryPtrVector geometries;
		geometries.push_back(mGeometry);
		context.executeTask(new TerrainShaderUpdateTask(geometries, shaders, areas, mHandler.EventLayerUpdated, mHandler.EventTerrainMaterialRecompiled, mMainLightDirection));
		if (mBridge.get()) {
			mBridge->updateTerrain(*mGeometry);
		}
	}

	bool executeTaskInMainThread()
	{
		if (mBridge.get()) {
			mBridge->terrainPageReady();
		}
		return true;
	}

};

TerrainHandler::TerrainHandler(unsigned int pageIndexSize, ICompilerTechniqueProvider& compilerTechniqueProvider, Eris::EventService& eventService) :
		mPageIndexSize(pageIndexSize), mCompilerTechniqueProvider(compilerTechniqueProvider), mTerrainInfo(new TerrainInfo(pageIndexSize)), mEventService(eventService), mTerrain(0), mHeightMax(std::numeric_limits<Ogre::Real>::min()), mHeightMin(std::numeric_limits<Ogre::Real>::max()), mHasTerrainInfo(false), mTaskQueue(new Tasks::TaskQueue(1, eventService)), mLightning(0), mHeightMap(0), mHeightMapBufferProvider(0), mSegmentManager(0), mTerrainEntity(nullptr)
{
	mTerrain = new Mercator::Terrain(Mercator::Terrain::SHADED);

	mSegmentManager = new SegmentManager(*mTerrain, 64);
	mSegmentManager->setEndlessWorldEnabled(true);
	mSegmentManager->setDefaultHeight(getDefaultHeight());
	//TODO: get these values from the server if possible
	mSegmentManager->setDefaultHeightVariation(10);
	//The mercator buffers are one size larger than the resolution
	mHeightMapBufferProvider = new HeightMapBufferProvider(mTerrain->getResolution() + 1);
	mHeightMap = new HeightMap(Mercator::Terrain::defaultLevel, mTerrain->getResolution());

	MainLoopController::getSingleton().EventFrameProcessed.connect(sigc::mem_fun(*this, &TerrainHandler::frameProcessed));

	EventTerrainEnabled.connect(sigc::mem_fun(*this, &TerrainHandler::terrainEnabled));
	EventTerrainDisabled.connect(sigc::mem_fun(*this, &TerrainHandler::terrainDisabled));

}

TerrainHandler::~TerrainHandler()
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

	delete mTerrain;
}

void TerrainHandler::shutdown()
{
	mTaskQueue->deactivate();
}

void TerrainHandler::setPageSize(unsigned int pageSize)
{
	// Wait for all current tasks to finish
//	mTaskQueue->pollProcessedTasks(TimeFrame(boost::posix_time::seconds(60)));
	// Delete all page-related data
	mPageBridges.clear();
	for (auto& page : mPages) {
		delete page;
	}
	mPages.clear();
	mTerrainPages.clear();

	mTerrainInfo.reset(new TerrainInfo(pageSize));
	mPageIndexSize = pageSize;
}

void TerrainHandler::getBasePoints(sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback)
{
	mTaskQueue->enqueueTask(new BasePointRetrieveTask(*mTerrain, asyncCallback));
}

TerrainShader* TerrainHandler::createShader(const TerrainLayerDefinition* layerDef, Mercator::Shader* mercatorShader)
{
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->getShaderName() <<" with index " << index);
	TerrainShader* shader = new TerrainShader(*mTerrain, index, *layerDef, mercatorShader);

	mBaseShaders.push_back(shader);
	mShaderMap[&shader->getShader()] = shader;

	EventShaderCreated.emit(*shader);
	return shader;
}

void TerrainHandler::markShaderForUpdate(const TerrainShader* shader, const WFMath::AxisBox<2>& affectedArea)
{
	if (shader) {
		ShaderUpdateRequest& updateRequest = mShadersToUpdate[shader];
		updateRequest.Areas.push_back(affectedArea);
		mEventService.runOnMainThread([&](){
			updateShaders();
		});
	}
}

const ShaderStore& TerrainHandler::getAllShaders() const
{
	return mShaderMap;
}

void TerrainHandler::addPage(TerrainPage* page)
{
	const TerrainPosition& pos = page->getWFPosition();

	auto oldPage = mTerrainPages[pos.x()][pos.y()];
	if (oldPage) {
		destroyPage(oldPage);
	}

	mTerrainPages[pos.x()][pos.y()] = page;
	mPages.push_back(page);

	//Since the height data for the page probably wasn't correctly set up before the page was created, we should adjust the positions for the entities that are placed on the page.
	std::set<TerrainPage*> pagesToUpdate;
	pagesToUpdate.insert(page);
}

void TerrainHandler::destroyPage(TerrainPage* page)
{
	const TerrainPosition& pos = page->getWFPosition();
	auto pageIter = std::find(mPages.begin(), mPages.end(), page);
	if (pageIter != mPages.end()) {
		mPages.erase(pageIter);
	}
	mTerrainPages[pos.x()][pos.y()] = nullptr;
	//We should delete the page first when all existing tasks are completed. This is because some of them might refer to the page.
	if (!mTaskQueue->isActive()) {
		delete page;
	} else {
		if (!mTaskQueue->enqueueTask(new TerrainPageDeletionTask(page))) {
			//If the task queue is inactive there's no risk of deleting it.
			delete page;
		}
	}
}

int TerrainHandler::getPageIndexSize() const
{
	return mPageIndexSize;
}

int TerrainHandler::getPageMetersSize() const
{
	return getPageIndexSize() - 1;
}

void TerrainHandler::getPlantsForArea(Foliage::PlantPopulator& populator, PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback)
{

	TerrainPosition wfPos(Convert::toWF(query.getCenter()));

	TerrainIndex index(std::floor(query.getCenter().x / (mPageIndexSize - 1)), -std::floor(query.getCenter().y / (mPageIndexSize - 1)));

	//If there's either no terrain page created, or it's not shown, we shouldn't create any foliage at this moment.
	//Later on when the terrain page actually is shown, the TerrainManager::EventTerrainShown signal will be emitted
	//and the foliage reloaded.
	//The main reasons for this are twofold:
	//1) Calculating foliage occupies the task queue at the expense of creating terrain page data. We much rather would want the pages to be created before foliage is handled.
	//2) If foliage is shown before the page is shown it just looks strange, with foliage levitating in the empty air.
	auto bridgeI = mPageBridges.find(index);
	if (bridgeI == mPageBridges.end()) {
		return;
	}
	if (!bridgeI->second->isPageShown()) {
		return;
	}

	int xIndex = static_cast<int>(std::floor(wfPos.x() / mTerrain->getResolution()));
	int yIndex = static_cast<int>(std::floor(wfPos.y() / mTerrain->getResolution()));
	SegmentRefPtr segmentRef = mSegmentManager->getSegmentReference(xIndex, yIndex);
	if (segmentRef.get()) {
		Ogre::ColourValue defaultShadowColour;
		if (mLightning) {
			defaultShadowColour = mLightning->getAmbientLightColour();
		}
		mTaskQueue->enqueueTask(new PlantQueryTask(segmentRef, populator, query, defaultShadowColour, asyncCallback));

	}
}

ICompilerTechniqueProvider& TerrainHandler::getCompilerTechniqueProvider()
{
	return mCompilerTechniqueProvider;
}

void TerrainHandler::removeBridge(const TerrainIndex& index)
{
	PageBridgeStore::iterator I = mPageBridges.find(index);
	if (I != mPageBridges.end()) {
		auto page = I->second->getTerrainPage();
		if (page) {
			destroyPage(page);
		}
		mPageBridges.erase(I);
	}
}

void TerrainHandler::updateShaders()
{
	//update shaders that needs updating
	if (mShadersToUpdate.size()) {
		GeometryPtrVector geometry;
		for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
			geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
		}
		//use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (ShaderUpdateSet::reverse_iterator I = mShadersToUpdate.rbegin(); I != mShadersToUpdate.rend(); ++I) {
			mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->first, I->second.Areas, EventLayerUpdated, EventTerrainMaterialRecompiled, mLightning->getMainLightDirection()), 0);
		}
		mShadersToUpdate.clear();
	}
}

void TerrainHandler::updateAllPages()
{
	GeometryPtrVector geometry;
	for (PageVector::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(**I, *mSegmentManager, getDefaultHeight())));
	}

	//Update all pages
	AreaStore areas;
	areas.push_back(mTerrainInfo->getWorldSizeInIndices());

	//Update all shaders on all pages
	for (ShaderStore::const_iterator I = mShaderMap.begin(); I != mShaderMap.end(); ++I) {
		mTaskQueue->enqueueTask(new TerrainShaderUpdateTask(geometry, I->second, areas, EventLayerUpdated, EventTerrainMaterialRecompiled, mLightning->getMainLightDirection()), 0);
	}
}

void TerrainHandler::terrainEnabled(EmberEntity& entity)
{
	mTerrainEntity = &entity;
}

void TerrainHandler::terrainDisabled()
{
	mTerrainEntity = nullptr;
}

EmberEntity* TerrainHandler::getTerrainHoldingEntity() {
	return mTerrainEntity;
}

const TerrainInfo& TerrainHandler::getTerrainInfo() const
{
	return *mTerrainInfo;
}

TerrainPage* TerrainHandler::getTerrainPageAtPosition(const TerrainPosition& worldPosition) const
{

	int xRemainder = static_cast<int>(getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int>(getMin().y()) % (getPageMetersSize());

	int xIndex = static_cast<int>(floor((worldPosition.x() + xRemainder) / (getPageMetersSize())));
	int yIndex = static_cast<int>(ceil((worldPosition.y() + yRemainder) / (getPageMetersSize())));

	return getTerrainPageAtIndex(TerrainIndex(xIndex, yIndex));
}

void TerrainHandler::setUpTerrainPageAtIndex(const TerrainIndex& index, ITerrainPageBridge* bridge)
{
	//_fpreset();

	int x = index.first;
	int y = index.second;

	std::shared_ptr<ITerrainPageBridge> bridgePtr(bridge);
	//Add to our store of page bridges
	mPageBridges.insert(PageBridgeStore::value_type(index, bridgePtr));

	S_LOG_INFO("Setting up TerrainPage at index [" << x << "," << y << "]");
	if (mTerrainPages[x][y] == 0) {
		WFMath::Vector<3> sunDirection = WFMath::Vector<3>(0, 0, -1);
		if (mLightning) {
			sunDirection = mLightning->getMainLightDirection();
		}
		if (!mTaskQueue->enqueueTask(new TerrainPageCreationTask(*this, index, bridgePtr, *mHeightMapBufferProvider, *mHeightMap, sunDirection))) {
			//We need to alert the bridge since it's holding up a thread waiting for this call.
			bridge->terrainPageReady();
		}
	} else {
		TerrainPage* page = mTerrainPages[x][y];
		TerrainPageGeometryPtr geometryInstance(new TerrainPageGeometry(*page, getSegmentManager(), getDefaultHeight()));

		if (!mTaskQueue->enqueueTask(new TerrainPageReloadTask(*this, bridgePtr, geometryInstance, getAllShaders(), page->getWorldExtent(), mLightning->getMainLightDirection()))) {
			//We need to alert the bridge since it's holding up a thread waiting for this call.
			bridge->terrainPageReady();
		}
	}
}

TerrainPage* TerrainHandler::getTerrainPageAtIndex(const TerrainIndex& index) const
{

	TerrainPagestore::const_iterator I = mTerrainPages.find(index.first);
	if (I != mTerrainPages.end()) {
		TerrainPagecolumn::const_iterator J = I->second.find(index.second);
		if (J != I->second.end()) {
			return J->second;
		}
	}
	return 0;
}

bool TerrainHandler::getHeight(const TerrainPosition& point, float& height) const
{
	WFMath::Vector<3> vector;

	return mHeightMap->getHeightAndNormal(point.x(), point.y(), height, vector);
}

void TerrainHandler::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const
{
	mHeightMap->blitHeights(xMin, xMax, yMin, yMax, heights);
}


void TerrainHandler::setLightning(ILightning* lightning)
{
	mLightning = lightning;
}

void TerrainHandler::updateShadows()
{
	if (mLightning) {
		//Only update if there's a shadow texture set.
		if (!mPages.empty()) {
			auto page = mPages.front();
			if (page->getSurface() && page->getSurface()->getShadow() && page->getSurface()->getShadow()->getShadowTextureName() != "") {
				S_LOG_VERBOSE("Updating precomputed shadows.");
				WFMath::Vector<3> sunDirection = mLightning->getMainLightDirection();

				GeometryPtrVector geometry;
				for (auto& page : mPages) {
					geometry.push_back(TerrainPageGeometryPtr(new TerrainPageGeometry(*page, *mSegmentManager, getDefaultHeight())));
				}

				mTaskQueue->enqueueTask(new ShadowUpdateTask(geometry, sunDirection));
			}
		}
	} else {
		S_LOG_WARNING("Tried to update shadows without having any ILightning instance set.");
	}
}

float TerrainHandler::getDefaultHeight() const
{
	return -12;
}

bool TerrainHandler::updateTerrain(const TerrainDefPointStore& terrainPoints)
{
	mTaskQueue->enqueueTask(new TerrainUpdateTask(*mTerrain, terrainPoints, *this, *mTerrainInfo, mHasTerrainInfo, *mSegmentManager));
	return true;
}

void TerrainHandler::reloadTerrain(const std::vector<TerrainPosition>& positions)
{
	std::vector<WFMath::AxisBox<2>> areas;
	for (std::vector<TerrainPosition>::const_iterator I(positions.begin()); I != positions.end(); ++I) {
		const TerrainPosition& worldPosition(*I);
		//Make an area which will cover the area affected by the basepoint
		int res = mTerrain->getResolution();
		areas.push_back(WFMath::AxisBox<2>(worldPosition - WFMath::Vector<2>(res, res), worldPosition + WFMath::Vector<2>(res, res)));
	}
	reloadTerrain(areas);
}

void TerrainHandler::reloadTerrain(const std::vector<WFMath::AxisBox<2>>& areas)
{
	if (mTaskQueue->isActive()) {
		std::set<TerrainPage*> pagesToUpdate;
		for (std::vector<WFMath::AxisBox<2>>::const_iterator I(areas.begin()); I != areas.end(); ++I) {
			const WFMath::AxisBox<2>& area = *I;
			for (PageVector::const_iterator pageI = mPages.begin(); pageI != mPages.end(); ++pageI) {
				TerrainPage* page = *pageI;
				if (WFMath::Contains(page->getWorldExtent(), area, false) || WFMath::Intersect(page->getWorldExtent(), area, false) || WFMath::Contains(area, page->getWorldExtent(), false)) {
					pagesToUpdate.insert(page);
				}
			}
		}

		EventBeforeTerrainUpdate(areas, pagesToUpdate);
		//Spawn a separate task for each page to not bog down processing with all pages at once
		for (std::set<TerrainPage*>::const_iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
			BridgeBoundGeometryPtrVector geometryToUpdate;
			TerrainPage* page = *I;
			ITerrainPageBridgePtr bridgePtr;
			PageBridgeStore::const_iterator J = mPageBridges.find(page->getWFIndex());
			if (J != mPageBridges.end()) {
				bridgePtr = J->second;
			}
			geometryToUpdate.push_back(BridgeBoundGeometryPtrVector::value_type(TerrainPageGeometryPtr(new TerrainPageGeometry(*page, *mSegmentManager, getDefaultHeight())), bridgePtr));
			mTaskQueue->enqueueTask(new GeometryUpdateTask(geometryToUpdate, areas, *this, mShaderMap, *mHeightMapBufferProvider, *mHeightMap, mLightning->getMainLightDirection()));
		}
	}
}

const TerrainPosition TerrainHandler::getMax() const
{
	return mTerrainInfo->getWorldSizeInIndices().highCorner();
}

const TerrainPosition TerrainHandler::getMin() const
{
	return mTerrainInfo->getWorldSizeInIndices().lowCorner();
}

SegmentManager& TerrainHandler::getSegmentManager()
{
	return *mSegmentManager;
}

void TerrainHandler::addTerrainMod(TerrainMod* terrainMod)
{
	// Listen for changes to the modifier
	terrainMod->EventModChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainMod_Changed), terrainMod));
	// Listen for deletion of the modifier
	terrainMod->EventModDeleted.connect(sigc::bind(sigc::mem_fun(*this, &TerrainHandler::TerrainMod_Deleted), terrainMod));

	mTaskQueue->enqueueTask(new TerrainModUpdateTask(*mTerrain, *terrainMod, *this));
}

void TerrainHandler::TerrainMod_Changed(TerrainMod* terrainMod)
{
	if (mTaskQueue->isActive()) {
		mTaskQueue->enqueueTask(new TerrainModUpdateTask(*mTerrain, *terrainMod, *this));
	}
}

void TerrainHandler::TerrainMod_Deleted(TerrainMod* terrainMod)
{
	if (mTaskQueue->isActive()) {
		mTaskQueue->enqueueTask(new TerrainModUpdateTask(*mTerrain, *terrainMod, *this));
	}
}

void TerrainHandler::updateArea(const std::string& id, Mercator::Area* terrainArea)
{
	auto I = mAreas.find(id);
	if (I == mAreas.end()) {
		if (terrainArea && terrainArea->getLayer() != 0 && terrainArea->shape().isValid()) {
			//There's no existing area, we need to add a new one.
			Mercator::Area* newArea = new Mercator::Area(*terrainArea);
			mAreas.insert(AreaMap::value_type(id, newArea));

			mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, newArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders));
		}
		//If there's no existing area, and no valid supplied one, just don't do anything.
	} else {
		Mercator::Area* existingArea = I->second;
		if (!terrainArea || terrainArea->getLayer() == 0 || !terrainArea->shape().isValid()) {
			//We should remove the area.
			const TerrainShader* shader = 0;
			if (mAreaShaders.count(existingArea->getLayer())) {
				shader = mAreaShaders[existingArea->getLayer()];
			}
			mAreas.erase(I);
			mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, existingArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader));
		} else {
			//Check if we need to swap the area (if the layer has changed) or if we just can update the shape.
			if (terrainArea->getLayer() != existingArea->getLayer()) {
				const TerrainShader* shader = 0;
				if (mAreaShaders.count(existingArea->getLayer())) {
					shader = mAreaShaders[existingArea->getLayer()];
				}

				mAreas.erase(I);
				mTaskQueue->enqueueTask(new TerrainAreaRemoveTask(*mTerrain, existingArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader));

				Mercator::Area* newArea = new Mercator::Area(*terrainArea);
				mAreas.insert(AreaMap::value_type(id, newArea));
				mTaskQueue->enqueueTask(new TerrainAreaAddTask(*mTerrain, newArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), *this, TerrainLayerDefinitionManager::getSingleton(), mAreaShaders));
			} else {
				const TerrainShader* shader = 0;
				if (mAreaShaders.count(terrainArea->getLayer())) {
					shader = mAreaShaders[terrainArea->getLayer()];
				}
				mTaskQueue->enqueueTask(new TerrainAreaUpdateTask(*mTerrain, existingArea, *terrainArea, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate), shader));
			}
		}
	}
}

void TerrainHandler::frameProcessed(const TimeFrame&, unsigned int)
{
	if (mLightning) {
		//Update shadows every hour
		if (!mLastLightingUpdateAngle.isValid() || WFMath::Angle(mLightning->getMainLightDirection(), mLastLightingUpdateAngle) > (WFMath::numeric_constants<float>::pi() / 12)) {
			updateShadows();
			mLastLightingUpdateAngle = mLightning->getMainLightDirection();
		}
	}
}

}

}

}
