//
// C++ Interface: TerrainMod
//
// Description: The purpose of this class is to wrap an Eris::TerrainMod instance
//
//		TerrainManager listens for changes in the modifier and
//		updates or removes the modifiers from the terrain as needed.
//
//
// Author: Tamas Bates <rhymer@gmail.com>, (C) 2008
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
#ifndef EMBEROGRETERRAINMOD_H
#define EMBEROGRETERRAINMOD_H

#include <Eris/Entity.h>
#include <sigc++/signal.h>
#include <string>

namespace Atlas
{
namespace Message {
	class Element;
}
}

namespace Mercator
{
	class TerrainMod;
}

namespace Ember
{
namespace OgreView
{

class EmberEntity;

namespace Terrain {

/**
@author Tamas Bates
@author Erik Hjortsberg
@brief Connects an Eris::Entity to a Mercator terrain mod.

Note that however the parsing and application of the terrain mod doesn't occur here. Instead it's handled by the TerrainMod*Task classes, which allows it to occur in a background thread.
This class only keeps track of the entity, and emit signals when the entity data has changed so that the terrain mod needs to be recalculated or removed.
*/
class TerrainMod
{
public:
	/**
	 * @brief Ctor.
	 * @param entity The entity to which this mod belongs.
	 */
	TerrainMod(Eris::Entity& entity);

	/**
	 * @brief Dtor.
	 */
	virtual ~TerrainMod();
	

    /**
     * @brief Sets up the observation of the entity.
     */
    virtual void init();

    /**
	 * @brief Gets the id of the entity which is observed.
	 * @returns The id of the entity to which this mod belongs.
	 */
	const std::string& getEntityId() const;

    /**
    * @brief Accessor for the entity which is observed.
    * @return The entity observed by this instance.
    */
    Eris::Entity& getEntity() const;

    /**
     * @brief Gets the atlas data which defines the terrain mod.
     */
    const Atlas::Message::Element& getAtlasData() const;

	/**
	 * Emitted whenever the modifier is changed or moved.
	 */
	sigc::signal<void> EventModChanged;

	/**
	 *Emitted just before the entity owning this mod is deleted.
	 *Should be caught by TerrainManager to remove this mod from the terrain.
	 */
	sigc::signal<void> EventModDeleted;

protected:

    /**
    @brief The observed entity, containing terrain mod data.
    */
    Eris::Entity& mEntity;

    /**
    * @brief Slot used to listen for changes to attributes in the Entity to which this mod belongs to.
    */
    Eris::Entity::AttrChangedSlot mAttrChangedSlot;


    /**
    * @brief Called before the ModChanged signal is emitted.
    */
    virtual void onModChanged();

    /**
    * @brief Called before the ModDeleted signal is emitted.
    */
    virtual void onModDeleted();

    /**
    * @brief Called whenever a modifier is changed and handles the update
    * @param attributeValue The new Atlas data for the terrain mod
    */
    void attributeChanged(const Atlas::Message::Element& attributeValue);

    /**
    *    @brief Called whenever a modifier is moved and handles the update
    */
    void entity_Moved();

    /**
    * @brief Sets up the previous three handler functions to be called when a change
    * is made to the entity holding the modifier.
    */
    virtual void observeEntity();
	
};

}
}
}

#endif
