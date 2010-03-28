//
// C++ Interface: EmberTerrainPageBridge
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
#ifndef EMBEROGREEMBERTERRAINPAGEBRIDGE_H
#define EMBEROGREEMBERTERRAINPAGEBRIDGE_H

#include "terrain/ITerrainPageBridge.h"

#include <boost/shared_array.hpp>

namespace Ogre
{
class PagingLandScapeData2DManager;
}

namespace EmberOgre {

class EmberPagingLandScapeData2D_HeightField;

/**
	Acts as a bridge between an instance of TerrainPage and EmberPagingLandScapeData2D_HeightField, making sure that the former is correctly updated when the terrain changes.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class EmberTerrainPageBridge : public ::EmberOgre::Terrain::ITerrainPageBridge
{
public:
	typedef std::pair<unsigned int, unsigned int> UnsignedIndexType;
	EmberTerrainPageBridge(Ogre::PagingLandScapeData2DManager& data2dManager, const boost::shared_array<float>& heightData, UnsignedIndexType index);

	virtual ~EmberTerrainPageBridge();

	void updateTerrain(Terrain::TerrainPageGeometry& geometry);

	void terrainPageReady();

protected:

	Ogre::PagingLandScapeData2DManager& mData2dManager;

	boost::shared_array<float> mHeightData;

	UnsignedIndexType mIndex;

	float mMaxHeight;

	EmberPagingLandScapeData2D_HeightField* getData2D();
};

}

#endif
