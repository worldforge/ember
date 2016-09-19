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

#include "TerrainModAddTask.h"
#include "TerrainHandler.h"
#include "TerrainMod.h"
#include "components/terrain/TerrainModTranslator.h"
#include <Mercator/TerrainMod.h>
#include <Mercator/Terrain.h>
#include <Mercator/Segment.h>
#include <wfmath/axisbox.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

TerrainModAddTask::TerrainModAddTask(Mercator::Terrain& terrain, const TerrainMod& terrainMod, TerrainHandler& handler, TerrainModMap& terrainMods) :
		TerrainModTaskBase(terrain, terrainMod.getEntityId(), handler, terrainMods), mPosition(terrainMod.getEntity().getPosition()), mOrientation(terrainMod.getEntity().getOrientation()), mTranslator(*terrainMod.getTranslator())
{

}

void TerrainModAddTask::executeTaskInBackgroundThread(Tasks::TaskExecutionContext& context)
{
	const Mercator::TerrainMod* existingMod = mTerrain.getMod(mId);
	const Mercator::TerrainMod* terrainMod = nullptr;
	if (mTranslator.isValid()) {

		Mercator::Segment* segment = mTerrain.getSegment(mPosition.x(), mPosition.y());
		if (segment) {

			WFMath::Point<3> modPos = mPosition;

			//If there's no mods we can just use position right away
			if (segment->getMods().empty()) {
				if (!segment->isValid()) {
					segment->populate();
				}
				segment->getHeight(modPos.x() - (segment->getXRef()), modPos.y() - (segment->getYRef()), modPos.z());
			} else {
				Mercator::HeightMap heightMap(segment->getResolution(), segment->getMin(), segment->getMax());
				segment->populateHeightMap(heightMap);

				heightMap.getHeight(modPos.x() - (segment->getXRef()), modPos.y() - (segment->getYRef()), modPos.z());
			}

			terrainMod = mTranslator.parseData(modPos, mOrientation);
		}

	}

	mTerrain.updateMod(mId, terrainMod);
	if (terrainMod && terrainMod->bbox().isValid()) {
		mUpdatedAreas.push_back(terrainMod->bbox());
	}

	if (existingMod) {
		if (existingMod->bbox().isValid()) {
			mUpdatedAreas.push_back(existingMod->bbox());
		}
		delete existingMod;
	}

}

void TerrainModAddTask::executeTaskInMainThread()
{
	mHandler.reloadTerrain(mUpdatedAreas);
}
}

}
}
