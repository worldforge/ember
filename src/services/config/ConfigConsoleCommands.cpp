/*
 Copyright (C) 2019 Erik Ogenvik

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

#include "ConfigConsoleCommands.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

namespace Ember {

ConfigConsoleCommands::ConfigConsoleCommands(ConfigService& configService) :
		SetValue("set_value", this, "Sets a value in the configuration. Usage: set_value <section> <key> <value>"),
		GetValue("get_value", this, "Gets a value from the configuration. Usage: get_value <section> <key>"),
		mConfigService(configService) {
}

void ConfigConsoleCommands::runCommand(const std::string& command, const std::string& args) {
	if (SetValue == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string section(tokeniser.nextToken());
		std::string key(tokeniser.nextToken());
		std::string value(tokeniser.remainingTokens());

		if (section.empty() || key.empty() || value.empty()) {
			ConsoleBackend::getSingleton().pushMessage("Usage: set_value <section> <key> <value>", "help");
		} else {
			mConfigService.setValue(section, key, value);
			ConsoleBackend::getSingleton().pushMessage("New value set, section: " + section + " key: " + key + " value: " + value, "info");
		}

	} else if (GetValue == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string section(tokeniser.nextToken());
		std::string key(tokeniser.nextToken());

		if (section.empty() || key.empty()) {
			ConsoleBackend::getSingleton().pushMessage("Usage: get_value <section> <key>", "help");
		} else {
			if (!mConfigService.hasItem(section, key)) {
				ConsoleBackend::getSingleton().pushMessage("No such value.", "error");
			} else {
				varconf::Variable value = mConfigService.getValue(section, key);
				ConsoleBackend::getSingleton().pushMessage(std::string("Value: ") + value.as_string(), "info");
			}
		}
	}
}


}