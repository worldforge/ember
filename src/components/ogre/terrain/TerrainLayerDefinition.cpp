//
// C++ Implementation: TerrainLayerDefinition
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainLayerDefinition.h"

namespace EmberOgre {

namespace Terrain {

TerrainLayerDefinition::TerrainLayerDefinition()
: mAreaId(0), mTileSize(1.0f)
{
}


TerrainLayerDefinition::~TerrainLayerDefinition()
{
}

void TerrainLayerDefinition::setName(const std::string& name)
{
	mName = name;
}

const std::string& TerrainLayerDefinition::getName() const
{
	return mName;
}


void TerrainLayerDefinition::setAreaId(unsigned int areaId)
{
	mAreaId = areaId;
}
unsigned int TerrainLayerDefinition::getAreaId() const
{
	return mAreaId;
}

void TerrainLayerDefinition::setDiffuseTextureName(const std::string& textureName)
{
	mDiffuseTextureName = textureName;
}
const std::string& TerrainLayerDefinition::getDiffuseTextureName() const
{
	return mDiffuseTextureName;
}

void TerrainLayerDefinition::setNormalMapTextureName(const std::string& textureName)
{
	mNormalMapTextureName = textureName;
}
const std::string& TerrainLayerDefinition::getNormalMapTextureName() const
{
	return mNormalMapTextureName;
}

void TerrainLayerDefinition::setShaderName(const std::string& shaderName)
{
	mShaderName = shaderName;
}
const std::string& TerrainLayerDefinition::getShaderName() const
{
	return mShaderName;
}

void TerrainLayerDefinition::setTileSize(float tileSize)
{
	mTileSize = tileSize;
}
float TerrainLayerDefinition::getTileSize() const
{
	return mTileSize;
}

const TerrainLayerDefinition::StringParamStore& TerrainFoliageDefinition::getParameters() const
{
	return mParameters;
}

TerrainLayerDefinition::StringParamStore& TerrainFoliageDefinition::getParameters()
{
	return mParameters;
}

const std::string& TerrainFoliageDefinition::getParameter(const std::string & key) const
{
	static std::string empty("");
	TerrainLayerDefinition::StringParamStore::const_iterator I = mParameters.find(key);
	if (I != mParameters.end()) {
		return I->second;
	}
	return empty;
}

bool TerrainFoliageDefinition::hasParameter(const std::string& key) const
{
	return mParameters.find(key) != mParameters.end();
}

const std::string& TerrainFoliageDefinition::getPlantType() const
{
	return mPlantType;
}


void TerrainFoliageDefinition::setPlantType(const std::string& theValue)
{
	mPlantType = theValue;
}


const TerrainLayerDefinition::TerrainFoliageDefinitionStore& TerrainLayerDefinition::getFoliages() const
{
	return mFoliages;
}

TerrainLayerDefinition::TerrainFoliageDefinitionStore& TerrainLayerDefinition::getFoliages()
{
	return mFoliages;
}

const std::string& TerrainFoliageDefinition::getPopulationTechnique() const
{
	return mPopulationTechnique;
}

void TerrainFoliageDefinition::setPopulationTechnique(const std::string& theValue)
{
	mPopulationTechnique = theValue;
}

const std::string& TerrainFoliageDefinition::getRenderTechnique() const
{
	return mRenderTechnique;
}

void TerrainFoliageDefinition::setRenderTechnique(const std::string& theValue)
{
	mRenderTechnique = theValue;
}

}
}

