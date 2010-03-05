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

#include "TerrainMaterialCompilationTask.h"
#include "Types.h"
#include "TerrainPage.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageSurface.h"
#include "TerrainPageGeometry.h"

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <boost/shared_ptr.hpp>
namespace EmberOgre
{

namespace Terrain
{

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(const GeometryPtrVector& geometry) :
	mGeometry(geometry)
{
}

TerrainMaterialCompilationTask::TerrainMaterialCompilationTask(TerrainPageGeometryPtr geometry)
{
	mGeometry.push_back(geometry);
}

TerrainMaterialCompilationTask::~TerrainMaterialCompilationTask()
{
}

void TerrainMaterialCompilationTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	for (GeometryPtrVector::const_iterator J = mGeometry.begin(); J != mGeometry.end(); ++J) {
		(*J)->repopulate();
		TerrainPageSurfaceCompilationInstance* compilationInstance = (*J)->getPage().getSurface()->createSurfaceCompilationInstance(*J);
		compilationInstance->prepare();
		mMaterialRecompilations.push_back(std::pair<TerrainPageSurfaceCompilationInstance*, TerrainPage*>(compilationInstance, &(*J)->getPage()));
	}
	//Release Segment references as soon as we can
	mGeometry.clear();
}

void TerrainMaterialCompilationTask::executeTaskInMainThread()
{
	for (CompilationInstanceStore::const_iterator J = mMaterialRecompilations.begin(); J != mMaterialRecompilations.end(); ++J) {
		TerrainPageSurfaceCompilationInstance* compilationInstance = J->first;
		TerrainPage* page = J->second;
		compilationInstance->compile(page->getMaterial());
		delete compilationInstance;
	}
	updateSceneManagersAfterMaterialsChange();
}

void TerrainMaterialCompilationTask::updateSceneManagersAfterMaterialsChange()
{
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
