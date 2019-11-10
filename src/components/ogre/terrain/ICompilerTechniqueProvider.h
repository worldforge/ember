//
// C++ Interface: ITerrainMaterialFinalizer
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2010
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
#ifndef EMBER_OGREVIEW_ITERRAINMATERIALFINALIZER_H
#define EMBER_OGREVIEW_ITERRAINMATERIALFINALIZER_H

#include "components/ogre/terrain/Types.h"

namespace Ember {
namespace OgreView {
namespace Terrain {

struct TerrainPageSurfaceCompilerTechnique;

class TerrainPageShadow;

/**
 *
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Implementation of this interface are tasked with creating new compilation techniques for terrain page surfaces.
 *
 * These techniques often are tightly bound to both Ogre and SceneManager instances, which is the main reason this is provided as an interface.
 */
struct ICompilerTechniqueProvider {

	/**
	 * @brief Dtor.
	 */
	virtual ~ICompilerTechniqueProvider() = default;

	/**
	 * @brief Creates a new compiler technique.
	 *
	 * @param geometry The geometry to compile a material for.
	 * @param terrainPageSurfaces The surfaces attached to the geometry.
	 * @param terrainPageShadow An optional shadow for the geometry.
	 */
	virtual TerrainPageSurfaceCompilerTechnique* createTechnique(const TerrainPageGeometryPtr& geometry,
																 const SurfaceLayerStore& terrainPageSurfaces,
																 const TerrainPageShadow* terrainPageShadow) const = 0;

};


}
}
}
#endif
