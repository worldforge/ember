//
// C++ Interface: EntityIconSlot
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
	@author Erik Hjortsberg <erik.hjortsberg@iteam.se>
*/
class EntityIconSlot : public EntityIconDragDropTarget
{
friend class EntityIconManager;
friend class EntityIcon;
public:
	
	/**
	 *    Adds an EntityIcon to the slot. If the slot already contains an icon, the method will return false and an error will be logged.
	 * @param icon 
	 * @return true if successful, else false
	 */
	
	bool addEntityIcon(EntityIcon* icon);
	
	/**
	 *    Removes and returns the contained EntityIcon. If none is contained, nothing will happen.
	 * @return The contained EntityIcon or null if none contained.
	 */
	EntityIcon* removeEntityIcon();
	
	/**
	 *    Gets the contained EntityIcon
	 * @return 
	 */
	EntityIcon* getEntityIcon();
	
	CEGUI::Window* getWindow();
	
	/**
	Call this from the gui code whenever an icon is dragged away from the slot.
	*/
	void notifyIconDraggedOff(EntityIcon* entityIcon);
	
	/**
	Emitted when an icon has been dragged away from the slot as a result of an user action.
	*/
	sigc::signal<void, EntityIcon*> EventIconDraggedOff;

	
	
protected:
	EntityIconSlot(EntityIconManager& manager, CEGUI::Window* container);
	virtual ~EntityIconSlot();
	
	EntityIconManager& mManager;
	CEGUI::Window* mContainer;
	EntityIcon* mContainedIcon;
		
	/**
	 *    Internal method called by the EntityIcon when it's removed from the slot.
	 */
	void notifyIconRemoved();

};

}

}

#endif
