//
// C++ Interface: TerrainPageSurfaceCompilerTechniqueSimple
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#ifndef EMBEROGRE_TERRAINTERRAINPAGESURFACECOMPILERTECHNIQUESIMPLE_H
#define EMBEROGRE_TERRAINTERRAINPAGESURFACECOMPILERTECHNIQUESIMPLE_H

#include "../EmberOgrePrerequisites.h"
#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPage.h"

namespace EmberOgre {

namespace Terrain {

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageSurfaceCompilerTechniqueSimple : public TerrainPageSurfaceCompilerTechnique
{
public:

    virtual void compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow);
    virtual void setPage(TerrainPage* page);


protected:
	TerrainPage* mPage;

	Ogre::Pass* addPassToTechnique(Ogre::Technique* technique, TerrainPageSurfaceLayer* layer);
	void addShadow(Ogre::Technique* technique, TerrainPageShadow* terrainPageShadow);
		

};

}

}

#endif
