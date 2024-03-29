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

#include "TerrainModUpdateTask.h"
#include "TerrainHandler.h"
#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>

namespace Ember {
namespace OgreView {

namespace Terrain {

TerrainModUpdateTask::TerrainModUpdateTask(Mercator::Terrain& terrain,
										   std::unique_ptr<Ember::Terrain::TerrainModTranslator> terrainModTranslator,
										   long terrainModId,
										   WFMath::Point<3> pos,
										   WFMath::Quaternion orientation,
										   std::function<void(const std::vector<WFMath::AxisBox<2>>&)> callback) :
		mTerrain(terrain),
		mId(terrainModId),
		mPosition(pos),
		mOrientation(orientation),
		mTranslator(std::move(terrainModTranslator)),
		mCallback(std::move(callback)) {

}

void TerrainModUpdateTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context) {
	std::unique_ptr<Mercator::TerrainMod> terrainMod;
	if (mTranslator->isValid()) {

		Mercator::Segment* segment = mTerrain.getSegmentAtPos((float)mPosition.x(), (float)mPosition.z());
		if (segment) {

			WFMath::Point<3> modPos = mPosition;
			float height;

			//If there's no mods we can just use position right away
			if (segment->getMods().empty()) {
				if (!segment->isValid()) {
					segment->populate();
				}
				segment->getHeight((float)modPos.x() - (float)(segment->getXRef()), (float)modPos.z() - (float)(segment->getZRef()), height);
			} else {
				Mercator::HeightMap heightMap(segment->getResolution());
				heightMap.allocate();
				segment->populateHeightMap(heightMap);

				heightMap.getHeight((float)modPos.x() - (float)(segment->getXRef()), (float)modPos.z() - (float)(segment->getZRef()), height);
			}
			modPos.y() = height;

			terrainMod = mTranslator->parseData(modPos, mOrientation);
		}

	}
	if (terrainMod && terrainMod->bbox().isValid()) {
		mUpdatedAreas.push_back(terrainMod->bbox());
	}
	auto oldAreas = mTerrain.updateMod(mId, std::move(terrainMod));
	if (oldAreas.isValid()) {
		mUpdatedAreas.push_back(oldAreas);
	}
}

bool TerrainModUpdateTask::executeTaskInMainThread() {
	if (mCallback) {
		mCallback(mUpdatedAreas);
	}
	return true;
}
}

}
}
