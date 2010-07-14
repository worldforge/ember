//
// C++ Interface: ActionBarIconManager
//
// Description:
//
//	Author Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
//	Based on the EntityIcon class by Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUIACTIONBARICONMANAGER_H
#define EMBEROGRE_GUIACTIONBARICONMANAGER_H

#include <sigc++/signal.h>
#include <vector>

namespace EmberOgre {
class EmberEntity;
class GUIManager;
namespace Gui {

namespace Icons
{
class Icon;
}

class ActionBarIconSlot;
class ActionBarIcon;

/**
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
@brief Handles and manages entity icons.

An entity icon is an icon which is connected to an entity. The icons can be dragged and dropped onto slots (for example when creating a system for outfits, or an action bar.
This class is responsible for handling all of them in the UI, creating new and destroying old.
You normally don't create instances of neither EntityIconSlot nor EntityIcon yourself, but use the create* and destroy* methods in this class instead.
*/
class ActionBarIconManager
{
public:

	/**
	 * @brief Ctor.
	 * @param guiManager The main GUI manager.
	 */
	ActionBarIconManager(GUIManager& guiManager);

	/**
	 * @brief Dtor.
	 *
	 * All icons and slots will be destroyed.
	 */
	virtual ~ActionBarIconManager();

	/**
	 * @brief Creates a new entity icon slot.
	 * @param pixelSize The size of the slot in pixels.
	 * @returns A new slot instance.
	 */
	ActionBarIconSlot* createSlot(unsigned int pixelSize = 64);

	/**
	 * @brief Destroys an entity icon slot.
	 * @param slot The slot to destroy.
	 */
	void destroySlot(ActionBarIconSlot* slot);

	/**
	 * @brief Creates a new entity icon.
	 * @param icon An empty icon instance which will be used by the entity icon.
	 * @param entity The entity to use.
	 * @param pixelSize The size of the icon in pixels.
	 * @returns An entity icon instance.
	 */
	ActionBarIcon* createIcon(Gui::Icons::Icon* icon, unsigned int pixelSize = 64);

	/**
	 * @brief Destroys an entity icon.
	 *
	 * @param The entity icon to destroy.
	 */
	void destroyIcon(ActionBarIcon* icon);

	/**
	 * @brief Emitted when a drag action of an entity icon has started.
	 */
	sigc::signal<void, ActionBarIcon*> EventIconDragStart;

	/**
	 * @brief Emitted when a drag action of an entity icon has ended.
	 */
	sigc::signal<void, ActionBarIcon*> EventIconDragStop;

protected:
	typedef std::vector<ActionBarIconSlot*> ActionBarIconSlotStore;
	typedef std::vector<ActionBarIcon*> ActionBarIconStore;

	/**
	 * @brief All of the entity icon slots managed by this instance.
	 */
	ActionBarIconSlotStore mSlots;

	/**
	 * @brief All of the entity icons managed by this instance.
	 */
	ActionBarIconStore mIcons;

	/**
	 * @brief The main GUI manager.
	 */
	GUIManager& mGuiManager;

	/**
	 * @brief A counter used for providing unique names for each icon window.
	 */
	int mIconsCounter;

	/**
	 * @brief A counter used for providing unique names for each slot window.
	 */
	int mSlotsCounter;
};

}

}

#endif
