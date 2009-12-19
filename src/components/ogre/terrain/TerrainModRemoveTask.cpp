/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "TerrainModRemoveTask.h"
#include "TerrainManager.h"
#include "TerrainMod.h"
#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Eris/TerrainMod.h>
#include <Eris/Entity.h>

namespace EmberOgre
{

namespace Terrain
{

TerrainModRemoveTask::TerrainModRemoveTask(Mercator::Terrain& terrain, Mercator::TerrainMod* existingMod, TerrainManager& manager, TerrainModMap terrainMods, const std::string& entityId) :
	mTerrain(terrain), mExistingMod(existingMod), mManager(manager), mTerrainMods(terrainMods), mEntityId(entityId)
{

}

TerrainModRemoveTask::~TerrainModRemoveTask()
{
}

void TerrainModRemoveTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	if (mExistingMod) {
		mTerrain.removeMod(mExistingMod);
	}
}

void TerrainModRemoveTask::executeTaskInMainThread()
{
	if (mExistingMod) {

		// Remove this mod from our list
		mTerrainMods.erase(mEntityId);

		std::vector<TerrainPosition> updatedPositions;
		updatedPositions.push_back(TerrainPosition(mExistingMod->bbox().getCenter().x(), mExistingMod->bbox().getCenter().y()));
		mManager.reloadTerrain(updatedPositions);
	}
}

}

}
