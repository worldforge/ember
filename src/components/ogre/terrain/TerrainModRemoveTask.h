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

#ifndef TERRAINMODREMOVETASK_H_
#define TERRAINMODREMOVETASK_H_

#include "framework/tasks/ITask.h"
#include "Types.h"
#include <string>

namespace Mercator
{
class Terrain;
class TerrainMod;
}

namespace EmberOgre
{

namespace Terrain
{
class TerrainManager;
class TerrainMod;

class TerrainModRemoveTask: public Ember::Tasks::ITask
{
public:
	TerrainModRemoveTask(Mercator::Terrain& terrain, Mercator::TerrainMod* existingMod, TerrainManager& manager, TerrainModMap terrainMods, const std::string& entityId);
	virtual ~TerrainModRemoveTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:
	Mercator::Terrain& mTerrain;
	Mercator::TerrainMod* mExistingMod;
	TerrainManager& mManager;
	TerrainModMap mTerrainMods;
	const std::string mEntityId;

};

}

}

#endif /* TERRAINMODREMOVETASK_H_ */
