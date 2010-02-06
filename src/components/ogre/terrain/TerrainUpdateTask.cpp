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

#include "TerrainUpdateTask.h"
#include "TerrainManager.h"
#include "TerrainDefPoint.h"
#include "TerrainInfo.h"
#include "ISceneManagerAdapter.h"
#include "framework/LoggingInstance.h"
#include <Mercator/Terrain.h>
#include <Mercator/BasePoint.h>
#include <wfmath/axisbox.h>
#include <OgreAxisAlignedBox.h>
#include <sstream>

namespace EmberOgre
{

namespace Terrain
{

TerrainUpdateTask::TerrainUpdateTask(Mercator::Terrain& terrain, const TerrainDefPointStore& terrainPoints, TerrainManager& terrainManager, TerrainInfo& terrainInfo, bool& hasTerrainInfo) :
	mTerrain(terrain), mTerrainPoints(terrainPoints), mTerrainManager(terrainManager), mTerrainInfo(terrainInfo), mHasTerrainInfo(hasTerrainInfo)
{

}

TerrainUpdateTask::~TerrainUpdateTask()
{
}

void TerrainUpdateTask::executeTaskInBackgroundThread(Ember::Tasks::TaskExecutionContext& context)
{
	int terrainRes = mTerrain.getResolution();
	for (TerrainDefPointStore::const_iterator I = mTerrainPoints.begin(); I != mTerrainPoints.end(); ++I) {
		Mercator::BasePoint bp;
		if (mTerrain.getBasePoint(static_cast<int> (I->getPosition().x()), static_cast<int> (I->getPosition().y()), bp) && (I->getHeight() == bp.height())) {
			S_LOG_VERBOSE("Point [" << I->getPosition().x() << "," << I->getPosition().y() << "] unchanged");
			continue;
		} else {
			S_LOG_VERBOSE("Setting base point [" << I->getPosition().x() << "," << I->getPosition().y() << "] to height " << I->getHeight());
		}
		bp.height() = I->getHeight();

		/// FIXME Sort out roughness and falloff, and generally verify this code is the same as that in Terrain layer
		mTerrain.setBasePoint(static_cast<int> (I->getPosition().x()), static_cast<int> (I->getPosition().y()), bp);
		mUpdatedBasePoints.push_back(UpdateBasePointStore::value_type(I->getPosition(), bp));
		mUpdatedPositions.push_back(TerrainPosition(I->getPosition().x() * terrainRes, I->getPosition().y() * terrainRes));
	}
}

void TerrainUpdateTask::executeTaskInMainThread()
{
	for (UpdateBasePointStore::const_iterator I = mUpdatedBasePoints.begin(); I != mUpdatedBasePoints.end(); ++I) {
		mTerrainInfo.setBasePoint(I->first, I->second);
	}
	mTerrainManager.EventWorldSizeChanged.emit();

	if (!mHasTerrainInfo) {
		initializeTerrain();
		mHasTerrainInfo = true;
	} else {
		if (!mUpdatedPositions.empty()) {
			///if it's an update, we need to reload all pages and adjust all entity positions
			mTerrainManager.reloadTerrain(mUpdatedPositions);
		}
	}
}

void TerrainUpdateTask::initializeTerrain()
{
	ISceneManagerAdapter* adapter = mTerrainManager.getAdapter();
	if (adapter) {
		adapter->setWorldPagesDimensions(mTerrainInfo.getTotalNumberOfPagesY(), mTerrainInfo.getTotalNumberOfPagesX(), mTerrainInfo.getPageOffsetY(), mTerrainInfo.getPageOffsetX());

		adapter->loadScene();
		const WFMath::AxisBox<2>& worldSize = mTerrainInfo.getWorldSizeInIndices();
		//		float heightMin = std::numeric_limits<float>::min();
		//		float heightMax = std::numeric_limits<float>::max();
		//		if (heightMax < heightMin) {
		float heightMin = -100;
		float heightMax = 100;
		//		}
		Ogre::AxisAlignedBox worldBox(worldSize.lowCorner().x(), heightMin, -worldSize.highCorner().y(), worldSize.highCorner().x(), heightMax, -worldSize.lowCorner().y());

		std::stringstream ss;
		ss << worldBox;
		S_LOG_INFO("New size of the world: " << ss.str());

		adapter->resize(worldBox, 16);

		S_LOG_INFO("Pages: X: " << mTerrainInfo.getTotalNumberOfPagesX() << " Y: " << mTerrainInfo.getTotalNumberOfPagesY() << " Total: " << mTerrainInfo.getTotalNumberOfPagesX() * mTerrainInfo.getTotalNumberOfPagesY());
		S_LOG_INFO("Page offset: X" << mTerrainInfo.getPageOffsetX() << " Y: " << mTerrainInfo.getPageOffsetY());

		///load the first page, thus bypassing the normal paging system. This is to prevent the user from floating in the thin air while the paging system waits for a suitable time to load the first page.
		adapter->loadFirstPage();
	}
}

}

}
