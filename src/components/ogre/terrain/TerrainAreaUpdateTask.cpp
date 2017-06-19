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

#include "TerrainAreaUpdateTask.h"
#include "TerrainArea.h"
#include <Mercator/Terrain.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{
TerrainAreaUpdateTask::TerrainAreaUpdateTask(Mercator::Terrain& terrain, Mercator::Area* area, const Mercator::Area& newArea, ShaderUpdateSlotType markForUpdateSlot, const TerrainShader* shader) :
	TerrainAreaTaskBase(terrain, area, markForUpdateSlot), mNewArea(newArea), mShader(shader)
{

}

TerrainAreaUpdateTask::~TerrainAreaUpdateTask()
{
}

void TerrainAreaUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	mOldShape = mArea->bbox();
	mArea->setShape(mNewArea.shape());
	mNewShape = mArea->bbox();

	mTerrain.updateArea(mArea);
}

bool TerrainAreaUpdateTask::executeTaskInMainThread()
{
	if (mShader) {
		//mark the shader for update
		//we'll not update immediately, we try to batch many area updates and then only update once per frame
		mShaderUpdateSlot(mShader, mNewShape);

		//Also mark the old shape, in case it resided on a different page.
		mShaderUpdateSlot(mShader, mOldShape);
	}
	return true;
}

}

}
}
