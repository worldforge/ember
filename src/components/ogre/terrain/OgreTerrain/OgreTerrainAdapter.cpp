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
#include <OgrePagedWorld.h>
#include <OgrePageManager.h>
#include <OgreGrid2DPageStrategy.h>

#define EMBER_OGRE_TERRAIN_HALF_RANGE 0x7FFF

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera, unsigned int terrainPageSize) :
		mLoadRadius(300),
		mHoldRadius(mLoadRadius * 2),
		mSceneManager(sceneManager),
		mPageManager(OGRE_NEW Ogre::PageManager()),
		mTerrainPaging(OGRE_NEW Ogre::TerrainPaging(mPageManager)),
		mPagedWorld(nullptr),
		mTerrainPagedWorldSection(nullptr),
		mTerrainGlobalOptions(OGRE_NEW Ogre::TerrainGlobalOptions()),
		mTerrainGroup(OGRE_NEW Ogre::TerrainGroup(&sceneManager, Ogre::Terrain::ALIGN_X_Z, terrainPageSize, Ogre::Real(terrainPageSize - 1))),
		mPageDataProvider(nullptr),
		mMaterialGenerator(nullptr)
{
	// Other params
	mTerrainGlobalOptions->setSkirtSize(30.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);

	setPageSize(terrainPageSize);

	// Set our own page provider which so far only prevents the page manager trying to load pages from disk
	mPageManager->setPageProvider(&mTerrainPageProvider);
	mPageManager->addCamera(mainCamera);
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


void OgreTerrainAdapter::setPageSize(unsigned int pageSize)
{
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->removeAllPages();
	}
	mTerrainGroup->removeAllTerrains();
	mTerrainGroup->setTerrainSize(static_cast<Ogre::uint16>(pageSize));
	mTerrainGroup->setTerrainWorldSize(Ogre::Real(pageSize - 1));
	Ogre::Vector3 origin;
	origin.x = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.z = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.y = 0;
	mTerrainGroup->setOrigin(origin);
	if (mMaterialGenerator) {
		mMaterialGenerator->setOrigin(origin.x, origin.z);
	}

	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->getGridStrategyData()->setOrigin(mTerrainGroup->getOrigin());
		mTerrainPagedWorldSection->getGridStrategyData()->setCellSize(mTerrainGroup->getTerrainWorldSize());
	}
}

void OgreTerrainAdapter::setLoadRadius(const Ogre::Real& loadRadius)
{
	mLoadRadius = loadRadius;
	mHoldRadius = mLoadRadius * 2;
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->setLoadRadius(mLoadRadius);
		mTerrainPagedWorldSection->setHoldRadius(mHoldRadius);
	}
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
	mPagedWorld = mPageManager->createWorld();
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, mLoadRadius, mHoldRadius,
			-EMBER_OGRE_TERRAIN_HALF_RANGE, -EMBER_OGRE_TERRAIN_HALF_RANGE, EMBER_OGRE_TERRAIN_HALF_RANGE, EMBER_OGRE_TERRAIN_HALF_RANGE, "", 0);
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
			page->dirty();
			page->update();
		}
	}
}

void OgreTerrainAdapter::reloadPageMaterial(const Domain::TerrainIndex& index)
{
	if (mTerrainPagedWorldSection) {
		Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page) {
			// This is a trick to get the terrain to reload it's material
			page->addLayer(0, 0.0, nullptr);
			page->removeLayer(0);
			//We must tell the page what area needs updating. For now we'll update the whole page.
			//But we should really look into only updating the area that has changed.
			page->_dirtyCompositeMapRect(Ogre::Rect(0, 0, page->getSize(), page->getSize()));
			page->updateCompositeMap();
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
	// This is a hack. Our custom material generator fires an event whenever a terrain area is updated
	if (mMaterialGenerator) {
		return new OgreTerrainObserver(mMaterialGenerator->EventTerrainAreaUpdated);
	} else {
		return nullptr;
	}
}

void OgreTerrainAdapter::destroyObserver(ITerrainObserver* observer)
{
	// Will only delete the observer if it was of the right type
	OgreTerrainObserver* ogreTerrainObserver = dynamic_cast<OgreTerrainObserver*>(observer);
	delete ogreTerrainObserver;
}

std::pair<bool, Ogre::Vector3> OgreTerrainAdapter::rayIntersects(const Ogre::Ray& ray) const
{
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray, mHoldRadius + mTerrainGroup->getTerrainWorldSize());
	return std::pair<bool, Ogre::Vector3>(result.hit, result.position);
}

void OgreTerrainAdapter::setPageDataProvider(IPageDataProvider* pageDataProvider)
{
	mPageDataProvider = pageDataProvider;
	// Use our own material generator
	// Initialized here because it needs a IPageDataProvider
	mMaterialGenerator = OGRE_NEW OgreTerrainMaterialGeneratorEmber(*mPageDataProvider, mTerrainGroup->getOrigin().x, mTerrainGroup->getOrigin().z);
	mTerrainGlobalOptions->setDefaultMaterialGenerator(Ogre::TerrainMaterialGeneratorPtr(mMaterialGenerator));
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */

