//
// C++ Implementation: CEGUILogger
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "CEGUILogger.h"
#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"

namespace EmberOgre {

namespace Gui {

CEGUILogger::CEGUILogger()
{
}


CEGUILogger::~CEGUILogger()
{
}

void CEGUILogger::logEvent(const CEGUI::String& message, CEGUI::LoggingLevel level)
{
	///just reroute to the Ember logging service
	static std::string ogre("(CEGUI) ");
	switch (level) {
		case CEGUI::Insane:
			///we won't handle the insane level since it's mainly for internal CEGUI debugging
			return;
		case CEGUI::Informative:
			///informative messages are a little too much to print out
			return;
/*			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("CEGUI", Ember::LoggingService::VERBOSE) << ogre << message.c_str() << Ember::LoggingService::END_MESSAGE;
			break;*/
		case CEGUI::Standard:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("CEGUI", Ember::LoggingService::INFO) << ogre << message.c_str() << Ember::LoggingService::END_MESSAGE;
			break;
		case CEGUI::Warnings:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("CEGUI", Ember::LoggingService::WARNING) << ogre << message.c_str() << Ember::LoggingService::END_MESSAGE;
			break;
		case CEGUI::Errors:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("CEGUI", Ember::LoggingService::FAILURE) << ogre << message.c_str() << Ember::LoggingService::END_MESSAGE;
			break;
			
	}

}

}

}
