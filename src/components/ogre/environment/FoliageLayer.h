//
// C++ Interface: FoliageLayer
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGELAYER_H
#define EMBEROGRE_ENVIRONMENTFOLIAGELAYER_H

#include "pagedgeometry/include/GrassLoader.h"

#include <sigc++/trackable.h>
#include <components/ogre/terrain/TerrainShader.h>

namespace Forests {
class PagedGeometry;

class ColorMap;
}


namespace Ember {
namespace OgreView {

namespace Terrain {
struct TerrainFoliageDefinition;
struct TerrainLayerDefinition;

class TerrainManager;

struct PlantAreaQueryResult;
}

namespace Environment {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class FoliageLayer : public Forests::GrassLayerBase, public virtual sigc::trackable {
public:
	FoliageLayer(Forests::PagedGeometry* geom, Forests::GrassLoader<FoliageLayer>* ldr);

	virtual ~FoliageLayer() = default;

	unsigned int prepareGrass(const Forests::PageInfo& page, float densityFactor, float volume, bool& isAvailable) override;

	Ogre::uint32 getColorAt(float x, float z);

	void configure(Terrain::TerrainManager* terrainManager,
				   const Terrain::TerrainLayer* terrainLayer,
				   const Terrain::TerrainFoliageDefinition* foliageDefinition);

	bool isColoursEnabled() const override;

	bool isNormalsEnabled() const override;

	bool isTangentsEnabled() const override;

	bool isCastShadowsEnabled() const override;

protected:
	friend class Forests::GrassLoader<FoliageLayer>;

	//Used by GrassLoader::loadPage() - populates an array with grass.
	//Returns the final number of grasses, which will always be <= grassCount
	unsigned int _populateGrassList(Forests::PageInfo page, float* posBuff, unsigned int grassCount) override;

	Forests::GrassLoader<FoliageLayer>* parent;

	Terrain::TerrainManager* mTerrainManager;
	const Terrain::TerrainLayer* mTerrainLayer;
	const Terrain::TerrainFoliageDefinition* mFoliageDefinition;
	float mDensity;

	const Terrain::PlantAreaQueryResult* mLatestPlantsResult;

	void plantQueryExecuted(const Terrain::PlantAreaQueryResult& queryResult);

};
}

}

}

#endif
