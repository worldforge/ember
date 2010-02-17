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

namespace EmberOgre
{

namespace Terrain
{

TerrainPageCreationTask::TerrainPageCreationTask(TerrainManager& TerrainManager, Mercator::Terrain& terrain, const TerrainPosition& pos, ITerrainPageBridge* bridge, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap, const WFMath::Vector<3>& mainLightDirection) :
	mTerrainManager(TerrainManager), mTerrain(terrain), mPage(0), mPos(pos), mBridge(bridge), mMainLightDirection(mainLightDirection), mHeightMapBufferProvider(heightMapBufferProvider), mHeightMap(heightMap)
{

}

TerrainPageCreationTask::~TerrainPageCreationTask()
{

}

void TerrainPageCreationTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	mPage = new TerrainPage(mPos, mTerrainManager, mTerrain);
	mPage->registerBridge(mBridge);

	//add the base shaders, this should probably be refactored into a server side thing in the future
	const std::list<TerrainShader*>& baseShaders = mTerrainManager.getBaseShaders();
	for (std::list<TerrainShader*>::const_iterator I = baseShaders.begin(); I != baseShaders.end(); ++I) {
		mPage->addShader(*I);
	}

	// setup foliage
	if (mTerrainManager.isFoliageShown()) {
		mPage->showFoliage();
	}

	GeometryUpdateTask::PageSet pageSet;
	pageSet.insert(mPage);
	std::vector<TerrainPosition> positions;
	//	positions.push_back(mPage->getWFPosition());
	context.executeTask(new GeometryUpdateTask(pageSet, positions, mTerrainManager, mTerrainManager.getAllShaders(), mHeightMapBufferProvider, mHeightMap));

}

void TerrainPageCreationTask::executeTaskInMainThread()
{
	if (mPage) {

		mTerrainManager.addPage(mPage);

		if (mBridge) {
			mBridge->terrainPageReady();
		}
	}
}

}

}
