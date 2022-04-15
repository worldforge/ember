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

namespace Mercator {
class Shader;

class Surface;

class Segment;
}

namespace Ember {
namespace OgreView {
namespace Terrain {

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
class TerrainPageSurface {
public:
	typedef std::map<int, TerrainPageSurfaceLayer> TerrainPageSurfaceLayerStore;

	/**
	 * @brief Ctor.
	 *
	 * @param compilerTechniqueProvider Provider for terrain surface compilation techniques.
	 */
	TerrainPageSurface(const TerrainPosition& terrainPosition,
					   ICompilerTechniqueProvider& compilerTechniqueProvider);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainPageSurface();


	void createSurfaceLayer(const TerrainLayerDefinition& definition, int surfaceIndex, const Mercator::Shader& shader);

	Ogre::MaterialPtr getMaterial() const;

	Ogre::MaterialPtr getCompositeMapMaterial() const;

	/**
	 * @brief Recompiles the material.
	 * @param reselectTechnique If true, we'll also see if we need to reselect the technique to use.
	 */
	std::unique_ptr<TerrainPageSurfaceCompilationInstance> createSurfaceCompilationInstance(const TerrainPageGeometryPtr& geometry) const;

	const TerrainPageSurfaceLayerStore& getLayers() const;

	TerrainPageSurfaceLayerStore& getLayers() {
		return mLayers;
	}

	void updateLayer(TerrainPageGeometry& geometry, int layerIndex, bool repopulate);


protected:

	std::string mMaterialName;
	TerrainPageSurfaceLayerStore mLayers;
	std::unique_ptr<TerrainPageSurfaceCompiler> mSurfaceCompiler;
	Ogre::MaterialPtr mMaterial;
	Ogre::MaterialPtr mMaterialComposite;

};

}
}
}
#endif
