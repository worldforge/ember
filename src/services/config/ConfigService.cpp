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
#include "framework/binreloc.h"

#include <iostream>
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#ifdef __WIN32__
#include <Windows.h>

//we need this for the PathRemoveFileSpec(...) method
#include <shlwapi.h>
#endif


using namespace std;

namespace Ember
{

    const std::string ConfigService::SETVALUE("set_value");
    const std::string ConfigService::GETVALUE("get_value");

    ConfigService::ConfigService() : Service()
    {
#ifdef __WIN32__
		char cwd[512];
		//get the full path for the current executable
		GetModuleFileName(0, cwd, 512);

		//use this utility function for removing the file part
		PathRemoveFileSpec(cwd);
		baseDir = std::string(cwd) + "\\";
#endif

#ifdef ENABLE_BINRELOC
    BrInitError error;

    if (br_init (&error) == 0 && error != BR_INIT_ERROR_DISABLED) {
        printf ("Warning: BinReloc failed to initialize (error code %d)\n", error);
        printf ("Will fallback to hardcoded default path.\n");
    }	
	
	char* br_datadir = br_find_data_dir(br_strcat(PREFIX, "/share"));
	sharedDataDir = std::string(br_datadir) + "/games/ember/";
	free(br_datadir);
	
	char* br_etcdir = br_find_etc_dir(br_strcat(PREFIX, "/etc"));
	etcDir = std::string(br_etcdir) + "/ember/";
	free(br_etcdir);
   
#endif

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
		configError_connection = varconf::Config::inst()->sige.connect(SigC::slot(*this, &ConfigService::configError));
		updatedConfig_connection = varconf::Config::inst()->sigv.connect(SigC::slot(*this, &ConfigService::updatedConfig));
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
				Ember::ConsoleBackend::getMainConsole()->pushMessage("New value set, section: " +  section + " key: " + key + " value: " + value);
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
   
	void ConfigService::configError(const char* error)
	{
		S_LOG_FAILURE(std::string(error));
	}
	
	const std::string ConfigService::getHomeDirectory() const
	{
		//taken from Sear
#ifdef __WIN32__
		std::string path = getenv("APPDATA");
		if (path.empty()) {
			const char *homedrive = getenv("HOMEDRIVE");
			const char *homepath = getenv("HOMEPATH");
		    
			if (!homedrive || !homepath) {
				std::cerr << "unable to determine homedir in Win32, using ." << std::endl;
				return ".";
			}
			path = std::string(homedrive) + std::string(homepath);
		}
		return path + "\\Ember\\";
#elif __APPLE__
			return getAppSupportDirPath() + "/Ember/";
#else
				return std::string(getenv("HOME")) + "/.ember/";
#endif
	}

	const std::string ConfigService::getSharedDataDirectory() const
	{
		if (itemExists("paths", "sharedir")) {
			return std::string(getValue("paths", "sharedir")) + "/";
		} else {
#ifdef __APPLE__
			return getBundleResourceDirPath();
#elif __WIN32__
			return baseDir;
#else
			return sharedDataDir;
#endif
		}

	}
	
	const std::string ConfigService::getSharedConfigDirectory() const
	{
#ifdef __APPLE__
		return getSharedDataDirectory() + "etc/ember/";
#elif __WIN32__ 
		return getSharedDataDirectory() + "etc/ember/";
#else
		return etcDir;
#endif
	}
	
	const std::string ConfigService::getEmberDataDirectory() const
	{
		if (itemExists("paths", "datadir")) {
			return std::string(getValue("paths", "datadir")) + "/";
		} else {
//#ifdef __APPLE__
//			return getBundleResourceDirPath();
//#elif __WIN32__
//			return baseDir;
//#else
//			return BR_DATADIR("/games/ember/");
//#endif
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
