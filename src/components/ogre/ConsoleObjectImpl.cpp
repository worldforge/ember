/*
	ConsoleObjectImpl.cpp by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// config headers
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// system headers

// library headers
#include "EmberOgrePrerequisites.h"

// local headers
#include "ConsoleObjectImpl.h"
#include "MediaDeployer.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include "EmberOgre.h"

namespace EmberOgre {

// List of Ogre's console commands
const char * const ConsoleObjectImpl::QUIT 		= "quit";
const char * const ConsoleObjectImpl::ADDMEDIA	= "addmedia";
const char * const ConsoleObjectImpl::MOVEMEDIA	= "movemedia";

ConsoleObjectImpl* ConsoleObjectImpl::_consoleObjectImplInstance = 0;

ConsoleObjectImpl & ConsoleObjectImpl::getSingleton(void)
{
	if(_consoleObjectImplInstance == 0)
		_consoleObjectImplInstance = new ConsoleObjectImpl;
	return *_consoleObjectImplInstance;
}

ConsoleObjectImpl::ConsoleObjectImpl(void)
{
	Ember::ConsoleBackend::getMainConsole()->registerCommand(QUIT,this);
	Ember::ConsoleBackend::getMainConsole()->registerCommand(ADDMEDIA,this);
	Ember::ConsoleBackend::getMainConsole()->registerCommand(MOVEMEDIA,this);
}
ConsoleObjectImpl::~ConsoleObjectImpl()
{

}


void ConsoleObjectImpl::runCommand(const std::string &command, const std::string &args)
{
	if(command == QUIT) {
		Ember::ConsoleBackend::getMainConsole()->pushMessage("Bye");
		quit();
	} else if(command == ADDMEDIA) {
	    // Split string
        Ember::Tokeniser tokeniser = Ember::Tokeniser();
        tokeniser.initTokens(args);
        std::string modelName = tokeniser.nextToken();
        std::string id = tokeniser.nextToken();
		Ogre::Vector3 position = Ogre::Vector3(0,0,0);
        MediaDeployer::getSingleton().addMedia(modelName,id,position);

	} else {
		Ember::ConsoleBackend::getMainConsole()->pushMessage("I don't understand this command yet.");
	}
}

void ConsoleObjectImpl::quit()
{
	EmberOgre::getSingleton().shutdown();
}

/*

*/
}

