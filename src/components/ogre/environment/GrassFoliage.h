//
// C++ Interface: GrassFoliage
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
#ifndef EMBEROGRE_ENVIRONMENTGRASSFOLIAGE_H
#define EMBEROGRE_ENVIRONMENTGRASSFOLIAGE_H

#include "FoliageBase.h"

#include "pagedgeometry/include/GrassLoader.h"

namespace Forests {
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

class FoliageLayer;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class GrassFoliage : public FoliageBase
{
public:
	GrassFoliage(const Terrain::TerrainLayerDefinition& terrainLayerDefinition, const Terrain::TerrainFoliageDefinition& foliageDefinition);
	virtual ~GrassFoliage();
	
	virtual void initialize();
	virtual void frameStarted(const Ogre::FrameEvent & evt);

protected:
	
	::Forests::PagedGeometry* mGrass;
	::Forests::GrassLoader<FoliageLayer>* mGrassLoader;
	
	float mMinHeight;
	float mMaxHeight;
	float mMinWidth;
	float mMaxWidth;

};

}

}

#endif
