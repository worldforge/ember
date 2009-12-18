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

#include "TerrainAreaAddTask.h"
#include "TerrainGenerator.h"
#include "TerrainLayerDefinitionManager.h"
#include "TerrainArea.h"

#include "Mercator/Area.h"
#include "Mercator/Terrain.h"
#include "Mercator/AreaShader.h"
namespace EmberOgre
{

namespace Terrain
{

TerrainAreaAddTask::TerrainAreaAddTask(Mercator::Terrain& terrain, TerrainArea& terrainArea, TerrainGenerator& terrainGenerator, TerrainLayerDefinitionManager& terrainLayerDefinitionManager, AreaShaderstore& areaShaders, ShaderUpdateSlotType shaderUpdateSlot) :
	mTerrainGenerator(terrainGenerator), mTerrainLayerDefinitionManager(terrainLayerDefinitionManager), mAreaShaders(areaShaders), mShaderUpdateSlot(shaderUpdateSlot), TerrainAreaTaskBase::TerrainAreaTaskBase(terrain, terrainArea)
{
}

TerrainAreaAddTask::~TerrainAreaAddTask()
{
}

void TerrainAreaAddTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	Mercator::Area* area = mTerrainArea.getArea();
	//   _fpreset();
	//_controlfp(_PC_64, _MCW_PC);
	//_controlfp(_RC_NEAR, _MCW_RC);
	mTerrain.addArea(area);
}

void TerrainAreaAddTask::executeTaskInMainThread()
{
	Mercator::Area* area = mTerrainArea.getArea();
	if (!mAreaShaders.count(area->getLayer())) {
		S_LOG_VERBOSE("Shader does not exists, creating new.");
		///try to get the materialdefinition for this kind of area
		const TerrainLayerDefinition* layerDef = mTerrainLayerDefinitionManager.getDefinitionForArea(area->getLayer());
		if (layerDef) {
			TerrainShader* shader = mTerrainGenerator.createShader(layerDef, new Mercator::AreaShader(area->getLayer()));
			mAreaShaders[area->getLayer()] = shader;
		}
	}
	if (mAreaShaders.count(area->getLayer())) {
		///mark the shader for update
		///we'll not update immediately, we try to batch many area updates and then only update once per frame
		mShaderUpdateSlot(mAreaShaders[area->getLayer()], area);
	}
}

}

}
