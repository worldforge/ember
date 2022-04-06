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

#include "TerrainPageCreationTask.h"

#include <utility>

#include "TerrainHandler.h"
#include "TerrainPage.h"
#include "TerrainShader.h"
#include "TerrainPageGeometry.h"
#include "GeometryUpdateTask.h"

#include "framework/tasks/TaskExecutionContext.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainPageCreationTask::TerrainPageCreationTask(TerrainHandler& handler,
												 std::shared_ptr<Terrain::TerrainPage> page,
												 HeightMapBufferProvider& heightMapBufferProvider,
												 HeightMap& heightMap) :
		mTerrainHandler(handler),
		mPage(std::move(page)),
		mHeightMapBufferProvider(heightMapBufferProvider),
		mHeightMap(heightMap) {
}


void TerrainPageCreationTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {

	//add the base shaders, this should probably be refactored into a server side thing in the future
	std::vector<TerrainShader> shaders;
	//TODO: race condition...
	shaders.reserve(mTerrainHandler.getAllShaders().size());


	for (auto& entry: mTerrainHandler.getAllShaders()) {
		shaders.push_back(TerrainShader{entry.second.layer, *entry.second.shader});
		mPage->addShader(entry.second.layer.layerDef, entry.second.layer.terrainIndex, *entry.second.shader);
	}


	auto geometryInstance = std::make_unique<TerrainPageGeometry>(mPage,
																  mTerrainHandler.getSegmentManager(),
																  mTerrainHandler.getDefaultHeight());
	std::vector<TerrainPageGeometryPtr> geometry;
	geometry.emplace_back(std::move(geometryInstance));
	std::vector<WFMath::AxisBox<2>> areas;
	areas.push_back(mPage->getWorldExtent());
	//	positions.push_back(mPage->getWFPosition());
	context.executeTask(std::make_unique<GeometryUpdateTask>(std::move(geometry),
															 areas,
															 mTerrainHandler,
															 std::move(shaders),
															 mHeightMapBufferProvider,
															 mHeightMap));

}

bool TerrainPageCreationTask::executeTaskInMainThread() {
	return true;
}

}

}
}
