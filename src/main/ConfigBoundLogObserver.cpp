//
// C++ Implementation: ConfigBoundLogObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "ConfigBoundLogObserver.h"

#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

namespace Ember {

ConfigBoundLogObserver::ConfigBoundLogObserver(std::ostream &out) 
: Ember::StreamLogObserver(out)
{
	Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &ConfigBoundLogObserver::ConfigService_EventChangedConfigItem));

}



ConfigBoundLogObserver::~ConfigBoundLogObserver()
{
}

/**
* Updates from the config. The relevant section is "general" and the key "logginglevel". It can have the values of verbose|info|warning|failure|critical
*/
void ConfigBoundLogObserver::updateFromConfig()
{
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("general", "logginglevel")) {
		std::string loggingLevel = static_cast<std::string>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("general", "logginglevel"));
		Ember::Log::MessageImportance importance(Ember::Log::INFO);
		if (loggingLevel == "verbose") {
			importance = Ember::Log::VERBOSE;
		} else if (loggingLevel == "info") {
			importance = Ember::Log::INFO;
		} else if (loggingLevel == "warning") {
			importance = Ember::Log::WARNING;
		} else if (loggingLevel == "failure") {
			importance = Ember::Log::FAILURE;
		} else if (loggingLevel == "critical") {
			importance = Ember::Log::CRITICAL;
		}
		setFilter(importance);
	}
}


/**
	*          React on changes to the config.
	* @param section 
	* @param key 
	*/
void ConfigBoundLogObserver::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "general") {
		if (key == "logginglevel") {
			updateFromConfig();
		}
	}
}

}
