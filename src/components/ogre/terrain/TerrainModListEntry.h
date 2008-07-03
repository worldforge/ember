//
// C++ Interface: TerrainModListEntry
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
#ifndef EMBEROGRETERRAINMODLISTENTRY_H
#define EMBEROGRETERRAINMODLISTENTRY_H
#include "../EmberOgrePrerequisites.h"
#include "TerrainPage.h"

namespace EmberOgre {
namespace Terrain {

class terrainModListEntry {
	public:
		terrainModListEntry();
		terrainModListEntry(int sx, int sy, int mx, int my, int mz, Mercator::TerrainMod *modifier);

		int X();
		int Y();
		int Z();
		TerrainPosition *Position();
		int SegX();
		int SegY();
		Mercator::TerrainMod *Modifier();

		int Id();
		
	private:
		 // A number to uniquely identify this entry
		int	mId;
		 // Segment the modifier is applied to
		int	seg_x;
		int	seg_y;
		 // Position of modifier relative to segment
		int	mod_x;
		int	mod_y;
		int	mod_z;

		Mercator::TerrainMod *modifier;
};

}
}

#endif
