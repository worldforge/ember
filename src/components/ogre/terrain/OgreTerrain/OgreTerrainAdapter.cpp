/*
 Copyright (C) 2013 Samuel Kogler

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

#include "OgreTerrainAdapter.h"

#include "OgreTerrainObserver.h"
#include "OgreTerrainDefiner.h"
#include "../ITerrainObserver.h"
#include "components/ogre/TerrainPageDataProvider.h"
#include "OgreTerrainMaterialGeneratorEmber.h"

#include <OgreSceneManager.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>
#include <OgrePageManager.h>
#include <OgreGrid2DPageStrategy.h>


namespace Ember
{
namespace OgreView
{
namespace Terrain
{

//TODO SK: provide proper arguments to terrain group, move to constants/configuration parameters
OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera, unsigned int terrainPageSize) :
		mTerrainPageSize(terrainPageSize),
		mLoadRadius(terrainPageSize / 2),
		mHoldRadius(terrainPageSize * 2),
		mSceneManager(sceneManager),
		mPageManager(OGRE_NEW Ogre::PageManager()),
		mTerrainPaging(OGRE_NEW Ogre::TerrainPaging(mPageManager)),
		mPagedWorld(nullptr),
		mTerrainPagedWorldSection(nullptr),
		mTerrainGlobalOptions(OGRE_NEW Ogre::TerrainGlobalOptions()),
		mTerrainGroup(OGRE_NEW Ogre::TerrainGroup(&sceneManager, Ogre::Terrain::ALIGN_X_Z, terrainPageSize, Ogre::Real(terrainPageSize))),
		mPageDataProvider(nullptr)
{
	// Other params
	mTerrainGlobalOptions->setSkirtSize(30.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);

	// Set our own page provider which so far only prevents the page manager trying to load pages from disk
	mPageManager->setPageProvider(&mTerrainPageProvider);
	mPageManager->addCamera(mainCamera);
	Ogre::Terrain::ImportData& defaultImportData = mTerrainGroup->getDefaultImportSettings();
	Ogre::Vector3 origin;

	origin.x = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.z = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.y = 0;
	mTerrainGroup->setOrigin(origin);
}

OgreTerrainAdapter::~OgreTerrainAdapter()
{
	OGRE_DELETE mTerrainPaging;
	mPageManager->destroyWorld(mPagedWorld);
	OGRE_DELETE mPageManager;
	OGRE_DELETE mTerrainGlobalOptions;
}

int OgreTerrainAdapter::getPageSize()
{
	return static_cast<int>(mTerrainGroup->getTerrainSize());
}

Ogre::Real OgreTerrainAdapter::getHeightAt(const Ogre::Real x, const Ogre::Real z)
{
	Ogre::Terrain* foundTerrain = nullptr;
	float height = mTerrainGroup->getHeightAtWorldPosition(x, 0.0, z, &foundTerrain);
	assert(foundTerrain);
	return Ogre::Real(height);
}

void OgreTerrainAdapter::setWorldPagesDimensions(int numberOfPagesWidth, int numberOfPagesHeight, int widthOffsetInPages, int heightOffsetInPages)
{
}

void OgreTerrainAdapter::setCamera(Ogre::Camera* camera)
{
	if (!mPageManager->hasCamera(camera)) {
		// For now, we want only one camera to affect paging
		for (auto currentCamera : mPageManager->getCameraList()) {
			mPageManager->removeCamera(currentCamera);
		}
		mPageManager->addCamera(camera);
	}
}

void OgreTerrainAdapter::setResourceGroupName(const std::string& groupName)
{
}

void OgreTerrainAdapter::loadOptions(const std::string& filePath)
{
}

void OgreTerrainAdapter::resize(Ogre::AxisAlignedBox newSize, int levels)
{
}

void OgreTerrainAdapter::loadScene()
{
	// Use our own material generator
	// Initialized here because it needs a IPageDataProvider
	mTerrainGlobalOptions->setDefaultMaterialGenerator(
			Ogre::TerrainMaterialGeneratorPtr(OGRE_NEW OgreTerrainMaterialGeneratorEmber(*mPageDataProvider,
					mTerrainGroup->getOrigin().x, mTerrainGroup->getOrigin().z)));

	mPagedWorld = mPageManager->createWorld();
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, mLoadRadius, mHoldRadius,
			-10, -10, 10, 10, "", 0);
	mTerrainPagedWorldSection->setDefiner(new OgreTerrainDefiner(mPageDataProvider));
}

void OgreTerrainAdapter::reset()
{
	mTerrainPagedWorldSection->removeAllPages();
}

void OgreTerrainAdapter::setOption(const std::string& strKey, const void* pValue)
{
}

void OgreTerrainAdapter::getOption(const std::string& strKey, void* pDestValue)
{
}

void OgreTerrainAdapter::setUninitializedHeight(float height)
{
}

void OgreTerrainAdapter::reloadAllPages()
{
	mTerrainPagedWorldSection->removeAllPages();
}

void OgreTerrainAdapter::reloadPage(const Domain::TerrainIndex& index)
{
	if (mTerrainPagedWorldSection) {
		Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page) {
			Ogre::uint32 pageID = mTerrainPagedWorldSection->getGridStrategy()->getPageID(page->getPosition(), mTerrainPagedWorldSection);

			// TODO performance: Unload synchronously, could cause lags
			mTerrainPagedWorldSection->unloadPage(pageID, true);
			mTerrainPagedWorldSection->loadPage(pageID);
		}
	}
}

void OgreTerrainAdapter::reloadPageMaterial(const Domain::TerrainIndex& index)
{
	if (mTerrainPagedWorldSection) {
		Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page) {
			// This is a trick to get the terrain to relaod it's material
			page->addLayer(0, 0.0, nullptr);
			page->removeLayer(0);
		}
	}
}

void OgreTerrainAdapter::loadFirstPage()
{
}

std::string OgreTerrainAdapter::getDebugInfo()
{
	return "Not available";
}

ITerrainObserver* OgreTerrainAdapter::createObserver()
{
	return new OgreTerrainObserver();
}

void OgreTerrainAdapter::destroyObserver(ITerrainObserver* observer)
{
	//TODO SK: add proper observer logic
	//This will not always delete the observer, but it also won't crash
	OgreTerrainObserver* ogreTerrainObserver = dynamic_cast<OgreTerrainObserver*>(observer);
	delete ogreTerrainObserver;
}

std::pair<bool, Ogre::Vector3> OgreTerrainAdapter::rayIntersects(const Ogre::Ray& ray) const
{
	//TODO SK: find limit value
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray, 1000.0f);
	return std::pair<bool, Ogre::Vector3>(result.hit, result.position);
}

void OgreTerrainAdapter::setPageDataProvider(IPageDataProvider* pageDataProvider)
{
	mPageDataProvider = pageDataProvider;
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */

