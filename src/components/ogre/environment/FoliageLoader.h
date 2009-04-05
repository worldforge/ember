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
}

namespace EmberOgre {

namespace Terrain
{
class TerrainFoliageDefinition;
class TerrainLayerDefinition;
}

namespace Environment {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class FoliageLoader : public ::PagedGeometry::PageLoader
{
public:
    FoliageLoader(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition);

    virtual ~FoliageLoader();

	virtual void loadPage(::PagedGeometry::PageInfo &page);

protected:

	const Terrain::TerrainLayerDefinition& mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition& mFoliageDefinition;
	
	Ogre::Entity* mEntity;
	
	float mMinScale, mMaxScale;

};

}

}

#endif
