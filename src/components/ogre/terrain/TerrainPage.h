//
// C++ Interface: TerrainPage
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGRETERRAINPAGE_H
#define EMBEROGRETERRAINPAGE_H

#include "../EmberOgrePrerequisites.h"
#include "domain/Types.h"

#include <wfmath/point.h>
#include <wfmath/axisbox.h>

#include <vector>
#include <cmath>

namespace WFMath
{
template<int> class Vector;
}

namespace Mercator
{
class Segment;
}

namespace Ogre
{
class TerrainOptions;
}

namespace Ember
{
namespace OgreView
{
namespace Terrain
{
class TerrainShader;
class TerrainPageSurface;
class TerrainPage;
class TerrainPageSurfaceLayer;
class TerrainPageGeometry;
class ICompilerTechniqueProvider;

/**

 @brief Represents one terrain page.

 This is a bridge class between one Ogre terrain page instance and one or many Mercator::Segment. Since each Segment is 64x64 meters, but one Ogre page often is much larger, we need to combine many Segments for every single Ogre page.
 An instance of this is very light weight as it doesn't keep any geometry data around. Each time an action involving page geometry is to occur an instance of TerrainPageGeometry must therefore be created.

 Instances of this is created by TerrainManager.

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class TerrainPage
{
	friend class TerrainPageShadow;
	friend class ITerrainPageShadowTechnique;
public:

	/**
	 * @brief Ctor.
	 * @param index The page index in WF space.
	 * @param pageSize The size of one side of a page.
	 * @param compilerTechniqueProvider A provider for terrain surface compiler techniques.
	 */
	TerrainPage(const TerrainIndex& index, int pageSize, ICompilerTechniqueProvider& compilerTechniqueProvider);

	/**
	 * @brief Dtor.
	 */
	~TerrainPage();

	/**
	 * @brief The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 * Mainly used to check if it's 1, in which case everything becomes much easier.
	 * @return
	 */
	int getNumberOfSegmentsPerAxis() const;

	/**
	 * @brief The total number of vertices used for this page.
	 * @return Number of vertices.
	 */
	int getVerticeCount() const;

	/**
	 * @brief The index position of the page in Worldforge space.
	 * @return The index position of the page.
	 */
	const TerrainPosition& getWFPosition() const;

	/**
	 * @brief The index of the page in the Worldforge space.
	 * @return The index of the page.
	 */
	const TerrainIndex& getWFIndex() const;

	/**
	 * @brief The material used for the page.
	 * @return
	 */
	const Ogre::MaterialPtr getMaterial() const;

	/**
	 * @brief The material used for rendering a page's composite map. The composite map is a precomputed version of the terrain texture(with blending, maybe shadows etc) which can be used to render far away pages faster.
	 * @return The composite map material.
	 */
	const Ogre::MaterialPtr getCompositeMapMaterial() const;

	/**
	 * @brief Adds a shader to the page, meaning that it will be used in rendering.
	 * @param shader The new shader to add.
	 */
	TerrainPageSurfaceLayer* addShader(const TerrainShader* shader);

	/**
	 * @brief Updates the shader texture for the specific shader.
	 * @param shader The shader to update.
	 */
	TerrainPageSurfaceLayer* updateShaderTexture(const TerrainShader* shader, TerrainPageGeometry& geometry, bool repopulate = true);

	/**
	 * @brief Updates all the shader textures of the page.
	 * You should usually call this after you've made a change to the terrain and already have called populateSurfaces().
	 */
	void updateAllShaderTextures(TerrainPageGeometry& geometry, bool repopulate = true);

	int getPageSize() const;

	/**
	 * @brief The size in pixels of one side of the blend map.
	 * @return
	 */
	int getBlendMapSize() const;

	/**
	 * @brief Gets the extent of this page in meters, in worldforge space.
	 * @return
	 */
	const WFMath::AxisBox<2>& getWorldExtent() const;

	/**
	 * @brief Accessor for the page surface belonging to this page.
	 * @returns The page surface instance belonging to this page.
	 */
	const TerrainPageSurface* getSurface() const;

	/**
	 * @brief Gets the normal at the specified local position.
	 * @param localPosition The position, local to the page.
	 * @param normal The normal will be placed here.
	 * @return True if a valid normal could be found at the specified position.
	 */
	bool getNormal(const TerrainPosition& localPosition, WFMath::Vector<3>& normal) const;

private:

	/**
	 * @brief Page index.
	 */
	TerrainIndex mIndex;

	/**
	 * @brief The size of one side of the page.
	 */
	int mPageSize;

	/**
	 * @brief Internal position
	 */
	TerrainPosition mPosition;

	std::unique_ptr<TerrainPageSurface> mTerrainSurface;

	/**
	 * @brief The extent of this page in meters, in WF space.
	 */
	const WFMath::AxisBox<2> mExtent;

	/**
	 * @brief How much to scale the blend map. This is done to avoid pixelated terrain (a blur filter is applied).
	 * This value is taken from the config file.
	 */
	unsigned int getBlendMapScale() const;
};

inline int TerrainPage::getBlendMapSize() const
{
	//While the page size calculates the size in number of vertices, the blend map should use pixel size, which is one less.
	return (getPageSize() - 1);
}
}
}

}

#endif
