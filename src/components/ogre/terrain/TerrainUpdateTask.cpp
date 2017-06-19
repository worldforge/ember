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

#include "TerrainUpdateTask.h"
#include "TerrainHandler.h"
#include "TerrainDefPoint.h"
#include "TerrainInfo.h"
#include "SegmentManager.h"
#include <Mercator/Terrain.h>
#include <OgreAxisAlignedBox.h>

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

void TerrainUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	int terrainRes = mTerrain.getResolution();
	for (TerrainDefPointStore::const_iterator I = mTerrainPoints.begin(); I != mTerrainPoints.end(); ++I) {
		Mercator::BasePoint bp;
		const TerrainPosition& pos = I->position;
		if (mTerrain.getBasePoint(static_cast<int> (pos.x()), static_cast<int> (pos.y()), bp) && (WFMath::Equal(I->height, bp.height()))
				&& (WFMath::Equal(I->roughness, bp.roughness()) && (WFMath::Equal(I->falloff, bp.falloff())))) {
			S_LOG_VERBOSE("Point [" << pos.x() << "," << pos.y() << "] unchanged");
			continue;
		} else {
			S_LOG_VERBOSE("Setting base point [" << pos.x() << "," << pos.y() << "] to height " << I->height);
		}
		bp.height() = I->height;
		bp.roughness() = I->roughness;
		bp.falloff() = I->falloff;

		// FIXME Sort out roughness and falloff, and generally verify this code is the same as that in Terrain layer
		mTerrain.setBasePoint(static_cast<int> (pos.x()), static_cast<int> (pos.y()), bp);
		mUpdatedBasePoints.push_back(UpdateBasePointStore::value_type(pos, bp));
		mUpdatedPositions.push_back(TerrainPosition(pos.x() * terrainRes, pos.y() * terrainRes));
	}
	mSegmentManager.syncWithTerrain();
}

bool TerrainUpdateTask::executeTaskInMainThread() {
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
	return true;
}


}

}
}
