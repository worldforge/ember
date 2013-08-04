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

#include <Terrain/OgreTerrainGroup.h>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainPageBridge::OgreTerrainPageBridge(Ogre::TerrainGroup& terrainGroup, IndexType index) :
		mTerrainGroup(terrainGroup), mIndex(index), mHeightData(nullptr)
{

}

OgreTerrainPageBridge::~OgreTerrainPageBridge()
{
	delete[] mHeightData;
}

void OgreTerrainPageBridge::updateTerrain(TerrainPageGeometry& geometry)
{
	mHeightData = new float[mTerrainGroup.getTerrainSize() * mTerrainGroup.getTerrainSize()];
	geometry.updateOgreHeightData(mHeightData);
}

void OgreTerrainPageBridge::terrainPageReady()
{
	S_LOG_INFO("Finished loading terrain page geometry: [" << mIndex.first << "," << mIndex.second << "]");
	mTerrainGroup.defineTerrain(mIndex.first, mIndex.second, mHeightData);
	// No need to keep height data around since it is copied on call
	delete[] mHeightData;
	mHeightData = nullptr;

	// Notify waiting threads such as OgreTerrainDefiner
	mConditionVariable.notify_all();
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
