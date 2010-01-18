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

namespace EmberOgre
{

namespace Terrain
{

GeometryUpdateTask::GeometryUpdateTask(const PageSet& pages, const std::vector<TerrainPosition>& positions, TerrainManager& manager, const ShaderStore& shaders, HeightMapBufferProvider& heightMapBufferProvider, HeightMap& heightMap) :
	mPages(pages), mPositions(positions), mManager(manager), mShaders(shaders), mHeightMapBufferProvider(heightMapBufferProvider), mHeightMap(heightMap)
{

}

GeometryUpdateTask::~GeometryUpdateTask()
{
}

void GeometryUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	std::vector<Mercator::Segment*> segments;

	//first populate the geometry for all pages, and then regenerate the shaders
	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		page->repopulateGeometry();
		const SegmentVector& segmentVector = page->getGeometry().getValidSegments();
		for (SegmentVector::const_iterator I = segmentVector.begin(); I != segmentVector.end(); ++I) {
			segments.push_back(I->segment);
		}
	}
	PageVector pageVector;
	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		pageVector.push_back(page);
	}
	for (ShaderStore::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
		context.executeTask(new TerrainShaderUpdateTask(pageVector, I->second, AreaStore(), true, mManager.EventLayerUpdated));
	}
	context.executeTask(new HeightMapUpdateTask(mHeightMapBufferProvider, mHeightMap, segments));

	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		if (page->getBridge()) {
			page->getBridge()->updateTerrain();
		}
	}
}

void GeometryUpdateTask::executeTaskInMainThread()
{
	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		page->signalGeometryChanged();
	}
	mManager.updateEntityPositions(mPages);
	mManager.EventAfterTerrainUpdate(mPositions, mPages);

}
}

}
