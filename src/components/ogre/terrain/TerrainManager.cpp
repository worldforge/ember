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

#include "TerrainHandler.h"
#include "TerrainInfo.h"
#include "TerrainShader.h"
#include "TerrainPage.h"

#include "ISceneManagerAdapter.h"

#include "TerrainLayerDefinition.h"
#include "PlantAreaQuery.h"

#include "techniques/CompilerTechniqueProvider.h"

#include "foliage/Vegetation.h"

#include "framework/LoggingInstance.h"

#include "services/config/ConfigService.h"

#include "../ShaderManager.h"
#include "../Scene.h"
#include "../Convert.h"

#include <Eris/TerrainMod.h>

#include <sigc++/object_slot.h>


#include <OgreRoot.h>
#include <OgreGpuProgramManager.h>

#ifdef WIN32
#include <tchar.h>
#define snprintf _snprintf
#include <io.h> // for _access, Win32 version of stat()
#include <direct.h> // for _mkdir
#endif

#include <limits>

//#include <boost/shared_ptr.hpp>

#include <sigc++/bind.h>

using namespace Ogre;
namespace Ember
{
namespace OgreView
{
namespace Terrain
{


TerrainManager::TerrainManager(ISceneManagerAdapter* adapter, Scene& scene, ShaderManager& shaderManager, sigc::signal<void, float>& erisEndPollSignal) :
	UpdateShadows("update_shadows", this, "Updates shadows in the terrain."), mCompilerTechniqueProvider(new Techniques::CompilerTechniqueProvider(shaderManager, scene.getSceneManager())), mHandler(new TerrainHandler(adapter->getPageSize(), *mCompilerTechniqueProvider)), mIsFoliageShown(false), mSceneManagerAdapter(0), mFoliageBatchSize(32), mVegetation(new Foliage::Vegetation()), mScene(scene), mIsInitialized(false)
{
	mSceneManagerAdapter = adapter;

	loadTerrainOptions();

	Ogre::Root::getSingleton().addFrameListener(this);

	registerConfigListener("graphics", "foliage", sigc::mem_fun(*this, &TerrainManager::config_Foliage));

	shaderManager.EventLevelChanged.connect(sigc::bind(sigc::mem_fun(*this, &TerrainManager::shaderManager_LevelChanged), &shaderManager));

	erisEndPollSignal.connect(sigc::mem_fun(*this, &TerrainManager::application_EndErisPoll));

	mHandler->EventShaderCreated.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_ShaderCreated));
	mHandler->EventAfterTerrainUpdate.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_AfterTerrainUpdate));
	mHandler->EventWorldSizeChanged.connect(sigc::mem_fun(*this, &TerrainManager::terrainHandler_WorldSizeChanged));
}

TerrainManager::~TerrainManager()
{
	Ogre::Root::getSingleton().removeFrameListener(this);


	getAdapter()->reset();

	delete mVegetation;


	delete mSceneManagerAdapter;

	delete mHandler;
	delete mCompilerTechniqueProvider;
}

void TerrainManager::loadTerrainOptions()
{

	getAdapter()->setResourceGroupName("General");

	getAdapter()->loadOptions(Ember::EmberServices::getSingletonPtr()->getConfigService()->getSharedConfigDirectory() + "terrain.cfg");

	getAdapter()->setCamera(&getScene().getMainCamera());

	getAdapter()->setUninitializedHeight(mHandler->getDefaultHeight());

}

bool TerrainManager::getHeight(const TerrainPosition& atPosition, float& height) const
{
	return mHandler->getHeight(atPosition, height);
}


const TerrainInfo& TerrainManager::getTerrainInfo() const
{
	return mHandler->getTerrainInfo();
}


ISceneManagerAdapter* TerrainManager::getAdapter() const
{
	return mSceneManagerAdapter;
}

void TerrainManager::getBasePoints(sigc::slot<void, std::map<int, std::map<int, Mercator::BasePoint> >& >& asyncCallback)
{
	mHandler->getBasePoints(asyncCallback);
}


bool TerrainManager::frameEnded(const Ogre::FrameEvent & evt)
{
	return true;
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
	if (GpuProgramManager::getSingleton().isSyntaxSupported("arbvp1") && variable.is_bool()) {
		mIsFoliageShown = static_cast<bool> (variable);
	} else {
		mIsFoliageShown = false;
	}
	updateFoliageVisibility();
}

void TerrainManager::terrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2> >& areas, const std::set<TerrainPage*>& pages)
{

	for (std::set<TerrainPage*>::const_iterator I = pages.begin(); I != pages.end(); ++I) {
		TerrainPage* page = *I;
		Ogre::Vector2 targetPage = Convert::toOgre<Ogre::Vector2>(page->getWFPosition());

		///note that we've switched the x and y offset here, since the terraininfo is in WF coords, but we now want Ogre coords
		Ogre::Vector2 adjustedOgrePos(targetPage.x + mHandler->getTerrainInfo().getPageOffsetY(), targetPage.y + mHandler->getTerrainInfo().getPageOffsetX());

		S_LOG_VERBOSE("Updating terrain page at position x: " << adjustedOgrePos.x << " y: " << adjustedOgrePos.y);
		getAdapter()->reloadPage(static_cast<unsigned int> (adjustedOgrePos.x), static_cast<unsigned int> (adjustedOgrePos.y));
		EventTerrainPageGeometryUpdated.emit(*page);
	}
}


void TerrainManager::terrainHandler_ShaderCreated(const TerrainShader& shader)
{
	size_t index = mHandler->getAllShaders().size() - 1;

	const TerrainLayerDefinition& layerDef = shader.getLayerDefinition();
	for (TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator I = layerDef.getFoliages().begin(); I != layerDef.getFoliages().end(); ++I) {
		mVegetation->createPopulator(*I, index);
	}
}

void TerrainManager::terrainHandler_WorldSizeChanged()
{
	if (!mIsInitialized) {
		initializeTerrain();
		mIsInitialized = true;
	}

}

void TerrainManager::initializeTerrain()
{
	ISceneManagerAdapter* adapter = getAdapter();
	if (adapter) {
		const TerrainInfo& terrainInfo = mHandler->getTerrainInfo();
		adapter->setWorldPagesDimensions(terrainInfo.getTotalNumberOfPagesY(), terrainInfo.getTotalNumberOfPagesX(), terrainInfo.getPageOffsetY(), terrainInfo.getPageOffsetX());

		adapter->loadScene();
		const WFMath::AxisBox<2>& worldSize = terrainInfo.getWorldSizeInIndices();
		//		float heightMin = std::numeric_limits<float>::min();
		//		float heightMax = std::numeric_limits<float>::max();
		//		if (heightMax < heightMin) {
		float heightMin = -100;
		float heightMax = 100;
		//		}
		Ogre::AxisAlignedBox worldBox(worldSize.lowCorner().x(), heightMin, -worldSize.highCorner().y(), worldSize.highCorner().x(), heightMax, -worldSize.lowCorner().y());

		std::stringstream ss;
		ss << worldBox;
		S_LOG_INFO("New size of the world: " << ss.str());

		adapter->resize(worldBox, 16);

		S_LOG_INFO("Pages: X: " << terrainInfo.getTotalNumberOfPagesX() << " Y: " << terrainInfo.getTotalNumberOfPagesY() << " Total: " << terrainInfo.getTotalNumberOfPagesX() * terrainInfo.getTotalNumberOfPagesY());
		S_LOG_INFO("Page offset: X" << terrainInfo.getPageOffsetX() << " Y: " << terrainInfo.getPageOffsetY());

		///load the first page, thus bypassing the normal paging system. This is to prevent the user from floating in the thin air while the paging system waits for a suitable time to load the first page.
		adapter->loadFirstPage();
	}
}
bool TerrainManager::isFoliageShown() const
{
	return mIsFoliageShown;
}

void TerrainManager::getPlantsForArea(PlantAreaQuery& query, sigc::slot<void, const PlantAreaQueryResult&> asyncCallback)
{
	Foliage::PlantPopulator* populator = mVegetation->getPopulator(query.getPlantType());
	if (populator) {
		mHandler->getPlantsForArea(*populator, query, asyncCallback);
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

void TerrainManager::application_EndErisPoll(float)
{
	mHandler->pollTasks();
}

TerrainHandler& TerrainManager::getHandler()
{
	return *mHandler;
}


}
}
}
