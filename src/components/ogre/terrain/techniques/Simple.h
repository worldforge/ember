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

namespace EmberOgre {

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

	Simple(const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page);

    virtual bool prepareMaterial();
    virtual bool compileMaterial(Ogre::MaterialPtr material);

protected:

	Ogre::Pass* addPassToTechnique(const TerrainPageGeometry& geometry, Ogre::Technique* technique, const TerrainPageSurfaceLayer* layer);
	void addShadow(Ogre::Technique* technique, const TerrainPageShadow* terrainPageShadow);


};

}

}

}

#endif
