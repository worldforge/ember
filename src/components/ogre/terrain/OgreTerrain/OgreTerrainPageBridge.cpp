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

#include <Terrain/OgreTerrainGroup.h>
#include "../TerrainPageGeometry.h"

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

OgreTerrainPageBridge::OgreTerrainPageBridge(Ogre::TerrainGroup& terrainGroup, const boost::shared_array<Ogre::Real>& heightData,
		size_t heightDataSize, UnsignedIndexType index) :
		mTerrainGroup(terrainGroup), mHeightData(heightData), mHeightDataSize(heightDataSize), mIndex(index)
{
}

OgreTerrainPageBridge::~OgreTerrainPageBridge()
{
}

void OgreTerrainPageBridge::updateTerrain(TerrainPageGeometry& geometry)
{
#if OGRE_DOUBLE_PRECISION
	//Mercator works with floats, so if we've built Ogre in double precision mode we need to send a float bitmap to Mercator, and then convert to double.
	std::vector<float> floatHeightData(mHeightDataSize);
	float* floatData = &floatHeightData.front();
	geometry.updateOgreHeightData(floatData);

	Ogre::Real* doubleData = mHeightData.get();
	for (size_t i = 0; i < mHeightDataSize; ++i) {
		doubleData[i] = (double)floatData[i];
	}
#else
	geometry.updateOgreHeightData(mHeightData.get());
#endif
}

void OgreTerrainPageBridge::terrainPageReady()
{
}

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
