/*
 Copyright (C) 2009 erik

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

#include "TerrainModTaskBase.h"
#include <wfmath/point.h>
#include <wfmath/axisbox.h>

namespace EmberOgre
{

namespace Terrain
{

TerrainModTaskBase::TerrainModTaskBase(Mercator::Terrain& terrain, Mercator::TerrainMod* managerLocalTerrainMod, const std::string& entityId, TerrainManager& manager, TerrainModMap& terrainMods) :
	mTerrain(terrain), mManagerLocalTerrainMod(managerLocalTerrainMod), mEntityId(entityId), mManager(manager), mTerrainMods(terrainMods)
{

}

TerrainModTaskBase::~TerrainModTaskBase()
{
}

}

}
