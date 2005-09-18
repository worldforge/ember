/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
                        Joel Schander         [nullstar]

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConfigService.h"
#include "services/logging/LoggingService.h"
#include "framework/prefix.h"

#include <iostream>
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

using namespace std;

namespace Ember
{

    const std::string ConfigService::SETVALUE("set_value");
    const std::string ConfigService::GETVALUE("get_value");

    ConfigService::ConfigService() : Service()
    {
		setName("Configuration Service");
		setDescription("Service for management of Ember user-defined configuration");
    	setStatusText("Configuration Service status OK.");
    }

    varconf::Variable ConfigService::getValue(const std::string& section, const std::string& key) const
    {
		string value;
		value = std::string(varconf::Config::inst()->getItem(section, key));
		return value;
    }

    void ConfigService::setValue(const std::string& section, const std::string& key, const varconf::Variable& value)
    {
		varconf::Config::inst()->setItem(section, key, value);
    }

    Service::Status ConfigService::start()
    {
		updatedConfig_connection = varconf::Config::inst()->sigv.connect(SigC::slot(*this, &ConfigService::updatedConfig));
		configError_connection = varconf::Config::inst()->sige.connect(SigC::slot(*this, &ConfigService::configError));
		registerConsoleCommands();
		setRunning(true);
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << getName() << " initialized" << ENDM;
		return Service::OK;
    }

    void ConfigService::stop(int code)
    {
    	updatedConfig_connection.disconnect();
    	configError_connection.disconnect();
		deregisterConsoleCommands();
		setRunning(false);
		return;
    }

    void ConfigService::deregisterConsoleCommands()
    {
		Ember::ConsoleBackend::getMainConsole()->deregisterCommand(SETVALUE);
		Ember::ConsoleBackend::getMainConsole()->deregisterCommand(GETVALUE);
	}
    
    void ConfigService::registerConsoleCommands()
    {
		Ember::ConsoleBackend::getMainConsole()->registerCommand(SETVALUE, this);
		Ember::ConsoleBackend::getMainConsole()->registerCommand(GETVALUE, this);
	}
	
    bool ConfigService::itemExists(const std::string& section, const std::string& key) const
    {
		return varconf::Config::inst()->find(section, key);
    }

    bool ConfigService::deleteItem(const std::string& section, const std::string& key)
    {
		return varconf::Config::inst()->erase(section, key);
    }

    bool ConfigService::loadSavedConfig(const std::string& filename)
    {
		return varconf::Config::inst()->readFromFile(filename);
    }

    bool ConfigService::saveConfig(const std::string& filename)
    {
		return varconf::Config::inst()->writeToFile(filename);
    }
	
	void ConfigService::runCommand(const std::string &command, const std::string &args)
	{
		if(command == SETVALUE)
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);
			std::string section(tokeniser.nextToken());
			std::string key(tokeniser.nextToken());
			std::string value(tokeniser.remainingTokens());
			
			if (section == "" || key == "" || value == "") {
				Ember::ConsoleBackend::getMainConsole()->pushMessage("Usage: set_value <section> <key> <value>");
			} else {
				setValue(section, key, value);
			}
		
		}
		else if(command == GETVALUE)
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens(args);
			std::string section(tokeniser.nextToken());
			std::string key(tokeniser.nextToken());
			
			if (section == "" || key == "") {
				Ember::ConsoleBackend::getMainConsole()->pushMessage("Usage: get_value <section> <key>");
			} else {
				if (!itemExists(section, key)) {
					Ember::ConsoleBackend::getMainConsole()->pushMessage("No such value.");
				} else {
					varconf::Variable value = getValue(section, key);
					Ember::ConsoleBackend::getMainConsole()->pushMessage(std::string("Value: ") + static_cast<std::string>(value));
				}
			}
		}
	}
	
    void ConfigService::updatedConfig(const std::string& section, const std::string& key)
    {
    	EventChangedConfigItem.emit(section, key);
    }
   
	void ConfigService::configError(const std::string& error)
	{
		S_LOG_FAILURE(std::string(error));
	}
	
	const std::string ConfigService::getHomeDirectory() const
	{
		return std::string(getenv("HOME")) + "/.ember/";

	}

	const std::string ConfigService::getSharedDataDirectory() const
	{
		if (itemExists("paths", "sharedir")) {
			return std::string(getValue("paths", "sharedir")) + "/";
		} else {
			return BR_DATADIR("/games/ember/");
		}

	}
	
	const std::string ConfigService::getEmberDataDirectory() const
	{
		if (itemExists("paths", "datadir")) {
			return std::string(getValue("paths", "datadir")) + "/";
		} else {
			return getHomeDirectory();
		}

	}

	const std::string ConfigService::getUserMediaDirectory() const
	{
		return getEmberDataDirectory() + "/ember-media/";
	}
	
	const std::string ConfigService::getSharedMediaDirectory() const
	{
		return getSharedDataDirectory() + "/media/";
	}

} // namespace Ember
