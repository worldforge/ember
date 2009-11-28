/*
 * Base.cpp
 *
 *  Created on: 26 nov 2009
 *      Author: erik
 */

#include "Base.h"

namespace EmberOgre
{

namespace Terrain
{

namespace Techniques
{

Base::Base(const TerrainPageGeometry& geometry, const SurfaceLayerStore& terrainPageSurfaces, const TerrainPageShadow* terrainPageShadow, const TerrainPage& page)
: mGeometry(geometry), mTerrainPageSurfaces(terrainPageSurfaces), mTerrainPageShadow(terrainPageShadow), mPage(page)
{
}

Base::~Base()
{
}

}

}

}
