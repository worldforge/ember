//
// C++ Interface: FoliageBase
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H
#define EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H

#include "../terrain/TerrainGenerator.h"

#include <sigc++/trackable.h>

#include <set>
#include <vector>


namespace Forests {
class PagedGeometry;
}

namespace EmberOgre {

namespace Terrain
{
	class TerrainArea;
	class TerrainFoliageDefinition;
	class TerrainLayerDefinition;
	class TerrainPage;
	class TerrainShader;
}

namespace Environment {

/**
	@brief Base class for all foliage layers.
	
	The foliage is composed of many different "layers", for example grass, ferns, flowers, reeds etc. This serves as a base class for every such layer.
	Ember should by default come with some basic layer implementations, but if you want to add additional layer types, this is the class to base them on. See the existing layer classes for how it's best done.
	All layers are handled by the Foliage class, which acts as a manager. The layers themselves are defined in an xml file and handled by the TerrainLayerDefinitionManager instance. A subset of the terrain layer definition is any optional foliage, and the implementation of how that is to be handled is taken care of by subclasses of this class.
	
	An instance of this class will contain an instance of PagedGeometry, which is the main entry class for a paged geometry instance. All interactions with the PagedGeometry should therefore mainly go through this class or subclasses of it.
	@see Foliage
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class FoliageBase : public sigc::trackable
{
public:
	/**
	@brief A store of terrain layer definition.
	*/
	typedef std::vector<const Terrain::TerrainLayerDefinition*> TerrainLayerDefinitionStore;

	/**
	 * @brief Ctor.
	 * Be sure to call initialize() after you've created an instance to properly set it up.
	 * @param terrainLayerDefinition The terrain layer definition which is to used for generation this layer. This might contain some info needed, but the bulk of the data to be used in setting up this layer will probably be found in the foliageDefinition argument instead.
	 * @param foliageDefinition The foliage definition which is to be used for generation of this layer. This should contain all info needed for properly setting up the layer.
	 */
	FoliageBase(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition);
	/**
	 * @brief Dtor. This will also delete the main PagedGeomtry instance held by this class.
	 */
	virtual ~FoliageBase();
	
	virtual void initialize() = 0;

	virtual void frameStarted(const Ogre::FrameEvent & evt) = 0;

protected:

	const Terrain::TerrainLayerDefinition& mTerrainLayerDefinition;
	const Terrain::TerrainFoliageDefinition& mFoliageDefinition;
	::Forests::PagedGeometry* mPagedGeometry;
	
	TerrainLayerDefinitionStore mDependentDefinitions;
	
	void initializeDependentLayers();
	void TerrainGenerator_LayerUpdated(const Terrain::TerrainShader* shader, ::EmberOgre::Terrain::TerrainGenerator::AreaStore* areas);
	void TerrainGenerator_EventShaderCreated(const Terrain::TerrainShader* shader);
	void TerrainGenerator_AfterTerrainUpdate(std::vector< ::EmberOgre::TerrainPosition >& terrainPositions, std::set< ::EmberOgre::Terrain::TerrainPage* >& pages);

};

float getTerrainHeight(float x, float z, void* userData = 0);

}

}

#endif
