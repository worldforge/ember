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
#include "TerrainHandler.h"
#include "TerrainDefPoint.h"
#include "TerrainInfo.h"
#include "ISceneManagerAdapter.h"
#include "SegmentManager.h"
#include "framework/LoggingInstance.h"
#include <Mercator/Terrain.h>
#include <Mercator/BasePoint.h>
#include <wfmath/axisbox.h>
#include <OgreAxisAlignedBox.h>
#include <sstream>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainUpdateTask::TerrainUpdateTask(Mercator::Terrain& terrain, const TerrainDefPointStore& terrainPoints, TerrainHandler& handler, TerrainInfo& terrainInfo, bool& hasTerrainInfo, SegmentManager& segmentManager) :
	mTerrain(terrain), mTerrainPoints(terrainPoints), mTerrainHandler(handler), mTerrainInfo(terrainInfo), mHasTerrainInfo(hasTerrainInfo), mSegmentManager(segmentManager)
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

		// FIXME Sort out roughness and falloff, and generally verify this code is the same as that in Terrain layer
		mTerrain.setBasePoint(static_cast<int> (I->getPosition().x()), static_cast<int> (I->getPosition().y()), bp);
		mUpdatedBasePoints.push_back(UpdateBasePointStore::value_type(I->getPosition(), bp));
		mUpdatedPositions.push_back(TerrainPosition(I->getPosition().x() * terrainRes, I->getPosition().y() * terrainRes));
	}
	mSegmentManager.syncWithTerrain();
}

void TerrainUpdateTask::executeTaskInMainThread()
{
	for (UpdateBasePointStore::const_iterator I = mUpdatedBasePoints.begin(); I != mUpdatedBasePoints.end(); ++I) {
		mTerrainInfo.setBasePoint(I->first, I->second);
	}
	mTerrainHandler.EventWorldSizeChanged.emit();

	if (!mHasTerrainInfo) {
		mHasTerrainInfo = true;
	} else {
		if (!mUpdatedPositions.empty()) {
			//if it's an update, we need to reload all pages and adjust all entity positions
			mTerrainHandler.reloadTerrain(mUpdatedPositions);
		}
	}
}


}

}
}
