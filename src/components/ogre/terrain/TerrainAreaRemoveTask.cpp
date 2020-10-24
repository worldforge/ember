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

#include "TerrainAreaRemoveTask.h"
#include <Mercator/Terrain.h>
#include <Mercator/Area.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainAreaRemoveTask::TerrainAreaRemoveTask(Mercator::Terrain& terrain,
											 std::shared_ptr<Mercator::Area> area,
											 ShaderUpdateSlotType markForUpdateSlot,
											 const TerrainShader* shader) :
		TerrainAreaTaskBase(terrain, std::move(area), std::move(markForUpdateSlot)), mShader(shader) {

}

TerrainAreaRemoveTask::~TerrainAreaRemoveTask() = default;

void TerrainAreaRemoveTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	mTerrain.removeArea(mArea.get());
}

bool TerrainAreaRemoveTask::executeTaskInMainThread() {
	if (mShader) {
		//mark the shader for update
		//we'll not update immediately, we try to batch many area updates and then only update once per frame

		//Note that since we've removed the area we can access the bbox in the main thread, since we're guaranteed that no other thread accesses it.
		mShaderUpdateSlot(mShader, mArea->bbox());
	}
	return true;
}

}

}
}
