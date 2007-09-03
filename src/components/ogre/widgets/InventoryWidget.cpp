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
#include "ColouredListItem.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
// #include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberEntityFactory.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../MathConverter.h"
#include "InventoryWidget.h"
#include <Eris/TypeInfo.h>

#include <elements/CEGUIPushButton.h>
#include <CEGUIPropertyHelper.h>


namespace EmberOgre {



// template<> WidgetLoader WidgetLoaderHolder<InventoryWidget>::loader("InventoryWidget", &createWidgetInstance);

InventoryWidget::InventoryWidget()
: mIconsUsed(0)
{
}

InventoryWidget::~InventoryWidget()
{
}

void InventoryWidget::buildWidget()
{
	

	loadMainSheet("InventoryWidget.xml", "Inventory/");
	mMainWindow->setVisible(false);
	
	mListBox = static_cast<CEGUI::Listbox*>(getWindow("ListBox"));
	
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &InventoryWidget::createdAvatarEmberEntity));
	
	CEGUI::PushButton* dropButton = static_cast<CEGUI::PushButton*>(getWindow("Drop"));
	BIND_CEGUI_EVENT(dropButton, CEGUI::ButtonBase::EventMouseClick, InventoryWidget::Drop_Click)
	
	CEGUI::PushButton* wieldButton = static_cast<CEGUI::PushButton*>(getWindow("Wield"));
	BIND_CEGUI_EVENT(wieldButton, CEGUI::ButtonBase::EventMouseClick, InventoryWidget::Wield_Click)

	enableCloseButton();
	
	mIconManager = new Gui::Icons::IconManager();
	
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
void InventoryWidget::addedEntity(EmberEntity* entity) {
	static int iconSize(64);
	CEGUI::String name(entity->getType()->getName() + " ("+ entity->getId() +" : "+entity->getName()+")");
// 	CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, atoi(entity->getId().c_str()), entity);
// 	mListBoxMap.insert(std::map<EmberEntity*, CEGUI::ListboxItem*>::value_type(entity, item));
// 	mListBox->addItem(item);
	
	Gui::Icons::Icon* icon = mIconManager->getIcon(iconSize, entity->getType());
	if (icon) {
		int yPosition = mIcons.size() / 4;
		int xPosition = mIcons.size() % 4;
		
		std::stringstream ss;
		ss << "inventoryIcon_" << mIcons.size();
		CEGUI::Window* iconWindow = createWindow("EmberLook/StaticImage", ss.str());
		if (iconWindow) {
			iconWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0, iconSize), CEGUI::UDim(0, iconSize)));
			iconWindow->setTooltipText(name);
			iconWindow->setProperty("BackgroundEnabled", "false");
			iconWindow->setProperty("FrameEnabled", "false");
			EntityIcon entityIcon(iconWindow, icon);
			mIcons.push_back(entityIcon);
			iconWindow->setProperty("Image", CEGUI::PropertyHelper::imageToString(icon->getImage()));
			CEGUI::Window* container = getWindow("IconContainer");
			if (container) {
				container->addChildWindow(iconWindow);
				iconWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, iconSize * xPosition), CEGUI::UDim(0, iconSize * yPosition)));
			}
		}
		
	}


}
void InventoryWidget::removedEntity(EmberEntity* entity) {
//	CEGUI::ListboxItem* item = mListBox->getListboxItemFromIndex(atoi(dimeEntity->getId().c_str()));
	CEGUI::ListboxItem* item = mListBoxMap[entity];
	if (item) {
		mListBox->removeItem(item);
		mListBoxMap.erase(mListBoxMap.find(entity));
	}
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


EntityIcon::EntityIcon(CEGUI::Window* image, Gui::Icons::Icon* icon)
: mImage(image), mIcon(icon)
{
}

CEGUI::Window* EntityIcon::getImage()
{
	return mImage;
}

Gui::Icons::Icon* EntityIcon::getIcon()
{
	return mIcon;
}


};
