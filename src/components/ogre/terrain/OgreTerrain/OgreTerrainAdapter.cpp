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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "OgreTerrainAdapter.h"

#include "OgreTerrainObserver.h"
#include "OgreTerrainDefiner.h"
#include "components/ogre/TerrainPageDataProvider.h"
#include "OgreTerrainMaterialGeneratorEmber.h"
#include "EmberTerrainGroup.h"
#include "EmberTerrain.h"
#include "CameraFocusedGrid2DPageStrategy.h"

#include <OgreTerrainPaging.h>
#include <OgrePagedWorld.h>

#define EMBER_OGRE_TERRAIN_HALF_RANGE 0x7FFF

namespace Ember {
namespace OgreView {
namespace Terrain {

OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera, int terrainPageSize) :
		mLoadRadius(300),
		mHoldRadius(mLoadRadius * 2),
		mSceneManager(sceneManager),
		mTerrainPageSize(terrainPageSize),
		mMaterialGenerator(std::make_shared<OgreTerrainMaterialGeneratorEmber>()),
		mTerrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>()),
		mPageManager(std::make_unique<Ogre::PageManager>()),
		mTerrainPaging(std::make_unique<Ogre::TerrainPaging>(mPageManager.get())),
		mPagedWorld(nullptr),
		mTerrainPagedWorldSection(nullptr),
		mTerrainGroup(OGRE_NEW EmberTerrainGroup(&sceneManager, terrainPageSize, mTerrainShownSignal, mTerrainAreaUpdated, mMaterialGenerator)),
		mPageDataProvider(nullptr),
		mMaterialProfile(nullptr),
		mPageStrategy(std::make_unique<CameraFocusedGrid2DPageStrategy>(mPageManager.get())),
		mEntity(nullptr) {

	// Other params
	mTerrainGlobalOptions->setSkirtSize(1.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);
	mTerrainGlobalOptions->setCompositeMapSize(512);
	mTerrainGlobalOptions->setDefaultMaterialGenerator(mMaterialGenerator);

	setOgrePageSize(terrainPageSize);

	// Set our own page provider which so far only prevents the page manager trying to load pages from disk
	mPageManager->setPageProvider(&mTerrainPageProvider);

	//This will overwrite the default 2Dgrid strategy with our own, which loads pages close to the camera first.
	mPageManager->addStrategy(mPageStrategy.get());

}

OgreTerrainAdapter::~OgreTerrainAdapter() {
	//Need to remove all cameras, since this isn't done by the PageManager itself.
	for (auto currentCamera : mPageManager->getCameraList()) {
		mPageManager->removeCamera(currentCamera);
	}
	mPageManager->destroyWorld(mPagedWorld);
}

int OgreTerrainAdapter::getPageSize() {
	return mTerrainGroup->getTerrainSize();
}

void OgreTerrainAdapter::setOgrePageSize(int pageSize) {
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->removeAllPages();
	}
	mTerrainGroup->removeAllTerrains();
	mTerrainGroup->setTerrainSize(static_cast<Ogre::uint16>(pageSize + 1));
	mTerrainGroup->setTerrainWorldSize(pageSize);
	Ogre::Vector3 origin;
	origin.x = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.z = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.y = 0;
	mTerrainGroup->setOrigin(origin);

	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->getGridStrategyData()->setOrigin(mTerrainGroup->getOrigin());
		mTerrainPagedWorldSection->getGridStrategyData()->setCellSize(mTerrainGroup->getTerrainWorldSize());
	}
}


void OgreTerrainAdapter::setPageSize(int pageSize) {
	setOgrePageSize(pageSize);

}

void OgreTerrainAdapter::setLoadRadius(Ogre::Real loadRadius) {
	mLoadRadius = loadRadius;
	mHoldRadius = mLoadRadius * 2;
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->setLoadRadius(mLoadRadius);
		mTerrainPagedWorldSection->setHoldRadius(mHoldRadius);
	}
}

bool OgreTerrainAdapter::getHeightAt(Ogre::Real x, Ogre::Real z, float& height) {
	Ogre::Terrain* foundTerrain = nullptr;
	height = mTerrainGroup->getHeightAtWorldPosition(x, 0.0, z, &foundTerrain);
	return foundTerrain != nullptr;
}

void OgreTerrainAdapter::setCamera(Ogre::Camera* camera) {
	if (!mPageManager->hasCamera(camera)) {
		// For now, we want only one camera to affect paging
		for (auto currentCamera : mPageManager->getCameraList()) {
			mPageManager->removeCamera(currentCamera);
		}
		mPageManager->addCamera(camera);
	}
}

void OgreTerrainAdapter::loadScene() {
	mPagedWorld = mPageManager->createWorld();
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, mLoadRadius, mHoldRadius,
																   -EMBER_OGRE_TERRAIN_HALF_RANGE, -EMBER_OGRE_TERRAIN_HALF_RANGE,
																   EMBER_OGRE_TERRAIN_HALF_RANGE, EMBER_OGRE_TERRAIN_HALF_RANGE,
																   "", 0);
	mTerrainPagedWorldSection->setDefiner(OGRE_NEW OgreTerrainDefiner(*mPageDataProvider)); //ownership is passed
}

void OgreTerrainAdapter::reset() {
	mTerrainGroup->removeAllTerrains();

	//If we've created a world section we've moved ownership of the terrain group there.
	if (mTerrainPagedWorldSection) {
		mPagedWorld->destroySection(mTerrainPagedWorldSection);
		mTerrainPagedWorldSection = nullptr;
		mTerrainGroup = OGRE_NEW EmberTerrainGroup(&mSceneManager, mTerrainPageSize, mTerrainShownSignal, mTerrainAreaUpdated, mMaterialGenerator);
		setOgrePageSize(mTerrainPageSize);
		mTerrainGroup->setPageDataProvider(mPageDataProvider);
	} else {
		OGRE_DELETE mTerrainGroup;
	}
//	if (mPagedWorld) {
//		mPageManager->destroyWorld(mPagedWorld);
//		mPagedWorld = nullptr;
//	}
}


void OgreTerrainAdapter::reloadAllPages() {
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->removeAllPages();
	}
}

void OgreTerrainAdapter::reloadPage(const TerrainIndex& index) {
	if (mTerrainPagedWorldSection) {
		Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page && !page->isDerivedDataUpdateInProgress()) {
			page->dirty();
			page->update();
		}
	}
}

void OgreTerrainAdapter::reloadPageMaterial(const TerrainIndex& index) {
	if (mTerrainGroup) {
		auto* page = dynamic_cast<EmberTerrain*>(mTerrainGroup->getTerrain(index.first, index.second));
		if (page) {
			//We must tell the page what area needs updating. For now we'll update the whole page.
			//But we should really look into only updating the area that has changed.
			page->_dirtyCompositeMapRect(Ogre::Rect(0, 0, page->getWorldSize(), page->getWorldSize()));
			page->regenerateMaterial();
			page->updateCompositeMap();
		}
	}
}

std::string OgreTerrainAdapter::getDebugInfo() {
	return "Not available";
}

std::unique_ptr<ITerrainObserver> OgreTerrainAdapter::createObserver() {
	return std::make_unique<OgreTerrainObserver>(mTerrainAreaUpdated);
}

std::pair<EmberEntity*, Ogre::Vector3> OgreTerrainAdapter::rayIntersects(const Ogre::Ray& ray) const {
	Ogre::TerrainGroup::RayResult result = mTerrainGroup->rayIntersects(ray, mHoldRadius + mTerrainGroup->getTerrainWorldSize());
	return std::make_pair(result.hit ? mEntity : nullptr, result.position);
}

void OgreTerrainAdapter::setPageDataProvider(IPageDataProvider* pageDataProvider) {
	mPageDataProvider = pageDataProvider;
	auto materialGenerator = mTerrainGlobalOptions->getDefaultMaterialGenerator();
	mMaterialProfile = std::make_unique<EmberTerrainProfile>(*mPageDataProvider, materialGenerator.get());
	materialGenerator->setActiveProfile(mMaterialProfile.get());
	mTerrainGroup->setPageDataProvider(pageDataProvider);
}

sigc::connection OgreTerrainAdapter::bindTerrainShown(sigc::slot<void, const Ogre::TRect<Ogre::Real>>& signal) {
	return mTerrainShownSignal.connect(signal);
}

void OgreTerrainAdapter::setTerrainEntity(EmberEntity* entity) {
	mEntity = entity;
}


} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */

