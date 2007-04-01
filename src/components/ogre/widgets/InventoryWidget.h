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
#ifndef DIMEOGREINVENTORYWIDGET_H
#define DIMEOGREINVENTORYWIDGET_H

#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxItem.h> 
#include <elements/CEGUIListboxTextItem.h> 
#include <Eris/Entity.h>



namespace EmberOgre {
class EmberEntity;
class AvatarEmberEntity;
class Widget;
class GUIManager;
class EmberEntityFactory;

/**
@author Erik Hjortsberg
*/
class InventoryWidget : public Widget
{
public:

    virtual ~InventoryWidget();
	void buildWidget();


protected:
	CEGUI::Listbox* mListBox;
	std::map<EmberEntity*, CEGUI::ListboxItem*> mListBoxMap;

	void removedEntity(EmberEntity* emberEntity);
	void addedEntity(EmberEntity* emberEntity);
	void createdAvatarEmberEntity(AvatarEmberEntity* entity);
	bool Drop_Click(const CEGUI::EventArgs& args);
	bool Wield_Click(const CEGUI::EventArgs& args);



	
	
};

};

#endif
