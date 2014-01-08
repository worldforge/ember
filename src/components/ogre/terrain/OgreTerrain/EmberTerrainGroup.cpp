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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberTerrainGroup.h"
#include "EmberTerrain.h"
#include "components/ogre/TerrainPageDataProvider.h"
#include "framework/LoggingInstance.h"

#include <OgreRoot.h>

#include <functional>

using namespace Ogre;

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

unsigned int EmberTerrainGroup::sLoadingTaskNum;

EmberTerrainGroup::EmberTerrainGroup(Ogre::SceneManager* sm, Ogre::Terrain::Alignment align, Ogre::uint16 terrainSize, Ogre::Real terrainWorldSize, sigc::signal<void, const Ogre::TRect<Ogre::Real>>& terrainShownSignal) :
		Ogre::TerrainGroup(sm, align, terrainSize, terrainWorldSize), mPageDataProvider(nullptr), mTerrainShownSignal(terrainShownSignal)
{
}

EmberTerrainGroup::~EmberTerrainGroup()
{
	while (sLoadingTaskNum > 0) {
		OGRE_THREAD_SLEEP(2);
		Root::getSingleton().getWorkQueue()->processResponses();
	}

}

void EmberTerrainGroup::loadAllTerrains(bool synchronous /*= false*/)
{
	// Just a straight iteration - for the numbers involved not worth
	// keeping a loaded / unloaded list
	for (auto entry : mTerrainSlots) {
		TerrainSlot* slot = entry.second;
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadTerrain(long x, long y, bool synchronous /*= false*/)
{
	TerrainSlot* slot = getTerrainSlot(x, y, false);
	if (slot) {
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadEmberTerrainImpl(TerrainSlot* slot, bool synchronous)
{
	assert(mPageDataProvider);
	if (!slot->instance) {

		if (slot->def.importData == nullptr) {
			return;
		}

		long x = slot->x;
		long y = slot->y;

		//The unloader function tells the page data provider that it should remove any terrain page bridge it has registered.
		std::function < void() > unloader = [=] {mPageDataProvider->removeBridge(IPageDataProvider::OgreIndex(x, y));};

		// Allocate in main thread so no race conditions
		EmberTerrain* terrain = OGRE_NEW EmberTerrain(unloader, mSceneManager, EventTerrainAreaUpdated, mTerrainShownSignal);
		terrain->setIndex(IPageDataProvider::OgreIndex(x, y));

		slot->instance = terrain;
		slot->instance->setResourceGroup(mResourceGroup);
		// Use shared pool of buffers
		slot->instance->setGpuBufferAllocator(&mBufferAllocator);

		LoadRequest req;
		req.slot = slot;
		req.origin = this;
		++sLoadingTaskNum;
		Ogre::Root::getSingleton().getWorkQueue()->addRequest(mWorkQueueChannel, WORKQUEUE_LOAD_REQUEST, Ogre::Any(req), 0, synchronous);

	}
}

void EmberTerrainGroup::handleResponse(const WorkQueue::Response* res, const WorkQueue* srcQ)
{
	--sLoadingTaskNum;

	LoadRequest lreq = any_cast<LoadRequest>(res->getRequest()->getData());

	if (res->succeeded()) {
		TerrainSlot* slot = lreq.slot;
		Ogre::Terrain* terrain = slot->instance;
		if (terrain) {
			terrain->setPosition(getTerrainSlotPosition(slot->x, slot->y));

			if (mAutoUpdateLod) {
				terrain->load(-1, false);
			} else {
				terrain->load(0, true);
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
		S_LOG_FAILURE("Failed to prepare terrain at " << lreq.slot->x << ", " << lreq.slot->y << ". Error: " << res->getMessages());
		lreq.slot->freeInstance();
	}
}

void EmberTerrainGroup::setPageDataProvider(IPageDataProvider* pageDataProvider)
{
	mPageDataProvider = pageDataProvider;
}

}
}
}
