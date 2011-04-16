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
#include "TerrainHandler.h"
#include "TerrainMod.h"

#include "framework/LoggingInstance.h"

#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Eris/TerrainModTranslator.h>
#include <Eris/Entity.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainModRemoveTask::TerrainModRemoveTask(Mercator::Terrain& terrain, const std::string& entityId, TerrainHandler& handler, TerrainModMap& terrainMods) :
	TerrainModTaskBase(terrain, entityId, handler, terrainMods)
{
}

void TerrainModRemoveTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	TerrainModMap::iterator I = mTerrainMods.find(mEntityId);
	if (I != mTerrainMods.end()) {
		Eris::TerrainModTranslator* terrainMod = I->second;
		mTerrainMods.erase(I);

		if (terrainMod->getModifier()) {
			mTerrain.removeMod(terrainMod->getModifier());
			mUpdatedAreas.push_back(terrainMod->getModifier()->bbox());
		}
		delete terrainMod;

	} else {
		S_LOG_WARNING("Got a delete signal for a terrain mod which isn't registered with the terrain handler. This shouldn't happen.");
	}
}

void TerrainModRemoveTask::executeTaskInMainThread()
{
	if (mUpdatedAreas.size()) {
		mHandler.reloadTerrain(mUpdatedAreas);
	}
}

}

}
}
