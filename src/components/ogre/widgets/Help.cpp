//
// C++ Implementation: Help
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
#include "Help.h"
#include "Widget.h"
#include "../GUIManager.h"
#include <CEGUIImagesetManager.h> 
#include <CEGUIImageset.h> 
#include "framework/ConsoleBackend.h"
#include <elements/CEGUIFrameWindow.h>
#include <elements/CEGUIStaticText.h>

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberOgre.h"

#include <CEGUIWindowManager.h>

namespace EmberOgre {

const std::string Help::HELP("help");

Help::Help()
 : mTimeUntilShowBlurb(30), mTimeBlurbShown(0), mTimeToShowBlurb(10), mBlurb(0)
{
	Ember::ConsoleBackend::getMainConsole()->registerCommand(HELP,this);

}


Help::~Help()
{
}


void Help::buildWidget()
{

	loadMainSheet("HelpWidget.xml", "Help/");
	
	enableCloseButton();
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getInstance()->getConfigService();

	if (configSrv->itemExists("general", "startuphelp") && !((bool)configSrv->getValue("general", "startuphelp"))) {
		mMainWindow->setVisible(false);
	}

	//connect to the creation of the avatar, since we want to show a help blurb about the movement
	EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &Help::EmberOgre_CreatedAvatarEntity));

}



void Help::show()
{
	if (mMainWindow) {
		mMainWindow->setVisible(true);
		mMainWindow->moveToFront();
	}
}

void Help::hide()
{
	if (mMainWindow) {
		mMainWindow->setVisible(false);
	}
}


void Help::runCommand(const std::string &command, const std::string &args)
{
	if(command == HELP)
	{
		show();
	} else {
		Widget::runCommand(command, args);
	}

}

void Help::EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity)
{
	mBlurb = static_cast<CEGUI::StaticText*>(mWindowManager->createWindow((CEGUI::utf8*)"TaharezLook/StaticText", (CEGUI::utf8*)"Help/Blurb"));
	mBlurb->setSize(CEGUI::Size(0.3f, 0.1f));
	mBlurb->setPosition(CEGUI::Point(0.35f, 0.3f));
	mBlurb->setFrameEnabled(false);
//	mBlurb->setInheritAlpha(true);
	//mEntityName->setBackgroundEnabled(false);
	mBlurb->setHorizontalFormatting(CEGUI::StaticText::WordWrapLeftAligned);
	mBlurb->setText("Press and hold right mouse button to switch between MOVEMENT and INPUT MODE.\nDouble right click to toggle on and off.");
	
	
	getMainSheet()->addChildWindow(mBlurb);
	mBlurb->setVisible(false);
	mTimeBlurbShown = 0;
	
}

void Help::frameStarted(const Ogre::FrameEvent& evt)
{
	if (mBlurb) {
		if (!mBlurb->isVisible()) {
			mTimeUntilShowBlurb -= evt.timeSinceLastFrame;
			if (mTimeUntilShowBlurb < 0) {
				mBlurb->setVisible(true);
			}
		} else {
			mTimeBlurbShown += evt.timeSinceLastFrame;
			mBlurb->setAlpha(1.0f - (mTimeBlurbShown / mTimeToShowBlurb));
			
			if (mTimeBlurbShown > mTimeToShowBlurb) {
				mWindowManager->destroyWindow(mBlurb);
				mBlurb = 0;
			}
		}
	}
}

};
