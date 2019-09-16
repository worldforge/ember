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

#include "services/config/ConfigService.h"

#include "../ShaderManager.h"
#include "../Scene.h"
#include "../Convert.h"

#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>

#ifdef WIN32
#include <tchar.h>
#define snprintf _snprintf
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
#endif

#include <sigc++/bind.h>

#include <utility>

using namespace Ogre;
namespace Ember
{
namespace OgreView
{
namespace Terrain
{


TerrainManager::TerrainManager(ITerrainAdapter* adapter, Scene& scene, ShaderManager& shaderManager, Eris::EventService& eventService) :
	UpdateShadows("update_shadows", this, "Updates shadows in the terrain."),
	mCompilerTechniqueProvider(new Techniques::CompilerTechniqueProvider(shaderManager, scene.getSceneManager())),
	mHandler(new TerrainHandler(adapter->getPageSize(), *mCompilerTechniqueProvider, eventService)),
	mIsFoliageShown(false),
	mTerrainAdapter(adapter),
	mFoliageBatchSize(32),
	mVegetation(new Foliage::Vegetation()),
	mScene(scene),
	mIsInitialized(false)
{
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
	adapter->bindTerrainShown(slot);

	mHandler->setPageSize(mTerrainAdapter->getPageSize());
	mHandler->updateAllPages();

	mHandler->EventTerrainEnabled.connect([&](EmberEntity& entity){ mTerrainAdapter->setTerrainEntity(&entity);});
}

TerrainManager::~TerrainManager()
{
	mTerrainAdapter->reset();

	//We must make sure that any tasks are purged in the handler before we destroy the terrain adapter.
    mHandler->shutdown();
}

void TerrainManager::startPaging()
{
	//Setting the camera will start the paging system
	mTerrainAdapter->setCamera(&getScene().getMainCamera());
}

bool TerrainManager::getHeight(const TerrainPosition& atPosition, float& height) const
{
	return mHandler->getHeight(atPosition, height);
}

void TerrainManager::blitHeights(int xMin, int xMax, int yMin, int yMax, std::vector<float>& heights) const
{
	mHandler->blitHeights(xMin, xMax, yMin, yMax, heights);
}


const TerrainInfo& TerrainManager::getTerrainInfo() const
{
	return mHandler->getTerrainInfo();
}


ITerrainAdapter* TerrainManager::getTerrainAdapter() const
{
	return mTerrainAdapter.get();
}

void TerrainManager::getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint>>& >& asyncCallback)
{
	mHandler->getBasePoints(asyncCallback);
}


void TerrainManager::updateFoliageVisibility()
{
	//	bool showFoliage = isFoliageShown();
	//
	//	PageVector::iterator I = mPages.begin();
	//	for (; I != mPages.end(); ++I) {
	//		if (showFoliage) {
	//			PageVector pages;
	//			pages.push_back(*I);
	//			mTaskQueue->enqueueTask(new FoliagePreparationTask(pages));
	//		} else {
	//			//TODO: implement destruction of the foliage data
	//		}
	//	}
}

void TerrainManager::config_Foliage(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_bool()) {
		mIsFoliageShown = static_cast<bool> (variable);
	} else {
		mIsFoliageShown = false;
	}
	updateFoliageVisibility();
}

void TerrainManager::config_TerrainTechnique(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	//TODO this is a bit crude and does more updates than necessary
	mHandler->updateAllPages();
}

void TerrainManager::config_TerrainPageSize(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_int()) {
		auto size = static_cast<unsigned int>(static_cast<int>(variable));
		mTerrainAdapter->setPageSize(size);
		mHandler->setPageSize(size);
		mHandler->updateAllPages();
	}
}

void TerrainManager::config_TerrainLoadRadius(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (variable.is_int()) {
		auto radius = static_cast<unsigned int>(static_cast<int>(variable));
		mTerrainAdapter->setLoadRadius(Ogre::Real(radius));
	}
}

void TerrainManager::terrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas, const std::set<TerrainPage*>& pages)
{

	for (auto page : pages) {
		const TerrainIndex& index = page->getWFIndex();

		S_LOG_VERBOSE("Updating terrain page [" << index.first << "|" << index.second << "]");
		mTerrainAdapter->reloadPage(index);
		EventTerrainPageGeometryUpdated.emit(*page);
	}
}


void TerrainManager::terrainHandler_ShaderCreated(const TerrainShader& shader)
{
	size_t index = mHandler->getAllShaders().size() - 1;

	const TerrainLayerDefinition& layerDef = shader.getLayerDefinition();
	for (const auto& foliage : layerDef.getFoliages()) {
		mVegetation->createPopulator(foliage, index);
	}
}

void TerrainManager::terrainHandler_WorldSizeChanged()
{
	if (!mIsInitialized) {
		initializeTerrain();
		mIsInitialized = true;
	}

}

void TerrainManager::terrainHandler_TerrainPageMaterialRecompiled(TerrainPage* page)
{
	if (mTerrainAdapter) {
		mTerrainAdapter->reloadPageMaterial(page->getWFIndex());
	}
}

void TerrainManager::initializeTerrain()
{
	assert(mTerrainAdapter);
	mTerrainAdapter->loadScene();
}

bool TerrainManager::isFoliageShown() const
{
	return mIsFoliageShown;
}

void TerrainManager::getPlantsForArea(PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback)
{
	Foliage::PlantPopulator* populator = mVegetation->getPopulator(query.getPlantType());
	if (populator) {
		mHandler->getPlantsForArea(*populator, query, std::move(asyncCallback));
	}
}

void TerrainManager::runCommand(const std::string& command, const std::string& args)
{
	if (UpdateShadows == command) {
		mHandler->updateShadows();
	}
}




Scene& TerrainManager::getScene() const
{
	return mScene;
}


void TerrainManager::shaderManager_LevelChanged(ShaderManager* shaderManager)
{
	mHandler->updateAllPages();
}

TerrainHandler& TerrainManager::getHandler()
{
	return *mHandler;
}


void TerrainManager::adapter_terrainShown(const Ogre::TRect<Ogre::Real>& rect)
{
	std::vector<Ogre::TRect<Ogre::Real>> areas;
	areas.push_back(rect);

	EventTerrainShown(areas);

}


}
}
}
