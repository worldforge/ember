/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#ifndef EMBEROGRETERRAINTECHNIQUESBASE_H_
#define EMBEROGRETERRAINTECHNIQUESBASE_H_

#include "components/ogre/terrain/TerrainPageSurfaceCompiler.h"

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

namespace Techniques
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Abstract base class for terrain techniques.
 */
class Base : public TerrainPageSurfaceCompilerTechnique
{
public:
    /**
     * @brief Ctor.
     * @param geometry The geometry to operate on.
     * @param terrainPageSurfaces The surfaces to generate a rendering technique for.
     * @param terrainPageShadow An optional shadow.
     */
	Base(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow);

	/**
	 * @brief Dtor.
	 */
	virtual ~Base();

	virtual std::string getShadowTextureName(const Ogre::MaterialPtr& material) const
	{
		return "";
	}

	virtual bool requiresPregenShadow() const
	{
		return false;
	}



protected:

	/**
	 * @brief The geometry to operate on.
	 */
	TerrainPageGeometryPtr mGeometry;

	/**
	 * @brief The surfaces to generate a rendering technique for.
	 */
	const SurfaceLayerStore mTerrainPageSurfaces;

	/**
	 * @brief An optional shadow.
	 */
	TerrainPageShadow* mTerrainPageShadow;

	/**
	 * @brief The affected page.
	 * @note This can be obtained through mGeometry, but we'll keep a copy for the case that the mGeometry pointer is cleared.
	 */
	const TerrainPage& mPage;

};

}

}

}

}

#endif /* EMBEROGRETERRAINTECHNIQUESBASE_H_ */
