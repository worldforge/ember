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

#include "TerrainShaderUpdateTask.h"
#include "TerrainPage.h"
#include "TerrainPageGeometry.h"
#include "TerrainPageSurface.h"
#include "TerrainMaterialCompilationTask.h"
#include "framework/tasks/TaskExecutionContext.h"

#include <wfmath/intersect.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainShaderUpdateTask::TerrainShaderUpdateTask(const GeometryPtrVector& geometry, const TerrainShader* shader, const AreaStore& areas, sigc::signal<void, const TerrainShader*, const AreaStore&>& signal, sigc::signal<void, TerrainPage*>& signalMaterialRecompiled, const WFMath::Vector<3>& lightDirection) :
	mGeometry(geometry), mAreas(areas), mSignal(signal), mSignalMaterialRecompiled(signalMaterialRecompiled), mLightDirection(lightDirection)
{
	mShaders.push_back(shader);
}

TerrainShaderUpdateTask::TerrainShaderUpdateTask(const GeometryPtrVector& geometry, const std::vector<const TerrainShader*>& shaders, const AreaStore& areas, sigc::signal<void, const TerrainShader*, const AreaStore&>& signal, sigc::signal<void, TerrainPage*>& signalMaterialRecompiled, const WFMath::Vector<3>& lightDirection) :
	mGeometry(geometry), mShaders(shaders), mAreas(areas), mSignal(signal), mSignalMaterialRecompiled(signalMaterialRecompiled), mLightDirection(lightDirection)
{
}

TerrainShaderUpdateTask::~TerrainShaderUpdateTask()
{
}

void TerrainShaderUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	GeometryPtrVector updatedPages;
	for (GeometryPtrVector::const_iterator J = mGeometry.begin(); J != mGeometry.end(); ++J) {
		TerrainPageGeometryPtr geometry = *J;
		TerrainPage& page = geometry->getPage();
		bool shouldUpdate = false;
		for (AreaStore::const_iterator K = mAreas.begin(); K != mAreas.end(); ++K) {
			if (WFMath::Intersect(page.getWorldExtent(), *K, true) || WFMath::Contains(page.getWorldExtent(), *K, true)) {
				shouldUpdate = true;
				break;
			}
		}
		if (shouldUpdate) {
			for (std::vector<const TerrainShader*>::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
				//repopulate the layer
				page.updateShaderTexture(*I, *geometry, true);
			}
			updatedPages.push_back(geometry);
		}
	}

	context.executeTask(new TerrainMaterialCompilationTask(updatedPages, mSignalMaterialRecompiled, mLightDirection));
	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool TerrainShaderUpdateTask::executeTaskInMainThread()
{
	for (std::vector<const TerrainShader*>::const_iterator I = mShaders.begin(); I != mShaders.end(); ++I) {
		mSignal(*I, mAreas);
	}
	return true;
}

}

}
}
