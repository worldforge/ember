/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainShader.h"

#include "TerrainLayerDefinition.h"

#include <Mercator/Terrain.h>
#include <Mercator/Shader.h>
#include <Mercator/Segment.h>

namespace EmberOgre {
namespace Terrain {

TerrainShader::TerrainShader(Mercator::Terrain& terrain, int terrainIndex, const TerrainLayerDefinition& layerDef, Mercator::Shader* shader)
: mLayerDef(layerDef)
, mShader(shader)
, mTerrain(terrain)
, mTerrainIndex(terrainIndex)
{
	mTerrain.addShader(shader, mTerrainIndex);
}


TerrainShader::~TerrainShader()
{
	delete mShader;
	///not available yet
	//mTerrain->removeShader(mShader)
}

const Mercator::Shader& TerrainShader::getShader() const
{
	return *mShader;
}

const TerrainLayerDefinition& TerrainShader::getLayerDefinition() const
{
	return mLayerDef;
}

Mercator::Surface* TerrainShader::getSurfaceForSegment(Mercator::Segment& segment) const
{

	Mercator::Surface* surface = 0;
	if (segment.getSurfaces().find(getTerrainIndex()) != segment.getSurfaces().end()) {
		surface = segment.getSurfaces().find(getTerrainIndex())->second;
	}
	return surface;
}

}
}
