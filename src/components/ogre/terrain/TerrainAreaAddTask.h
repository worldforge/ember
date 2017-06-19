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

#ifndef EMBEROGRETERRAINTERRAINAREAADDTASK_H_
#define EMBEROGRETERRAINTERRAINAREAADDTASK_H_

#include "TerrainAreaTaskBase.h"
#include "Types.h"

#include <wfmath/point.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
class TerrainHandler;
class TerrainLayerDefinitionManager;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A task for adding a new terrain area to the terrain.
 */
class TerrainAreaAddTask: public TerrainAreaTaskBase
{
public:
	TerrainAreaAddTask(Mercator::Terrain& terrain, Mercator::Area* area, ShaderUpdateSlotType markForUpdateSlot, TerrainHandler& terrainHandler, TerrainLayerDefinitionManager& terrainLayerDefinitionManager, AreaShaderstore& areaShaders);
	virtual ~TerrainAreaAddTask();

	virtual void executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context);

	virtual bool executeTaskInMainThread();
private:

	TerrainHandler& mTerrainHandler;
	TerrainLayerDefinitionManager& mTerrainLayerDefinitionManager;
	AreaShaderstore& mAreaShaders;
	WFMath::AxisBox<2> mNewBbox;
};

}

}

}

#endif /* EMBEROGRETERRAINTERRAINAREAADDTASK_H_ */
