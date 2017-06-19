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

#include "TerrainAreaAddTask.h"
#include "TerrainHandler.h"
#include "TerrainLayerDefinitionManager.h"

#include "Mercator/Area.h"
#include "Mercator/Terrain.h"
#include "Mercator/AreaShader.h"
namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainAreaAddTask::TerrainAreaAddTask(Mercator::Terrain& terrain, Mercator::Area* area, ShaderUpdateSlotType markForUpdateSlot, TerrainHandler& terrainHandler, TerrainLayerDefinitionManager& terrainLayerDefinitionManager, AreaShaderstore& areaShaders) :
	TerrainAreaTaskBase(terrain, area, markForUpdateSlot), mTerrainHandler(terrainHandler), mTerrainLayerDefinitionManager(terrainLayerDefinitionManager), mAreaShaders(areaShaders)
{
}

TerrainAreaAddTask::~TerrainAreaAddTask()
{
}

void TerrainAreaAddTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	//We know by now that this area is valid, so we don't need to check the layer or the shape for validity.
	mTerrain.addArea(mArea);
	//We can only access the bbox in the background thread, so lets pass on a copy of the bbox to the main thread.
	mNewBbox = mArea->bbox();
}

bool TerrainAreaAddTask::executeTaskInMainThread()
{
	//Note that "layer" never gets updated, so it's ok to access that from the main thread.
	if (mArea->getLayer() != 0) {
		if (!mAreaShaders.count(mArea->getLayer())) {
			S_LOG_VERBOSE("Shader does not exists, creating new.");
			//try to get the materialdefinition for this kind of area
			const TerrainLayerDefinition* layerDef = mTerrainLayerDefinitionManager.getDefinitionForArea(mArea->getLayer());
			if (layerDef) {
				TerrainShader* shader = mTerrainHandler.createShader(layerDef, new Mercator::AreaShader(mArea->getLayer()));
				mAreaShaders[mArea->getLayer()] = shader;
			}
		}
		if (mAreaShaders.count(mArea->getLayer())) {
			//mark the shader for update
			//we'll not update immediately, we try to batch many area updates and then only update once per frame
			mShaderUpdateSlot(mAreaShaders[mArea->getLayer()], mNewBbox);
		}
	}
	return true;
}

}

}
}
