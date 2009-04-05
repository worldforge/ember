//
// C++ Interface: ShrubberyFoliage
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_ENVIRONMENTSHRUBBERYFOLIAGE_H
#define EMBEROGRE_ENVIRONMENTSHRUBBERYFOLIAGE_H

#include "FoliageBase.h"

namespace PagedGeometry {
class PagedGeometry;
}


namespace EmberOgre {

namespace Terrain
{
class TerrainFoliageDefinition;
class TerrainLayerDefinition;
class TerrainShader;
class TerrainArea;
}

namespace Environment {

class FoliageLoader;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class ShrubberyFoliage : public FoliageBase
{
public:
	ShrubberyFoliage(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition);
	virtual ~ShrubberyFoliage();
	
	virtual void frameStarted(const Ogre::FrameEvent & evt);
	
	virtual void initialize();

protected:
	FoliageLoader* mLoader;

};
}

}

#endif
