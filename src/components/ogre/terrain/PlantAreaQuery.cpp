/*
    Copyright (C) 2009 Erik Hjortsberg

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

#include "PlantAreaQuery.h"
#include "TerrainLayerDefinition.h"


namespace EmberOgre {

namespace Terrain {


PlantAreaQuery::PlantAreaQuery(const TerrainLayerDefinition& layerDef, unsigned char threshold, const std::string& plantType, Ogre::TRect<float> area, PlantStore& store)
: mLayerDef(layerDef), mThreshold(threshold), mPlantType(plantType), mArea(area), mStore(store)
{
}

const TerrainLayerDefinition& PlantAreaQuery::getLayerDef() const
{
	return mLayerDef;
}
unsigned char PlantAreaQuery::getThreshold() const
{
	return mThreshold;
}

const std::string& PlantAreaQuery::getPlantType() const
{
	return mPlantType;
}

const Ogre::TRect<float>& PlantAreaQuery::getArea() const
{
	return mArea;
}

const PlantAreaQuery::PlantStore& PlantAreaQuery::getStore() const
{
	return mStore;
}

}

}
