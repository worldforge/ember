//
// C++ Implementation: InventoryWidget
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2004
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

#include "InventoryWidget.h"

#include "services/server/ServerService.h"
#include "services/EmberServices.h"

#include "ColouredListItem.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
// #include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../MathConverter.h"
#include "../GUIManager.h"
#include <Eris/TypeInfo.h>

#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIDragContainer.h>
#include <CEGUIPropertyHelper.h>
#include "EntityIconManager.h"
#include "EntityIcon.h"
#include "EntityIconSlot.h"

using namespace CEGUI;
namespace EmberOgre {
namespace Gui {



// template<> WidgetLoader WidgetLoaderHolder<InventoryWidget>::loader("InventoryWidget", &createWidgetInstance);

InventoryWidget::InventoryWidget()
: mIconsUsed(0), mEntityIconManager(0), mIconSize(64)
{
}

InventoryWidget::~InventoryWidget()
{
}

void InventoryWidget::buildWidget()
{
	

	loadMainSheet("InventoryWidget.layout", "Inventory/");
	mMainWindow->setVisible(false);
	
	mListBox = static_cast<CEGUI::Listbox*>(getWindow("ListBox"));
	
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &InventoryWidget::createdAvatarEmberEntity));
	
	CEGUI::PushButton* dropButton = static_cast<CEGUI::PushButton*>(getWindow("Drop"));
	BIND_CEGUI_EVENT(dropButton, CEGUI::PushButton::EventClicked, InventoryWidget::Drop_Click)
	
	CEGUI::PushButton* wieldButton = static_cast<CEGUI::PushButton*>(getWindow("Wield"));
	BIND_CEGUI_EVENT(wieldButton, CEGUI::PushButton::EventClicked, InventoryWidget::Wield_Click)

	enableCloseButton();
	
	mIconManager = mGuiManager->getIconManager();
	mEntityIconManager = new EntityIconManager(*mGuiManager);

	for (int i = 0; i < 8; ++i) {
		addSlot();	
	}
	
}

void InventoryWidget::createdAvatarEmberEntity(AvatarEmberEntity* entity)
{
	EmberOgre::getSingleton().getAvatar()->EventAddedEntityToInventory.connect(sigc::mem_fun(*this, &InventoryWidget::addedEntity));
	EmberOgre::getSingleton().getAvatar()->EventRemovedEntityFromInventory.connect(sigc::mem_fun(*this, &InventoryWidget::removedEntity));
	registerConsoleVisibilityToggleCommand("inventory");

///only show the inventory by default if we're not an admin
	if (!entity->getAvatar()->isAdmin()) {
		mMainWindow->setVisible(true);
	}

}

EntityIconSlot* InventoryWidget::addSlot()
{
	CEGUI::Window* container = getWindow("IconContainer");
	if (container) {
		int yPosition = mSlots.size() / 4;
		int xPosition = mSlots.size() % 4;
		
		EntityIconSlot* slot = mEntityIconManager->createSlot();
		slot->getWindow()->setPosition(CEGUI::UVector2(CEGUI::UDim(0, mIconSize * xPosition), CEGUI::UDim(0, mIconSize * yPosition)));
		container->addChildWindow(slot->getWindow());
		mSlots.push_back(slot);
		return slot;
	}
	return 0;
}

void InventoryWidget::addedEntity(EmberEntity* entity) {
	static int iconSize(64);
	std::string name(entity->getType()->getName() + " ("+ entity->getId() +" : "+entity->getName()+")");
// 	CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, atoi(entity->getId().c_str()), entity);
// 	mListBoxMap.insert(std::map<EmberEntity*, CEGUI::ListboxItem*>::value_type(entity, item));
// 	mListBox->addItem(item);
 	CEGUI::Window* container = getWindow("IconContainer");
	if (container) {
	
		Gui::Icons::Icon* icon = mIconManager->getIcon(iconSize, entity->getType());
		if (icon) {
			EntityIconSlot* slot = addSlot();
			
			EntityIcon* entityIcon = mEntityIconManager->createIcon(icon, entity);
			mIcons.push_back(entityIcon);
			entityIcon->setTooltipText(name);
			slot->addEntityIcon(entityIcon);
			
/*			CEGUI::Window* iconContainer = createWindow("DefaultGUISheet", ss3.str());
			if (iconContainer) {
			
				iconContainer->setSize(CEGUI::UVector2(CEGUI::UDim(0, iconSize), CEGUI::UDim(0, iconSize)));
				iconContainer->setPosition(CEGUI::UVector2(CEGUI::UDim(0, iconSize * xPosition), CEGUI::UDim(0, iconSize * yPosition)));
				container->addChildWindow(iconContainer);
			
			
				std::stringstream ss1;
				ss1 << "inventoryDraggableContainer_" << mIcons.size();
				CEGUI::DragContainer* item = static_cast<CEGUI::DragContainer*>(createWindow("DragContainer", ss1.str()));
				
				if (item) {
					item->setSize(CEGUI::UVector2(CEGUI::UDim(0, iconSize), CEGUI::UDim(0, iconSize)));
					item->setTooltipText(name);
					iconContainer->addChildWindow(item);
					
					std::stringstream ss2;
					ss2 << "inventoryIcon_" << mIcons.size();
					CEGUI::Window* iconWindow = createWindow("EmberLook/StaticImage", ss2.str());
					if (iconWindow) {
						iconWindow->setProperty("BackgroundEnabled", "false");
						iconWindow->disable();
			// 			iconWindow->setProperty("FrameEnabled", "false");
						item->addChildWindow(iconWindow);
						EntityIcon entityIcon(iconWindow, icon);
						mIcons.push_back(entityIcon);
						iconWindow->setProperty("Image", CEGUI::PropertyHelper::imageToString(icon->getImage()));
					}
				}
			}*/
		}
	}


}
void InventoryWidget::removedEntity(EmberEntity* entity) {
//	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getId().c_str()));
// 	CEGUI::ListboxItem* item = mListBoxMap[entity];
// 	if (item) {
// 		mListBox->removeItem(item);
// 		mListBoxMap.erase(mListBoxMap.find(entity));
// 	}
}

bool InventoryWidget::Drop_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mListBox->getFirstSelectedItem();
	if (item) {
		//drop if one meter in front of the avatar
		Ogre::Vector3 o_vector(1,0,0);
		Ogre::Vector3 o_pos = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getSceneNode()->getOrientation() * o_vector;

		EmberEntity* entity = static_cast<EmberEntity*>(item->getUserData());
		Ember::EmberServices::getSingletonPtr()->getServerService()->drop(entity, Ogre2Atlas_Vector3(o_pos));
	}
	return true;
}

bool InventoryWidget::Wield_Click(const CEGUI::EventArgs& args)
{
	CEGUI::ListboxItem* item = mListBox->getFirstSelectedItem();
	if (item) {
		EmberEntity* entity = static_cast<EmberEntity*>(item->getUserData());
		Ember::EmberServices::getSingletonPtr()->getServerService()->wield(entity);
	}
	return true;
}













}

};
