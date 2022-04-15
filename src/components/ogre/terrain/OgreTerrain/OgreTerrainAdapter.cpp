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
#include "framework/MainLoopController.h"

#include <OgreTerrainGroup.h>
#include <OgreTerrainAutoUpdateLod.h>
#include <OgreViewport.h>
#include <OgreTerrainQuadTreeNode.h>

namespace {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

/**
 * This function is copied from Ogre and licensed under the MIT license.
 * Copyright (c) 2000-2014 Torus Knot Software Ltd
 */
int traverseTreeByDistance(Ogre::TerrainQuadTreeNode* node,
						   const Ogre::Camera* cam,
						   Ogre::Real cFactor,
						   const Ogre::Real holdDistance) {
	if (!node->isLeaf()) {
		int tmp = -1;
		for (int i = 0; i < 4; ++i) {
			int ret = traverseTreeByDistance(node->getChild(i), cam, cFactor, holdDistance);
			if (ret != -1) {
				if (tmp == -1 || ret < tmp)
					tmp = ret;
			}
		}

		if (tmp != -1)
			return tmp;
	}

	auto localPos = cam->getDerivedPosition() - node->getLocalCentre() - node->getTerrain()->getPosition();
	// distance to tile centre
	auto dist = localPos.length();
	// deduct half the radius of the box, assume that on average the
	// worst case is best approximated by this
	dist -= (node->getBoundingRadius() * 0.5f);

	// For each LOD, the distance at which the LOD will transition *downwards*
	// is given by
	// distTransition = maxDelta * cFactor;
	for (std::uint16_t lodLevel = 0; lodLevel < node->getLodCount(); ++lodLevel) {
		// If we have no parent, and this is the lowest LOD, we always render
		// this is the 'last resort' so to speak, we always enoucnter this last
		if (lodLevel + 1 == node->getLodCount() && !node->getParent())
			return lodLevel + node->getBaseLod();
		else {
			// Calculate or reuse transition distance
			Ogre::Real distTransition;
			if (Ogre::Math::RealEqual(cFactor, node->getLodLevel(lodLevel)->lastCFactor))
				distTransition = node->getLodLevel(lodLevel)->lastTransitionDist;
			else
				distTransition = node->getLodLevel(lodLevel)->maxHeightDelta * cFactor;

			if ((dist - holdDistance) < distTransition)
				return lodLevel + node->getBaseLod();
		}
	}

	return -1;
}

#pragma clang diagnostic pop

}

namespace Ember {
namespace OgreView {
namespace Terrain {

OgreTerrainAdapter::OgreTerrainAdapter(Ogre::SceneManager& sceneManager, int terrainPageSize) :
		mLoadRadius(300),
		mHoldRadius(mLoadRadius * 2),
		mCamera(nullptr),
		mMaterialGenerator(std::make_shared<OgreTerrainMaterialGeneratorEmber>()),
		mTerrainGlobalOptions(std::make_unique<Ogre::TerrainGlobalOptions>()),
		mTerrainGroup(OGRE_NEW Ogre::TerrainGroup(&sceneManager,
												  Ogre::Terrain::ALIGN_X_Z,
												  terrainPageSize + 1,
												  (Ogre::Real) terrainPageSize)),
		mPageDataProvider(nullptr),
		mMaterialProfile(nullptr),
		mEntity(nullptr) {

	//Set an auto update lod instance just to make the terrain engine not do LOD calculations on the main thread.
	//We'll do that ourselves in the "updateLods()" method.
	mTerrainGroup->setAutoUpdateLod(Ogre::TerrainAutoUpdateLodFactory::getAutoUpdateLod(Ogre::BY_DISTANCE));

	// Other params
	mTerrainGlobalOptions->setSkirtSize(1.0f);
	mTerrainGlobalOptions->setCastsDynamicShadows(true);
	mTerrainGlobalOptions->setMaxPixelError(8);
	mTerrainGlobalOptions->setCompositeMapSize(512);
	mTerrainGlobalOptions->setDefaultMaterialGenerator(mMaterialGenerator);

	setOgrePageSize(terrainPageSize);


}

OgreTerrainAdapter::~OgreTerrainAdapter() = default;

bool OgreTerrainAdapter::frameRenderingQueued(const Ogre::FrameEvent& evt) {
	updateLods();
	return true;
}

void OgreTerrainAdapter::updateLods() {
	if (mCamera && mCamera->getViewport()) {


		auto A = 1.0f / Ogre::Math::Tan(mCamera->getFOVy() * 0.5f);
		auto pixelError = 3.0 * mCamera->_getLodBiasInverse();
		auto T = 2.0f * pixelError / (Ogre::Real) mCamera->getViewport()->getActualHeight();

		auto cFactor = Ogre::Real(A / T);


		for (auto terrainSlot: mTerrainGroup->getTerrainSlots()) {
			auto terrain = terrainSlot.second->instance;
			if (terrain && terrain->isLoaded() && !terrain->isDerivedDataUpdateInProgress()) {
				int maxLod = traverseTreeByDistance(terrain->getQuadTree(), mCamera, cFactor, 1000.0f);
				if (maxLod >= 0)
					terrain->load(maxLod, false);
			}
		}
	}
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
	mCamera = camera;
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

void OgreTerrainAdapter::updateExistingTerrain(const Ogre::Terrain* terrainPtr, const TerrainIndex& index, std::vector<float> heightData) {
	//Note! We don't access the instance through the "terrainPtr", that's only here to check that the instance hasn't changed in the terrainGroup.
	auto terrain = mTerrainGroup->getTerrain(index.first, index.second);
	//Check that the terrain still exists and it's the same instance as previously (i.e. it hasn't been removed and loaded again).
	if (terrain && terrain == terrainPtr) {
		//Check that there's no background process or that it's being prepared for creation.
		if (terrain->isDerivedDataUpdateInProgress() || !terrain->isLoaded()) {
			MainLoopController::getSingleton().getEventService().runOnMainThreadDelayed(
					[=, heightData = std::move(heightData)] { updateExistingTerrain(terrainPtr, index, heightData); },
					std::chrono::milliseconds(1),
					mActiveMarker);
		} else {
			//Copy height data
			std::memcpy(terrain->getHeightData(), heightData.data(), sizeof(float) * terrain->getSize() * terrain->getSize());
			terrain->dirty();
			terrain->update(false);
		}
	}
}

void OgreTerrainAdapter::showPage(std::shared_ptr<TerrainPageGeometry> geometry) {
	TimedLog log("showPage " + std::to_string(geometry->mPageIndex.first) + ":" + std::to_string(geometry->mPageIndex.second));
	std::vector<float> heightData(mTerrainGroup->getTerrainSize() * mTerrainGroup->getTerrainSize());
	geometry->updateOgreHeightData(heightData.data());
	log.report("updated ogre data");
	auto terrain = mTerrainGroup->getTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second);
	if (terrain) {
		updateExistingTerrain(terrain, geometry->getPage()->getWFIndex(), std::move(heightData));
	} else {
		Ogre::Terrain::LayerInstanceList layers;
		mTerrainGroup->defineTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second, heightData.data(), &layers);
		log.report("defined terrain");
		mTerrainGroup->loadTerrain(geometry->getPage()->getWFIndex().first, geometry->getPage()->getWFIndex().second, false);
		log.report("loaded terrain");
	}

}

void OgreTerrainAdapter::removePage(const TerrainIndex& index) {
	S_LOG_VERBOSE("removing page " << index.first << ":" << index.second);
	auto terrain = mTerrainGroup->getTerrain(index.first, index.second);
	//We can't delete the page if it's either 1) not yet fully created 2) being altered in a background thread.
	//So we check for both of these, and if that's the case we'll delay removal.
	if ((terrain && (terrain->isDerivedDataUpdateInProgress() || !terrain->isLoaded()))) {
		MainLoopController::getSingleton().getEventService().runOnMainThreadDelayed(
				[=] { removePage(index); },
				std::chrono::milliseconds(1),
				mActiveMarker);
	} else {
		mTerrainGroup->removeTerrain(index.first, index.second);
	}
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

