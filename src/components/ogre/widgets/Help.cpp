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

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"


namespace EmberOgre {

const std::string Help::HELP("help");

Help::Help()
 : Widget()
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


};
