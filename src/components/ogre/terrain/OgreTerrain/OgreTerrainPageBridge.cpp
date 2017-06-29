/*
 Copyright (C) 2013 Samuel Kogler

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

#include "OgreTerrainPageBridge.h"

#include "../TerrainPageGeometry.h"
#include "framework/LoggingInstance.h"
#include "EmberTerrain.h"

#include <Terrain/OgreTerrainGroup.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainPageBridge::OgreTerrainPageBridge(Ogre::TerrainGroup& terrainGroup, IndexType index) :
		mTerrainGroup(terrainGroup), mIndex(index)
{

}

OgreTerrainPageBridge::~OgreTerrainPageBridge()
{
	mHeightData.reset();
}

void OgreTerrainPageBridge::updateTerrain(TerrainPageGeometry& geometry)
{
	S_LOG_VERBOSE("Updating terrain page height data: [" << mIndex.first << "," << mIndex.second << "]");

	//Make a copy of the shared ptr, making sure it can't be deleted by the terrainPageReady() method
	auto heightData = mHeightData;

	if (!heightData) {
		heightData.reset(new float[mTerrainGroup.getTerrainSize() * mTerrainGroup.getTerrainSize()], std::default_delete<float[]>());
		geometry.updateOgreHeightData(heightData.get());
	}
	//If the mHeightData field has been reset by the terrainPageReady() method we'll now
	mHeightData = heightData;

}

void OgreTerrainPageBridge::terrainPageReady()
{
	S_LOG_INFO("Finished loading or updating terrain page geometry: [" << mIndex.first << "," << mIndex.second << "]");
	if (mHeightData) {
		auto terrain = static_cast<EmberTerrain*>(mTerrainGroup.getTerrain(mIndex.first, mIndex.second));
		if (terrain) {
			terrain->scheduleGeometryUpdate(mHeightData);
		} else {
			mTerrainGroup.defineTerrain(mIndex.first, mIndex.second, mHeightData.get());
		}

	}
	// No need to keep height data around since it is copied on call.
	// Note that the data won't be deleted until heightDataPtr runs out of scope, unless updateTerrain(...) is active at the same time.
	mHeightData.reset();
	// Notify waiting threads such as OgreTerrainDefiner
	mConditionVariable.notify_all();
}

bool OgreTerrainPageBridge::isPageShown() const
{
	auto terrain = mTerrainGroup.getTerrain(mIndex.first, mIndex.second);
	return terrain != nullptr && terrain->isLoaded();
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
