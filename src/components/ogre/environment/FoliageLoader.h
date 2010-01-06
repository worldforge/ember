//
// C++ Interface: FoliageLoader
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGELOADER_H
#define EMBEROGRE_ENVIRONMENTFOLIAGELOADER_H

#include "pagedgeometry/include/PagedGeometry.h"

namespace Ogre
{
class Entity;
class SceneManager;
}

namespace EmberOgre {

namespace Terrain
{
class TerrainFoliageDefinition;
class TerrainLayerDefinition;
class TerrainManager;
class PlantAreaQueryResult;
}

namespace Environment {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class FoliageLoader : public ::Forests::PageLoader
{
public:
    FoliageLoader(Ogre::SceneManager& sceneMgr, Terrain::TerrainManager& terrainManager, const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition, ::Forests::PagedGeometry& pagedGeometry);

    virtual ~FoliageLoader();

	virtual void loadPage(::Forests::PageInfo &page);

protected:
	Terrain::TerrainManager& mTerrainManager;
	const Terrain::TerrainLayerDefinition& mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition& mFoliageDefinition;
	::Forests::PagedGeometry& mPagedGeometry;
	
	Ogre::Entity* mEntity;
	
	float mMinScale, mMaxScale;

	const Terrain::PlantAreaQueryResult* mLatestPlantsResult;

	void plantQueryExecuted(const Terrain::PlantAreaQueryResult& queryResult);

};

}

}

#endif
