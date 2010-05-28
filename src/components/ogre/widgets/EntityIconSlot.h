//
// C++ Interface: EntityIconSlot
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUIENTITYICONSLOT_H
#define EMBEROGRE_GUIENTITYICONSLOT_H

#include <sigc++/signal.h>
#include "EntityIconDragDropTarget.h"

namespace CEGUI {
class DragContainer;
class Window;
class EventArgs;
}

namespace EmberOgre {

namespace Gui {

class EntityIcon;
class EntityIconManager;

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

@brief A slot for an entity icon.

An instance of this represent a slot into which an entity icon can be placed. Icons can be dragged both to and from a slot.
A typical usage example would be an inventory, where each entity icon is stored in a slot. By dragging entity icons between different slots the inventory is rearranged.

@note You don't normally directly create or destroy an instance of this. Instead use the EntityIconManager.
*/
class EntityIconSlot : public EntityIconDragDropTarget
{
friend class EntityIconManager;
friend class EntityIcon;
public:
	
	/**
	 * @brief Adds an EntityIcon to the slot. If the slot already contains an icon, the method will return false and an error will be logged.
	 * @param icon 
	 * @return true if successful, else false
	 */
	
	bool addEntityIcon(EntityIcon* icon);
	
	/**
	 * @brief Removes and returns the contained EntityIcon. If none is contained, nothing will happen.
	 * @return The contained EntityIcon or null if none contained.
	 */
	EntityIcon* removeEntityIcon();
	
	/**
	 * @brief Gets the contained EntityIcon
	 * @return 
	 */
	EntityIcon* getEntityIcon();
	
	/**
	 * @brief Gets the window which this slot is using.
	 * @returns The window used by this slot.
	 */
	CEGUI::Window* getWindow();
	
	/**
	 * @brief Call this from the gui code whenever an icon is dragged away from the slot.
	 */
	void notifyIconDraggedOff(EntityIcon* entityIcon);
	
	/**
	 * @brief Emitted when an icon has been dragged away from the slot as a result of an user action.
	 */
	sigc::signal<void, EntityIcon*> EventIconDraggedOff;

	
	
protected:
	/**
	 * @brief Ctor.
	 *
	 * @param The main entity icon manager.
	 * @param container The CEGUI window which will be used by the slot. This can be any window, but should preferably be something which makes it clear that it's a slot onto which an icon can be dragged and dropped.
	 */
	EntityIconSlot(CEGUI::Window* container);

	/**
	 * @brief Dtor.
	 * Any icon contained in the slot will be removed upon destruction.
	 */
	virtual ~EntityIconSlot();
	
	/**
	 * @brief The CEGUI window representing the slot.
	 */
	CEGUI::Window* mContainer;

	/**
	 * @brief Holds any entity icon currently contained in the slot.
	 */
	EntityIcon* mContainedIcon;
		
	/**
	 * @brief Internal method called by the EntityIcon when it's removed from the slot.
	 */
	void notifyIconRemoved();

};

}

}

#endif
