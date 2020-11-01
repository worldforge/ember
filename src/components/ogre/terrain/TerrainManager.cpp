/*
 Copyright (C) 2004  Erik Ogenvik

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

#include "TerrainManager.h"

#include "TerrainHandler.h"
#include "TerrainInfo.h"
#include "TerrainShader.h"
#include "TerrainPage.h"

#include "ITerrainAdapter.h"

#include "TerrainLayerDefinition.h"
#include "PlantAreaQuery.h"

#include "techniques/CompilerTechniqueProvider.h"

#include "foliage/Vegetation.h"

#include "framework/TimeFrame.h"

#include "../ShaderManager.h"
#include "../Scene.h"
#include "../Convert.h"

#include "components/ogre/environment/Foliage.h"
#include "components/ogre/environment/FoliageDetailManager.h"

#include <Eris/View.h>

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>

#ifdef WIN32
#include <tchar.h>
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
#endif

#include <sigc++/bind.h>

#include <utility>
#include <framework/MainLoopController.h>

using namespace Ogre;
namespace Ember {
namespace OgreView {
namespace Terrain {


TerrainManager::TerrainManager(std::unique_ptr<ITerrainAdapter> adapter,
							   Scene& scene,
							   Eris::View& view,
							   ShaderManager& shaderManager,
							   Eris::EventService& eventService,
							   GraphicalChangeAdapter& graphicalChangeAdapter) :
		mCompilerTechniqueProvider(std::make_unique<Techniques::CompilerTechniqueProvider>(shaderManager, scene.getSceneManager())),
		mHandler(std::make_unique<TerrainHandler>(adapter->getPageSize(), *mCompilerTechniqueProvider, eventService)),
		mIsFoliageShown(false),
		mTerrainAdapter(std::move(adapter)),
		mFoliageBatchSize(32),
		mVegetation(std::make_unique<Foliage::Vegetation>()),
		mScene(scene),
		mGraphicalChangeAdapter(graphicalChangeAdapter),
		mView(view),
		mIsInitialized(false) {
	registerConfigListener("graphics", "foliage", sigc::mem_fun(*this, &TerrainManager::config_Foliage));
	registerConfigListener("terrain", "preferredtechnique", sigc::mem_fun(*this, &TerrainManager::config_TerrainTechnique), false);
	registerConfigListener("terrain", "pagesize", sigc::mem_fun(*this, &TerrainManager::config_TerrainPageSize), false);
	registerConfigListener("terrain", "loadradius", sigc::mem_fun(*this, &TerrainManager::config_TerrainLoadRadius));

	shaderManager.EventLevelChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::shaderManager_LevelChanged), &shaderManager));

	mHandler->EventShaderCreated.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_ShaderCreated));
	mHandler->EventAfterTerrainUpdate.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_AfterTerrainUpdate));
	mHandler->EventWorldSizeChanged.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_WorldSizeChanged));
	mHandler->EventTerrainMaterialRecompiled.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_TerrainPageMaterialRecompiled));

	sigc::slot<void, const Ogre::TRect<Ogre::Real>> slot = sigc::mem_fun(*this, &TerrainManager::adapter_terrainShown);
	mTerrainAdapter->bindTerrainShown(slot);

	mHandler->setPageSize(mTerrainAdapter->getPageSize());
	mHandler->updateAllPages();

	mHandler->EventTerrainEnabled.connect([&](EmberEntity& entity) {
		mTerrainAdapter->setTerrainEntity(&entity);
		updateFoliageVisibility();
	});
	mHandler->EventTerrainDisabled.connect([&]() {
		mTerrainAdapter->reset();
		mIsInitialized = false;
		updateFoliageVisibility();
	});
}

TerrainManager::~TerrainManager() {
	//Reset adapter before shutting down handler.
	mTerrainAdapter->reset();

	//We must make sure that any tasks are purged in the handler before we destroy the terrain adapter.
	mHandler->shutdown();
}

void TerrainManager::startPaging() {
	//Setting the camera will start the paging system
	mTerrainAdapter->setCamera(&getScene().getMainCamera());
}

bool TerrainManager::getHeight(const TerrainPosition& atPosition, float& height) const {
	return mHandler->getHeight(atPosition, height);
}

void TerrainManager::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const {
	mHandler->blitHeights(xMin, xMax, yMin, yMax, heights);
}


const TerrainInfo& TerrainManager::getTerrainInfo() const {
	return mHandler->getTerrainInfo();
}


ITerrainAdapter* TerrainManager::getTerrainAdapter() const {
	return mTerrainAdapter.get();
}

void TerrainManager::getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint>>&>& asyncCallback) {
	mHandler->getBasePoints(asyncCallback);
}


void TerrainManager::updateFoliageVisibility() {
	if (mIsFoliageShown && mIsInitialized) {
		if (!mFoliage) {
			//create the foliage
			mFoliage = std::make_unique<Environment::Foliage>(*this);
			EventFoliageCreated.emit();
			mFoliageInitializer = std::make_unique<DelayedFoliageInitializer>([this]() { initializeFoliage(); }, mView, 1000, 15000);
		}
	} else {
		mFoliageDetailManager.reset();
		mFoliageInitializer.reset();
		mFoliage.reset();
	}
}

void TerrainManager::config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_bool()) {
		mIsFoliageShown = static_cast<bool> (variable);
	} else {
		mIsFoliageShown = false;
	}
	updateFoliageVisibility();
}

void TerrainManager::config_TerrainTechnique(const std::string& section, const std::string& key, varconf::Variable& variable) {
	//TODO this is a bit crude and does more updates than necessary
	mHandler->updateAllPages();
}

void TerrainManager::config_TerrainPageSize(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_int()) {
		auto size = static_cast<unsigned int>(static_cast<int>(variable));
		mTerrainAdapter->setPageSize(size);
		mHandler->setPageSize(size);
		mHandler->updateAllPages();
	}
}

void TerrainManager::config_TerrainLoadRadius(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (variable.is_int()) {
		auto radius = static_cast<unsigned int>(static_cast<int>(variable));
		mTerrainAdapter->setLoadRadius(Ogre::Real(radius));
	}
}

void TerrainManager::terrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<TerrainPage*>& pages) {

	for (auto page : pages) {
		const TerrainIndex& index = page->getWFIndex();

		S_LOG_VERBOSE("Updating terrain page [" << index.first << "|" << index.second << "]");
		mTerrainAdapter->reloadPage(index);
		EventTerrainPageGeometryUpdated.emit(*page);
	}
}


void TerrainManager::terrainHandler_ShaderCreated(const TerrainLayer& layer) {
	if (mFoliage) {
		mFoliage->initializeLayer(layer);
		for (const auto& foliage : layer.layerDef.mFoliages) {
			mVegetation->createPopulator(foliage, layer.terrainIndex);
		}
	}
}

void TerrainManager::terrainHandler_WorldSizeChanged() {
	if (!mIsInitialized) {
		initializeTerrain();
		mIsInitialized = true;
		updateFoliageVisibility();
	}

}

void TerrainManager::terrainHandler_TerrainPageMaterialRecompiled(TerrainPage& page) {
	if (mTerrainAdapter) {
		mTerrainAdapter->reloadPageMaterial(page.getWFIndex());
	}
}

void TerrainManager::initializeTerrain() {
	assert(mTerrainAdapter);
	mTerrainAdapter->loadScene();
}

bool TerrainManager::isFoliageShown() const {
	return mIsFoliageShown;
}

void TerrainManager::getPlantsForArea(PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback) {
	auto* populator = mVegetation->getPopulator(query.mPlantType);
	if (populator) {
		mHandler->getPlantsForArea(*populator, query, std::move(asyncCallback));
	}
}

Scene& TerrainManager::getScene() const {
	return mScene;
}


void TerrainManager::shaderManager_LevelChanged(ShaderManager*) {
	mHandler->updateAllPages();
}

TerrainHandler& TerrainManager::getHandler() {
	return *mHandler;
}


void TerrainManager::adapter_terrainShown(const Ogre::TRect<Ogre::Real>& rect) {
	std::vector<Ogre::TRect<Ogre::Real>> areas;
	areas.push_back(rect);

	EventTerrainShown(areas);

}


void TerrainManager::initializeFoliage() {
	if (mFoliage) {
		mFoliageDetailManager = std::make_unique<Environment::FoliageDetailManager>(*mFoliage, mGraphicalChangeAdapter);
		mFoliageDetailManager->initialize();
	}
}

DelayedFoliageInitializer::DelayedFoliageInitializer(std::function<void()> callback, Eris::View& view, unsigned int intervalMs, unsigned int maxTimeMs) :
		mCallback(std::move(callback)),
		mView(view),
		mIntervalMs(intervalMs),
		mMaxTimeMs(maxTimeMs),
		mTimeout(std::make_unique<Eris::TimedEvent>(view.getEventService(), std::chrono::milliseconds(intervalMs), [&]() { this->timout_Expired(); })),
		mTotalElapsedTime(0) {
	//don't load the foliage directly, instead wait some seconds for all terrain areas to load
	//the main reason is that new terrain areas will invalidate the foliage causing a reload
	//by delaying the foliage we can thus in most cases avoid those reloads
	//wait three seconds
}

DelayedFoliageInitializer::~DelayedFoliageInitializer() = default;

void DelayedFoliageInitializer::timout_Expired() {
	//load the foliage if either all queues entities have been loaded, or 15 seconds has elapsed
	if (mView.lookQueueSize() == 0 || mTotalElapsedTime > mMaxTimeMs) {
		mCallback();
	} else {
		mTotalElapsedTime += mIntervalMs;
		mTimeout = std::make_unique<Eris::TimedEvent>(mView.getEventService(), std::chrono::milliseconds(mIntervalMs), [&]() { this->timout_Expired(); });
	}
}


}
}
}
