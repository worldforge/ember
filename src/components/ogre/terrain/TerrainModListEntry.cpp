*//
// C++ Implementation: TerrainModListEntry
//
// Description: 
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
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

#include "TerrainModListEntry.h"

#include "TerrainPage.h"
#include "TerrainGenerator.h"
#include "../EmberOgrePrerequisites.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include <wfmath/stream.h>

namespace EmberOgre {
namespace Terrain {

/** Used for the terrainModListEntry class **/
terrainModListEntry::terrainModListEntry()
{
	seg_x = 0;
	seg_y = 0;
	mod_x = 0;
	mod_y = 0;
	mod_z = 0;
	modifier = NULL;
	mId = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainPage(TerrainPosition(0,0))->getTerrainModCount();
}

terrainModListEntry::terrainModListEntry(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *newmod)
{
	seg_x = sx;
	seg_y = sy;

	mod_x = mx;
	mod_y = my;
	mod_z = mz;

	modifier = newmod;

	mId = EmberOgre::getSingleton().getTerrainGenerator()->getTerrainPage(TerrainPosition(seg_x*64,seg_y*64))->getTerrainModCount();

	S_LOG_INFO("Adding new terrain modifier to segment: " << seg_x << "," << seg_y);
	S_LOG_INFO("Adding new terrain modifier to position: " << mod_x << "," << mod_y << "," << mod_z);
}

int terrainModListEntry::SegX()
{
	return seg_x;
}

int terrainModListEntry::SegY()
{
	return seg_y;
}

int terrainModListEntry::X()
{
	return mod_x;
}

int terrainModListEntry::Y()
{
	return mod_y;
}

int terrainModListEntry::Z()
{
	return mod_z;
}

Mercator::TerrainMod *terrainModListEntry::Modifier()
{
	return modifier;
}

TerrainPosition *terrainModListEntry::Position()
{
	return new TerrainPosition(-seg_x*64 - mod_x, seg_y*64 + mod_y);
}

int terrainModListEntry::Id()
{
	return mId;
}

}
}