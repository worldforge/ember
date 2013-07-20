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

#include <OgreSceneManager.h>
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>
#include <OgrePageManager.h>

#define TERRAIN_SIZE 257

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
//TODO SK: provide proper arguments to terrain group, move to constants/configuration parameters
OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, Ogre::Camera* mainCamera) :
		mSceneManager(sceneManager),
				mPageManager(OGRE_NEW Ogre::PageManager()),
				mTerrainPaging(OGRE_NEW Ogre::TerrainPaging(mPageManager)),
				mPagedWorld(nullptr),
				mTerrainPagedWorldSection(nullptr),
				mTerrainGlobalOptions(OGRE_NEW Ogre::TerrainGlobalOptions()),
				mTerrainGroup(OGRE_NEW Ogre::TerrainGroup(&sceneManager, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, Ogre::Real(TERRAIN_SIZE)))

{
	// Set our own page provider which so far only prevents the page manager trying to load pages from disk
	mPageManager->setPageProvider(&mTerrainPageProvider);
	mPageManager->addCamera(mainCamera);
	Ogre::Terrain::ImportData& defaultImportData = mTerrainGroup->getDefaultImportSettings();
	Ogre::Vector3 origin;

	origin.x = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.z = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.y = 0;
	mTerrainGroup->setOrigin(origin);
	mPageManager->setDebugDisplayLevel(2);
	defaultImportData.layerList.resize(1);
	defaultImportData.layerList[0].worldSize = 9;
	defaultImportData.layerList[0].textureNames.push_back("3d_objects/environment/textures/ground_grass/D.png");
	defaultImportData.layerList[0].textureNames.push_back("3d_objects/environment/textures/ground_grass/N.png");
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
	mPagedWorld = mPageManager->createWorld();
	const Ogre::Real LOAD_RADIUS = 128;
	const Ogre::Real HOLD_RADIUS = 200;
	mTerrainPagedWorldSection = mTerrainPaging->createWorldSection(mPagedWorld, mTerrainGroup, LOAD_RADIUS, HOLD_RADIUS);
	mTerrainPagedWorldSection->setDefiner(new FlatTerrainDefiner());
}

void OgreTerrainAdapter::reset()
{
	mTerrainGroup->removeAllTerrains();
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
	mTerrainGroup->loadAllTerrains(false);
}

void OgreTerrainAdapter::reloadPage(unsigned int x, unsigned int z)
{
	mTerrainGroup->loadTerrain(x, z);
}

void OgreTerrainAdapter::loadFirstPage()
{

}

std::string OgreTerrainAdapter::getDebugInfo()
{
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


} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */

