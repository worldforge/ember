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

#include "TerrainGenerator.h"
#include "TerrainPage.h"
#include "TerrainShader.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/environment/Environment.h"

#include <OgreMaterial.h>

namespace EmberOgre
{

namespace Terrain
{

TerrainPageCreationTask::TerrainPageCreationTask(TerrainGenerator& terrainGenerator, const TerrainPosition& pos, ITerrainPageBridge* bridge) :
	mTerrainGenerator(terrainGenerator), mPage(0), mPos(pos), mBridge(bridge)
{

}

TerrainPageCreationTask::~TerrainPageCreationTask()
{

}

void TerrainPageCreationTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	mPage = new TerrainPage(mPos, mTerrainGenerator);
	mPage->registerBridge(mBridge);

	//add the base shaders, this should probably be refactored into a server side thing in the future
	const std::list<TerrainShader*>& baseShaders = mTerrainGenerator.getBaseShaders();
	for (std::list<TerrainShader*>::const_iterator I = baseShaders.begin(); I != baseShaders.end(); ++I) {
		mPage->addShader(*I);
	}

	mPage->updateAllShaderTextures(true);

	mPage->createShadowData(EmberOgre::getSingleton().getEntityFactory()->getWorld()->getEnvironment()->getSun()->getSunDirection());

	// setup foliage
	if (mTerrainGenerator.isFoliageShown()) {
		mPage->showFoliage();
	}
}

void TerrainPageCreationTask::executeTaskInMainThread()
{
	if (mPage) {

		mPage->loadShadow();
		mPage->generateTerrainMaterials(false);

		mTerrainGenerator.addPage(mPage);

		mPage->notifyBridgePageReady();
	}
}

}

}
