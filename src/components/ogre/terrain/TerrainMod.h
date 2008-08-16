//
// C++ Interface: TerrainMod
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
	
	/**
	Used to retrieve a pointer to this modifier
	*/
	inline Mercator::TerrainMod* getMod() const;

	/**
	Used to set the pointer to this modifier
	*/
	inline void setMod(Mercator::TerrainMod* area);
	
	/**
	Emitted whenever the modifier is changed or moved.
	*/
	sigc::signal<void, TerrainMod*> EventModChanged;
	
	/**
	Emitted just before the entity owning this mod is deleted.
	Should be caught by TerrainGenerator to remove this mod from the terrain.
	*/
	sigc::signal<void, TerrainMod*> EventModDeleted;

	/**
	The owner of this modifier
	*/
	EmberEntity* mEntity;

protected:
	/**
	A pointer to the modifier. Used to update and remove it from the terrain.
	*/
	Mercator::TerrainMod* mModifier;
	Eris::Entity::AttrChangedSlot mAttrChangedSlot;

	/**
	Called whenever a modifier is changed and handles the update
	*/	
	void attributeChanged(const Atlas::Message::Element& attributeValue);

	/**
	Called whenever a modifier is moved and handles the update
	*/
	void entity_Moved();

	/**
	Called whenever the entity holding a modifier is deleted and handles
	removing the mod from the terrain
	*/
	void entity_Deleted();

	/**
	Sets up the previous three handler functions to be called when a change
	is made to the entity holding the modifier
	*/
	void observeEntity();

	/**
	Parses the Atlas data for a modifier
	*/
	bool parseMod();

	/// \brief creates a CraterTerrainMod based on a shape and position
	Mercator::TerrainMod * newCraterMod(const Atlas::Message::MapType, WFMath::Point<3>);
	/// \brief Creates a LevelTerrainMod based on a shape and position
	Mercator::TerrainMod * newLevelMod(const Atlas::Message::MapType, WFMath::Point<3>);
	/// \brief Creates a SlopeTerrainMod based on a shape, position, and two slopes
	Mercator::TerrainMod * newSlopeMod(const Atlas::Message::MapType, WFMath::Point<3>, float, float);
	/// \brief Creates an AdjustTerrainMod based on a shape and position
	Mercator::TerrainMod * newAdjustMod(const Atlas::Message::MapType, WFMath::Point<3>);

};

Mercator::TerrainMod* TerrainMod::getMod() const { return mModifier; }
void TerrainMod::setMod(Mercator::TerrainMod* modifier) { mModifier = modifier; }

}
};

#endif
