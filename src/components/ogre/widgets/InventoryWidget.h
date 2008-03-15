//
// C++ Interface: InventoryWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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
#ifndef EMBEROGRE_INVENTORYWIDGET_H
#define EMBEROGRE_INVENTORYWIDGET_H

#include "Widget.h"

#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <Eris/Entity.h>

#include <sigc++/slot.h>
#include "icons/Icon.h"
#include "icons/IconManager.h"

namespace CEGUI {
class DragContainer;
}


namespace EmberOgre {
class EmberEntity;
class AvatarEmberEntity;
class Widget;
class GUIManager;
class EmberEntityFactory;

namespace Gui {

class EntityIconManager;
class EntityIconSlot;
class EntityIcon;
class EntityIconUserData;






/**
@author Erik Hjortsberg
*/
class InventoryWidget : public Widget
{
public:
    InventoryWidget();
    virtual ~InventoryWidget();
	void buildWidget();


protected:
	CEGUI::Listbox* mListBox;
	std::map<EmberEntity*, CEGUI::ListboxItem*> mListBoxMap;
	typedef std::vector<EntityIconSlot*> IconSlotStore;
	typedef std::vector<EntityIcon*> IconStore;

	void removedEntity(EmberEntity* emberEntity);
	void addedEntity(EmberEntity* emberEntity);
	void createdAvatarEmberEntity(AvatarEmberEntity* entity);
	bool Drop_Click(const CEGUI::EventArgs& args);
	bool Wield_Click(const CEGUI::EventArgs& args);

	EntityIconSlot* addSlot();
	
	
	Gui::Icons::IconManager* mIconManager;
	int mIconsUsed;
	
	IconSlotStore mSlots;
	IconStore mIcons;
	EntityIconManager* mEntityIconManager;
	
	int mIconSize;
	
};
};
};

#endif
