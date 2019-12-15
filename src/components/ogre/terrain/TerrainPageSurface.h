//
// C++ Interface: TerrainPageSurface
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRETERRAINPAGESURFACE_H
#define EMBEROGRETERRAINPAGESURFACE_H

#include "../EmberOgrePrerequisites.h"
#include "Types.h"
#include "TerrainPage.h"
#include <OgreMaterial.h>

#include <map>

namespace Mercator
{
class Shader;
class Surface;
class Segment;
}

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

class TerrainPageSurfaceLayer;
class TerrainPage;
class TerrainPageSurfaceCompiler;
class TerrainPageShadow;
class TerrainPageGeometry;
struct TerrainLayerDefinition;
class TerrainPageSurfaceCompilationInstance;
struct ICompilerTechniqueProvider;

/**
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class TerrainPageSurface
{
public:
	typedef std::map<int, std::unique_ptr<TerrainPageSurfaceLayer>> TerrainPageSurfaceLayerStore;
	/**
	 * @brief Ctor.
	 *
	 * @param terrainPage The terrain page to which this surface belongs.
	 * @param compilerTechniqueProvider Provider for terrain surface compilation techniques.
	 */
	TerrainPageSurface(const TerrainPage& terrainPage, ICompilerTechniqueProvider& compilerTechniqueProvider);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainPageSurface();

	unsigned int getPixelWidth() const;

	/**
	 * @brief The position of the page in Worldforge space
	 * @return
	 */
	const TerrainPosition& getWFPosition() const;

	/**
	 * @brief The number of Mercator::Segments for each axis. I.e. the root of the total number of segments.
	 * @return
	 */
	int getNumberOfSegmentsPerAxis() const;

	void createSurfaceLayer(const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader);

	Ogre::MaterialPtr getMaterial() const;

	Ogre::MaterialPtr getCompositeMapMaterial() const;

	/**
	 * @brief Recompiles the material.
	 * @param reselectTechnique If true, we'll also see if we need to reselect the technique to use.
	 */
	TerrainPageSurfaceCompilationInstance* createSurfaceCompilationInstance(const TerrainPageGeometryPtr& geometry) const;

	const TerrainPageSurfaceLayerStore& getLayers() const;

	void updateLayer(TerrainPageGeometry& geometry, int layerIndex, bool repopulate);


protected:

	std::string mMaterialName;
	const TerrainPage& mTerrainPage;
	TerrainPageSurfaceLayerStore mLayers;
	std::unique_ptr<TerrainPageSurfaceCompiler> mSurfaceCompiler;
	Ogre::MaterialPtr mMaterial;
	Ogre::MaterialPtr mMaterialComposite;

};

}
}
}
#endif
