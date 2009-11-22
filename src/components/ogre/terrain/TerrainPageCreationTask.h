//
// C++ Interface: TerrainPageCreationTask
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifndef TERRAINPAGECREATIONTASK_H_
#define TERRAINPAGECREATIONTASK_H_

#include "components/ogre/Types.h"
#include "framework/tasks/ITask.h"
#include <wfmath/point.h>

namespace EmberOgre
{

namespace Terrain
{

class TerrainGenerator;
class TerrainPage;
class ITerrainPageBridge;

class TerrainPageCreationTask : public Ember::Tasks::ITask
{
public:
	TerrainPageCreationTask(TerrainGenerator& terrainGenerator, const TerrainPosition& pos, ITerrainPageBridge* bridge);
	virtual ~TerrainPageCreationTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();

private:
	TerrainGenerator& mTerrainGenerator;

	TerrainPage* mPage;
	TerrainPosition mPos;
	ITerrainPageBridge* mBridge;
};

}

}

#endif /* TERRAINPAGECREATIONTASK_H_ */
