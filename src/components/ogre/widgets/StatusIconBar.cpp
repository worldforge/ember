//
// C++ Implementation: StatusIconBar
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "StatusIconBar.h"

#include "../GUIManager.h"
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include "framework/ConsoleBackend.h"
#include <elements/CEGUIFrameWindow.h>
#include "../EmberOgre.h"
#include "../input/Input.h"

#include "IconBase.h"

using namespace CEGUI;
namespace EmberOgre {

StatusIconBar::StatusIconBar() : mOriginalCursorImage(0), mIconBar("statusIcons")
{
}


StatusIconBar::~StatusIconBar()
{
}


void StatusIconBar::buildWidget()
{

	getMainSheet()->addChildWindow(mIconBar.getWindow());
	
	const Image* background = IconBase::loadImageFromImageset("iconset_standard", "background_A");
	const Image* borderactive = IconBase::loadImageFromImageset("iconset_standard", "ring_over");
	const Image* borderinactive = IconBase::loadImageFromImageset("iconset_standard", "ring");
	const Image* foreground;
	IconBase* helpIconBase;
	
	foreground = IconBase::loadImageFromImageset("iconset_standard", "close2");
	helpIconBase = new IconBase("close2", background, foreground, borderinactive, borderactive);
	mIconBar.addIcon(helpIconBase);
	helpIconBase->getButton()->setTooltipText("Click here to exit Ember.");
	
	BIND_CEGUI_EVENT(helpIconBase->getButton(), ButtonBase::EventMouseClick, StatusIconBar::close_MouseClick);
	
	foreground = IconBase::loadImageFromImageset("iconset_standard", "question");
	helpIconBase = new IconBase("help", background, foreground, borderinactive, borderactive);
	mIconBar.addIcon(helpIconBase);
	helpIconBase->getButton()->setTooltipText("Click here to access the help.");
	
	BIND_CEGUI_EVENT(helpIconBase->getButton(), ButtonBase::EventMouseClick, StatusIconBar::help_MouseClick);
	
	
	//load all of the movement status images
	mMovementImage_walk	= IconBase::loadImageFromImageset("iconset_standard", "walk");
	mMovementImage_run	= IconBase::loadImageFromImageset("iconset_standard", "run");
	mMovementImage_gui	= IconBase::loadImageFromImageset("iconset_standard", "abc");
	
	mMovementModeIcon = new IconBase("movementmode", background, mMovementImage_gui, borderinactive, borderactive);
	mIconBar.addIcon(mMovementModeIcon);
	mMovementModeIcon->getButton()->setTooltipText("This shows your current input mode.\nUse the right mouse button for movement mode.\nDouble click also switches modes. Press and hold shift to run.");
	
	BIND_CEGUI_EVENT(mMovementModeIcon->getButton(), ButtonBase::EventMouseClick, StatusIconBar::movement_MouseClick);
	
	Input::getSingleton().EventChangedInputMode.connect(sigc::mem_fun(*this, &StatusIconBar::Input_InputModeChanged));
	EmberOgre::getSingleton().getAvatarController()->EventMovementModeChanged.connect(sigc::mem_fun(*this, &StatusIconBar::AvatarController_MovementModeChanged));
	
	float height = mIconBar.getAbsoluteHeight();
	mIconBar.getWindow()->setPosition(UVector2(UDim(0,0), UDim(1, -height)));
	
/*	
	foreground = IconBase::loadImageFromFile("attack.png");
	helpIconBase = new IconBase("attack", background, foreground, borderinactive, borderactive);
	getMainSheet()->addChildWindow(helpIconBase->getContainer());
	helpIconBase->getContainer()->setPosition(Absolute, Point(350, 700));
	
	foreground = IconBase::loadImageFromFile("defend.png");
	helpIconBase = new IconBase("defend", background, foreground, borderinactive, borderactive);
	getMainSheet()->addChildWindow(helpIconBase->getContainer());
	helpIconBase->getContainer()->setPosition(Absolute, Point(400, 700));
	
	foreground = IconBase::loadImageFromFile("spell.png");
	helpIconBase = new IconBase("spell", background, foreground, borderinactive, borderactive);
	getMainSheet()->addChildWindow(helpIconBase->getContainer());
	helpIconBase->getContainer()->setPosition(Absolute, Point(450, 700));*/


}

bool StatusIconBar::help_MouseClick(const EventArgs& args)
{
	Ember::ConsoleBackend::getMainConsole()->runCommand("/help");
	return true;
}

bool StatusIconBar::close_MouseClick(const EventArgs& args)
{
	Ember::ConsoleBackend::getMainConsole()->runCommand("/softquit");
	return true;
}

bool StatusIconBar::movement_MouseClick(const EventArgs& args)
{
	mGuiManager->getInput().toggleInputMode();
	return true;
}

void StatusIconBar::Input_InputModeChanged(Input::InputMode mode) {
	mCurrentMode = mode;
	if (mode == Input::IM_GUI) {
		mMovementModeIcon->setForeground(mMovementImage_gui);
		if (mOriginalCursorImage) {
			MouseCursor::getSingleton().setImage(mOriginalCursorImage);
			mOriginalCursorImage = 0;
		}
	} else {
		checkMovementMode();
	}
	

}

void StatusIconBar::AvatarController_MovementModeChanged(AvatarMovementMode::Mode mode)
{
	if (mCurrentMode == Input::IM_MOVEMENT) {
		checkMovementMode();
	}

}
	
void StatusIconBar::checkMovementMode()
{
	if (mOriginalCursorImage == 0) {
		mOriginalCursorImage = MouseCursor::getSingleton().getImage();
	}
	
	if (EmberOgre::getSingleton().getAvatarController()->getCurrentMovement().mode == AvatarMovementMode::MM_RUN) {
		mMovementModeIcon->setForeground(mMovementImage_run);
		MouseCursor::getSingleton().setImage(mMovementImage_run);
	} else {
		mMovementModeIcon->setForeground(mMovementImage_walk);
		MouseCursor::getSingleton().setImage(mMovementImage_walk);
	}
}




}
