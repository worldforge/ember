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
#include <string>
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
@brief Handles and manages action bar icons.

An action bar icon is an icon which can be used to execute actions on a number of different objects. The icons can be dragged and dropped onto slots and created when other icons are dragged on the action bar.
This class is responsible for handling all of them in the UI, creating new and destroying old.
You normally don't create instances of neither ActionBarIconSlot nor ActionBarIcon yourself, but use the create* and destroy* methods in this class instead.
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
	 * @brief Creates a new action bar icon slot.
	 * @param pixelSize The size of the slot in pixels.
	 * @returns A new slot instance.
	 */
	ActionBarIconSlot* createSlot(unsigned int pixelSize = 64);

	/**
	 * @brief Destroys an action bar icon slot.
	 * @param slot The slot to destroy.
	 */
	void destroySlot(ActionBarIconSlot* slot);

	/**
	 * @brief Creates a new action bar icon.
	 * @param icon An empty icon instance which will be used by the entity icon.
	 * @param pixelSize The size of the icon in pixels.
	 * @returns An action bar icon instance.
	 */
	ActionBarIcon* createIcon(Gui::Icons::Icon* icon, unsigned int pixelSize = 64);

	/**
	 * @brief Destroys an action bar icon.
	 *
	 * @param The action bar icon to destroy.
	 */
	void destroyIcon(ActionBarIcon* icon);

	const std::string getSavedValue(const std::string& name) const;

	void saveValue(const std::string& key, const std::string& value);

	void eraseValue(const std::string& key);
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
	 * @brief All of the action bar icon slots managed by this instance.
	 */
	ActionBarIconSlotStore mSlots;

	/**
	 * @brief All of the action bar icons managed by this instance.
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
