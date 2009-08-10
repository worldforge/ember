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

namespace Ogre
{
class MemoryDataStream;
class Vector2;
}

namespace EmberOgre {

namespace Terrain {

class TerrainLayerDefinition;

class PlantAreaQuery
{
public:
	/**
	A store of plant positions. We keep this in ogre space for performance reasons.
	*/
	typedef std::vector<Ogre::Vector2, Ogre::STLAllocator<Ogre::Vector2, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GEOMETRY> > > PlantStore;

	PlantAreaQuery(const TerrainLayerDefinition& layerDef, unsigned char threshold, const std::string& plantType, Ogre::TRect<float> area, PlantStore& store);

	const TerrainLayerDefinition& getLayerDef() const;
	unsigned char getThreshold() const;
	const std::string& getPlantType() const;
	const Ogre::TRect<float>& getArea() const;
	const PlantStore& getStore() const;
private:
	const TerrainLayerDefinition& mLayerDef;
	unsigned char mThreshold;
	const std::string& mPlantType;
	Ogre::TRect<float> mArea;
	PlantStore& mStore;
};
}
}

#endif /* PLANTAREAQUERY_H_ */
