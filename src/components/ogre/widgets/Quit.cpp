//
// C++ Implementation: Quit
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
#include "Quit.h"
#include "../EmberOgre.h"
#include <elements/CEGUIPushButton.h>

#include "framework/ConsoleBackend.h"

namespace EmberOgre {

const std::string Quit::SOFTQUIT("softquit");

Quit::Quit()
{
}


Quit::~Quit()
{
}

void Quit::buildWidget()
{
	
	Ember::ConsoleBackend::getMainConsole()->registerCommand(SOFTQUIT,this);
	
	loadMainSheet("Quit.widget", "Quit/");
	
	EmberOgre::getSingleton().EventRequestQuit.connect(SigC::slot(*this, &Quit::EmberOgre_RequestQuit));
	
	CEGUI::PushButton* yesButton = static_cast<CEGUI::PushButton*>(getWindow("YesButton"));
	CEGUI::PushButton* noButton = static_cast<CEGUI::PushButton*>(getWindow("NoButton"));
	
	BIND_CEGUI_EVENT(noButton, CEGUI::ButtonBase::EventMouseClick,Quit::No_Click );
	BIND_CEGUI_EVENT(yesButton, CEGUI::ButtonBase::EventMouseClick,Quit::Yes_Click );
	
	
	registerConsoleVisibilityToggleCommand("quit");
	enableCloseButton();
	
	mMainWindow->setVisible(false);
}

bool Quit::Yes_Click(const CEGUI::EventArgs& args)
{
	EmberOgre::getSingleton().shutdown();
	return true;
}

bool Quit::No_Click(const CEGUI::EventArgs& args)
{
	mMainWindow->setVisible(false);
	return true;
}


void Quit::EmberOgre_RequestQuit(bool& handled) 
{
	handled = true;
	//if the window system twice requests a quit, do it
	if (mMainWindow->isVisible()) {
		EmberOgre::getSingleton().shutdown();
	} else {
		softquit();
	}
}

void Quit::softquit()
{
	mMainWindow->activate();
	mMainWindow->moveToFront();
	mMainWindow->setVisible(true);
}


void Quit::runCommand(const std::string &command, const std::string &args)
{
	if(command == SOFTQUIT)
	{
		softquit();
	} else {
		Widget::runCommand(command, args);
	}

}

};
