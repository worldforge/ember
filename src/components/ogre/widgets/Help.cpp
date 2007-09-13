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
#include <elements/CEGUIGUISheet.h>

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
// #include "../PersonEmberEntity.h"
#include "../AvatarEmberEntity.h"
#include "../EmberOgre.h"

#include <CEGUIWindowManager.h>
#include <CEGUIExceptions.h>

using namespace CEGUI;
namespace EmberOgre {
namespace Gui {


Help::Help()
 : HelpCommand("help", this, "Display the help.")
 , mTimeUntilShowBlurb(30)
 , mTimeBlurbShown(0)
 , mTimeToShowBlurb(10)
 , mBlurb(0)
{

}


Help::~Help()
{
}


void Help::buildWidget()
{

	loadMainSheet("HelpWidget.xml", "Help/");
	
	enableCloseButton();
	
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();

	if (configSrv->itemExists("general", "startuphelp") && ((bool)configSrv->getValue("general", "startuphelp"))) {
		mMainWindow->setVisible(true);
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
	if(HelpCommand == command)
	{
		show();
	} else {
		Widget::runCommand(command, args);
	}

}

void Help::EmberOgre_CreatedAvatarEntity(AvatarEmberEntity* entity)
{
	///Show a small helpful blurb about the gui system
	if (!mBlurb) {
		try {
			mBlurb = static_cast<CEGUI::GUISheet*>(mWindowManager->createWindow(getDefaultScheme() + "/StaticText", (CEGUI::utf8*)"Help/Blurb"));
			mBlurb->setSize(UVector2(UDim(0.3f, 0), UDim(0.1f, 0)));
			mBlurb->setPosition(UVector2(UDim(0.35f, 0), UDim(0.3f, 0)));
// 			mBlurb->setFrameEnabled(false);
		//	mBlurb->setInheritAlpha(true);
			//mEntityName->setBackgroundEnabled(false);
			mBlurb->setProperty("HorzFormatting", "WordWrapLeftAligned");
			mBlurb->setText("Click right mouse button to switch between MOVEMENT and INPUT MODE.");
			
			
			getMainSheet()->addChildWindow(mBlurb);
			mBlurb->setVisible(false);
			mTimeBlurbShown = 0;
		} catch (const CEGUI::Exception& ex) {
			S_LOG_FAILURE("Error when creating help blurb. Message:\n" << ex.getMessage().c_str());
		}
	}
	
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
}
};
