/*
    Copyright (C) 2002  Miguel Guzman Miranda [Aglanor]
                        Joel Schander         [nullstar]
                        Erik Hjortsberg

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
// #include "framework/binreloc.h"

#include <iostream>
#include <cstdlib>
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#ifdef __WIN32__
#include <Windows.h>

//we need this for the PathRemoveFileSpec(...) method
#include <shlwapi.h>
#endif

// #include <iostream>
#include <fstream>

using namespace std;

namespace Ember
{

	const std::string ConfigService::SETVALUE ( "set_value" );
	const std::string ConfigService::GETVALUE ( "get_value" );

	ConfigService::ConfigService() : 
	Service()
	, mSharedDataDir(DATADIR "/ember/")
	, mEtcDir(SYSCONFDIR "/ember/")
	, mHomeDir ( "" )
	, mConfig(new varconf::Config())
	{
#ifdef __WIN32__
		char cwd[512];
		//get the full path for the current executable
		GetModuleFileName ( 0, cwd, 512 );

		//use this utility function for removing the file part
		PathRemoveFileSpec ( cwd );
		baseDir = std::string ( cwd ) + "\\";
#endif

		setName ( "Configuration Service" );
		setDescription ( "Service for management of Ember user-defined configuration" );

		setStatusText ( "Configuration Service status OK." );
	}


	void ConfigService::setPrefix ( const std::string& prefix )
	{
		mSharedDataDir = prefix + "/share/ember/";
		mEtcDir = prefix + "/etc/ember/";
	}

	void ConfigService::setHomeDirectory ( const std::string& path )
	{
		mHomeDir = path;
	}

	const ConfigService::SectionMap& ConfigService::getSection ( const std::string& sectionName )
	{
		return mConfig->getSection ( sectionName );
	}


	varconf::Variable ConfigService::getValue ( const std::string& section, const std::string& key ) const
	{
		return mConfig->getItem ( section, key );
	}

	bool ConfigService::getValue ( const std::string& section, const std::string& key, varconf::Variable& value ) const
	{
		if ( hasItem ( section, key ) )
		{
			value = getValue ( section, key );
			return true;
		}
		return false;
	}

	void ConfigService::setValue ( const std::string& section, const std::string& key, const varconf::Variable& value )
	{
		mConfig->setItem ( section, key, value );
	}

	bool ConfigService::isItemSet ( const std::string& section, const std::string& key, const std::string& value ) const
	{
		return ( hasItem ( section, key ) && getValue ( section, key ) == value );
	}

	Service::Status ConfigService::start()
	{
		mConfig->sige.connect ( sigc::mem_fun ( *this, &ConfigService::configError ) );
		mConfig->sigv.connect ( sigc::mem_fun ( *this, &ConfigService::updatedConfig ) );
		registerConsoleCommands();
		setRunning ( true );
		///can't do this since we must be the first thing initialized
		//LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << getName() << " initialized" << ENDM;
		return Service::OK;
	}

	void ConfigService::stop ( int code )
	{
		Service::stop ( code );
		deregisterConsoleCommands();
		return;
	}

	void ConfigService::deregisterConsoleCommands()
	{
		Ember::ConsoleBackend::getSingletonPtr()->deregisterCommand ( SETVALUE );
		Ember::ConsoleBackend::getSingletonPtr()->deregisterCommand ( GETVALUE );
	}

	void ConfigService::registerConsoleCommands()
	{
		Ember::ConsoleBackend::getSingletonPtr()->registerCommand ( SETVALUE, this, "Sets a value in the configuration. Usage: set_value <section> <key> <value>" );
		Ember::ConsoleBackend::getSingletonPtr()->registerCommand ( GETVALUE, this, "Gets a value from the configuration. Usage: get_value <section> <key>" );
	}

	bool ConfigService::itemExists(const std::string& section, const std::string& key) const
	{
		return hasItem(section, key);
	}
	
	bool ConfigService::hasItem ( const std::string& section, const std::string& key ) const
	{
		return mConfig->find ( section, key );
	}

	bool ConfigService::deleteItem ( const std::string& section, const std::string& key )
	{
		return mConfig->erase ( section, key );
	}

	bool ConfigService::loadSavedConfig ( const std::string& filename )
	{
		S_LOG_INFO ( "Loading shared config file from " << getSharedConfigDirectory() + "/"+ filename << "." );
		bool success = mConfig->readFromFile ( getSharedConfigDirectory() + "/"+ filename, varconf::GLOBAL );
		std::string userConfigPath ( getHomeDirectory() + "/" + filename );
		std::ifstream file ( userConfigPath.c_str() );
		if ( !file.fail() )
		{
			S_LOG_INFO ( "Loading user config file from "<< getHomeDirectory() + "/" + filename <<"." );
			try
			{
				mConfig->parseStream ( file, varconf::USER );
			}
			catch ( varconf::ParseError p )
			{
				std::string p_str ( p );
				S_LOG_FAILURE ( "Error loading user config file: " << p_str );
				return false;
			}
		}
		else
		{
			S_LOG_INFO ( "Could not find any user config file." );
		}

		return success;
	}

	bool ConfigService::saveConfig ( const std::string& filename )
	{
		return mConfig->writeToFile ( filename );
	}

	void ConfigService::runCommand ( const std::string &command, const std::string &args )
	{
		if ( command == SETVALUE )
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens ( args );
			std::string section ( tokeniser.nextToken() );
			std::string key ( tokeniser.nextToken() );
			std::string value ( tokeniser.remainingTokens() );

			if ( section == "" || key == "" || value == "" )
			{
				Ember::ConsoleBackend::getSingletonPtr()->pushMessage ( "Usage: set_value <section> <key> <value>" );
			}
			else
			{
				setValue ( section, key, value );
				Ember::ConsoleBackend::getSingletonPtr()->pushMessage ( "New value set, section: " +  section + " key: " + key + " value: " + value );
			}

		}
		else if ( command == GETVALUE )
		{
			Ember::Tokeniser tokeniser;
			tokeniser.initTokens ( args );
			std::string section ( tokeniser.nextToken() );
			std::string key ( tokeniser.nextToken() );

			if ( section == "" || key == "" )
			{
				Ember::ConsoleBackend::getSingletonPtr()->pushMessage ( "Usage: get_value <section> <key>" );
			}
			else
			{
				if ( !hasItem ( section, key ) )
				{
					Ember::ConsoleBackend::getSingletonPtr()->pushMessage ( "No such value." );
				}
				else
				{
					varconf::Variable value = getValue ( section, key );
					Ember::ConsoleBackend::getSingletonPtr()->pushMessage ( std::string ( "Value: " ) + static_cast<std::string> ( value ) );
				}
			}
		}
	
	}
	
	void ConfigService::updatedConfig (const std::string& section, const std::string& key)
	{
		EventChangedConfigItem.emit ( section, key );
	}
	
	void ConfigService::configError ( const char* error )
	{
		S_LOG_FAILURE ( std::string ( error ) );
	}

	const std::string& ConfigService::getHomeDirectory() const
	{
		///check if the home directory is set, and if so use the setting. If else, fall back to the default path.
		if ( mHomeDir != "" )
		{
			return mHomeDir;
		}
		else
		{
			//taken from Sear
#ifdef __WIN32__
			static std::string finalPath;
			static std::string fallbackPath ( "." );
			if ( !finalPath.empty() )
			{
				return finalPath;
			}
			std::string path ( getenv ( "APPDATA" ) );
			if ( path.empty() )
			{
				const char *homedrive = getenv ( "HOMEDRIVE" );
				const char *homepath = getenv ( "HOMEPATH" );

				if ( !homedrive || !homepath )
				{
					std::cerr << "unable to determine homedir in Win32, using ." << std::endl;
					return fallbackPath;
				}
				path = std::string ( homedrive ) + std::string ( homepath );
			}
			finalPath = path + "\\Ember\\";
			return finalPath;
#elif __APPLE__
			static std::string path ( getAppSupportDirPath() + "/Ember/" );
			return path;
#else
			static std::string path ( std::string ( getenv ( "HOME" ) ) + "/.ember/" );
			return path;
#endif
		}
	}

	const std::string& ConfigService::getSharedDataDirectory() const
	{
		if ( hasItem ( "paths", "sharedir" ) )
		{
			static std::string path ( static_cast<std::string> ( getValue ( "paths", "sharedir" ) ) + "/" );
			return path;
		}
		else
		{
#ifdef __APPLE__
			return getBundleResourceDirPath();
#elif __WIN32__
			return baseDir;
#else
			return mSharedDataDir;
#endif
		}

	}

	const std::string& ConfigService::getSharedConfigDirectory() const
	{
#ifdef __APPLE__
		static std::string path ( getSharedDataDirectory() + "/etc/ember/" );
		return path;
#elif __WIN32__
		static std::string path ( getSharedDataDirectory() + "/etc/ember/" );
		return path;
#else
		return mEtcDir;
#endif
	}

	const std::string& ConfigService::getEmberDataDirectory() const
	{
		if ( hasItem ( "paths", "datadir" ) )
		{
			static std::string path ( static_cast<std::string> ( getValue ( "paths", "datadir" ) ) + "/" );
			return path;
		}
		else
		{
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

	const std::string& ConfigService::getEmberMediaDirectory() const
	{
		static std::string path;
		///look for a media channel key in the config, and if found use that, else use the version of ember as a standard path
		if ( hasItem ( "wfut", "channel" ) )
		{
			path = getEmberDataDirectory() + "/" + static_cast<std::string> ( getValue ( "wfut", "channel" ) ) + "/";
		}
		else
		{
			path = getEmberDataDirectory() + "/ember-media-" + std::string ( VERSION ) + "/";
		}
		return path;
	}


	const std::string& ConfigService::getUserMediaDirectory() const
	{
		static std::string path ( getHomeDirectory() + "/user-media/" );
		return path;
	}

	const std::string& ConfigService::getSharedMediaDirectory() const
	{
		static std::string path ( getSharedDataDirectory() + "media/shared/" );
		return path;
	}

} // namespace Ember
