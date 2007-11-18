//
// C++ Implementation: TerrainPageSurfaceCompiler
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainPageSurfaceCompiler.h"
#include "TerrainPageShadow.h"
#include "TerrainPageSurfaceLayer.h"
#include "TerrainPageSurfaceCompilerTechniqueShader.h"
#include "TerrainPageSurfaceCompilerTechniqueSimple.h"

namespace EmberOgre {
namespace Terrain {

TerrainPageSurfaceCompiler::TerrainPageSurfaceCompiler()
{
	selectTechnique();
}


TerrainPageSurfaceCompiler::~TerrainPageSurfaceCompiler()
{
}

void TerrainPageSurfaceCompiler::compileMaterial(Ogre::MaterialPtr material, std::map<int, TerrainPageSurfaceLayer*>& terrainPageSurfaces, TerrainPageShadow* terrainPageShadow, TerrainPage& page)
{
	mTechnique->setPage(&page);
	mTechnique->compileMaterial(material, terrainPageSurfaces, terrainPageShadow);
}

void TerrainPageSurfaceCompiler::selectTechnique()
{
	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueShader());
	
//	mTechnique = std::auto_ptr<TerrainPageSurfaceCompilerTechnique>(new TerrainPageSurfaceCompilerTechniqueSimple());
}



}
}
