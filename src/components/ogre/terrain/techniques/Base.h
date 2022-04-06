/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef EMBEROGRETERRAINTECHNIQUESBASE_H_
#define EMBEROGRETERRAINTECHNIQUESBASE_H_

#include "components/ogre/terrain/TerrainPageSurfaceCompiler.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

namespace Techniques {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Abstract base class for terrain techniques.
 */
class Base : public TerrainPageSurfaceCompilerTechnique {
public:
	/**
	 * @brief Ctor.
	 * @param geometry The geometry to operate on.
	 * @param terrainPageSurfaces The surfaces to generate a rendering technique for.
	 * @param terrainPageShadow An optional shadow.
	 */
	Base(const TerrainPageGeometryPtr& geometry,
		 SurfaceLayerStore terrainPageSurfaces);

	/**
	 * @brief Dtor.
	 */
	~Base() override;

protected:

	/**
	 * @brief The geometry to operate on.
	 */
	TerrainPageGeometryPtr mGeometry;

	/**
	 * @brief The surfaces to generate a rendering technique for.
	 */
	const SurfaceLayerStore mTerrainPageSurfaces;

};

}

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESBASE_H_ */
