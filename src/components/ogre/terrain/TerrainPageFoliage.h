//
// C++ Interface: TerrainPageFoliage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#include <OgreCommon.h>
#include <OgreSharedPtr.h>
#include <OgreDataStream.h>
#include <OgreMemoryAllocatorConfig.h>

#include <string>
#include <map>
#include <vector>


namespace Ogre
{
class MemoryDataStream;
class Vector2;
}

namespace EmberOgre {

namespace Terrain {

class TerrainShader;
class TerrainManager;
class TerrainPageSurface;
class TerrainPage;
class TerrainPageFoliage;
class TerrainLayerDefinition;
class TerrainPageGeometry;
class PlantAreaQuery;
// struct PlantPosition
// {
// 	Ogre::Vector2 pos;
// 	unsigned char rotation;
// 	float scale;
// };

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageFoliage
{
public:
	/**
	A store of plant positions. We keep this in ogre space for performance reasons.
	*/
	typedef std::vector<Ogre::Vector2, Ogre::STLAllocator<Ogre::Vector2, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GEOMETRY> > > PlantStore;
	typedef std::map<int, PlantStore> PlantBatchColumn;
	typedef std::map<int, PlantBatchColumn> PlantBatchStore;
	typedef std::map<std::string, PlantBatchStore> PlantStoreMap;

	TerrainPageFoliage(TerrainManager& manager, TerrainPage& page);
	virtual ~TerrainPageFoliage();


	/**
	 *    Generates the plant positions for all registered plant types.
	 */
	void generatePlantPositions();


	/**
	 *    Regenerates the coverage map which can be used for quick lookup for plant probability.
	 */
	void generateCoverageMap();

	/**
	 *    Gets all plants.
	 * @return
	 */
	const PlantStoreMap& getPlants() const;

	/**
	 *    Place the plants for the supplied area in the supplied store.
	 * @param layer The layer which we should use as base for determining what plants to get.
	 * @param plantType The plant type.
	 * @param area The enclosing area.
	 * @param store The store in which to place the plants.
	 */
	void getPlantsForArea(const TerrainPageGeometry& geometry, PlantAreaQuery& query) const;

	TerrainPage& getTerrainPage() const;

	unsigned int getCoverageMapPixelWidth() const;

protected:
	/**
	The positions of the plants. These are precalulcated and not changed.
	*/
	PlantStoreMap mPlantStores;

	TerrainManager& mManager;
	TerrainPage& mTerrainPage;
	/**
	*we need to create a new lookup image for where grass should be placed. This should be based on the core grass coverage image, but with all layers that are above it substracted. Thus grass won't show up on roads and fields.
	Note that this currently is disabled since it's faster to do the lookups through Mercator instead.
	*/
	Ogre::MemoryDataStream* mFoliageCoverageDataStream;
	Ogre::DataStreamPtr mFoliageCoverageDataStreamPtr;

	unsigned int mCoverageMapPixelWidth;

	void setupBatches();

};

class PlantPopulator
{
public:

	PlantPopulator(const TerrainPageFoliage& terrainPageFoliage);
	virtual ~PlantPopulator();

	virtual void populate(TerrainPageFoliage::PlantBatchStore& plantBatchStore, int plantIndex, unsigned int batchSize) = 0;

protected:

	const TerrainPageFoliage& mTerrainPageFoliage;

};

class ClusterPopulator : public PlantPopulator
{
public:
	ClusterPopulator(const TerrainPageFoliage& terrainPageFoliage);
	virtual ~ClusterPopulator();

	virtual void populate(TerrainPageFoliage::PlantBatchStore& plantBatchStore, int plantIndex, unsigned int batchSize);

	void setMinClusterRadius ( float theValue );
	float getMinClusterRadius() const;

	void setMaxClusterRadius ( float theValue );
	float getMaxClusterRadius() const;

	void setDensity ( float theValue );
	float getDensity() const;

	void setFalloff ( float theValue );
	float getFalloff() const;

	void setClusterDistance ( float theValue );
	float getClusterDistance() const;

protected:

	float mMinClusterRadius;
	float mMaxClusterRadius;
	float mClusterDistance;
	float mDensity;
	float mFalloff;

};

}

}

#endif
