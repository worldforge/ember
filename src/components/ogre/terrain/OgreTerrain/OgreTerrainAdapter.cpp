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
#include "components/ogre/TerrainPageDataProvider.h"
#include "OgreTerrainMaterialGeneratorEmber.h"
#include "EmberTerrainGroup.h"
#include "EmberTerrain.h"
#include "CameraFocusedGrid2DPageStrategy.h"

#include <OgreTerrainPaging.h>
#include <OgrePagedWorld.h>

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
		mMaterialGenerator(OGRE_NEW OgreTerrainMaterialGeneratorEmber()),
		mTerrainGlobalOptions(OGRE_NEW Ogre::TerrainGlobalOptions()),
		mPageManager(OGRE_NEW Ogre::PageManager()),
		mTerrainPaging(OGRE_NEW Ogre::TerrainPaging(mPageManager)),
		mPagedWorld(nullptr),
		mTerrainPagedWorldSection(nullptr),
		mTerrainGroup(OGRE_NEW EmberTerrainGroup(&sceneManager, terrainPageSize, mTerrainShownSignal, mMaterialGenerator)),
		mPageDataProvider(nullptr),
		mMaterialProfile(nullptr),
		mPageStrategy(OGRE_NEW CameraFocusedGrid2DPageStrategy(mPageManager))
{

	// Other params
	mTerrainGlobalOptions->setSkirtSize(1.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);
	mTerrainGlobalOptions->setCompositeMapSize(512);
	mTerrainGlobalOptions->setDefaultMaterialGenerator(mMaterialGenerator);

	setPageSize(terrainPageSize);

	// Set our own page provider which so far only prevents the page manager trying to load pages from disk
	mPageManager->setPageProvider(&mTerrainPageProvider);

	//This will overwrite the default 2Dgrid strategy with our own, which loads pages close to the camera first.
	mPageManager->addStrategy(mPageStrategy);

}

OgreTerrainAdapter::~OgreTerrainAdapter()
{
	OGRE_DELETE mTerrainPaging;
	mPageManager->destroyWorld(mPagedWorld);
	OGRE_DELETE mPageManager;
	OGRE_DELETE mTerrainGlobalOptions;
	OGRE_DELETE mPageStrategy;
	OGRE_DELETE mMaterialProfile;
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

	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->getGridStrategyData()->setOrigin(mTerrainGroup->getOrigin());
		mTerrainPagedWorldSection->getGridStrategyData()->setCellSize(mTerrainGroup->getTerrainWorldSize());
	}
}

void OgreTerrainAdapter::setLoadRadius(Ogre::Real loadRadius)
{
	mLoadRadius = loadRadius;
	mHoldRadius = mLoadRadius * 2;
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->setLoadRadius(mLoadRadius);
		mTerrainPagedWorldSection->setHoldRadius(mHoldRadius);
	}
}

Ogre::Real OgreTerrainAdapter::getHeightAt(Ogre::Real x, Ogre::Real z)
{
	Ogre::Terrain* foundTerrain = nullptr;
	float height = mTerrainGroup->getHeightAtWorldPosition(x, 0.0, z, &foundTerrain);
	assert(foundTerrain);
	return Ogre::Real(height);
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

void OgreTerrainAdapter::loadScene()
{
	mPagedWorld = mPageManager->createWorld();
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, mLoadRadius, mHoldRadius, -EMBER_OGRE_TERRAIN_HALF_RANGE, -EMBER_OGRE_TERRAIN_HALF_RANGE, EMBER_OGRE_TERRAIN_HALF_RANGE, EMBER_OGRE_TERRAIN_HALF_RANGE, "", 0);
	mTerrainPagedWorldSection->setDefiner(new OgreTerrainDefiner(*mPageDataProvider));
}

void OgreTerrainAdapter::reset()
{
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->removeAllPages();
	}
}


void OgreTerrainAdapter::reloadAllPages()
{
	if (mTerrainPagedWorldSection) {
		mTerrainPagedWorldSection->removeAllPages();
	}
}

void OgreTerrainAdapter::reloadPage(const TerrainIndex& index)
{
	if (mTerrainPagedWorldSection) {
		Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page) {
			page->dirty();
			page->update();
		}
	}
}

void OgreTerrainAdapter::reloadPageMaterial(const TerrainIndex& index)
{
	if (mTerrainGroup) {
		EmberTerrain* page = static_cast<EmberTerrain*>(mTerrainGroup->getTerrain(index.first, index.second));
		if (page) {
			//We must tell the page what area needs updating. For now we'll update the whole page.
			//But we should really look into only updating the area that has changed.
			page->_dirtyCompositeMapRect(Ogre::Rect(0, 0, page->getSize(), page->getSize()));
			page->regenerateMaterial();
			page->updateCompositeMap();
		}
	}
}

std::string OgreTerrainAdapter::getDebugInfo()
{
	return "Not available";
}

ITerrainObserver* OgreTerrainAdapter::createObserver()
{
	return new OgreTerrainObserver(mTerrainGroup->EventTerrainAreaUpdated);
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
	auto materialGenerator = mTerrainGlobalOptions->getDefaultMaterialGenerator();
	mMaterialProfile = OGRE_NEW EmberTerrainProfile(*mPageDataProvider, materialGenerator.get());
	materialGenerator->setActiveProfile(mMaterialProfile);
	mTerrainGroup->setPageDataProvider(pageDataProvider);
}

sigc::connection OgreTerrainAdapter::bindTerrainShown(sigc::slot<void, const Ogre::TRect<Ogre::Real>>& signal)
{
	return mTerrainShownSignal.connect(signal);
}


} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */

