//
// C++ Implementation: InventoryWidget
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
#include "services/server/ServerService.h"
#include "services/EmberServices.h"

#include "Widget.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "InventoryWidget.h"


#include <elements/CEGUIPushButton.h>


namespace EmberOgre {



// template<> WidgetLoader WidgetLoaderHolder<InventoryWidget>::loader("InventoryWidget", &createWidgetInstance);


InventoryWidget::~InventoryWidget()
{
}

void InventoryWidget::buildWidget()
{
	
	mMainWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"cegui/widgets/InventoryWidget.xml", "Inventory/");
	mMainWindow->setVisible(false);
//	mMainWindow->setAlwaysOnTop(true);
	
	mListBox = static_cast<CEGUI::Listbox*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Inventory/ListBox"));
	getMainSheet()->addChildWindow(mMainWindow); 
	
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(SigC::slot(*this, &InventoryWidget::createdAvatarEmberEntity));
	
	CEGUI::PushButton* dropButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Inventory/Drop"));
	BIND_CEGUI_EVENT(dropButton, CEGUI::ButtonBase::EventMouseClick, InventoryWidget::Drop_Click)
	
	CEGUI::PushButton* wieldButton = static_cast<CEGUI::PushButton*>(CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)"Inventory/Wield"));
	BIND_CEGUI_EVENT(wieldButton, CEGUI::ButtonBase::EventMouseClick, InventoryWidget::Wield_Click)

	enableCloseButton();
	
	
	
}

void InventoryWidget::createdAvatarEmberEntity(AvatarEmberEntity* entity)
{
	mMainWindow->setVisible(true);
	EmberOgre::getSingleton().getAvatar()->EventAddedEntityToInventory.connect(SigC::slot(*this, &InventoryWidget::addedEntity));
	EmberOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.connect(SigC::slot(*this, &InventoryWidget::removedEntity));
	registerConsoleVisibilityToggleCommand("inventory");

}
void InventoryWidget::addedEntity(EmberEntity* dimeEntity) {
	
	CEGUI::String name(dimeEntity->getType()->getName() + " ("+ dimeEntity->getId() +" : "+dimeEntity->getName()+")");
	CEGUI::ListboxItem* item = new ColoredListItem(name, atoi(dimeEntity->getId().c_str()), dimeEntity);
	mListBoxMap.insert(std::map<EmberEntity*, CEGUI::ListboxItem*>::value_type(dimeEntity, item));
	mListBox->addItem(item);

}
void InventoryWidget::removedEntity(EmberEntity* dimeEntity) {
//	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getId().c_str()));
	CEGUI::ListboxItem* item = mListBoxMap[dimeEntity];
	if (item) {
		mListBox->removeItem(item);
		mListBoxMap.erase(mListBoxMap.find(dimeEntity));
	}
}

bool InventoryWidget::Drop_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mListBox->getFirstSelectedItem();
	if (item) {
		EmberEntity* entity = static_cast<EmberEntity*>(item->getUserData());
		Ember::EmberServices::getInstance()->getServerService()->drop(entity, WFMath::Vector<3>(0,1,0));
	}
}

bool InventoryWidget::Wield_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mListBox->getFirstSelectedItem();
	if (item) {
		EmberEntity* entity = static_cast<EmberEntity*>(item->getUserData());
		Ember::EmberServices::getInstance()->getServerService()->wield(entity);
	}
}


};
