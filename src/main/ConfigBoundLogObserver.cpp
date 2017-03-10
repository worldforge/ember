//
// C++ Implementation: ConfigBoundLogObserver
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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

#include "services/config/ConfigService.h"

namespace Ember {

ConfigBoundLogObserver::ConfigBoundLogObserver(ConfigService& configService, std::ostream &out)
: StreamLogObserver(out), mConfigService(configService)
{
	configService.EventChangedConfigItem.connect(sigc::mem_fun(*this, &ConfigBoundLogObserver::ConfigService_EventChangedConfigItem));

}


ConfigBoundLogObserver::~ConfigBoundLogObserver()
{
}

void ConfigBoundLogObserver::updateFromConfig()
{
	if (mConfigService.itemExists("general", "logginglevel")) {
		std::string loggingLevel = static_cast<std::string>(mConfigService.getValue("general", "logginglevel"));
		Log::MessageImportance importance(Log::INFO);
		if (loggingLevel == "verbose") {
			importance = Log::VERBOSE;
		} else if (loggingLevel == "info") {
			importance = Log::INFO;
		} else if (loggingLevel == "warning") {
			importance = Log::WARNING;
		} else if (loggingLevel == "failure") {
			importance = Log::FAILURE;
		} else if (loggingLevel == "critical") {
			importance = Log::CRITICAL;
		}
		setFilter(importance);
	}
	if (mConfigService.itemExists("general", "loggingdetailed")) {
		varconf::Variable detailed = mConfigService.getValue("general", "loggingdetailed");
		if (detailed.is_bool()) {
			setDetailed(static_cast<bool>(detailed));
		}
	}
}

void ConfigBoundLogObserver::ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key)
{
	if (section == "general") {
		if (key == "logginglevel" || key == "loggingdetailed") {
			updateFromConfig();
		}
	}
}

}
