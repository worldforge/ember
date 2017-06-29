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

#include "TerrainPageCreationTask.h"

#include "TerrainHandler.h"
#include "TerrainPage.h"
#include "TerrainShader.h"
#include "TerrainPageGeometry.h"
#include "GeometryUpdateTask.h"
#include "ITerrainPageBridge.h"

#include "framework/tasks/TaskExecutionContext.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainPageCreationTask::TerrainPageCreationTask(TerrainHandler& handler,
												 const TerrainIndex& index,
												 const std::shared_ptr<ITerrainPageBridge>& bridge,
												 HeightMapBufferProvider& heightMapBufferProvider,
												 HeightMap& heightMap,
												 const WFMath::Vector<3>& mainLightDirection) :
	mTerrainHandler(handler),
	mPage(nullptr),
	mIndex(index),
	mBridge(bridge),
	mMainLightDirection(mainLightDirection),
	mHeightMapBufferProvider(heightMapBufferProvider),
	mHeightMap(heightMap)
{

}

TerrainPageCreationTask::~TerrainPageCreationTask()
{
}

void TerrainPageCreationTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	mPage = new TerrainPage(mIndex, mTerrainHandler.getPageIndexSize(), mTerrainHandler.getCompilerTechniqueProvider());
	mBridge->bindToTerrainPage(mPage);

	//add the base shaders, this should probably be refactored into a server side thing in the future
	const std::list<TerrainShader*>& baseShaders = mTerrainHandler.getBaseShaders();
	for (std::list<TerrainShader*>::const_iterator I = baseShaders.begin(); I != baseShaders.end(); ++I) {
		mPage->addShader(*I);
	}

	TerrainPageGeometryPtr geometryInstance(new TerrainPageGeometry(*mPage, mTerrainHandler.getSegmentManager(), mTerrainHandler.getDefaultHeight()));
	BridgeBoundGeometryPtrVector geometry;
	geometry.push_back(BridgeBoundGeometryPtrVector::value_type(geometryInstance, mBridge));
	std::vector<WFMath::AxisBox<2>> areas;
	areas.push_back(mPage->getWorldExtent());
	//	positions.push_back(mPage->getWFPosition());
	context.executeTask(new GeometryUpdateTask(geometry, areas, mTerrainHandler, mTerrainHandler.getAllShaders(), mHeightMapBufferProvider, mHeightMap, mMainLightDirection));

}

bool TerrainPageCreationTask::executeTaskInMainThread()
{
	if (mPage) {
		S_LOG_VERBOSE("Adding loaded terrain page to TerrainHandler: " << "[" << mPage->getWFIndex().first << "|" << mPage->getWFIndex().second <<"]");
		mTerrainHandler.addPage(mPage);
	}
	return true;
}

}

}
}
