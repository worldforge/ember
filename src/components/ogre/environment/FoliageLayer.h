//
// C++ Interface: FoliageLayer
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGELAYER_H
#define EMBEROGRE_ENVIRONMENTFOLIAGELAYER_H

#include "pagedgeometry/include/GrassLoader.h"
namespace PagedGeometry
{
	class PagedGeometry;
	class ColorMap;
}



namespace EmberOgre {

namespace Terrain
{
	class TerrainFoliageDefinition;
	class TerrainLayerDefinition;
}

namespace Environment {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class FoliageLayer : public PagedGeometry::GrassLayerBase
{
public:
    FoliageLayer(PagedGeometry::PagedGeometry *geom, PagedGeometry::GrassLoader<FoliageLayer> *ldr);

    virtual ~FoliageLayer();
	/**
	 *    Calculates the max number of grass instances for this layer.
	 * @param densityFactor The density factor set on the grass loader
	 * @param volume The volume, in world units, to fill
	 * @return The max number of grass instances to create.
	 */
	virtual unsigned int calculateMaxGrassCount(float densityFactor, float volume);
	
	Ogre::uint32 getColorAt(float x, float z);
	
	void configure(const Terrain::TerrainLayerDefinition* terrainLayerDefinition, const Terrain::TerrainFoliageDefinition* foliageDefinition);
	
protected: 
	friend class PagedGeometry::GrassLoader<FoliageLayer>;

	//Used by GrassLoader::loadPage() - populates an array with grass.
	//Returns the final number of grasses, which will always be <= grassCount
	virtual unsigned int _populateGrassList(PagedGeometry::PageInfo page, float *posBuff, unsigned int grassCount);
	PagedGeometry::GrassLoader<FoliageLayer> *parent;
	
	const Terrain::TerrainLayerDefinition* mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition* mFoliageDefinition;
	float mDensity;
	
};
}

}

#endif
