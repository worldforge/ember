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

#include "DimeOgre.h"
#include "Avatar.h"
#include "Widget.h"
#include "DimeEntity.h"
#include "MousePicker.h"
#include "GUIManager.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "AvatarDimeEntity.h"


#include "EntityPickerWidget.h"

//#include <CEGUIWindowProperties.h> 

namespace DimeOgre {

EntityPickerWidget::EntityPickerWidget(GUIManager* guiManager, MousePicker* mousePicker) : Widget::Widget(guiManager),
mMousePicker(mousePicker)
{

	mousePicker->EventPickedEntity.connect(SigC::slot(*this, &EntityPickerWidget::pickedEntity));
	mousePicker->EventPickedNothing.connect(SigC::slot(*this, &EntityPickerWidget::pickedNothing));

}


EntityPickerWidget::~EntityPickerWidget()
{
}

void EntityPickerWidget::buildWidget()
{

	mMainWindow = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"EntityPickerWidget/MainWindow");
	mMainWindow->setPosition(CEGUI::Point(0.25, 0.25f));
	mMainWindow->setSize(CEGUI::Size(0.2f, 0.2f));
	
	mMenuWindow = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"EntityPickerWidget/Menu");
	mMenuWindow->setPosition(CEGUI::Point(0.1, 0.3f));
	mMenuWindow->setSize(CEGUI::Size(0.8f, 0.6f));
	mMainWindow->setVisible(false);
	mMainWindow->setEnabled(false);
	mMainWindow->addChildWindow(mMenuWindow);
	mMainWindow->setAlwaysOnTop(true);
	
	
	mEntityName = static_cast<CEGUI::StaticText*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"EntityPickerWidget/EntityName"));
	mEntityName->setSize(CEGUI::Size(1.0f, 0.3f));
	mEntityName->setPosition(CEGUI::Point(0.0f, 0.0f));
	mMainWindow->addChildWindow(mEntityName);



/*
	mMainWindow = static_cast<CEGUI::GUISheet*>(mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"EntityPickerWidget/Menu"));
	mMainWindow->setSize(CEGUI::Size(0.2f, 0.2f));
	mMainWindow->setAlwaysOnTop(true);
*/	
	getMainSheet()->addChildWindow(mMainWindow);
	

	CEGUI::PushButton* touchButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/TouchButton"));
	
	touchButton->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp, 
		boost::bind(&EntityPickerWidget::buttonTouch_Click, this, _1));
	touchButton->setText((CEGUI::utf8*)"Touch");
	touchButton->setSize(CEGUI::Size(1.0f, 0.5f));
	touchButton->setPosition(CEGUI::Point(0.0f, 0.0f));
	mMenuWindow->addChildWindow(touchButton);
	mButtonSet.insert(touchButton);
	

	CEGUI::PushButton* takeButton = static_cast<CEGUI::PushButton*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/Button", (CEGUI::utf8*)"EntityPickerWidget/TakeButton"));
	takeButton->subscribeEvent(CEGUI::ButtonBase::EventMouseButtonUp, 
		boost::bind(&EntityPickerWidget::buttonTake_Click, this, _1));
	takeButton->setText((CEGUI::utf8*)"Take");
	takeButton->setSize(CEGUI::Size(1.0f, 0.5f));
	takeButton->setPosition(CEGUI::Point(0.0f, 0.5f));
	mMenuWindow->addChildWindow(takeButton);
	mButtonSet.insert(takeButton);	
	

}

void EntityPickerWidget::showMenu(CEGUI::Point position)
{
	mMainWindow->setEnabled(true);
	mMainWindow->setVisible(true);
	mMainWindow_MouseOut_Connection = mMainWindow->subscribeEvent(CEGUI::Window::EventMouseLeaves, 
		boost::bind(&EntityPickerWidget::mainWindow_MouseOut, this, _1));
	position.d_x = position.d_x - 10.0f;
	position.d_y = position.d_y - 10.0f;
	mMainWindow->setPosition(CEGUI::Absolute, position );

}

void EntityPickerWidget::pickedEntity(DimeEntity* entity, const CEGUI::MouseEventArgs& args)
{
	showMenu(args.position);
	mEntityName->setText(entity->getName());
	mPickedEntity = entity;
	
	//TODO: bind the guiManagers onMouseUp event to remove the menu
	

}

void EntityPickerWidget::pickedNothing(const CEGUI::MouseEventArgs& args)
{
	if (mMainWindow->isVisible()) {
		removeMenu();
	}
	
	//showMenu(args.position);
	

}


bool EntityPickerWidget::buttonTouch_Click(const CEGUI::EventArgs& args)
{
	DimeOgre::getSingleton().getAvatar()->getAvatarDimeEntity()->getErisAvatar()->touch(mPickedEntity);
	mGuiManager->setDebugText(std::string("Touched ") + mPickedEntity->getName() );
	removeMenu();
	return true;
}

bool EntityPickerWidget::buttonTake_Click(const CEGUI::EventArgs& args)
{
	DimeOgre::getSingleton().getAvatar()->getAvatarDimeEntity()->getErisAvatar()->take(mPickedEntity);
	mGuiManager->setDebugText(std::string("Took ") + mPickedEntity->getName());
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

	mMainWindow_MouseOut_Connection.disconnect();
	mMainWindow->setEnabled(false);
	mMainWindow->setVisible(false);
}

};
