//
// C++ Interface: FoliageBase
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
#ifndef EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H
#define EMBEROGRE_ENVIRONMENTFOLIAGEBASE_H

#include "components/ogre/terrain/Types.h"
#include "components/ogre/OgreIncludes.h"

#include <sigc++/trackable.h>

#include <set>
#include <vector>
#include <components/ogre/terrain/TerrainLayerDefinition.h>
#include <components/ogre/terrain/TerrainShader.h>


namespace Forests {
class PagedGeometry;
}

namespace Ember {
namespace OgreView {

namespace Terrain {
class TerrainArea;

struct TerrainFoliageDefinition;

struct TerrainLayerDefinition;

class TerrainPage;

struct TerrainShader;

class TerrainManager;
}

namespace Environment {

/**
 * @brief Structure that can be used to store distance detail information for a single foliage page type.
 * @see Forests::PagedGeometry::addDetailLevel
 */
struct DistanceStore {
	float farDistance;
	float nearDistance;
	float transition;
};

/**
	@brief Base class for all foliage layers.
	
	The foliage is composed of many different "layers", for example grass, ferns, flowers, reeds etc. This serves as a base class for every such layer.
	Ember should by default come with some basic layer implementations, but if you want to add additional layer types, this is the class to base them on. See the existing layer classes for how it's best done.
	All layers are handled by the Foliage class, which acts as a manager. The layers themselves are defined in an xml file and handled by the TerrainLayerDefinitionManager instance. A subset of the terrain layer definition is any optional foliage, and the implementation of how that is to be handled is taken care of by subclasses of this class.
	
	An instance of this class will contain an instance of PagedGeometry, which is the main entry class for a paged geometry instance. All interactions with the PagedGeometry should therefore mainly go through this class or subclasses of it.
	@see Foliage
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class FoliageBase : public virtual sigc::trackable {
public:
	/**
	 * @brief Ctor.
	 * Be sure to call initialize() after you've created an instance to properly set it up.
	 * @param terrainLayerDefinition The terrain layer definition which is to used for generation this layer. This might contain some info needed, but the bulk of the data to be used in setting up this layer will probably be found in the foliageDefinition argument instead.
	 * @param foliageDefinition The foliage definition which is to be used for generation of this layer. This should contain all info needed for properly setting up the layer.
	 */
	FoliageBase(Terrain::TerrainManager& terrainManager,
				Terrain::TerrainLayer terrainLayer,
				Terrain::TerrainFoliageDefinition foliageDefinition);

	/**
	 * @brief Dtor. This will also delete the main PagedGeomtry instance held by this class.
	 */
	virtual ~FoliageBase();

	void reloadAtPosition(const WFMath::Point<2>& worldPosition);

	virtual void initialize() = 0;

	virtual void frameStarted() = 0;

	/**
	 * @brief Sets the density factor of the foliage.
	 * This affects the overall density of the foliage by a factor.
	 * eg. passing 0.1 will make the foliage density = (the default defined layer density) * 0.1;
	 * @param newDensity the new density factor in float, where 1 implies normal or full density and 0 implies no density.
	 * @note Should be overridden in the deriving foliage layer to provide ability to set the far distance.
	 */
	virtual void setDensity(float) {}

	/**
	 * @brief Sets the far distance factor of the foliage.
	 * This affects the far distance of the foliage by a factor.
	 * eg. passing 0.1 will make the furthest distance foliage appears at = default far distance * 0.1;
	 * @param factor the new far distance factor, where 1 implies normal or max far distance and 0 implies minimum far distance;
	 * @note Should be overridden in the deriving foliage layer to provide ability to set the far distance.
	 */
	virtual void setFarDistance(float) {};

protected:

	Terrain::TerrainManager& mTerrainManager;

	const Terrain::TerrainLayer mTerrainLayer;
	const Terrain::TerrainFoliageDefinition mFoliageDefinition;
	std::unique_ptr<::Forests::PagedGeometry> mPagedGeometry;

	/**
	 * A store of all distances for foliage detail levels added to this foliage.
	 * @see Forests::PagedGeometry::addDetailLevel
	 */
	std::list<DistanceStore> mDistanceStore;

	void TerrainHandler_LayerUpdated(const Terrain::TerrainShader& shader, const Terrain::AreaStore& areas);

	void TerrainHandler_EventShaderCreated(const Terrain::TerrainLayer&);

	void TerrainHandler_AfterTerrainUpdate(const std::vector<WFMath::AxisBox<2>>& areas,
										   const std::vector<std::shared_ptr<Terrain::TerrainPageGeometry>>& pages);

	void TerrainManager_TerrainShown(const std::vector<Ogre::TRect<Ogre::Real>>& areas);

};

float getTerrainHeight(float x, float z, void* userData = nullptr);

double getTerrainHeight(double x, double z, void* userData = nullptr);

}

}

}

#endif
