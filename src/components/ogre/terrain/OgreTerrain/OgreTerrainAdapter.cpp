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
#include "components/ogre/terrain/TerrainPageGeometry.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "components/ogre/TerrainPageDataProvider.h"
#include "OgreTerrainMaterialGeneratorEmber.h"
#include "framework/TimedLog.h"

#include <OgreTerrainGroup.h>


namespace Ember {
namespace OgreView {
namespace Terrain {

OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, int terrainPageSize) :
		mLoadRadius(300),
		mHoldRadius(mLoadRadius * 2),
		mMaterialGenerator(std::make_shared<OgreTerrainMaterialGeneratorEmber>()),
		mTerrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>()),
		mTerrainGroup(OGRE_NEW Ogre::TerrainGroup(&sceneManager,
												  Ogre::Terrain::ALIGN_X_Z,
												  terrainPageSize + 1,
												  (Ogre::Real) terrainPageSize)),
		mPageDataProvider(nullptr),
		mMaterialProfile(nullptr),
		mEntity(nullptr) {

	// Other params
	mTerrainGlobalOptions->setSkirtSize(1.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);
	mTerrainGlobalOptions->setCompositeMapSize(512);
	mTerrainGlobalOptions->setDefaultMaterialGenerator(mMaterialGenerator);

	setOgrePageSize(terrainPageSize);


}

OgreTerrainAdapter::~OgreTerrainAdapter() {

}

int OgreTerrainAdapter::getPageSize() {
	return mTerrainGroup->getTerrainSize();
}

void OgreTerrainAdapter::setOgrePageSize(int pageSize) {
	mTerrainGroup->removeAllTerrains();
	mTerrainGroup->setTerrainSize(static_cast<Ogre::uint16>(pageSize + 1));
	mTerrainGroup->setTerrainWorldSize((Ogre::Real) pageSize);
	Ogre::Vector3 origin;
	origin.x = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.z = mTerrainGroup->getTerrainWorldSize() / 2;
	origin.y = 0;
	mTerrainGroup->setOrigin(origin);

}


void OgreTerrainAdapter::setPageSize(int pageSize) {
	setOgrePageSize(pageSize);

}

void OgreTerrainAdapter::setLoadRadius(Ogre::Real loadRadius) {
	mLoadRadius = loadRadius;
	mHoldRadius = mLoadRadius * 2;
}

bool OgreTerrainAdapter::getHeightAt(Ogre::Real x, Ogre::Real z, float& height) {
	Ogre::Terrain* foundTerrain = nullptr;
	height = mTerrainGroup->getHeightAtWorldPosition(x, 0.0, z, &foundTerrain);
	return foundTerrain != nullptr;
}

void OgreTerrainAdapter::setCamera(Ogre::Camera* camera) {

}

void OgreTerrainAdapter::loadScene() {

}

void OgreTerrainAdapter::reset() {
	if (mTerrainGroup) {
		mTerrainGroup->removeAllTerrains();
		mTerrainGroup = nullptr;
	}
}


void OgreTerrainAdapter::reloadAllPages() {
//	if (mTerrainPagedWorldSection) {
//		mTerrainPagedWorldSection->removeAllPages();
//	}
}

void OgreTerrainAdapter::showPage(std::shared_ptr<TerrainPageGeometry> geometry) {
	TimedLog log("showPage");
	std::vector<float> heightData(mTerrainGroup->getTerrainSize() * mTerrainGroup->getTerrainSize());
	geometry->updateOgreHeightData(heightData.data());
	log.report("updated ogre data");
	auto terrain = mTerrainGroup->getTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second);
	if (terrain) {
		//Copy height data
		std::memcpy(terrain->getHeightData(), heightData.data(), sizeof(float) * terrain->getSize() * terrain->getSize());
		terrain->dirty();
		terrain->update(false);
	} else {
		Ogre::Terrain::LayerInstanceList layers;
		mTerrainGroup->defineTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second, heightData.data(), &layers);
		log.report("defined terrain");
		mTerrainGroup->loadTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second, false);
		log.report("loaded terrain");
	}

}

void OgreTerrainAdapter::removePage(const TerrainIndex& index) {
	mTerrainGroup->removeTerrain(index.first, index.second);
}



void OgreTerrainAdapter::reloadPage(const TerrainIndex& index) {
	Ogre::Terrain* page = mTerrainGroup->getTerrain(index.first, index.second);
	if (page && page->isLoaded() && !page->isDerivedDataUpdateInProgress()) {
		page->dirty();
		page->update(false);
	}
}

void OgreTerrainAdapter::reloadPageMaterial(const TerrainIndex& index) {
	if (mTerrainGroup) {
		auto* page = mTerrainGroup->getTerrain(index.first, index.second);
		if (page && page->isLoaded()) {
			//We must tell the page what area needs updating. For now we'll update the whole page.
			//But we should really look into only updating the area that has changed.
			page->_dirtyCompositeMapRect(Ogre::Rect(0, 0, (int) page->getWorldSize(), (int) page->getWorldSize()));
			mMaterialProfile->generate(page);
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
	mMaterialProfile = std::make_unique<EmberTerrainProfile>(*mPageDataProvider, *mTerrainGroup, materialGenerator.get(), mTerrainShownSignal);
	materialGenerator->setActiveProfile(mMaterialProfile.get());
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

