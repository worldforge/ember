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

#ifndef EMBEROGRETERRAINTERRAINAREAADDTASK_H_
#define EMBEROGRETERRAINTERRAINAREAADDTASK_H_

#include "TerrainAreaTaskBase.h"
#include "Types.h"

namespace EmberOgre
{

namespace Terrain
{
class TerrainManager;
class TerrainLayerDefinitionManager;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A task for adding a new terrain area to the terrain.
 */
class TerrainAreaAddTask: public TerrainAreaTaskBase
{
public:
	TerrainAreaAddTask(Mercator::Terrain& terrain, Mercator::Area* area, ShaderUpdateSlotType markForUpdateSlot, TerrainManager& TerrainManager, TerrainLayerDefinitionManager& terrainLayerDefinitionManager, AreaShaderstore& areaShaders, AreaMap& areas, const std::string& entityId);
	virtual ~TerrainAreaAddTask();

	virtual void executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context);

	virtual void executeTaskInMainThread();
private:

	TerrainManager& mTerrainManager;
	TerrainLayerDefinitionManager& mTerrainLayerDefinitionManager;
	AreaShaderstore& mAreaShaders;
	AreaMap& mAreas;
	const std::string mEntityId;
};

}

}

#endif /* EMBEROGRETERRAINTERRAINAREAADDTASK_H_ */
