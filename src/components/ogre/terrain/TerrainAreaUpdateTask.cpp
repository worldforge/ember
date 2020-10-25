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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "TerrainAreaUpdateTask.h"
#include "TerrainLayerDefinition.h"
#include <Mercator/Terrain.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Terrain {
TerrainAreaUpdateTask::TerrainAreaUpdateTask(Mercator::Terrain& terrain,
											 long id,
											 std::unique_ptr<Mercator::Area> area,

											 ShaderUpdateSlotType markForUpdateSlot) :
		mTerrain(terrain),
		mId(id),
		mArea(std::move(area)),
		mShaderUpdateSlot(std::move(markForUpdateSlot)),
		mLayerId(mArea ? mArea->getLayer() : 0) {
}

TerrainAreaUpdateTask::~TerrainAreaUpdateTask() = default;

void TerrainAreaUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	if (mArea) {
		mNewShape = mArea->bbox();
	}
	mOldShape = mTerrain.updateArea(mId, std::move(mArea));
}

bool TerrainAreaUpdateTask::executeTaskInMainThread() {
	//Note that "layer" never gets updated, so it's ok to access that from the main thread.
	if (mLayerId > 0) {

		if (mNewShape.isValid()) {
			//mark the shader for update
			//we'll not update immediately, we try to batch many area updates and then only update once per frame
			mShaderUpdateSlot(mLayerId, mNewShape);
		}

		if (mOldShape.isValid()) {
			//Also mark the old shape, in case it resided on a different page.
			mShaderUpdateSlot(mLayerId, mOldShape);
		}
	}
	return true;
}

}

}
}
