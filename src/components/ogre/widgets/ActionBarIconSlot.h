//
// C++ Interface: ActionBarIconSlot
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIcon class by Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_GUIACTIONBARICONSLOT_H
#define EMBEROGRE_GUIACTIONBARICONSLOT_H

#include <sigc++/signal.h>
#include "ActionBarIconDragDropTarget.h"
#include "CEGUIUtils.h"

namespace CEGUI {
class DragContainer;

class Window;

class EventArgs;
}

namespace Ember {
namespace OgreView {

namespace Gui {

class ActionBarIcon;

class ActionBarIconManager;

class EntityIcon;

/**
@author Erik Ogenvik <erik@ogenvik.org>

@brief A slot for an action bar icon.

An instance of this represent a slot into which an action bar icon can be placed. Icons can be dragged both to and from a slot.
A typical usage example would be an action bar which contains slots that the user can drag other icons to.

@note You don't normally directly create or destroy an instance of this. Instead use the EntityIconManager.
*/
class ActionBarIconSlot : public ActionBarIconDragDropTarget {
	friend class ActionBarIconManager;

	friend class ActionBarIcon;

public:
	/**
	 * @brief Ctor.
	 *
	 * @param The main action bar icon manager.
	 * @param container The CEGUI window which will be used by the slot. This can be any window, but should preferably be something which makes it clear that it's a slot onto which an icon can be dragged and dropped.
	 */
	explicit ActionBarIconSlot(UniqueWindowPtr<CEGUI::Window> container);

	/**
	 * @brief Dtor.
	 * Any icon contained in the slot will be removed upon destruction.
	 */
	~ActionBarIconSlot() override;

	/**
	 * @brief Adds an ActionBarIcon to the slot. If the slot already contains an icon, the method will return false and an error will be logged.
	 * @param icon
	 * @return true if successful, else false
	 */
	bool addActionBarIcon(ActionBarIcon* icon);

	/**
	 * @brief Removes and returns the contained ActionBarIcon. If none is contained, nothing will happen.
	 * @return The contained ActionBarIcon or null if none contained.
	 */
	ActionBarIcon* removeActionBarIcon();

	/**
	 * @brief Gets the contained ActionBarIcon.
	 * @return
	 */
	ActionBarIcon* getActionBarIcon();

	/**
	 * @brief Gets the window which this slot is using.
	 * @returns The window used by this slot.
	 */
	CEGUI::Window* getWindow();

	/**
	 * @brief Call this from the gui code whenever an icon is dragged away from the slot.
	 */
	void notifyIconDraggedOff(ActionBarIcon* entityIcon);

	/**
	 * @brief Emitted when an icon has been dragged away from the slot as a result of an user action.
	 */
	sigc::signal<void(ActionBarIcon*)> EventIconDraggedOff;


protected:



	/**
	 * @brief The CEGUI window representing the slot.
	 */
	UniqueWindowPtr<CEGUI::Window> mContainer;

	/**
	 * @brief Holds any entity icon currently contained in the slot.
	 */
	ActionBarIcon* mContainedIcon;

	/**
	 * @brief Internal method called by the ActionBarIcon when it's removed from the slot.
	 */
	void notifyIconRemoved();

};

}

}

}

#endif
