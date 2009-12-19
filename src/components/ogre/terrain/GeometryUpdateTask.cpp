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

#include "framework/tasks/TaskExecutionContext.h"

namespace EmberOgre
{

namespace Terrain
{

GeometryUpdateTask::GeometryUpdateTask(const PageSet& pages, const std::vector<TerrainPosition>& positions, TerrainManager& manager, const ShaderStore& shaders) :
	mPages(pages), mPositions(positions), mManager(manager), mShaders(shaders)
{

}

GeometryUpdateTask::~GeometryUpdateTask()
{
}

void GeometryUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	//first populate the geometry for all pages, and then regenerate the shaders
	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		page->repopulateGeometry();
	}
	PageVector pageVector;
	for (PageSet::const_iterator I = mPages.begin(); I != mPages.end(); ++I) {
		TerrainPage* page = *I;
		pageVector.push_back(page);
	}
	for (ShaderStore::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
		context.executeTask(new TerrainShaderUpdateTask(pageVector, I->second, AreaStore(), true, mManager.EventLayerUpdated));

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
