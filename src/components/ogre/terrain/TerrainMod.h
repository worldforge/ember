//
// C++ Interface: TerrainMod
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifndef EMBEROGRETERRAINMOD_H
#define EMBEROGRETERRAINMOD_H

#include <sigc++/signal.h>
#include <Eris/Entity.h>

namespace Mercator
{
    class TerrainMod;
}

namespace EmberOgre {

class EmberEntity;

namespace Terrain {

/**
@author Tamas Bates
*/
class TerrainMod
{
public:
    TerrainMod(EmberEntity* entity);

    ~TerrainMod();
	bool init();
	
	inline Mercator::TerrainMod* getMod() const;
	inline void setMod(Mercator::TerrainMod* area);
	
	sigc::signal<void, TerrainMod*> EventModChanged;

protected:

	Mercator::TerrainMod* mModifier;
	EmberEntity* mEntity;
	Eris::Entity::AttrChangedSlot mAttrChangedSlot;
	
	void attributeChanged(const Atlas::Message::Element& attributeValue);
	void entity_Moved();
	void observeEntity();
	bool parseMod();

};

Mercator::TerrainMod* TerrainMod::getMod() const { return mModifier; }
void TerrainMod::setMod(Mercator::TerrainMod* modifier) { mModifier = modifier; }

}
};

#endif
