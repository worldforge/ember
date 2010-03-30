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

#include "TerrainPageCreationTask.h"

#include "TerrainManager.h"
#include "TerrainPage.h"
#include "TerrainShader.h"
#include "TerrainPageGeometry.h"
#include "GeometryUpdateTask.h"
#include "ITerrainPageBridge.h"

#include "framework/tasks/TaskExecutionContext.h"
#include <boost/shared_ptr.hpp>

namespace EmberOgre
{

namespace Terrain
{

TerrainPageCreationTask::TerrainPageCreationTask(TerrainManager& terrainManager, const TerrainIndex& index, const boost::shared_ptr<ITerrainPageBridge>& bridge, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap, const WFMath::Vector<3>& mainLightDirection) :
	mTerrainManager(terrainManager), mPage(0), mIndex(index), mBridge(bridge), mMainLightDirection(mainLightDirection), mHeightMapBufferProvider(heightMapBufferProvider), mHeightMap(heightMap)
{

}

TerrainPageCreationTask::~TerrainPageCreationTask()
{
}

void TerrainPageCreationTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	mPage = new TerrainPage(mIndex, mTerrainManager);

	//add the base shaders, this should probably be refactored into a server side thing in the future
	const std::list<TerrainShader*>& baseShaders = mTerrainManager.getBaseShaders();
	for (std::list<TerrainShader*>::const_iterator I = baseShaders.begin(); I != baseShaders.end(); ++I) {
		mPage->addShader(*I);
	}

	TerrainPageGeometryPtr geometryInstance(new TerrainPageGeometry(*mPage, mTerrainManager.getSegmentManager(), mTerrainManager.getDefaultHeight()));
	BridgeBoundGeometryPtrVector geometry;
	geometry.push_back(BridgeBoundGeometryPtrVector::value_type(geometryInstance, mBridge));
	std::vector<TerrainPosition> positions;
	//	positions.push_back(mPage->getWFPosition());
	context.executeTask(new GeometryUpdateTask(geometry, positions, mTerrainManager, mTerrainManager.getAllShaders(), mHeightMapBufferProvider, mHeightMap));

}

void TerrainPageCreationTask::executeTaskInMainThread()
{
	if (mPage) {

		mTerrainManager.addPage(mPage);

		if (mBridge.get()) {
			mBridge->terrainPageReady();
		}
	}
}

}

}
