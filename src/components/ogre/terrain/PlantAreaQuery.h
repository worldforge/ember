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

#ifndef PLANTAREAQUERY_H_
#define PLANTAREAQUERY_H_

#include <string>
#include <vector>

#include <OgreCommon.h>
#include <OgreMemoryAllocatorConfig.h>
#include <OgreVector2.h>

namespace Ogre
{
class MemoryDataStream;
}

namespace EmberOgre {

namespace Terrain {

class TerrainLayerDefinition;

class PlantAreaQuery
{
public:

	PlantAreaQuery(const TerrainLayerDefinition& layerDef, const std::string& plantType, Ogre::TRect<float> area, Ogre::Vector2 center);

	const TerrainLayerDefinition& getLayerDef() const;
	const std::string& getPlantType() const;
	const Ogre::TRect<float>& getArea() const;
	const Ogre::Vector2& getCenter() const;
private:
	const TerrainLayerDefinition& mLayerDef;
	const std::string& mPlantType;
	Ogre::Vector2 mCenter;
	Ogre::TRect<float> mArea;
};
}
}

#endif /* PLANTAREAQUERY_H_ */
