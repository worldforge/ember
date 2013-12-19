//
// C++ Interface: TerrainPageSurfaceCompilerTechniqueSimple
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRETERRAINTECHNIQUESSIMPLE_H
#define EMBEROGRETERRAINTECHNIQUESSIMPLE_H

#include "Base.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/terrain/TerrainPage.h"
#include "components/ogre/terrain/OgreImage.h"

namespace Ember {
namespace OgreView {

namespace Terrain {

namespace Techniques {

/**
	@brief A simple technique, using only the fixed function pipeline.
	This is suitable for older cards which don't have shader support. Note that memory usage will increase and performance will degrade compared to the shader enabled version.

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Simple : public Base
{
public:

	/**
	 * @brief Used for preparing layer data in the background, to be used in the foreground when compiling the material.
	 */
	struct Layer {
		const TerrainPageSurfaceLayer& surfaceLayer;
		OgreImage* blendMap;
	};

	Simple(const TerrainPageGeometryPtr& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow);
	virtual ~Simple();

    virtual bool prepareMaterial();
    virtual bool compileMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const;
	virtual bool compileCompositeMapMaterial(Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const;

protected:

	/**
	 * @brief The layers that are active for the geometry.
	 *
	 * This is prepared in the background.
	 */
	std::list<Layer> mLayers;

	Ogre::Pass* addPassToTechnique(const TerrainPageGeometry& geometry, Ogre::Technique* technique, const Layer& layer, std::set<std::string>& managedTextures) const;
	void addShadow(Ogre::Technique* technique, const TerrainPageShadow* terrainPageShadow, Ogre::MaterialPtr material, std::set<std::string>& managedTextures) const;

	Ogre::TexturePtr updateShadowTexture(Ogre::MaterialPtr material, const TerrainPageShadow* terrainPageShadow, std::set<std::string>& managedTextures) const;



};

}

}

}

}

#endif
