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

#include "GeometryUpdateTask.h"

#include <utility>
#include "TerrainPage.h"
#include "TerrainHandler.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainPageGeometry.h"
#include "HeightMapUpdateTask.h"
#include "ITerrainPageBridge.h"

#include "framework/tasks/TaskExecutionContext.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

GeometryUpdateTask::GeometryUpdateTask(BridgeBoundGeometryPtrVector pages,
									   std::vector<WFMath::AxisBox<2>> areas,
									   TerrainHandler& handler,
									   std::vector<const Terrain::TerrainShader*> shaders,
									   HeightMapBufferProvider& heightMapBufferProvider,
									   HeightMap& heightMap) :
		mGeometry(std::move(pages)),
		mAreas(std::move(areas)),
		mHandler(handler),
		mShaders(std::move(shaders)),
		mHeightMapBufferProvider(heightMapBufferProvider),
		mHeightMap(heightMap) {

}

void GeometryUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	std::vector<Mercator::Segment*> segments;

	//first populate the geometry for all pages, and then regenerate the shaders
	for (const auto& geometryEntry : mGeometry) {
		auto& geometry = geometryEntry.first;
		geometry->repopulate();
		const SegmentVector& segmentVector = geometry->getValidSegments();
		for (const auto& entry : segmentVector) {
			segments.push_back(entry.segment);
		}
		GeometryPtrVector geometries;
		geometries.push_back(geometry);

		context.executeTask(std::make_unique<TerrainShaderUpdateTask>(geometries, mShaders, mAreas, mHandler.EventLayerUpdated, mHandler.EventTerrainMaterialRecompiled));
	}
	context.executeTask(std::make_unique<HeightMapUpdateTask>(mHeightMapBufferProvider, mHeightMap, segments));

	for (const auto& geometryEntry : mGeometry) {
		const TerrainPageGeometryPtr& geometry = geometryEntry.first;
		const ITerrainPageBridgePtr& bridge = geometryEntry.second;
		if (bridge) {
			bridge->updateTerrain(*geometry);
			mBridgesToNotify.insert(bridge);
		}
	}
	for (const auto& geometryEntry : mGeometry) {
		TerrainPage* page = &(geometryEntry.first)->getPage();
		mPages.insert(page);
	}

	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool GeometryUpdateTask::executeTaskInMainThread() {
	if (!mBridgesToNotify.empty()) {
		auto I = mBridgesToNotify.begin();
		(*I)->terrainPageReady();
		mBridgesToNotify.erase(I);
		return false;
	}
	mHandler.EventAfterTerrainUpdate(mAreas, mPages);
	return true;
}
}

}
}
