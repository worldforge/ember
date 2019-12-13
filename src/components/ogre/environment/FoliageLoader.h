//
// C++ Interface: FoliageLoader
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGELOADER_H
#define EMBEROGRE_ENVIRONMENTFOLIAGELOADER_H

#include "pagedgeometry/include/PagedGeometry.h"
#include <sigc++/trackable.h>

namespace Ogre {
class Entity;

class SceneManager;
}

namespace Ember {
namespace OgreView {

namespace Terrain {
class TerrainFoliageDefinition;

class TerrainLayerDefinition;

class TerrainManager;

class PlantAreaQueryResult;
}

namespace Environment {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class FoliageLoader : public ::Forests::PageLoader, public virtual sigc::trackable {
public:
	FoliageLoader(Ogre::SceneManager& sceneMgr,
				  Terrain::TerrainManager& terrainManager,
				  const Terrain::TerrainLayerDefinition& terrainLayerDefinition,
				  const Terrain::TerrainFoliageDefinition& foliageDefinition,
				  ::Forests::PagedGeometry& pagedGeometry);

	~FoliageLoader() override;

	void loadPage(::Forests::PageInfo& page) override;

	bool preparePage(::Forests::PageInfo& page) override;


	/** 
	 * @brief Sets the global density factor for this loader.
	 * This function can be used to increase or decrease the density of the foliage
	 * handled by this loader by a percentage. eg. 0.1 causes density of foliage to 
	 * go down to 0.1 * default density. 1 refers to full density and 0 refers to
	 * no foliage.
	 * @param density The new density for this foliage loader. 
	*/
	void setDensityFactor(float density);

protected:
	Terrain::TerrainManager& mTerrainManager;
	const Terrain::TerrainLayerDefinition& mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition& mFoliageDefinition;
	::Forests::PagedGeometry& mPagedGeometry;

	Ogre::Entity* mEntity;

	float mMinScale, mMaxScale;

	const Terrain::PlantAreaQueryResult* mLatestPlantsResult;

	void plantQueryExecuted(const Terrain::PlantAreaQueryResult& queryResult);

	/**
	 * The density factor used by this loader to determine the density of the foliage
	 * loaded by it.
	 */
	float mDensityFactor;

};

}

}

}

#endif
