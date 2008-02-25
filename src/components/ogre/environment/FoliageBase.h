//
// C++ Interface: FoliageBase
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H
#define EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H

#include <OgreFrameListener.h>

namespace PagedGeometry {
class PagedGeometry;
}

namespace EmberOgre {

namespace Terrain
{
class TerrainFoliageDefinition;
class TerrainLayerDefinition;
class TerrainShader;
class TerrainArea;
}

namespace Environment {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class FoliageBase
{
public:
	typedef std::vector<const Terrain::TerrainLayerDefinition*> TerrainLayerDefinitionStore;

	FoliageBase(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition);
	virtual ~FoliageBase();
	
	virtual void frameStarted(const Ogre::FrameEvent & evt) {};
	
	virtual void initialize() = 0;

protected:

	const Terrain::TerrainLayerDefinition& mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition& mFoliageDefinition;
	::PagedGeometry::PagedGeometry* mPagedGeometry;
	
	TerrainLayerDefinitionStore mDependentDefinitions;
	
	void initializeDependentLayers();
	void TerrainGenerator_LayerUpdated(Terrain::TerrainShader* shader, std::vector<Terrain::TerrainArea*>* areas);

};

}

}

#endif
