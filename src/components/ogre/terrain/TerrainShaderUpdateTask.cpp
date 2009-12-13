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

#include "TerrainShaderUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainPageSurface.h"
#include "TerrainMaterialCompilationTask.h"
#include <wfmath/axisbox.h>
#include <wfmath/intersect.h>

#include "framework/tasks/TaskExecutionContext.h"


namespace EmberOgre
{

namespace Terrain
{

TerrainShaderUpdateTask::TerrainShaderUpdateTask(PageStore& pages, const TerrainShader* shader, const AreaStore& areas, bool updateAll, sigc::signal<void, const TerrainShader*, const AreaStore*>& signal) :
	mPages(pages), mShader(shader), mAreas(areas), mUpdateAll(updateAll), mSignal(signal)
{

}

TerrainShaderUpdateTask::~TerrainShaderUpdateTask()
{
}

void TerrainShaderUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	PageVector updatedPages;
	///We should either update all pages at once, or only those pages that intersect or contains the areas that have been changed
	if (mUpdateAll) {
		for (PageStore::const_iterator J = mPages.begin(); J != mPages.end(); ++J) {
			///repopulate the layer
			J->second->updateShaderTexture(mShader, true);
			updatedPages.push_back(J->second);
		}
	} else {
		for (PageStore::const_iterator J = mPages.begin(); J != mPages.end(); ++J) {
			bool shouldUpdate = false;
			for (AreaStore::const_iterator K = mAreas.begin(); K != mAreas.end(); ++K) {
				if (WFMath::Intersect(J->second->getExtent(), *K, true) || WFMath::Contains(J->second->getExtent(), *K, true)) {
					shouldUpdate = true;
					break;
				}
			}
			if (shouldUpdate) {
				///repopulate the layer
				J->second->updateShaderTexture(mShader, true);
				updatedPages.push_back(J->second);
			}
		}
	}

	context.executeTask(new TerrainMaterialCompilationTask(updatedPages));
}

void TerrainShaderUpdateTask::executeTaskInMainThread()
{

	if (mAreas.size()) {
		mSignal(mShader, &mAreas);
	} else {
		mSignal(mShader, 0);
	}
}

}

}
