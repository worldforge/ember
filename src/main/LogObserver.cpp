//
// C++ Implementation: LogObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "LogObserver.h"

#include "services/EmberServices.h"
#include "services/logging/LoggingService.h"
#include "services/config/ConfigService.h"

namespace Ember {

LogObserver::LogObserver(std::ostream &out) 
: Ember::StreamLogObserver(out)
{
	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &LogObserver::ConfigService_EventChangedConfigItem));

}



LogObserver::~LogObserver()
{
}

/**
	* Updates from the config. The relevant section is "general" and the key "logginglevel". It can have the values of verbose|info|warning|failure|critical
	*/
void LogObserver::updateFromConfig()
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("general", "logginglevel")) {
		std::string loggingLevel = static_cast<std::string>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("general", "logginglevel"));
		Ember::LoggingService::MessageImportance importance(Ember::LoggingService::INFO);
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
void LogObserver::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "general") {
		if (key == "logginglevel") {
			updateFromConfig();
		}
	}
}

}
