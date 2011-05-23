/*
 Copyright (C) 2002  Hans Häggström
 Copyright (C) 2005	Erik Hjortsberg

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

#ifndef EMBER_EMBERSERVICES_H
#define EMBER_EMBERSERVICES_H

#include "framework/Singleton.h"
#include <memory>

namespace Ember
{

// some forward declarations before we start
class LoggingService;
class ConfigService;
class InputService;
class MetaserverService;
class ServerService;
class SoundService;
class TestService;
class ScriptingService;
class WfutService;
namespace Services
{
class Time;
class ServerSettings;
}


/**
 * This is a singleton class that is used to access instances of all the
 * different Ember services.
 *
 * There's a getServiceName() method for each service. <p>
 *
 * TODO: Should this class also create the instances of the services,
 * or should it have set methods for them?  <p>
 *
 * Example: <p>
 * <code>
 *
 *   EmberServices.getInstance()->getLoggingService()->log( ... );  <br/>
 *   ... = EmberServices.getInstance()->getMetaServerService()->getMetaServerList();
 *
 * </code>
 *
 * @author Hans Häggström
 */
class EmberServices: public Singleton<EmberServices>
{
public:

	EmberServices();

	/**
	 * Deletes a EmberServices instance.
	 */
	virtual ~EmberServices();

	/**
	 * Returns an instance of the TestService.
	 */
	TestService *getTestService();

	/**
	 * Returns an instance of the LoggingService
	 */
	LoggingService *getLoggingService();

	/**
	 * Returns an instance of the ConfigService
	 */
	ConfigService *getConfigService();

	/**
	 * Returns an instance of the InputService
	 */
	InputService *getInputService();

	/**
	 * Returns an instance of the MetaserverService
	 */
	MetaserverService *getMetaserverService();

	/**
	 * Returns an instance of the ServerService
	 */
	ServerService *getServerService();

	/**
	 * Returns an instance of the SoundService
	 */
	SoundService *getSoundService();

	/**
	 * Returns an instance of the ScriptingService
	 */
	ScriptingService *getScriptingService();

	/**
	 * Returns an instance of the wfut service
	 */
	WfutService *getWfutService();

	/**
	 * Returns an instance of the time service
	 */
	Services::Time* getTimeService();

	/**
	 * @brief Returns the server settings service.
	 *
	 * @returns The server settings service.
	 */
	Services::ServerSettings* getServerSettingsService();

	//----------------------------------------------------------------------
	// Setters

	//======================================================================
	// Disabled constructors and operators
	//======================================================================
private:

	/**
	 * Copy constructor not provided.
	 */
	EmberServices(const EmberServices &source)
	{
	}

	/**
	 * Assignment operator not provided.
	 */
	EmberServices &operator=(const EmberServices &source)
	{
		return *this;
	}

private:

	std::auto_ptr<LoggingService> mLoggingService;
	std::auto_ptr<ScriptingService> mScriptingService;
	std::auto_ptr<SoundService> mSoundService;
	std::auto_ptr<ServerService> mServerService;
	std::auto_ptr<MetaserverService> mMetaserverService;
	std::auto_ptr<InputService> mInputService;
	std::auto_ptr<WfutService> mWfutService;
	std::auto_ptr<ConfigService> mConfigService;
	std::auto_ptr<Services::Time> mTimeService;
	std::auto_ptr<Services::ServerSettings> mServerSettingsService;

};
}

#endif
