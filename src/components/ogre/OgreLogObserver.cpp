//
// C++ Implementation: OgreLogObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "OgreLogObserver.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"


using namespace Ogre;
namespace EmberOgre {



OgreLogObserver::OgreLogObserver()
{
}

OgreLogObserver::~OgreLogObserver()
{
	
}


			
void OgreLogObserver::messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName )	
{
	static std::string ogre("(Ogre) ");
	///if there's a problem setting material name, log as verbose as these messages are quite common and not necessarily errors since the Model format in many cases overrides the material defined in the mesh
	if (Ogre::StringUtil::startsWith(message, "Can't assign material", false)) {
		lml = Ogre::LML_TRIVIAL;
	}
	switch (lml) {
		case Ogre::LML_TRIVIAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("Ogre", Ember::LoggingService::VERBOSE) << ogre << message << Ember::LoggingService::END_MESSAGE;
			break;
		case Ogre::LML_NORMAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("Ogre", Ember::LoggingService::INFO) << ogre << message << Ember::LoggingService::END_MESSAGE;
			break;
		case Ogre::LML_CRITICAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->slog("Ogre", Ember::LoggingService::FAILURE) << ogre << message << Ember::LoggingService::END_MESSAGE;
			break;
			
	}
}
			



}
