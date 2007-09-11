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
class EntityIconManager;
class EntityIconSlot;
class EntityIcon;
class EntityIconUserData;

class EntityIconSlot
{
friend class EntityIconManager;
public:
	
	bool addEntityIcon(EntityIcon* icon);
	EntityIcon* removeEntityIcon();
	
	CEGUI::Window* getWindow();
	
	void notifyIconRemoved();
protected:
	EntityIconSlot(EntityIconManager& manager, CEGUI::Window* container);
	virtual ~EntityIconSlot();
	
	EntityIconManager& mManager;
	CEGUI::Window* mContainer;
	EntityIcon* mContainedIcon;
	
	bool handleDragEnter(const CEGUI::EventArgs& args);
	bool handleDragLeave(const CEGUI::EventArgs& args);
	bool handleDragDropped(const CEGUI::EventArgs& args);
	

};

class EntityIconUserData
{
public:
	EntityIconUserData(EntityIcon& entityIcon);
	
	EntityIcon& getEntityIcon();
private:
	EntityIcon& mEntityIcon;
};

class EntityIcon
{
friend class EntityIconManager;
public:
	
	CEGUI::Window* getImage();
	CEGUI::DragContainer* getDragContainer();
	Gui::Icons::Icon* getIcon();
	void setSlot(EntityIconSlot* slot);
	EntityIconSlot* getSlot();

protected:
	EntityIcon(EntityIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon);
	
	EntityIconManager& mManager;
	CEGUI::DragContainer* mDragContainer;
	CEGUI::Window* mImage;
	Gui::Icons::Icon* mIcon;
	EntityIconUserData mUserData;
	EntityIconSlot* mCurrentSlot;
};



class EntityIconManager
{
public:
typedef std::vector<EntityIconSlot*> EntityIconSlotStore;
typedef std::vector<EntityIcon*> EntityIconStore;

EntityIconManager(GUIManager& guiManager);

EntityIconSlot* createSlot();
void destroySlot(EntityIconSlot* slot);

EntityIcon* createIcon(Gui::Icons::Icon* icon);
void destroyIcon(EntityIcon* icon);

protected:
EntityIconSlotStore mSlots;
EntityIconStore mIcons;
GUIManager& mGuiManager;
int mIconsCounter, mSlotsCounter;
};

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

	void removedEntity(EmberEntity* emberEntity);
	void addedEntity(EmberEntity* emberEntity);
	void createdAvatarEmberEntity(AvatarEmberEntity* entity);
	bool Drop_Click(const CEGUI::EventArgs& args);
	bool Wield_Click(const CEGUI::EventArgs& args);

	Gui::Icons::IconManager* mIconManager;
	int mIconsUsed;
	
	IconSlotStore mSlots;
	EntityIconManager* mEntityIconManager;
	
};

};

#endif
