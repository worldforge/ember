//
// C++ Implementation: EmberTerrainPageBridge
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EmberTerrainPageBridge.h"
#include "EmberPagingLandScapeData2D_HeightField.h"
#include "terrain/TerrainPage.h"
#include "terrain/TerrainPageGeometry.h"


namespace EmberOgre {

EmberTerrainPageBridge::EmberTerrainPageBridge(EmberPagingLandScapeData2D_HeightField& heightField)
: ITerrainPageBridge(), mHeightField(heightField)
{
}


EmberTerrainPageBridge::~EmberTerrainPageBridge()
{
	if (mTerrainPage) {
		mTerrainPage->unregisterBridge();
	}
}

void EmberTerrainPageBridge::updateTerrain(Terrain::TerrainPageGeometry& geometry)
{
	geometry.updateOgreHeightData(mHeightField.getHeightData());
	mHeightField.setMaxHeight(geometry.getMaxHeight());
}

void EmberTerrainPageBridge::terrainPageReady()
{
	mHeightField.eventTerrainPageLoaded();
}

}
