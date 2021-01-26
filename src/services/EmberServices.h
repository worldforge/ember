/*
 Copyright (C) 2002  Hans Häggström
 Copyright (C) 2005	Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBER_EMBERSERVICES_H
#define EMBER_EMBERSERVICES_H

#include <memory>
#include "framework/Singleton.h"

namespace Eris
{
struct Session;
}

namespace Ember
{

// some forward declarations before we start
class ConfigService;
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
	explicit EmberServices(Eris::Session& session, ConfigService& configService);

	/**
	 * @brief Dtor.
	 */
	~EmberServices() override;

	/**
	 * @brief Returns an instance of the ConfigService.
	 *
	 * @return The config service.
	 */
	ConfigService& getConfigService();

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

private:
	ConfigService& mConfigService;

	std::unique_ptr<ScriptingService> mScriptingService;
	std::unique_ptr<SoundService> mSoundService;
	std::unique_ptr<ServerService> mServerService;
	std::unique_ptr<MetaserverService> mMetaserverService;
	std::unique_ptr<WfutService> mWfutService;
	std::unique_ptr<Services::ServerSettings> mServerSettingsService;

};
}

#endif
