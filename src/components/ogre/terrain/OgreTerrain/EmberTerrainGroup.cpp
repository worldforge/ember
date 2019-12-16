/*
 Copyright (C) 2013 Erik Ogenvik


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

#include "EmberTerrainGroup.h"
#include "EmberTerrain.h"
#include "framework/LoggingInstance.h"

#include <OgreRoot.h>

using namespace Ogre;

namespace Ember {
namespace OgreView {
namespace Terrain {

unsigned int EmberTerrainGroup::sLoadingTaskNum;

EmberTerrainGroup::EmberTerrainGroup(Ogre::SceneManager* sm,
									 Ogre::uint16 terrainSize,
									 sigc::signal<void, const Ogre::TRect<Ogre::Real>&>& terrainShownSignal,
									 sigc::signal<void, const Ogre::TRect<Ogre::Real>&>& terrainAreaUpdatedSignal,
									 Ogre::TerrainMaterialGeneratorPtr materialGenerator) :
		Ogre::TerrainGroup(sm, Ogre::Terrain::ALIGN_X_Z, terrainSize + 1, terrainSize),
		mPageDataProvider(nullptr),
		mTerrainShownSignal(terrainShownSignal),
		mTerrainAreaUpdated(terrainAreaUpdatedSignal),
		mMaterialGenerator(materialGenerator) {

	//Setting this to 65 makes the terrain system use much less batches, which is good for modern GPUs.
	mDefaultImportData.minBatchSize = 65;
}

EmberTerrainGroup::~EmberTerrainGroup() {
	while (sLoadingTaskNum > 0) {
		S_LOG_VERBOSE("Sleeping 2 milliseconds while waiting for tasks to complete in EmberTerrainGroup destructor.");
		OGRE_THREAD_SLEEP(2);
		Root::getSingleton().getWorkQueue()->processResponses();
	}

}

void EmberTerrainGroup::loadAllTerrains(bool synchronous) {
	// Just a straight iteration - for the numbers involved not worth
	// keeping a loaded / unloaded list
	for (auto entry : mTerrainSlots) {
		TerrainSlot* slot = entry.second;
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadTerrain(long x, long y, bool synchronous) {
	TerrainSlot* slot = getTerrainSlot(x, y, false);
	if (slot) {
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadEmberTerrainImpl(TerrainSlot* slot, bool synchronous) {
	assert(mPageDataProvider);
	if (!slot->instance) {

		if (slot->def.importData == nullptr) {
			return;
		}

		//Don't assign an instance to the supplied slot until it has been fully prepared in the background (i.e. on "handleResponse).
		//The main reason is that other parts of the system will try to access the non-prepared instance through getTerrain(...), and cause segfaults


		//This slot is temporary and will be deleted in "handleResponse", after transferring the instance to the "main" slot.
		TerrainSlot* newSlot = OGRE_NEW TerrainSlot(slot->x, slot->y);
		newSlot->def = slot->def;

		//Import data is transferred to "newSlot"
		slot->def.importData = nullptr;

		long x = slot->x;
		long y = slot->y;

		//The unloader function tells the page data provider that it should remove any terrain page bridge it has registered.
		std::function<void()> unloader = [=] { mPageDataProvider->removeBridge(IPageDataProvider::OgreIndex(x, y)); };

		// Allocate in main thread so no race conditions
		EmberTerrain* terrain = OGRE_NEW EmberTerrain(unloader, mSceneManager, mTerrainAreaUpdated, mTerrainShownSignal, mMaterialGenerator);
		terrain->setIndex(IPageDataProvider::OgreIndex(x, y));

		terrain->setResourceGroup(mResourceGroup);
		// Use shared pool of buffers
		terrain->setGpuBufferAllocator(&mBufferAllocator);

		newSlot->instance = terrain;

		LoadRequest req{};
		req.slot = newSlot;
		req.origin = this;
		++sLoadingTaskNum;
		Ogre::Root::getSingleton().getWorkQueue()->addRequest(mWorkQueueChannel, WORKQUEUE_LOAD_REQUEST, Ogre::Any(req), 0, synchronous);

	}
}

void EmberTerrainGroup::handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ) {
	--sLoadingTaskNum;

	if (res->getRequest()->getType() == WORKQUEUE_LOAD_REQUEST) {
		LoadRequest lreq = any_cast<LoadRequest>(res->getRequest()->getData());

		if (res->succeeded()) {
			//Transfer the instance from the temporary slot in the request, and delete it afterwards.
			TerrainSlot* newSlot = lreq.slot;
			TerrainSlot* slot = getTerrainSlot(newSlot->x, newSlot->y);
			if (slot) {
				slot->instance = newSlot->instance;
				newSlot->instance = nullptr; //need to set to null, else it gets deleted by the slot's destructor
				OGRE_DELETE newSlot;

				Ogre::Terrain* terrain = slot->instance;
				if (terrain) {
					terrain->setPosition(getTerrainSlotPosition(slot->x, slot->y));

					if (mAutoUpdateLod) {
						terrain->load(-1, false);
					} else {
						terrain->load(0, false);
					}

					for (int i = -1; i <= 1; ++i) {
						for (int j = -1; j <= 1; ++j) {
							if (i != 0 || j != 0) {
								connectNeighbour(slot, i, j);
							}
						}
					}
				}
			} else {
				lreq.slot->freeInstance();
			}
		} else {
			S_LOG_FAILURE("Failed to prepare terrain at " << lreq.slot->x << ", " << lreq.slot->y << ". Error: " << res->getMessages());
			lreq.slot->freeInstance();
		}
	}
}

void EmberTerrainGroup::setPageDataProvider(IPageDataProvider* pageDataProvider) {
	mPageDataProvider = pageDataProvider;
}

}
}
}
