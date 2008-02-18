//
// C++ Interface: TerrainPageFoliage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRE_TERRAINTERRAINPAGEFOLIAGE_H
#define EMBEROGRE_TERRAINTERRAINPAGEFOLIAGE_H

#include <vector>
#include <OgreCommon.h>
#include <OgreSharedPtr.h>
#include <OgreDataStream.h>

namespace Ogre
{
class MemoryDataStream;
class Vector2;
}

namespace EmberOgre {

namespace Terrain {

class TerrainShader;
class TerrainGenerator;
class TerrainPageSurface;
class TerrainPage;

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageFoliage
{
public:
	/**
	A store of plant positions. We keep this in ogre space for performance reasons.
	*/
	typedef std::vector<Ogre::Vector2> PlantStore;
	typedef std::map<std::string, PlantStore> PlantStoreMap;

	TerrainPageFoliage(TerrainGenerator& generator, TerrainPage& page);
	virtual ~TerrainPageFoliage();

	void generatePlantPositions();
	void generateCoverageMap();
	
	const PlantStore& getPlants() const;
	void getPlantsForArea(Ogre::TRect<float> area, PlantStore& store);
	
protected:
	/**
	The positions of the plants. These are precalulcated and not changed.
	*/
	PlantStore mPlants;
	
	PlantStoreMap mPlantStores;
	
	TerrainGenerator& mGenerator;
	TerrainPage& mTerrainPage;
	/**
	*we need to create a new lookup image for where grass should be placed. This should be based on the core grass coverage image, but with all layers that are above it substracted. Thus grass won't show up on roads and fields.
	Note that this currently is disabled since it's faster to do the lookups through Mercator instead.
	*/
	Ogre::MemoryDataStream* mFoliageCoverageDataStream;
	Ogre::DataStreamPtr mFoliageCoverageDataStreamPtr;
	
	unsigned int mCoverageMapPixelWidth;

};

}

}

#endif
