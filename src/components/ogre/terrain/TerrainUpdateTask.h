/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

class TerrainHandler;
class TerrainInfo;
class SegmentManager;

class TerrainUpdateTask : public Tasks::TemplateNamedTask<TerrainUpdateTask>
{
public:
	TerrainUpdateTask(Mercator::Terrain& terrain, const TerrainDefPointStore& terrainPoints, TerrainHandler& handler, TerrainInfo& terrainInfo, bool& hasTerrainInfo, SegmentManager& segmentManager);
	virtual ~TerrainUpdateTask();

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context);

	virtual bool executeTaskInMainThread();

private:

	typedef std::vector<std::pair<WFMath::Point<2>, Mercator::BasePoint >> UpdateBasePointStore;

	Mercator::Terrain& mTerrain;
	const TerrainDefPointStore mTerrainPoints;
	TerrainHandler& mTerrainHandler;
	TerrainInfo& mTerrainInfo;
	bool& mHasTerrainInfo;
	SegmentManager& mSegmentManager;

	std::vector<TerrainPosition> mUpdatedPositions;
	UpdateBasePointStore mUpdatedBasePoints;
};

}

}

}

#endif /* TERRAINUPDATETASK_H_ */
