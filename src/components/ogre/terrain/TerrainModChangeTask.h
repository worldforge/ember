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

#ifndef TERRAINMODCHANGETASK_H_
#define TERRAINMODCHANGETASK_H_

#include "framework/tasks/ITask.h"
#include "Types.h"

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

class TerrainModChangeTask: public Ember::Tasks::ITask
{
public:
	TerrainModChangeTask(Mercator::Terrain& terrain, TerrainMod& terrainMod, TerrainManager& manager, TerrainModMap& terrainMods, Mercator::TerrainMod* existingMod);
	virtual ~TerrainModChangeTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:
	Mercator::Terrain& mTerrain;
	TerrainMod& mTerrainMod;
	TerrainManager& mManager;
	TerrainModMap& mTerrainMods;
	Mercator::TerrainMod* mExistingMod;

	std::vector<TerrainPosition> mUpdatedPositions;
	Mercator::TerrainMod* mAppliedMod;
};

}

}

#endif /* TERRAINMODCHANGETASK_H_ */
