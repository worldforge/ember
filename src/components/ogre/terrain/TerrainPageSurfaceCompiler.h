//
// C++ Interface: TerrainPageSurfaceCompiler
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
#ifndef EMBEROGRETERRAINPAGESURFACECOMPILER_H
#define EMBEROGRETERRAINPAGESURFACECOMPILER_H

#include "../EmberOgrePrerequisites.h"

namespace EmberOgre {
namespace Terrain {

class TerrainPageSurfaceLayer;
class TerrainPageShadow;
class TerrainPage;

class TerrainPageSurfaceCompilerTechnique
{
public:
	virtual ~TerrainPageSurfaceCompilerTechnique() {}
    virtual void compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow) = 0;
    virtual void setPage(TerrainPage* page) = 0;

protected:
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class TerrainPageSurfaceCompiler{
public:
    TerrainPageSurfaceCompiler();

    virtual ~TerrainPageSurfaceCompiler();
    
    void compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, TerrainPage& page);
    
private:

	void selectTechnique();
	std::auto_ptr<TerrainPageSurfaceCompilerTechnique> mTechnique;

};

}
}

#endif
