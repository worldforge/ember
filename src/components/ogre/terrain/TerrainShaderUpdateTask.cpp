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

#include "TerrainShaderUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainPageGeometry.h"
#include "TerrainPageSurface.h"
#include "TerrainShader.h"
#include "TerrainMaterialCompilationTask.h"
#include "TerrainPageSurfaceCompiler.h"
#include "framework/tasks/TaskExecutionContext.h"

#include <wfmath/intersect.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainShaderUpdateTask::TerrainShaderUpdateTask(GeometryPtrVector geometry,
												 std::vector<TerrainShader> shaders,
												 AreaStore areas,
												 sigc::signal<void, const TerrainShader&, const AreaStore&>& signal,
												 sigc::signal<void, TerrainPage&>& signalMaterialRecompiled) :
		mGeometry(std::move(geometry)),
		mShaders(std::move(shaders)),
		mAreas(std::move(areas)),
		mSignal(signal),
		mSignalMaterialRecompiled(signalMaterialRecompiled) {
}

TerrainShaderUpdateTask::~TerrainShaderUpdateTask() = default;

void TerrainShaderUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	GeometryPtrVector updatedPages;
	for (auto& geometry : mGeometry) {
		TerrainPage& page = geometry->getPage();
		bool shouldUpdate = false;
		for (const auto& area : mAreas) {
			if (WFMath::Intersect(page.getWorldExtent(), area, true) || WFMath::Contains(page.getWorldExtent(), area, true)) {
				shouldUpdate = true;
				break;
			}
		}
		if (shouldUpdate) {
			for (auto& shader : mShaders) {
				//repopulate the layer
				page.updateShaderTexture(shader.layer.layerDef, shader.layer.terrainIndex, shader.shader, *geometry, true);
			}
			updatedPages.push_back(geometry);
		}
	}

	context.executeTask(std::make_unique<TerrainMaterialCompilationTask>(updatedPages, mSignalMaterialRecompiled));
	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool TerrainShaderUpdateTask::executeTaskInMainThread() {
	for (auto& shader : mShaders) {
		mSignal(shader, mAreas);
	}
	return true;
}

}

}
}
