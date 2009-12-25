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

TerrainModRemoveTask::TerrainModRemoveTask(Mercator::Terrain& terrain, Mercator::TerrainMod* managerLocalTerrainMod, const std::string& entityId, TerrainManager& manager, TerrainModMap& terrainMods) :
		TerrainModTaskBase::TerrainModTaskBase(terrain, managerLocalTerrainMod, entityId, manager, terrainMods)
{

}

TerrainModRemoveTask::~TerrainModRemoveTask()
{
}

void TerrainModRemoveTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	if (mManagerLocalTerrainMod) {
		mTerrain.removeMod(mManagerLocalTerrainMod);
		mUpdatedPositions.push_back(TerrainPosition(mManagerLocalTerrainMod->bbox().getCenter().x(), mManagerLocalTerrainMod->bbox().getCenter().y()));
		delete mManagerLocalTerrainMod;
	}
}

void TerrainModRemoveTask::executeTaskInMainThread()
{
	if (mUpdatedPositions.size()) {

		mManager.reloadTerrain(mUpdatedPositions);
	}
}

}

}
