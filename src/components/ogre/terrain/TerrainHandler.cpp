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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainHandler.h"

#include "ITerrainPageBridge.h"
#include "TerrainDefPoint.h"
#include "TerrainShader.h"
#include "TerrainPage.h"
#include "TerrainPageSurface.h"
#include "TerrainPageGeometry.h"
#include "TerrainInfo.h"
#include "TerrainPageCreationTask.h"
#include "TerrainPageDeletionTask.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainAreaUpdateTask.h"
#include <components/ogre/terrain/TerrainModUpdateTask.h>
#include "TerrainUpdateTask.h"

#include "TerrainLayerDefinitionManager.h"
#include "TerrainLayerDefinition.h"

#include "GeometryUpdateTask.h"
#include "PlantQueryTask.h"
#include "HeightMap.h"
#include "HeightMapBufferProvider.h"
#include "PlantAreaQuery.h"
#include "SegmentManager.h"

#include "../Convert.h"
#include "../ILightning.h"

#include "framework/tasks/TaskQueue.h"
#include "framework/tasks/TaskExecutionContext.h"

#include <Eris/EventService.h>

#include <Mercator/Segment.h>
#include <Mercator/Terrain.h>
#include <Mercator/AreaShader.h>

#include <sigc++/bind.h>

#include <memory>
#include <utility>

namespace Ember {

namespace OgreView {

namespace Terrain {

class BasePointRetrieveTask : public Tasks::TemplateNamedTask<BasePointRetrieveTask> {

private:
	Mercator::Terrain& mTerrain;
	sigc::slot<void, Mercator::Terrain::Pointstore&> mAsyncCallback;
	Mercator::Terrain::Pointstore mPoints;

public:
	BasePointRetrieveTask(Mercator::Terrain& terrain, sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback) :
			mTerrain(terrain), mAsyncCallback(asyncCallback) {
	}

	~BasePointRetrieveTask() override = default;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override {
		mPoints = mTerrain.getPoints();
	}

	bool executeTaskInMainThread() override {
		mAsyncCallback(mPoints);
		return true;
	}

};

class TerrainPageReloadTask : public Tasks::TemplateNamedTask<TerrainPageReloadTask> {
private:
	TerrainHandler& mHandler;
	ITerrainPageBridgePtr mBridge;
	std::unique_ptr<TerrainPageGeometry> mGeometry;
	const std::vector<const TerrainShader*> mShaders;
	const WFMath::AxisBox<2> mArea;

public:
	TerrainPageReloadTask(TerrainHandler& handler,
						  ITerrainPageBridgePtr bridge,
						  std::unique_ptr<TerrainPageGeometry> geometry,
						  std::vector<const TerrainShader*> shaders,
						  const WFMath::AxisBox<2>& area) :
			mHandler(handler),
			mBridge(std::move(bridge)),
			mGeometry(std::move(geometry)),
			mShaders(std::move(shaders)),
			mArea(area) {
	}

	~TerrainPageReloadTask() override = default;

	void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) override {
		mGeometry->repopulate();
		AreaStore areas;
		areas.push_back(mArea);
		if (mBridge) {
			mBridge->updateTerrain(*mGeometry);
		}
		std::vector<std::shared_ptr<TerrainPageGeometry>> geometries;
		geometries.emplace_back(std::move(mGeometry));
		context.executeTask(std::make_unique<TerrainShaderUpdateTask>(std::move(geometries),
																	  mShaders,
																	  areas,
																	  mHandler.EventLayerUpdated,
																	  mHandler.EventTerrainMaterialRecompiled));
	}

	bool executeTaskInMainThread() override {
		if (mBridge) {
			mBridge->terrainPageReady();
		}
		return true;
	}

};

TerrainHandler::TerrainHandler(int pageIndexSize,
							   ICompilerTechniqueProvider& compilerTechniqueProvider,
							   Eris::EventService& eventService) :
		mPageIndexSize(pageIndexSize),
		mCompilerTechniqueProvider(compilerTechniqueProvider),
		mTerrainInfo(std::make_unique<TerrainInfo>(pageIndexSize)),
		mEventService(eventService),
		mTerrain(std::make_unique<Mercator::Terrain>(Mercator::Terrain::SHADED)),
		mSegmentManager(std::make_unique<SegmentManager>(*mTerrain, 64)),
		//The mercator buffers are one size larger than the resolution
		mHeightMapBufferProvider(std::make_unique<HeightMapBufferProvider>(mTerrain->getResolution() + 1)),
		mHeightMap(std::make_unique<HeightMap>(Mercator::Terrain::defaultLevel, mTerrain->getResolution())),
		mTaskQueue(std::make_unique<Tasks::TaskQueue>(1, eventService)),
		mHeightMax(std::numeric_limits<Ogre::Real>::min()), mHeightMin(std::numeric_limits<Ogre::Real>::max()),
		mHasTerrainInfo(false),
		mTerrainEntity(nullptr),
		mAreaCounter(0),
		mModCounter(0) {
	mSegmentManager->setEndlessWorldEnabled(true);
	mSegmentManager->setDefaultHeight(getDefaultHeight());
	//TODO: get these values from the server if possible
	mSegmentManager->setDefaultHeightVariation(10);

	EventTerrainEnabled.connect(sigc::mem_fun(*this, &TerrainHandler::terrainEnabled));
	EventTerrainDisabled.connect(sigc::mem_fun(*this, &TerrainHandler::terrainDisabled));

}

TerrainHandler::~TerrainHandler() = default;

void TerrainHandler::shutdown() {
	mTaskQueue->deactivate();
}

void TerrainHandler::setPageSize(int pageSize) {
	// Wait for all current tasks to finish
//	mTaskQueue->pollProcessedTasks(TimeFrame(boost::posix_time::seconds(60)));
	// Delete all page-related data
	mPageBridges.clear();
	mPages.clear();

	mTerrainInfo = std::make_unique<TerrainInfo>(pageSize + 1); //The number of vertices is always one more than the number of "units".
	mPageIndexSize = pageSize;
}

void TerrainHandler::getBasePoints(sigc::slot<void, Mercator::Terrain::Pointstore&>& asyncCallback) {
	mTaskQueue->enqueueTask(std::make_unique<BasePointRetrieveTask>(*mTerrain, asyncCallback));
}

TerrainShader* TerrainHandler::createShader(const TerrainLayerDefinition* layerDef, std::unique_ptr<Mercator::Shader> mercatorShader) {
	size_t index = mShaderMap.size();
	S_LOG_VERBOSE("Creating new shader for shader " << layerDef->mShaderName << " with index " << index);
	auto shader = std::make_unique<TerrainShader>(*mTerrain, index, *layerDef, std::move(mercatorShader));

	auto result = mShaderMap.emplace(&shader->getShader(), std::move(shader));

	if (result.second) {
		mBaseShaders.emplace_back(result.first->second.get());
		EventShaderCreated.emit(*result.first->second);
		return result.first->second.get();
	} else {
		return nullptr;
	}
}

void TerrainHandler::markShaderForUpdate(int layer, const WFMath::AxisBox<2>& affectedArea) {
	if (layer > 0) {
		auto I = mAreaShaders.find(layer);
		if (I == mAreaShaders.end()) {
			//Create new layer
			const TerrainLayerDefinition* layerDef = TerrainLayerDefinitionManager::getSingleton().getDefinitionForArea(layer);
			if (layerDef) {
				TerrainShader* shader = createShader(layerDef, std::make_unique<Mercator::AreaShader>(layer));
				mAreaShaders[layer] = shader;
			}
		} else {
			ShaderUpdateRequest& updateRequest = mShadersToUpdate[I->second];
			updateRequest.Areas.push_back(affectedArea);
			mEventService.runOnMainThread([&]() {
				updateShaders();
			}, mActiveMarker);
		}
	}
}

const std::map<const Mercator::Shader*, std::unique_ptr<TerrainShader>>& TerrainHandler::getAllShaders() const {
	return mShaderMap;
}

//void TerrainHandler::addPage(TerrainPage* page) {
//	const TerrainPosition& pos = page->getWFPosition();
//
//	auto oldPage = mTerrainPages[pos.x()][pos.y()];
//	if (oldPage) {
//		destroyPage(oldPage);
//	}
//
//	mTerrainPages[pos.x()][pos.y()] = page;
//	mPages.push_back(page);
//
//	//Since the height data for the page probably wasn't correctly set up before the page was created, we should adjust the positions for the entities that are placed on the page.
//	std::set<TerrainPage*> pagesToUpdate;
//	pagesToUpdate.insert(page);
//}

void TerrainHandler::destroyPage(TerrainPage* page) {
	const auto& index = page->getWFIndex();
	auto I = mPages.find(index);
	if (I != mPages.end()) {
		auto pagePtr = std::move(I->second);
		mPages.erase(I);
		//We should delete the page first when all existing tasks are completed. This is because some of them might refer to the page.
		if (mTaskQueue->isActive()) {
			mTaskQueue->enqueueTask(std::make_unique<TerrainPageDeletionTask>(std::move(pagePtr)));
		}
	}

}

int TerrainHandler::getPageIndexSize() const {
	return mPageIndexSize;
}

int TerrainHandler::getPageMetersSize() const {
	return getPageIndexSize() - 1;
}

void TerrainHandler::getPlantsForArea(Foliage::PlantPopulator& populator, PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback) {

	TerrainPosition wfPos(Convert::toWF(query.mCenter));

	TerrainIndex index(static_cast<int>(std::floor(query.mCenter.x / (mPageIndexSize - 1))),
					   static_cast<int>(-std::floor(query.mCenter.y / (mPageIndexSize - 1))));

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

	auto xIndex = static_cast<int>(std::floor(wfPos.x() / mTerrain->getResolution()));
	auto yIndex = static_cast<int>(std::floor(wfPos.y() / mTerrain->getResolution()));
	SegmentRefPtr segmentRef = mSegmentManager->getSegmentReference(xIndex, yIndex);
	if (segmentRef) {
		mTaskQueue->enqueueTask(std::make_unique<PlantQueryTask>(segmentRef, populator, query, std::move(asyncCallback)));

	}
}

ICompilerTechniqueProvider& TerrainHandler::getCompilerTechniqueProvider() {
	return mCompilerTechniqueProvider;
}

void TerrainHandler::removeBridge(const TerrainIndex& index) {
	auto I = mPageBridges.find(index);
	if (I != mPageBridges.end()) {
		auto page = I->second->getTerrainPage();
		if (page) {
			destroyPage(page);
		}
		mPageBridges.erase(I);
	}
}

void TerrainHandler::updateShaders() {
	//update shaders that needs updating
	if (!mShadersToUpdate.empty()) {
		GeometryPtrVector geometry;
		for (auto& page : mPages) {
			geometry.emplace_back(std::make_shared<TerrainPageGeometry>(*page.second, *mSegmentManager, getDefaultHeight()));
		}
		//use a reverse iterator, since we need to update top most layers first, since lower layers might depend on them for their foliage positions
		for (auto& entry : mShadersToUpdate) {
			mTaskQueue->enqueueTask(std::make_unique<TerrainShaderUpdateTask>(geometry,
																			  entry.first,
																			  entry.second.Areas,
																			  EventLayerUpdated,
																			  EventTerrainMaterialRecompiled)
			);
		}
		mShadersToUpdate.clear();
	}
}

void TerrainHandler::updateAllPages() {
	GeometryPtrVector geometry;
	for (auto& page : mPages) {
		geometry.emplace_back(std::make_shared<TerrainPageGeometry>(*page.second, *mSegmentManager, getDefaultHeight()));
	}

	//Update all pages
	AreaStore areas;
	areas.push_back(mTerrainInfo->getWorldSizeInIndices());

	//Update all shaders on all pages
	for (auto& entry : mShaderMap) {
		mTaskQueue->enqueueTask(std::make_unique<TerrainShaderUpdateTask>(geometry,
																		  entry.second.get(),
																		  areas,
																		  EventLayerUpdated,
																		  EventTerrainMaterialRecompiled),
								0);
	}
}

void TerrainHandler::terrainEnabled(EmberEntity& entity) {
	mTerrainEntity = &entity;
}

void TerrainHandler::terrainDisabled() {
	mTerrainEntity = nullptr;
}

EmberEntity* TerrainHandler::getTerrainHoldingEntity() {
	return mTerrainEntity;
}

const TerrainInfo& TerrainHandler::getTerrainInfo() const {
	return *mTerrainInfo;
}

TerrainPage* TerrainHandler::getTerrainPageAtPosition(const TerrainPosition& worldPosition) const {

	int xRemainder = static_cast<int>(getMin().x()) % (getPageMetersSize());
	int yRemainder = static_cast<int>(getMin().y()) % (getPageMetersSize());

	int xIndex = static_cast<int>(floor((worldPosition.x() + xRemainder) / (getPageMetersSize())));
	int yIndex = static_cast<int>(floor((worldPosition.y() + yRemainder) / (getPageMetersSize())));

	return getTerrainPageAtIndex(TerrainIndex(xIndex, yIndex));
}

void TerrainHandler::setUpTerrainPageAtIndex(const TerrainIndex& index, std::shared_ptr<Terrain::ITerrainPageBridge> bridge) {
	mEventService.runOnMainThread([this, index, bridge]() {
		//_fpreset();

		int x = index.first;
		int y = index.second;

		//Add to our store of page bridges
		mPageBridges.insert(PageBridgeStore::value_type(index, bridge));

		S_LOG_INFO("Setting up TerrainPage at index [" << x << "," << y << "]");
		auto I = mPages.find(index);
		if (I == mPages.end()) {

			auto resultI = mPages.emplace(index, std::make_unique<TerrainPage>(index, getPageIndexSize(), getCompilerTechniqueProvider()));
			if (resultI.second) {
				auto& page = resultI.first->second;
				bridge->bindToTerrainPage(page.get());


				S_LOG_VERBOSE("Adding terrain page to TerrainHandler: [" << index.first << "|" << index.second << "]");

				if (!mTaskQueue->enqueueTask(std::make_unique<TerrainPageCreationTask>(*this, page.get(), bridge, *mHeightMapBufferProvider, *mHeightMap))) {
					//We need to alert the bridge since it's holding up a thread waiting for this call.
					bridge->terrainPageReady();
				}
			} else {
				S_LOG_WARNING("Could not insert terrain page at [" << index.first << "|" << index.second << "]");
			}
		} else {
			auto& page = I->second;
			auto geometryInstance = std::make_unique<TerrainPageGeometry>(*page, getSegmentManager(), getDefaultHeight());

			std::vector<const TerrainShader*> shaders;
			shaders.reserve(mShaderMap.size());
			for (auto& entry : mShaderMap) {
				shaders.push_back(entry.second.get());
			}

			if (!mTaskQueue->enqueueTask(std::make_unique<TerrainPageReloadTask>(*this,
																				 bridge,
																				 std::move(geometryInstance),
																				 std::move(shaders),
																				 page->getWorldExtent()))) {
				//We need to alert the bridge since it's holding up a thread waiting for this call.
				bridge->terrainPageReady();
			}
		}
	}, mActiveMarker);

}

TerrainPage* TerrainHandler::getTerrainPageAtIndex(const TerrainIndex& index) const {

	auto I = mPages.find(index);
	if (I != mPages.end()) {
		return I->second.get();
	}

	return nullptr;
}

bool TerrainHandler::getHeight(const TerrainPosition& point, float& height) const {
	WFMath::Vector<3> vector;

	return mHeightMap->getHeightAndNormal(point.x(), point.y(), height, vector);
}

void TerrainHandler::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const {
	mHeightMap->blitHeights(xMin, xMax, yMin, yMax, heights);
}


float TerrainHandler::getDefaultHeight() const {
	return -12.f;
}

bool TerrainHandler::updateTerrain(const TerrainDefPointStore& terrainPoints) {
	mTaskQueue->enqueueTask(std::make_unique<TerrainUpdateTask>(*mTerrain, terrainPoints, *this, *mTerrainInfo, mHasTerrainInfo, *mSegmentManager));
	return true;
}

void TerrainHandler::reloadTerrain(const std::vector<TerrainPosition>& positions) {
	std::vector<WFMath::AxisBox<2>> areas;
	for (const auto& worldPosition : positions) {
		//Make an area which will cover the area affected by the basepoint
		int res = mTerrain->getResolution();
		areas.emplace_back(worldPosition - WFMath::Vector<2>(res, res), worldPosition + WFMath::Vector<2>(res, res));
	}
	reloadTerrain(areas);
}

void TerrainHandler::reloadTerrain(const std::vector<WFMath::AxisBox<2>>& areas) {
	if (mTaskQueue->isActive()) {
		std::set<TerrainPage*> pagesToUpdate;
		for (const auto& area : areas) {
			for (auto& entry : mPages) {
				auto* page = entry.second.get();
				if (WFMath::Contains(page->getWorldExtent(), area, false) || WFMath::Intersect(page->getWorldExtent(), area, false) || WFMath::Contains(area, page->getWorldExtent(), false)) {
					pagesToUpdate.insert(page);
				}
			}
		}

		EventBeforeTerrainUpdate(areas, pagesToUpdate);
		//Spawn a separate task for each page to not bog down processing with all pages at once
		for (auto page : pagesToUpdate) {
			BridgeBoundGeometryPtrVector geometryToUpdate;
			ITerrainPageBridgePtr bridgePtr;
			PageBridgeStore::const_iterator J = mPageBridges.find(page->getWFIndex());
			if (J != mPageBridges.end()) {
				bridgePtr = J->second;
			}
			geometryToUpdate.emplace_back(std::make_shared<TerrainPageGeometry>(*page, *mSegmentManager, getDefaultHeight()), bridgePtr);
			std::vector<const TerrainShader*> shaders;
			shaders.reserve(mShaderMap.size());
			for (auto& entry : mShaderMap) {
				shaders.push_back(entry.second.get());
			}
			mTaskQueue->enqueueTask(std::make_unique<GeometryUpdateTask>(geometryToUpdate,
																		 areas,
																		 *this,
																		 std::move(shaders),
																		 *mHeightMapBufferProvider,
																		 *mHeightMap));
		}
	}
}

TerrainPosition TerrainHandler::getMax() const {
	return mTerrainInfo->getWorldSizeInIndices().highCorner();
}

TerrainPosition TerrainHandler::getMin() const {
	return mTerrainInfo->getWorldSizeInIndices().lowCorner();
}

SegmentManager& TerrainHandler::getSegmentManager() {
	return *mSegmentManager;
}

void TerrainHandler::updateMod(const std::string& id,
							   WFMath::Point<3> pos,
							   WFMath::Quaternion orientation,
							   std::unique_ptr<Ember::Terrain::TerrainModTranslator> translator) {
	long modId = 0;
	auto I = mTerrainModsIndex.find(id);
	if (I == mTerrainModsIndex.end()) {
		//No existing mod, create one.
		if (translator) {
			modId = ++mModCounter;
			mTerrainModsIndex.emplace(id, modId);
		} else {
			return;
		}
	} else {
		modId = I->second;
	}


	if (pos.isValid() && orientation.isValid()) {
		mTaskQueue->enqueueTask(std::make_unique<TerrainModUpdateTask>(*mTerrain,
																	   std::move(translator),
																	   modId,
																	   std::move(pos),
																	   std::move(orientation),
																	   [=](const std::vector<WFMath::AxisBox<2>>& areas) {
																		   reloadTerrain(areas);
																	   }));
	}
}

const std::unordered_map<std::string, long>& TerrainHandler::getAreas() const {
	return mAreas;
}


void TerrainHandler::updateArea(const std::string& id, std::unique_ptr<Mercator::Area> terrainArea) {
	auto I = mAreas.find(id);
	if (I == mAreas.end()) {
		if (terrainArea && terrainArea->getLayer() != 0 && terrainArea->shape().isValid()) {
			//There's no existing area, we need to add a new one.
			auto areaId = mAreaCounter++;
			mAreas.emplace(id, areaId);
			mTaskQueue->enqueueTask(std::make_unique<TerrainAreaUpdateTask>(*mTerrain, areaId, std::move(terrainArea), sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate)));
		}
		//If there's no existing area, and no valid supplied one, just don't do anything.
	} else {
		auto existingAreaId = I->second;
		if (!terrainArea || terrainArea->getLayer() == 0 || !terrainArea->shape().isValid()) {
			//We should remove the area.
			mAreas.erase(I);
			mTaskQueue->enqueueTask(std::make_unique<TerrainAreaUpdateTask>(*mTerrain, existingAreaId, nullptr, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate)));
		} else {
			auto existingArea = mTerrain->getArea(existingAreaId);
			//Check if we need to swap the area (if the layer has changed) or if we just can update the shape.
			if (existingArea && terrainArea->getLayer() != existingArea->getLayer()) {
				//Remove and add if we've switched layers.
				mTaskQueue->enqueueTask(std::make_unique<TerrainAreaUpdateTask>(*mTerrain, existingAreaId, nullptr, sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate)));
				mTaskQueue->enqueueTask(std::make_unique<TerrainAreaUpdateTask>(*mTerrain, existingAreaId, std::move(terrainArea), sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate)));
			} else {
				mTaskQueue->enqueueTask(std::make_unique<TerrainAreaUpdateTask>(*mTerrain, existingAreaId, std::move(terrainArea), sigc::mem_fun(*this, &TerrainHandler::markShaderForUpdate)));
			}
		}
	}
}

}

}

}
