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

#ifndef TERRAINPAGECREATIONTASK_H_
#define TERRAINPAGECREATIONTASK_H_

#include "components/ogre/Types.h"
#include "framework/tasks/ITask.h"
#include <wfmath/point.h>

namespace EmberOgre
{

namespace Terrain
{

class TerrainManager;
class TerrainPage;
class ITerrainPageBridge;

class HeightMapBufferProvider;
class HeightMap;

class TerrainPageCreationTask : public Ember::Tasks::ITask
{
public:
	TerrainPageCreationTask(TerrainManager& TerrainManager, const TerrainPosition& pos, ITerrainPageBridge* bridge, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap);
	virtual ~TerrainPageCreationTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:
	TerrainManager& mTerrainManager;

	TerrainPage* mPage;
	TerrainPosition mPos;
	ITerrainPageBridge* mBridge;

	HeightMapBufferProvider& mHeightMapBufferProvider;
	HeightMap& mHeightMap;
};

}

}

#endif /* TERRAINPAGECREATIONTASK_H_ */
