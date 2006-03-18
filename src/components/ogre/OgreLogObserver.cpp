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
#include "OgreLogObserver.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
//#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"


namespace EmberOgre {



OgreLogObserver::OgreLogObserver(std::ostream &out) : Ember::StreamLogObserver(out)
{
	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &OgreLogObserver::ConfigService_EventChangedConfigItem));

}

OgreLogObserver::~OgreLogObserver()
{
	
}

/**
	* Updates from the config. The relevant section is "general" and the key "logginglevel". It can have the values of verbose|info|warning|failure|critical
	*/
void OgreLogObserver::updateFromConfig()
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("general", "logginglevel")) {
		std::string loggingLevel = static_cast<std::string>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("general", "logginglevel"));
		Ember::LoggingService::MessageImportance importance;
		if (loggingLevel == "verbose") {
			importance = Ember::LoggingService::VERBOSE;
		} else if (loggingLevel == "info") {
			importance = Ember::LoggingService::INFO;
		} else if (loggingLevel == "warning") {
			importance = Ember::LoggingService::WARNING;
		} else if (loggingLevel == "failure") {
			importance = Ember::LoggingService::FAILURE;
		} else if (loggingLevel == "critical") {
			importance = Ember::LoggingService::CRITICAL;
		}
		setFilter(importance);
	}
}


/**
	*          React on changes to the config.
	* @param section 
	* @param key 
	*/
void OgreLogObserver::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "general") {
		if (key == "logginglevel") {
			updateFromConfig();
		}
	}
}
			
			
void OgreLogObserver::write( const Ogre::String& name,
							const Ogre::String& message, 
							Ogre::LogMessageLevel lml, 
							bool maskDebug )
{
	switch (lml) {
		case Ogre::LML_TRIVIAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->log("Ogre", Ember::LoggingService::VERBOSE,  message.c_str());
			break;
		case Ogre::LML_NORMAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->log("Ogre", Ember::LoggingService::INFO,  message.c_str());
			break;
		case Ogre::LML_CRITICAL:
			Ember::EmberServices::getSingletonPtr()->getLoggingService()->log("Ogre", Ember::LoggingService::FAILURE,  message.c_str());
			break;
	}
}
			



}
