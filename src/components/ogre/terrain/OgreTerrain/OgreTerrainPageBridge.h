/*
 Copyright (C) 2013 Samuel Kogler

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

#ifndef OGRETERRAINPAGEBRIDGE_H_
#define OGRETERRAINPAGEBRIDGE_H_

#include "../ITerrainPageBridge.h"
#include <OgrePrerequisites.h>
#include <boost/shared_array.hpp>

namespace Ogre
{
class TerrainGroup;
}

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

//TODO SK: implement
/**
 * @brief Updates Mercator terrain data based on data provided by the Ogre Terrain component.
 */
class OgreTerrainPageBridge: public Ember::OgreView::Terrain::ITerrainPageBridge
{
public:
	typedef std::pair<unsigned int, unsigned int> UnsignedIndexType;

	OgreTerrainPageBridge(Ogre::TerrainGroup& terrainGroup, const boost::shared_array<Ogre::Real>& heightData, size_t heightDataSize, UnsignedIndexType index);

	virtual ~OgreTerrainPageBridge();

	virtual void updateTerrain(TerrainPageGeometry& geometry);

	virtual void terrainPageReady();

private:
	Ogre::TerrainGroup& mTerrainGroup;

	boost::shared_array<Ogre::Real> mHeightData;

	size_t mHeightDataSize;

	UnsignedIndexType mIndex;
};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* OGRETERRAINPAGEBRIDGE_H_ */
