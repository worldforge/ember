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

#include "TerrainMaterialCompilationTask.h"
#include "TerrainPage.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageSurface.h"
#include "TerrainPageGeometry.h"

#include "framework/TimedLog.h"

#include <OgreRoot.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(GeometryPtrVector geometry,
															   sigc::signal<void, TerrainPage*>& signal,
															   const WFMath::Vector<3>& lightDirection) :
		mGeometry(std::move(geometry)),
		mSignal(signal),
		mLightDirection(lightDirection) {
}

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(TerrainPageGeometryPtr geometry,
															   sigc::signal<void, TerrainPage*>& signal,
															   const WFMath::Vector<3>& lightDirection) :
		mSignal(signal),
		mLightDirection(lightDirection) {
	mGeometry.push_back(std::move(geometry));
}

void TerrainMaterialCompilationTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	for (auto & geometry : mGeometry) {
		geometry->repopulate();
		TerrainPage& page = geometry->getPage();
		TerrainPageSurfaceCompilationInstance* compilationInstance = page.getSurface()->createSurfaceCompilationInstance(geometry);

		if (compilationInstance->prepare()) {
			mMaterialRecompilations.push_back(std::pair<TerrainPageSurfaceCompilationInstance*, TerrainPage*>(compilationInstance, &geometry->getPage()));
		}
	}
	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool TerrainMaterialCompilationTask::executeTaskInMainThread() {
	TimedLog timedLog("TerrainMaterialCompilationTask::executeTaskInMainThread");
	if (!mMaterialRecompilations.empty()) {
		auto J = mMaterialRecompilations.begin();
		auto compilationInstance = J->first;
		auto page = J->second;
		compilationInstance->compile(page->getMaterial());
		S_LOG_VERBOSE("Compiling terrain page composite map material");
		compilationInstance->compileCompositeMap(page->getCompositeMapMaterial());
		S_LOG_VERBOSE("Recompiled material for terrain page " << "[" << page->getWFIndex().first << "|" << page->getWFIndex().second << "]");
		mSignal(page); // Notify the terrain system of the material change
		delete compilationInstance;
		std::stringstream ss;
		ss << "Compiled for page [" << page->getWFIndex().first << "|" << page->getWFIndex().second << "]";
		timedLog.report(ss.str());
		updateSceneManagersAfterMaterialsChange();
		mMaterialRecompilations.erase(J);
		return mMaterialRecompilations.empty();
	}
	return true;
}

void TerrainMaterialCompilationTask::updateSceneManagersAfterMaterialsChange() {
	//We need to do this to prevent stale hashes in Ogre, which will lead to crashes during rendering.
	if (!Ogre::Pass::getDirtyHashList().empty() || !Ogre::Pass::getPassGraveyard().empty()) {
		for (const auto& entry : Ogre::Root::getSingleton().getSceneManagers()) {
			Ogre::SceneManager* pScene = entry.second;
			if (pScene) {
				Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
				if (pQueue) {
					for (auto& groupEntry : pQueue->_getQueueGroups()) {
						if (groupEntry) {
							groupEntry->clear(false);
						}
					}
				}
			}
		}

		// Now trigger the pending pass updates
		Ogre::Pass::processPendingPassUpdates();

	}
}

}

}
}
