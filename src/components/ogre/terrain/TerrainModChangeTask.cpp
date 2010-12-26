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
#include "TerrainHandler.h"
#include "TerrainMod.h"

#include "framework/LoggingInstance.h"

#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Eris/TerrainMod.h>
#include <Eris/Entity.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainModChangeTask::TerrainModChangeTask(Mercator::Terrain& terrain, const TerrainMod& terrainMod, TerrainHandler& handler, TerrainModMap& terrainMods) :
		TerrainModTaskBase::TerrainModTaskBase(terrain, terrainMod.getEntityId(), handler, terrainMods), mModData(terrainMod.getAtlasData()), mPosition(terrainMod.getEntity().getPredictedPos()), mOrientation(terrainMod.getEntity().getOrientation())
{

}

void TerrainModChangeTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	TerrainModMap::iterator I = mTerrainMods.find(mEntityId);
	if (I != mTerrainMods.end()) {
		Eris::InnerTerrainMod* terrainMod = I->second;
		Mercator::TerrainMod* oldMercTerrainMod = terrainMod->getModifier();
		if (mModData.isMap()) {
			Atlas::Message::MapType mapData = mModData.asMap();
			bool success = terrainMod->parseData(mPosition, mOrientation, mapData);
			if (success && terrainMod->getModifier()) {
				Mercator::Terrain::Rect oldRect = mTerrain.updateMod(terrainMod->getModifier());
				if (oldRect.isValid()) {
					mUpdatedAreas.push_back(oldRect);
				}
				if (terrainMod->getModifier()->bbox() != oldRect) {
					mUpdatedAreas.push_back(terrainMod->getModifier()->bbox());
				}
			} else {
				mTerrain.removeMod(oldMercTerrainMod);
			}
		} else {
			mTerrain.removeMod(oldMercTerrainMod);
		}
	} else {
		S_LOG_WARNING("Got a change signal for a terrain mod which isn't registered with the terrain handler. This shouldn't happen.");
	}
}

void TerrainModChangeTask::executeTaskInMainThread()
{
	mHandler.reloadTerrain(mUpdatedAreas);
}
}

}
}
