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

namespace Eris
{
class Session;
}

namespace Ember
{

// some forward declarations before we start
class LoggingService;
class ConfigService;
class InputService;
class MetaserverService;
class ServerService;
class SoundService;
class ScriptingService;
class WfutService;
namespace Services
{
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

	/**
	 * @brief Ctor.
	 */
	EmberServices(Eris::Session& session);

	/**
	 * @brief Dtor.
	 */
	virtual ~EmberServices();

	/**
	 * @brief Returns an instance of the LoggingService
	 *
	 * @return The logging service.
	 */
	LoggingService& getLoggingService();

	/**
	 * @brief Returns an instance of the ConfigService.
	 *
	 * @return The config service.
	 */
	ConfigService& getConfigService();

	/**
	 * @brief Returns an instance of the InputService
	 *
	 * @return The input service.
	 */
	InputService& getInputService();

	/**
	 * @brief Returns an instance of the MetaserverService.
	 *
	 * @return The meta server service.
	 */
	MetaserverService& getMetaserverService();

	/**
	 * @brief Returns an instance of the ServerService
	 *
	 * @return The server service.
	 */
	ServerService& getServerService();

	/**
	 * @brief Returns an instance of the SoundService
	 *
	 * @return The sound service.
	 */
	SoundService& getSoundService();

	/**
	 * @brief Returns an instance of the ScriptingService
	 *
	 * @return The scripting service.
	 */
	ScriptingService& getScriptingService();

	/**
	 * @brief Returns an instance of the update service
	 *
	 * @return The update service.
	 */
	WfutService& getWfutService();

	/**
	 * @brief Returns the server settings service.
	 *
	 * @return The server settings service.
	 */
	Services::ServerSettings& getServerSettingsService();

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

	LoggingService* mLoggingService;
	ScriptingService* mScriptingService;
	SoundService* mSoundService;
	ServerService* mServerService;
	MetaserverService* mMetaserverService;
	InputService* mInputService;
	WfutService* mWfutService;
	ConfigService* mConfigService;
	Services::ServerSettings* mServerSettingsService;

};
}

#endif
