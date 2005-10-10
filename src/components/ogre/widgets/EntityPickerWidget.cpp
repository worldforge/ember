//
// C++ Implementation: EntityPickerWidget
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


#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../EmberEntity.h"
#include "../MousePicker.h"
#include "../GUIManager.h"



#include "EntityPickerWidget.h"

#include <CEGUIWindowManager.h>
#include <elements/CEGUIStaticText.h>

//#include <CEGUIWindowProperties.h> 

namespace EmberOgre {

/*template<> WidgetLoader WidgetLoaderHolder<EntityPickerWidget>::loader("EntityPickerWidget", &createWidgetInstance);*/
//WidgetLoader Widget::loader("EntityPickerWidget", &createWidgetInstance<EntityPickerWidget>);


EntityPickerWidget::~EntityPickerWidget()
{
}

void EntityPickerWidget::buildWidget()
{
	mMousePicker = mGuiManager->getMousePicker();
	mMousePicker->EventPickedEntity.connect(SigC::slot(*this, &EntityPickerWidget::pickedEntity));
	mMousePicker->EventPickedNothing.connect(SigC::slot(*this, &EntityPickerWidget::pickedNothing));

	mMainWindow = mWindowManager->createWindow("DefaultGUISheet", (CEGUI::utf8*)"EntityPickerWidget/MainWindow");
	mMainWindow->setPosition(CEGUI::Point(0.25, 0.25f));
	mMainWindow->setSize(CEGUI::Size(0.15f, 0.2f));
	mMainWindow->setAlpha(0.5f);
	mMainWindow->setVisible(false);
	mMainWindow->setEnabled(false);
	mMainWindow->setAlwaysOnTop(true);
	
	mMenuWindow = mWindowManager->createWindow("DefaultGUISheet", "EntityPickerWidget/Menu");
	mMenuWindow->setPosition(CEGUI::Point(0.0, 0.3f));
	mMenuWindow->setSize(CEGUI::Size(1.0f, 0.7f));
	mMenuWindow->setInheritsAlpha(true);	
	mMainWindow->addChildWindow(mMenuWindow);
	
	
	mEntityName = static_cast<CEGUI::StaticText*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"EntityPickerWidget/EntityName"));
	mEntityName->setSize(CEGUI::Size(1.0f, 0.3f));
	mEntityName->setPosition(CEGUI::Point(0.0f, 0.0f));
	mEntityName->setInheritsAlpha(true);	
	mEntityName->setFrameEnabled(false);
	//mEntityName->setBackgroundEnabled(false);
	mEntityName->setHorizontalFormatting(CEGUI::StaticText::HorzCentred);
	mMainWindow->addChildWindow(mEntityName);



/*
	mMainWindow = static_cast<CEGUI::GUISheet*>(mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"EntityPickerWidget/Menu"));
	mMainWindow->setSize(CEGUI::Size(0.2f, 0.2f));
	mMainWindow->setAlwaysOnTop(true);
*/	
	getMainSheet()->addChildWindow(mMainWindow);
	

	CEGUI::PushButton* touchButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/TouchButton"));
	
	BIND_CEGUI_EVENT(touchButton, CEGUI::ButtonBase::EventMouseButtonUp,EntityPickerWidget::buttonTouch_Click );
	touchButton->setText((CEGUI::utf8*)"Touch");
	touchButton->setSize(CEGUI::Size(1.0f, 0.2f));
	touchButton->setPosition(CEGUI::Point(0.0f, 0.0f));
	touchButton->setInheritsAlpha(true);	
	mMenuWindow->addChildWindow(touchButton);
	mButtonSet.insert(touchButton);
	

	CEGUI::PushButton* takeButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/TakeButton"));
	BIND_CEGUI_EVENT(takeButton, CEGUI::ButtonBase::EventMouseButtonUp, EntityPickerWidget::buttonTake_Click);
	takeButton->setText((CEGUI::utf8*)"Take");
	takeButton->setSize(CEGUI::Size(1.0f, 0.2f));
	takeButton->setPosition(CEGUI::Point(0.0f, 0.2f));
	takeButton->setInheritsAlpha(true);	
	mMenuWindow->addChildWindow(takeButton);
	mButtonSet.insert(takeButton);	
	
	
	CEGUI::PushButton* giveButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/GiveButton"));
	BIND_CEGUI_EVENT(giveButton, CEGUI::ButtonBase::EventMouseButtonUp,EntityPickerWidget::buttonGive_Click );
	giveButton->setText((CEGUI::utf8*)"Give");
	giveButton->setSize(CEGUI::Size(1.0f, 0.2f));
	giveButton->setPosition(CEGUI::Point(0.0f, 0.4f));
	giveButton->setInheritsAlpha(true);	
	mMenuWindow->addChildWindow(giveButton);
	mButtonSet.insert(giveButton);	
	
	CEGUI::PushButton* inspectButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/InspectButton"));
	BIND_CEGUI_EVENT(inspectButton, CEGUI::ButtonBase::EventMouseButtonUp,EntityPickerWidget::buttonInspect_Click );
	inspectButton->setText((CEGUI::utf8*)"Inspect");
	inspectButton->setSize(CEGUI::Size(1.0f, 0.2f));
	inspectButton->setPosition(CEGUI::Point(0.0f, 0.6f));
	inspectButton->setInheritsAlpha(true);	
	mMenuWindow->addChildWindow(inspectButton);
	mButtonSet.insert(inspectButton);	

	mUseButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/UseButton"));
	BIND_CEGUI_EVENT(mUseButton, CEGUI::ButtonBase::EventMouseButtonUp, EntityPickerWidget::buttonUse_Click);
	mUseButton->setText((CEGUI::utf8*)"Use");
	mUseButton->setSize(CEGUI::Size(1.0f, 0.2f));
	mUseButton->setPosition(CEGUI::Point(0.0f, 0.8f));
	mUseButton->setInheritsAlpha(true);	
	mMenuWindow->addChildWindow(mUseButton);
	mButtonSet.insert(mUseButton);	
		
		
	GUIManager::getSingleton().getInput()->EventMouseButtonReleased.connect(SigC::slot(*this, &EntityPickerWidget::input_MouseButtonReleased));

}

void EntityPickerWidget::showMenu(CEGUI::Point position)
{
	mMainWindow->setEnabled(true);
	mMainWindow->setVisible(true);
	//mMainWindow_MouseOut_Connection = mMainWindow->subscribeEvent(CEGUI::Window::EventMouseLeaves, 
//		boost::bind(&EntityPickerWidget::mainWindow_MouseOut, this, _1));
	position.d_x = position.d_x - 10.0f;
	position.d_y = position.d_y - 10.0f;
	mMainWindow->setPosition(CEGUI::Absolute, position );

}

void EntityPickerWidget::pickedEntity(const EntityPickResult& result, const MousePickerArgs& args)
{
	EmberEntity* entity = result.entity;
	showMenu(CEGUI::Point(args.windowX, args.windowY));
	CEGUI::String name(entity->getType()->getName() + " ("+ entity->getName() +")");
	mEntityName->setText(name);
	mPickResult = result;
	checkUse();
	
	//TODO: bind the guiManagers onMouseUp event to remove the menu
	

}

void EntityPickerWidget::checkUse()
{
	//try to find the default operation for the wielded entity
	std::string defaultOp = "";
	EmberEntity* wieldedEntity = EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getEntityAttachedToPoint("right_hand_wield");
	if (wieldedEntity) {
		mUseButton->setVisible(true);
		const Eris::Entity::AttrMap::const_iterator I_attribute = wieldedEntity->getAttributes().find("operations");
		if (I_attribute != wieldedEntity->getAttributes().end()) {
			Atlas::Message::Element operations = I_attribute->second;
			if (operations.isList()) {
				const Atlas::Message::ListType & plist = operations.asList();
				Atlas::Message::ListType::const_iterator J = plist.begin();
				if (J != plist.end() && J->isString()) {
					defaultOp = J->asString();
				}				
			}
		} 
		if (defaultOp == "") {
			mUseButton->setText("Use with " + wieldedEntity->getType()->getName());
		} else {
			mUseButton->setText(defaultOp + " with " + wieldedEntity->getType()->getName());
		}
	} else {
		mUseButton->setVisible(false);
	}
}

void EntityPickerWidget::pickedNothing(const MousePickerArgs& args)
{
	if (mMainWindow->isVisible()) {
		removeMenu();
	}
	
	//showMenu(args.position);
	

}


bool EntityPickerWidget::buttonTouch_Click(const CEGUI::EventArgs& args)
{
	EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getErisAvatar()->touch(mPickResult.entity);
	mGuiManager->EventEntityAction("touch", mPickResult.entity);
	//mGuiManager->setDebugText(std::string("Touched ") + mPickResult.entity->getName() );
	removeMenu();
	return true;
}

bool EntityPickerWidget::buttonTake_Click(const CEGUI::EventArgs& args)
{
	EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getErisAvatar()->take(mPickResult.entity);
	mGuiManager->EventEntityAction("take", mPickResult.entity);
//	mGuiManager->setDebugText(std::string("Took ") + mPickedEntity->getName());
	removeMenu();
	return true;
}

bool EntityPickerWidget::buttonInspect_Click(const CEGUI::EventArgs& args)
{
	mGuiManager->EventEntityAction("inspect", mPickResult.entity);
	removeMenu();
	return true;
}

bool EntityPickerWidget::buttonGive_Click(const CEGUI::EventArgs& args)
{
	mGuiManager->EventEntityAction("give", mPickResult.entity);
	removeMenu();
	return true;
}

bool EntityPickerWidget::buttonUse_Click(const CEGUI::EventArgs& args)
{
	//mGuiManager->setDebugText(std::string("Performin Use on  ") + mPickResult.entity->getName() );
	mGuiManager->EventEntityAction("use", mPickResult.entity);
	Ember::EmberServices::getInstance()->getServerService()->use(mPickResult.entity, Ogre2Atlas(mPickResult.position));
	removeMenu();
	return true;
}


bool EntityPickerWidget::mainWindow_MouseOut(const CEGUI::EventArgs& args)
{
/*	const CEGUI::MouseEventArgs& mmargs = static_cast<const CEGUI::MouseEventArgs&>(args);
	CEGUI::Window* aWindow =  mGuiManager->getMainSheet()->getChildAtPosition(mmargs.position);
	if (!mMainWindow->isAncestor(aWindow )) {
		removeMenu();
	}*/
	return true;
}

void EntityPickerWidget::removeMenu()
{
	/*
	std::set<CEGUI::PushButton*>::iterator I = mButtonSet.begin();
	std::set<CEGUI::PushButton*>::iterator I_end = mButtonSet.end();
	
	for (; I != I_end; ++I) {
		mWindowManager->destroyWindow(*I);
	}
	
	mButtonSet.empty();
	*/

//	mMainWindow_MouseOut_Connection.disconnect();
	mMainWindow->setEnabled(false);
	mMainWindow->setVisible(false);
}

void EntityPickerWidget::input_MouseButtonReleased(const Input::MouseButton& button, Input::InputMode mode)
{
	//only show the menu while the left mouse button is pressed
	if (button == Input::MouseButtonLeft) {
		removeMenu();
	}
}



};
