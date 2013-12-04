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

#include <OgreRoot.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

EmberTerrainGroup::	EmberTerrainGroup(Ogre::SceneManager* sm, Ogre::Terrain::Alignment align, Ogre::uint16 terrainSize,
		Ogre::Real terrainWorldSize)
: Ogre::TerrainGroup(sm, align, terrainSize, terrainWorldSize)
{
}

EmberTerrainGroup::~EmberTerrainGroup()
{
}

void EmberTerrainGroup::loadAllTerrains(bool synchronous /*= false*/)
{
	// Just a straight iteration - for the numbers involved not worth
	// keeping a loaded / unloaded list
	for (TerrainSlotMap::iterator i = mTerrainSlots.begin(); i != mTerrainSlots.end(); ++i)
	{
		TerrainSlot* slot = i->second;
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadTerrain(long x, long y, bool synchronous /*= false*/)
{
	TerrainSlot* slot = getTerrainSlot(x, y, false);
	if (slot)
	{
		loadEmberTerrainImpl(slot, synchronous);
	}

}

void EmberTerrainGroup::loadEmberTerrainImpl(TerrainSlot* slot, bool synchronous)
{
	if (!slot->instance &&
		(!slot->def.filename.empty() || slot->def.importData))
	{
		// Allocate in main thread so no race conditions
		slot->instance = OGRE_NEW EmberTerrain(mSceneManager);
		slot->instance->setResourceGroup(mResourceGroup);
		// Use shared pool of buffers
		slot->instance->setGpuBufferAllocator(&mBufferAllocator);

		LoadRequest req;
		req.slot = slot;
		req.origin = this;
		++LoadRequest::loadingTaskNum;
		Ogre::Root::getSingleton().getWorkQueue()->addRequest(
			mWorkQueueChannel, WORKQUEUE_LOAD_REQUEST,
			Ogre::Any(req), 0, synchronous);

	}
}

}
}
}
