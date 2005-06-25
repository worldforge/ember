//
// C++ Interface: EntityPickerWidget
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
#ifndef DIMEOGREENTITYPICKERWIDGET_H
#define DIMEOGREENTITYPICKERWIDGET_H

#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <CEGUIEvent.h> 

namespace EmberOgre {

class Widget;
class EmberEntity;
class MousePicker;
struct MousePickerArgs;

/**
@author Erik Hjortsberg

This widget will pop up when a EmberEntity is picked
*/
class EntityPickerWidget : public Widget {
public:
 
    ~EntityPickerWidget();
	
	//virtual void frameStarted(const Ogre::FrameEvent& evt);
	virtual void buildWidget();
	
protected:
	MousePicker* mMousePicker;
	CEGUI::Window* mMenuWindow;
	
	CEGUI::StaticText* mEntityName;
	
	EmberEntity* mPickedEntity;
	
	std::set<CEGUI::PushButton*> mButtonSet;
	
	void pickedEntity(EmberEntity* entity, const MousePickerArgs& args);
	void pickedNothing(const MousePickerArgs& args);
	
	bool buttonTouch_Click(const CEGUI::EventArgs& args);
	bool buttonTake_Click(const CEGUI::EventArgs& args);
	bool buttonInspect_Click(const CEGUI::EventArgs& args);
	bool buttonGive_Click(const CEGUI::EventArgs& args);
	bool buttonUse_Click(const CEGUI::EventArgs& args);
	
	
	bool mainWindow_MouseOut(const CEGUI::EventArgs& args);
	
	CEGUI::Event::Connection mMainWindow_MouseOut_Connection;

	CEGUI::PushButton* mUseButton;
	void checkUse();

	void removeMenu();
	void showMenu(CEGUI::Point position);

};

};

#endif
