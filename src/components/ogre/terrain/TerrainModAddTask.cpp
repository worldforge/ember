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

#include "TerrainModAddTask.h"
#include "TerrainHandler.h"
#include "TerrainMod.h"
#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Eris/TerrainModTranslator.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainModAddTask::TerrainModAddTask(Mercator::Terrain& terrain, const TerrainMod& terrainMod, TerrainHandler& handler, TerrainModMap& terrainMods) :
	TerrainModTaskBase(terrain, terrainMod.getEntityId(), handler, terrainMods), mModData(terrainMod.getAtlasData()), mPosition(terrainMod.getEntity().getPredictedPos()), mOrientation(terrainMod.getEntity().getOrientation())
{

}

void TerrainModAddTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	Eris::TerrainModTranslator* terrainMod = new Eris::TerrainModTranslator();
	if (mModData.isMap()) {
		Atlas::Message::MapType mapData = mModData.asMap();
		bool success = terrainMod->parseData(mPosition, mOrientation, mapData);
		if (success) {
			mTerrain.addMod(terrainMod->getModifier());
			mUpdatedAreas.push_back(terrainMod->getModifier()->bbox());
		}
	}
	mTerrainMods.insert(TerrainModMap::value_type(mEntityId, terrainMod));
}

void TerrainModAddTask::executeTaskInMainThread()
{
	mHandler.reloadTerrain(mUpdatedAreas);
}
}

}
}
