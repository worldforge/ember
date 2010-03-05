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

#include "GeometryUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainManager.h"
#include "TerrainShaderUpdateTask.h"
#include "TerrainPageGeometry.h"
#include "HeightMapUpdateTask.h"
#include "ITerrainPageBridge.h"

#include "framework/tasks/TaskExecutionContext.h"
#include <boost/shared_ptr.hpp>
namespace EmberOgre
{

namespace Terrain
{

GeometryUpdateTask::GeometryUpdateTask(const GeometryPtrVector& pages, const std::vector<TerrainPosition>& positions, TerrainManager& manager, const ShaderStore& shaders, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap) :
	mGeometry(pages), mPositions(positions), mManager(manager), mShaders(shaders), mHeightMapBufferProvider(heightMapBufferProvider), mHeightMap(heightMap)
{

}

GeometryUpdateTask::~GeometryUpdateTask()
{
}

void GeometryUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	std::vector<Mercator::Segment*> segments;

	AreaStore affectedAreas;
	//first populate the geometry for all pages, and then regenerate the shaders
	for (GeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		TerrainPageGeometryPtr geometry= *I;
		geometry->repopulate();
		const SegmentVector& segmentVector = geometry->getValidSegments();
		for (SegmentVector::const_iterator I = segmentVector.begin(); I != segmentVector.end(); ++I) {
			segments.push_back(I->segment);
		}
		affectedAreas.push_back(geometry->getPage().getWorldExtent());
		for (ShaderStore::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
			context.executeTask(new TerrainShaderUpdateTask(mGeometry, I->second, affectedAreas, mManager.EventLayerUpdated));
		}
	}
	context.executeTask(new HeightMapUpdateTask(mHeightMapBufferProvider, mHeightMap, segments));

	for (GeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		TerrainPage* page = &(*I)->getPage();
		if (page->getBridge()) {
			page->getBridge()->updateTerrain(**I);
		}
	}

	for (GeometryPtrVector::const_iterator I = mGeometry.begin(); I != mGeometry.end(); ++I) {
		TerrainPage* page = &(*I)->getPage();
		mPages.insert(page);
	}
	//Release Segment references as soon as we can
	mGeometry.clear();
}

void GeometryUpdateTask::executeTaskInMainThread()
{
	for (std::set<TerrainPage*>::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		(*I)->signalGeometryChanged();
	}

	mManager.updateEntityPositions(mPages);
	mManager.EventAfterTerrainUpdate(mPositions, mPages);

}
}

}
