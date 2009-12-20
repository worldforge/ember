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

#include "TerrainModChangeTask.h"
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

TerrainModChangeTask::TerrainModChangeTask(Mercator::Terrain& terrain, TerrainMod& terrainMod, TerrainManager& manager, TerrainModMap& terrainMods, Mercator::TerrainMod* existingMod) :
	mTerrain(terrain), mTerrainMod(terrainMod), mManager(manager), mTerrainMods(terrainMods), mExistingMod(existingMod), mAppliedMod(0)
{

}

TerrainModChangeTask::~TerrainModChangeTask()
{
}

void TerrainModChangeTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	if (mExistingMod) {
		mUpdatedPositions.push_back(TerrainPosition(mExistingMod->bbox().getCenter().x(), mExistingMod->bbox().getCenter().y()));
		// Use the pointer returned from addMod() to remove it
		mTerrain.removeMod(mExistingMod);
	}

	mAppliedMod = mTerrain.addMod(*mTerrainMod.getMercatorMod());
	if (mAppliedMod) {
		mUpdatedPositions.push_back(TerrainPosition(mAppliedMod->bbox().getCenter().x(), mAppliedMod->bbox().getCenter().y()));
	}

}

void TerrainModChangeTask::executeTaskInMainThread()
{
	mTerrainMods.erase(mTerrainMod.getErisMod()->getEntity()->getId());

	if (mAppliedMod) {
		mTerrainMods.insert(TerrainModMap::value_type(mTerrainMod.getErisMod()->getEntity()->getId(), mAppliedMod));
	}
	mManager.reloadTerrain(mUpdatedPositions);
}
}

}
