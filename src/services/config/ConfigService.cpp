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
#include "framework/LoggingInstance.h"
// #include "framework/binreloc.h"

#include <iostream>
#include <cstdlib>
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#ifdef _WIN32
#include "platform/platform_windows.h"

//we need this for the PathRemoveFileSpec(...) method
#include <shlwapi.h>
#include <Shlobj.h>
#endif

// #include <iostream>
#include <fstream>

// From sear
#ifdef __APPLE__

#include <CoreFoundation/CFBundle.h>
#include <CoreServices/CoreServices.h>

using namespace Ember;

std::string getBundleResourceDirPath()
{
	/* the following code looks for the base package directly inside
	the application bundle. This can be changed fairly easily by
	fiddling with the code below. And yes, I know it's ugly and verbose.
	*/
	std::string result;
#ifdef BUILD_WEBEMBER

	const char* strBundleID = "com.WebEmberLib.WebEmber";
	CFStringRef bundleID = CFStringCreateWithCString(kCFAllocatorDefault,strBundleID,kCFStringEncodingMacRoman);
	CFBundleRef appBundle = CFBundleGetBundleWithIdentifier(bundleID);
	CFRelease(bundleID);
	if(!appBundle){
		S_LOG_FAILURE("Bundle with identifier " << strBundleID << " not found!");
	}else{
#else
		CFBundleRef appBundle = CFBundleGetMainBundle();
#endif
		CFURLRef resUrl = CFBundleCopyResourcesDirectoryURL(appBundle);
		CFURLRef absResUrl = CFURLCopyAbsoluteURL(resUrl);

		// now convert down to a path, and the a c-string
		CFStringRef path = CFURLCopyFileSystemPath(absResUrl, kCFURLPOSIXPathStyle);
		result = CFStringGetCStringPtr(path, CFStringGetSystemEncoding());

		CFRelease(resUrl);
		CFRelease(absResUrl);
		CFRelease(path);
#ifdef BUILD_WEBEMBER
	}
#endif
	return result;
}

std::string getAppSupportDirPath()
{
	FSRef fs;
	OSErr err = FSFindFolder(kUserDomain, kApplicationSupportFolderType, true, &fs);
	if (err != noErr) {
		S_LOG_FAILURE("error doing FindFolder");
		return std::string();
	}

	CFURLRef dirURL = CFURLCreateFromFSRef(kCFAllocatorSystemDefault, &fs);
	char fsRepr[1024];
	if (!CFURLGetFileSystemRepresentation(dirURL, true, (UInt8*) fsRepr, 1024)) {
		std::cerr << "error invoking CFURLGetFileSystemRepresentation" << std::endl;
		return std::string();
	}

	CFRelease(dirURL);
	return fsRepr;
}

#endif

namespace Ember
{
	
	const std::string ConfigService::SETVALUE ( "set_value" );
	const std::string ConfigService::GETVALUE ( "get_value" );

	ConfigService::ConfigService() :
	Service("Configuration")
	, mSharedDataDir( "" )
	, mEtcDir( "" )
	, mHomeDir ( "" )
	, mPrefix(PREFIX)
	, mGlobalConfig(new varconf::Config())
	, mUserConfig(new varconf::Config())
	, mCommandLineConfig(new varconf::Config())
	, mInstanceConfig(new varconf::Config())
	{
#ifdef __WIN32__
		char cwd[512];
		//get the full path for the current executable
		GetModuleFileName ( 0, cwd, 512 );

		//use this utility function for removing the file part
		PathRemoveFileSpec ( cwd );
		baseDir = std::string ( cwd ) + "\\";
		mSharedDataDir = baseDir + "\\..\\share\\ember\\";
		mEtcDir = baseDir + "\\..\\etc\\ember\\";
		
#endif

#if !defined(__APPLE__) && !defined(__WIN32__)
		mSharedDataDir = EMBER_DATADIR "/ember/";
		mEtcDir = EMBER_SYSCONFDIR "/ember/";
#endif

	}

	ConfigService::~ConfigService()
	{
		delete mGlobalConfig;
		delete mUserConfig;
		delete mCommandLineConfig;
		delete mInstanceConfig;
	}


	void ConfigService::setPrefix ( const std::string& prefix )
	{
		mPrefix = prefix;
		mSharedDataDir = prefix + "/share/ember/";
		mEtcDir = prefix + "/etc/ember/";
	}

	const std::string& ConfigService::getPrefix() const
	{
		return mPrefix;
	}


	void ConfigService::setHomeDirectory ( const std::string& path )
	{
		mHomeDir = path;
	}

	ConfigService::SectionMap ConfigService::getSection ( const std::string& sectionName )
	{
		SectionMap combinedSection;

		if (mInstanceConfig->findSection(sectionName)) {
			const SectionMap& section = mInstanceConfig->getSection(sectionName);
			combinedSection.insert(section.begin(), section.end());
		}
		if (mCommandLineConfig->findSection(sectionName)) {
			const SectionMap& section = mCommandLineConfig->getSection(sectionName);
			combinedSection.insert(section.begin(), section.end());
		}
		if (mUserConfig->findSection(sectionName)) {
			const SectionMap& section = mUserConfig->getSection(sectionName);
			combinedSection.insert(section.begin(), section.end());
		}
		if (mGlobalConfig->findSection(sectionName)) {
			const SectionMap& section = mGlobalConfig->getSection(sectionName);
			combinedSection.insert(section.begin(), section.end());
		}
		return combinedSection;
	}


	varconf::Variable ConfigService::getValue ( const std::string& section, const std::string& key ) const
	{
		if (mInstanceConfig->findItem(section, key)) {
			return mInstanceConfig->getItem(section, key);
		}
		if (mCommandLineConfig->findItem(section, key)) {
			return mCommandLineConfig->getItem(section, key);
		}
		if (mUserConfig->findItem(section, key)) {
			return mUserConfig->getItem(section, key);
		}
		return mGlobalConfig->getItem(section, key);
	}

	bool ConfigService::getValue ( const std::string& section, const std::string& key, varconf::Variable& value ) const
	{
		if (mInstanceConfig->findItem(section, key)) {
			value = mInstanceConfig->getItem(section, key);
			return true;
		} else if (mCommandLineConfig->findItem(section, key)) {
			value = mCommandLineConfig->getItem(section, key);
			return true;
		} else if (mUserConfig->findItem(section, key)) {
			value = mUserConfig->getItem(section, key);
			return true;
		} else if (mGlobalConfig->findItem(section, key)) {
			value = mGlobalConfig->getItem(section, key);
			return true;
		}
		return false;
	}

	void ConfigService::setValue ( const std::string& section, const std::string& key, const varconf::Variable& value, int iscope)
	{
		varconf::Scope scope = static_cast<varconf::Scope>(iscope);
		
		varconf::Config* config = mInstanceConfig;
		if (scope == varconf::GLOBAL) {
			config = mGlobalConfig;
		} else if (scope == varconf::USER) {
			config = mUserConfig;
		}
		config->setItem ( section, key, value );
	}

	bool ConfigService::isItemSet ( const std::string& section, const std::string& key, const std::string& value ) const
	{
		return ( hasItem ( section, key ) && getValue ( section, key ) == value );
	}

	bool ConfigService::start()
	{
		mGlobalConfig->sige.connect ( sigc::mem_fun ( *this, &ConfigService::configError ) );
		mGlobalConfig->sigv.connect ( sigc::mem_fun ( *this, &ConfigService::updatedConfig ) );
		mUserConfig->sige.connect ( sigc::mem_fun ( *this, &ConfigService::configError ) );
		mUserConfig->sigv.connect ( sigc::mem_fun ( *this, &ConfigService::updatedConfig ) );
		mInstanceConfig->sige.connect ( sigc::mem_fun ( *this, &ConfigService::configError ) );
		mInstanceConfig->sigv.connect ( sigc::mem_fun ( *this, &ConfigService::updatedConfig ) );
		registerConsoleCommands();
		setRunning ( true );
		return true;
	}

	void ConfigService::stop()
	{
		Service::stop();
		deregisterConsoleCommands();
		return;
	}

	void ConfigService::deregisterConsoleCommands()
	{
		ConsoleBackend::getSingleton().deregisterCommand ( SETVALUE );
		ConsoleBackend::getSingleton().deregisterCommand ( GETVALUE );
	}

	void ConfigService::registerConsoleCommands()
	{
		ConsoleBackend::getSingleton().registerCommand ( SETVALUE, this, "Sets a value in the configuration. Usage: set_value <section> <key> <value>" );
		ConsoleBackend::getSingleton().registerCommand ( GETVALUE, this, "Gets a value from the configuration. Usage: get_value <section> <key>" );
	}

	bool ConfigService::itemExists(const std::string& section, const std::string& key) const
	{
		return hasItem(section, key);
	}

	bool ConfigService::hasItem ( const std::string& section, const std::string& key ) const
	{
		return mGlobalConfig->find ( section, key ) || mUserConfig->find ( section, key ) || mInstanceConfig->find ( section, key ) || mCommandLineConfig->find ( section, key );
	}

	bool ConfigService::deleteItem ( const std::string& section, const std::string& key )
	{
		return mGlobalConfig->erase ( section, key ) | mUserConfig->erase ( section, key ) | mInstanceConfig->erase ( section, key ) | mCommandLineConfig->erase ( section, key );
	}

	bool ConfigService::loadSavedConfig ( const std::string& filename, const StringConfigMap& commandLineSettings )
	{
		S_LOG_INFO ( "Loading shared config file from " << getSharedConfigDirectory() + "/"+ filename << "." );
		bool success = mGlobalConfig->readFromFile ( getSharedConfigDirectory() + "/"+ filename, varconf::GLOBAL );
		std::string userConfigPath ( getHomeDirectory() + "/" + filename );
		std::ifstream file ( userConfigPath.c_str() );
		if ( !file.fail() )
		{
			S_LOG_INFO ( "Loading user config file from "<< getHomeDirectory() + "/" + filename <<"." );
			try
			{
				mUserConfig->parseStream ( file, varconf::USER );
			}
			catch ( varconf::ParseError& p )
			{
				S_LOG_FAILURE ( "Error loading user config file: " << p );
				success = false;
			}
		}
		else
		{
			S_LOG_INFO ( "Could not find any user config file." );
		}

		//after loading the config from file, override with command time settings
		for (StringConfigMap::const_iterator I = commandLineSettings.begin(); I != commandLineSettings.end(); ++I) {
			for (std::map<std::string, std::string>::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
				S_LOG_INFO("Setting command line config option " << I->first << ":" << J->first << " to " << J->second);
				mCommandLineConfig->setItem(I->first, J->first, J->second);
				EventChangedConfigItem(I->first, J->first);
			}
		}
		return success;
	}

	bool ConfigService::saveConfig ( const std::string& filename, int scopeMask )
	{
		//Go through all user config values and save those (as they were defined in the original user config file).
		//Also save any instance values that aren't present in the user config if they differ from the global value.
		varconf::Config exportConfig;

		//First get the instance values (i.e. those values which have been changed at runtime).
		//But only get those that differs from the global config.
		const varconf::conf_map& instanceSections = mInstanceConfig->getSections();
		for (varconf::conf_map::const_iterator I = instanceSections.begin(); I != instanceSections.end(); ++I) {
			const varconf::sec_map& section = I->second;
			for (varconf::sec_map::const_iterator J = section.begin(); J != section.end(); ++J) {
				//only set the value if it differs from the global one
				if (mGlobalConfig->getItem(I->first, J->first) != J->second) {
					exportConfig.setItem(I->first, J->first, J->second, varconf::INSTANCE);
				}
			}
		}

		//Then also add all user settings, i.e. those that already had been set in the user config file (often ~/.ember/ember.conf).
		const varconf::conf_map& userSections = mUserConfig->getSections();
		for (varconf::conf_map::const_iterator I = userSections.begin(); I != userSections.end(); ++I) {
			const varconf::sec_map& section = I->second;
			for (varconf::sec_map::const_iterator J = section.begin(); J != section.end(); ++J) {
				//We can't directly use the value, as it might have been updated in the instance config. We therefore needs to go through getValue
				exportConfig.setItem(I->first, J->first, getValue(I->first, J->first), varconf::INSTANCE);
			}
		}

		return exportConfig.writeToFile(filename);
	}

	void ConfigService::runCommand ( const std::string &command, const std::string &args )
	{
		if ( command == SETVALUE )
		{
			Tokeniser tokeniser;
			tokeniser.initTokens ( args );
			std::string section ( tokeniser.nextToken() );
			std::string key ( tokeniser.nextToken() );
			std::string value ( tokeniser.remainingTokens() );

			if ( section == "" || key == "" || value == "" )
			{
				ConsoleBackend::getSingleton().pushMessage ( "Usage: set_value <section> <key> <value>", "help" );
			}
			else
			{
				setValue ( section, key, value );
				ConsoleBackend::getSingleton().pushMessage ( "New value set, section: " +  section + " key: " + key + " value: " + value, "info" );
			}

		}
		else if ( command == GETVALUE )
		{
			Tokeniser tokeniser;
			tokeniser.initTokens ( args );
			std::string section ( tokeniser.nextToken() );
			std::string key ( tokeniser.nextToken() );

			if ( section == "" || key == "" )
			{
				ConsoleBackend::getSingleton().pushMessage ( "Usage: get_value <section> <key>", "help" );
			}
			else
			{
				if ( !hasItem ( section, key ) )
				{
					ConsoleBackend::getSingleton().pushMessage ( "No such value.", "error" );
				}
				else
				{
					varconf::Variable value = getValue ( section, key );
					ConsoleBackend::getSingleton().pushMessage ( std::string ( "Value: " ) + static_cast<std::string> ( value ), "info" );
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
		//check if the home directory is set, and if so use the setting. If else, fall back to the default path.
		if ( mHomeDir != "" )
		{
			return mHomeDir;
		}
		else
		{
#ifdef __WIN32__
			static std::string finalPath;
			if ( !finalPath.empty() )
			{
				return finalPath;
			}

			//special folders in windows:
			//http://msdn.microsoft.com/en-us/library/bb762494%28v=vs.85%29.aspx
			//CSIDL_MYDOCUMENTS would probably be more appropriate, but that's not available in msys (as of 2011-05-23).
			char path[MAX_PATH];
			if(SHGetSpecialFolderPath(nullptr, path, CSIDL_PERSONAL, TRUE)){
				finalPath = path;
			}else{
				finalPath = ".";
			}
			finalPath += "\\Ember\\";
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
			static std::string path ( getBundleResourceDirPath() );
			return path;
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
//			return BR_EMBER_DATADIR("/games/ember/");
//#endif
			return getHomeDirectory();
		}

	}

	const std::string& ConfigService::getEmberMediaDirectory() const
	{
		static std::string path;
		//look for a media channel key in the config, and if found use that, else use the version of ember as a standard path
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
		static std::string path ( getSharedDataDirectory() + "/media/shared/" );
		return path;
	}

} // namespace Ember
