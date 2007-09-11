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
#include "../GUIManager.h"
#include "InventoryWidget.h"
#include <Eris/TypeInfo.h>

#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIDragContainer.h>
#include <CEGUIPropertyHelper.h>

using namespace CEGUI;
namespace EmberOgre {



// template<> WidgetLoader WidgetLoaderHolder<InventoryWidget>::loader("InventoryWidget", &createWidgetInstance);

InventoryWidget::InventoryWidget()
: mIconsUsed(0), mEntityIconManager(0)
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
	
	mIconManager = mGuiManager->getIconManager();
	mEntityIconManager = new EntityIconManager(*mGuiManager);
	CEGUI::Window* container = getWindow("IconContainer");
	
	int iconSize(64);
	for (int i = 0; i < 8; ++i) {
		int yPosition = mSlots.size() / 4;
		int xPosition = mSlots.size() % 4;
		
		EntityIconSlot* slot = mEntityIconManager->createSlot();
		slot->getWindow()->setPosition(CEGUI::UVector2(CEGUI::UDim(0, iconSize * xPosition), CEGUI::UDim(0, iconSize * yPosition)));
		container->addChildWindow(slot->getWindow());
		mSlots.push_back(slot);
	
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
void InventoryWidget::addedEntity(EmberEntity* entity) {
	static int iconSize(64);
	CEGUI::String name(entity->getType()->getName() + " ("+ entity->getId() +" : "+entity->getName()+")");
// 	CEGUI::ListboxItem* item = new Gui::ColouredListItem(name, atoi(entity->getId().c_str()), entity);
// 	mListBoxMap.insert(std::map<EmberEntity*, CEGUI::ListboxItem*>::value_type(entity, item));
// 	mListBox->addItem(item);
	CEGUI::Window* container = getWindow("IconContainer");
	if (container) {
	
		Gui::Icons::Icon* icon = mIconManager->getIcon(iconSize, entity->getType());
		if (icon) {
			int yPosition = mSlots.size() / 4;
			int xPosition = mSlots.size() % 4;
			
			EntityIconSlot* slot = mEntityIconManager->createSlot();
			slot->getWindow()->setPosition(CEGUI::UVector2(CEGUI::UDim(0, iconSize * xPosition), CEGUI::UDim(0, iconSize * yPosition)));
			container->addChildWindow(slot->getWindow());
			EntityIcon* entityIcon = mEntityIconManager->createIcon(icon);
			slot->addEntityIcon(entityIcon);
			mSlots.push_back(slot);
			
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


EntityIcon::EntityIcon(EntityIconManager& manager, CEGUI::DragContainer* dragContainer, CEGUI::Window* image, Gui::Icons::Icon* icon)
: mManager(manager), mDragContainer(dragContainer), mImage(image), mIcon(icon), mUserData(*this), mCurrentSlot(0)
{
	mDragContainer->setUserData(&mUserData);
}

CEGUI::Window* EntityIcon::getImage()
{
	return mImage;
}

Gui::Icons::Icon* EntityIcon::getIcon()
{
	return mIcon;
}

CEGUI::DragContainer* EntityIcon::getDragContainer()
{
	return mDragContainer;
}

void EntityIcon::setSlot(EntityIconSlot* slot)
{
	if (mCurrentSlot) {
		mCurrentSlot->notifyIconRemoved();
	}
	mCurrentSlot = slot;
}

EntityIconSlot* EntityIcon::getSlot()
{
	return mCurrentSlot;
}



EntityIconUserData::EntityIconUserData(EntityIcon& entityIcon)
: mEntityIcon(entityIcon)
{
}

EntityIcon& EntityIconUserData::getEntityIcon()
{
	return mEntityIcon;
}







EntityIconSlot::EntityIconSlot(EntityIconManager& manager, CEGUI::Window* container)
: mManager(manager), mContainer(container), mContainedIcon(0)
{
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemEnters, EntityIconSlot::handleDragEnter)
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemLeaves, EntityIconSlot::handleDragLeave)
	BIND_CEGUI_EVENT(container, CEGUI::Window::EventDragDropItemDropped, EntityIconSlot::handleDragDropped)
}

EntityIconSlot::~EntityIconSlot()
{
}


bool EntityIconSlot::addEntityIcon(EntityIcon* icon)
{
	if (!mContainedIcon) {
		mContainedIcon = icon;
		mContainer->addChildWindow(icon->getDragContainer()); 
		icon->setSlot(this);
	} else {
		S_LOG_WARNING("Trying to add entity icon to slot that already has one icon contained.");
	}
	return true;
}

EntityIcon* EntityIconSlot::removeEntityIcon()
{
	if (mContainedIcon) {
		mContainer->removeChildWindow(mContainedIcon->getDragContainer());
		EntityIcon* icon = mContainedIcon;
		icon->setSlot(0);
		return icon;
	} else {
		return 0;
	}
}

void EntityIconSlot::notifyIconRemoved()
{
	mContainedIcon = 0;
}

CEGUI::Window* EntityIconSlot::getWindow()
{
	return mContainer;
}

bool EntityIconSlot::handleDragEnter(const CEGUI::EventArgs& args)
{
	return true;
}
bool EntityIconSlot::handleDragLeave(const CEGUI::EventArgs& args)
{
	return true;
}
bool EntityIconSlot::handleDragDropped(const CEGUI::EventArgs& args)
{
	const DragDropEventArgs& ddea = static_cast<const DragDropEventArgs&>(args);
	DragContainer* container = ddea.dragDropItem;
	if (container) {
		EntityIconUserData* mUserData = static_cast<EntityIconUserData*>(container->getUserData());
		if (mUserData) {
			EntityIcon& entityIcon = mUserData->getEntityIcon();
			addEntityIcon(&entityIcon);
		}
	}
	return true;
}



EntityIconManager::EntityIconManager(GUIManager& guiManager)
: mGuiManager(guiManager), mIconsCounter(0), mSlotsCounter(0)
{
}


EntityIconSlot* EntityIconManager::createSlot()
{
	std::stringstream ss;
	ss << "entityIconSlot" << mSlotsCounter++;
	
	CEGUI::Window* container = mGuiManager.createWindow("DefaultGUISheet", ss.str());
	container->setSize(CEGUI::UVector2(CEGUI::UDim(0, 64), CEGUI::UDim(0, 64)));
	EntityIconSlot* slot = new EntityIconSlot(*this, container);
	mSlots.push_back(slot);
	return slot;
}

EntityIcon* EntityIconManager::createIcon(Gui::Icons::Icon* icon)
{
	std::stringstream ss;
	ss << "entityIcon" << mIconsCounter++;
	
	CEGUI::DragContainer* item = static_cast<CEGUI::DragContainer*>(mGuiManager.createWindow("DragContainer", ss.str()));
	
	if (item) {
		item->setSize(CEGUI::UVector2(CEGUI::UDim(0, 64), CEGUI::UDim(0, 64)));
		//item->setTooltipText(name);
		
		ss << "Image" ;
		CEGUI::Window* iconWindow = mGuiManager.createWindow("EmberLook/StaticImage", ss.str());
		if (iconWindow) {
			iconWindow->setProperty("BackgroundEnabled", "false");
			iconWindow->disable();
// 			iconWindow->setProperty("FrameEnabled", "false");
			iconWindow->setProperty("Image", CEGUI::PropertyHelper::imageToString(icon->getImage()));
			item->addChildWindow(iconWindow);
			
			EntityIcon* entityIcon = new EntityIcon(*this, item, iconWindow, icon);
			mIcons.push_back(entityIcon);
			return entityIcon;
		}
	}
	return 0;
}

void EntityIconManager::destroyIcon(EntityIcon* icon)
{
	EntityIconStore::iterator I = std::find(mIcons.begin(), mIcons.end(), icon);
	if (I != mIcons.end()) {
		mIcons.erase(I);
		delete icon;
	}
}

void EntityIconManager::destroySlot(EntityIconSlot* slot)
{
	EntityIconSlotStore::iterator I = std::find(mSlots.begin(), mSlots.end(), slot);
	if (I != mSlots.end()) {
		mSlots.erase(I);
		delete slot;
	}
}

};
