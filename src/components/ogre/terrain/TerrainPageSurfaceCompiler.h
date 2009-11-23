//
// C++ Interface: TerrainPageSurfaceCompiler
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
#ifndef EMBEROGRETERRAINPAGESURFACECOMPILER_H
#define EMBEROGRETERRAINPAGESURFACECOMPILER_H

#include "../EmberOgrePrerequisites.h"
#include <memory>
#include <map>
#include <OgreMaterial.h>

namespace EmberOgre {
namespace Terrain {

class TerrainPageSurfaceLayer;
class TerrainPageShadow;
class TerrainPage;
class TerrainPageGeometry;

class TerrainPageSurfaceCompilerTechnique
{
public:
	virtual ~TerrainPageSurfaceCompilerTechnique() {}
    virtual bool compileMaterial(const TerrainPageGeometry& geometry, Ogre::MaterialPtr material, std::map<int, const TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow) = 0;
    virtual void setPage(const TerrainPage* page) = 0;

protected:
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainPageSurfaceCompiler{
public:
    TerrainPageSurfaceCompiler();

    virtual ~TerrainPageSurfaceCompiler();

    void compileMaterial(const TerrainPageGeometry& geometry, Ogre::MaterialPtr material, std::map<int, const TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, const TerrainPage& page);

private:

	void selectTechnique();
	void fallback(const TerrainPageGeometry& geometry, Ogre::MaterialPtr material, std::map<int, const TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, const TerrainPage& page);
	std::auto_ptr<TerrainPageSurfaceCompilerTechnique> mTechnique;

};

}
}

#endif
