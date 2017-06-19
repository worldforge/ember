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

#include "TerrainMaterialCompilationTask.h"
#include "TerrainPage.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageSurface.h"
#include "TerrainPageShadow.h"
#include "TerrainPageGeometry.h"

#include "framework/TimedLog.h"

#include <OgreRoot.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(const GeometryPtrVector& geometry, sigc::signal<void, TerrainPage*>& signal, const WFMath::Vector<3>& lightDirection) :
		mGeometry(geometry), mSignal(signal), mLightDirection(lightDirection) {
}

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(TerrainPageGeometryPtr geometry, sigc::signal<void, TerrainPage*>& signal, const WFMath::Vector<3>& lightDirection) :
		mSignal(signal), mLightDirection(lightDirection) {
	mGeometry.push_back(geometry);
}

TerrainMaterialCompilationTask::~TerrainMaterialCompilationTask() {
}

void TerrainMaterialCompilationTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	for (GeometryPtrVector::const_iterator J = mGeometry.begin(); J != mGeometry.end(); ++J) {
		(*J)->repopulate();
		TerrainPage& page = (*J)->getPage();
		TerrainPageSurfaceCompilationInstance* compilationInstance = page.getSurface()->createSurfaceCompilationInstance(*J);
		//If the technique requires a pregenerated shadow we must also populate normals.
		if (compilationInstance->requiresPregenShadow()) {
			(*J)->repopulate(true);
			page.getSurface()->getShadow()->updateShadow(**J);
		}
		if (compilationInstance->prepare()) {
			mMaterialRecompilations.push_back(std::pair<TerrainPageSurfaceCompilationInstance*, TerrainPage*>(compilationInstance, &(*J)->getPage()));
		}
	}
	//Release Segment references as soon as we can
	mGeometry.clear();
}

bool TerrainMaterialCompilationTask::executeTaskInMainThread() {
	TimedLog timedLog("TerrainMaterialCompilationTask::executeTaskInMainThread");
	if (!mMaterialRecompilations.empty()) {
		auto J = mMaterialRecompilations.begin();
		TerrainPageSurfaceCompilationInstance* compilationInstance = J->first;
		TerrainPage* page = J->second;
		compilationInstance->compile(page->getMaterial());
		S_LOG_VERBOSE("Compiling terrain page composite map material");
		compilationInstance->compileCompositeMap(page->getCompositeMapMaterial());
		S_LOG_VERBOSE("Recompiled material for terrain page " << "[" << page->getWFIndex().first << "|" << page->getWFIndex().second << "]");
		page->getSurface()->getShadow()->setShadowTextureName(compilationInstance->getShadowTextureName(page->getMaterial()));
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
	if (Ogre::Pass::getDirtyHashList().size() != 0 || Ogre::Pass::getPassGraveyard().size() != 0) {
		Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = Ogre::Root::getSingleton().getSceneManagerIterator();

		while (scenesIter.hasMoreElements()) {
			Ogre::SceneManager* pScene = scenesIter.getNext();
			if (pScene) {
				Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
				if (pQueue) {
					Ogre::RenderQueue::QueueGroupIterator groupIter = pQueue->_getQueueGroupIterator();
					while (groupIter.hasMoreElements()) {
						Ogre::RenderQueueGroup* pGroup = groupIter.getNext();
						if (pGroup)
							pGroup->clear(false);
					}//end_while(groupIter.hasMoreElements())
				}//end_if(pScene)
			}//end_if(pScene)
		}//end_while(scenesIter.hasMoreElements())

		// Now trigger the pending pass updates
		Ogre::Pass::processPendingPassUpdates();

	}//end_if(m_Root..
}

}

}
}
