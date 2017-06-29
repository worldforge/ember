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

#include "GeometryUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainHandler.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainPageGeometry.h"
#include "HeightMapUpdateTask.h"
#include "ITerrainPageBridge.h"

#include "framework/tasks/TaskExecutionContext.h"
namespace Ember
{
namespace OgreView
{

namespace Terrain
{

GeometryUpdateTask::GeometryUpdateTask(const BridgeBoundGeometryPtrVector& pages,
									   const std::vector<WFMath::AxisBox<2>>& areas,
									   TerrainHandler& handler,
									   const ShaderStore& shaders,
									   HeightMapBufferProvider& heightMapBufferProvider,
									   HeightMap& heightMap,
									   const WFMath::Vector<3> lightDirection) :
	mGeometry(pages),
	mAreas(areas),
	mHandler(handler),
	mShaders(shaders),
	mHeightMapBufferProvider(heightMapBufferProvider),
	mHeightMap(heightMap),
	mLightDirection(lightDirection)
{

}

GeometryUpdateTask::~GeometryUpdateTask()
{
}

void GeometryUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	std::vector<Mercator::Segment*> segments;
	// build a vector of shaders so we can more efficiently update them
	std::vector<const Terrain::TerrainShader*> shaderList;

	for (auto& entry : mShaders) {
		shaderList.push_back(entry.second);
	}

	//first populate the geometry for all pages, and then regenerate the shaders
	for (BridgeBoundGeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		TerrainPageGeometryPtr geometry = I->first;
		geometry->repopulate();
		const SegmentVector& segmentVector = geometry->getValidSegments();
		for (SegmentVector::const_iterator I = segmentVector.begin(); I != segmentVector.end(); ++I) {
			segments.push_back(I->segment);
		}
		GeometryPtrVector geometries;
		geometries.push_back(geometry);

		context.executeTask(new TerrainShaderUpdateTask(geometries, shaderList, mAreas, mHandler.EventLayerUpdated, mHandler.EventTerrainMaterialRecompiled, mLightDirection));
	}
	context.executeTask(new HeightMapUpdateTask(mHeightMapBufferProvider, mHeightMap, segments));

	for (BridgeBoundGeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		const TerrainPageGeometryPtr& geometry = I->first;
		const ITerrainPageBridgePtr& bridge = I->second;
		if (bridge.get()) {
			bridge->updateTerrain(*geometry);
			mBridgesToNotify.insert(bridge);
		}
	}
	for (BridgeBoundGeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		TerrainPage* page = &(I->first)->getPage();
		mPages.insert(page);
	}

	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool GeometryUpdateTask::executeTaskInMainThread()
{
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
