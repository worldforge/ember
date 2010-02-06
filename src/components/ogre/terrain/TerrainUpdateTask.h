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

#ifndef TERRAINUPDATETASK_H_
#define TERRAINUPDATETASK_H_

#include "framework/tasks/TemplateNamedTask.h"
#include "Types.h"

namespace Mercator
{
class Terrain;
class BasePoint;
}

namespace EmberOgre
{

namespace Terrain
{

class TerrainManager;
class TerrainInfo;

class TerrainUpdateTask : public Ember::Tasks::TemplateNamedTask<TerrainUpdateTask>
{
public:
	TerrainUpdateTask(Mercator::Terrain& terrain, const TerrainDefPointStore& terrainPoints, TerrainManager& terrainManager, TerrainInfo& terrainInfo, bool& hasTerrainInfo);
	virtual ~TerrainUpdateTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:

	typedef std::vector<std::pair<WFMath::Point<2>, Mercator::BasePoint > > UpdateBasePointStore;

	Mercator::Terrain& mTerrain;
	const TerrainDefPointStore mTerrainPoints;
	TerrainManager& mTerrainManager;
	TerrainInfo& mTerrainInfo;
	bool& mHasTerrainInfo;

	std::vector<TerrainPosition> mUpdatedPositions;
	UpdateBasePointStore mUpdatedBasePoints;

	void initializeTerrain();

};

}

}

#endif /* TERRAINUPDATETASK_H_ */
