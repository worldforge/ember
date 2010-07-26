//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2010
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


#ifndef EMBER_SERVICES_SERVERSETTINGS_H_
#define EMBER_SERVICES_SERVERSETTINGS_H_

#include "framework/Service.h"
#include <varconf/variable.h>
#include <string>
#include <memory>
#include <map>

namespace varconf
{
class Config;
}

namespace Ember {

namespace Services {

class ServerSettingsCredentials;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Stores server settings.
 *
 * This service stores settings for each server. It provides a simple interface for querying for existing settings as well as adding new ones.
 * While the settings will be stored to disk when the service shuts down, users are encouraged to call writeToDisk() as soon as any new settings has been added, to guarantee that no settings gets lost in the case of a crash.
 *
 * In order to query about the settings for a specific server an instance of ServerSettingsCredentials. This allows the service to manage server verification and lookup.
 *
 * All server settings are stored in one file, by default "serversettings.conf", to be found in the Ember home directory. Each server is represented by one section in this config file.
 * The mapping between servers and sections is handled by the getSectionForServerCredentials(...) method. This method currently only uses the host name to map servers to config file section, but it might be extended with more complex authentication methods for making sure that the right server is matched to the correct section.
 */
class ServerSettings : public Service
{
public:
	/**
	 * @brief Ctor.
	 */
	ServerSettings();

	/**
	 * @brief Dtor.
	 */
	virtual ~ServerSettings();

	/**
	 * @copydoc Service::start()
	 */
	Service::Status start();

	/**
	 * @copydoc Service::stop()
	 */
	void stop(int code);

	/**
	 * @brief Checks for the existence of a specific key.
	 *
	 * @param credentials The server credentials.
	 * @param key The key to look for.
	 * @returns True of there exists a key value pair for the supplied key.
	 */
	bool findItem(const ServerSettingsCredentials& credentials, const std::string& key) const;

	/**
	 * @brief Gets a setting with the specified key.
	 *
	 * @param credentials The server credentials.
	 * @param key The key to look for.
	 * @returns The value of the specified key.
	 */
	varconf::Variable getItem(const ServerSettingsCredentials& credentials, const std::string& key) const;

	/**
	 * @brief Sets an item.
	 *
	 * @param credentials The server credentials.
	 * @param key The key to set.
	 * @param item The value to attach to the key.
	 */
	void setItem(const ServerSettingsCredentials& credentials, const std::string& key, const varconf::Variable& item);

	/**
	 * @brief Erases an item.
	 *
	 * @param credentials The server credentials.
	 * @param key The key to erase.
	 * @returns True if a key could be found and erased.
	 */
	bool eraseItem(const ServerSettingsCredentials& credentials, const std::string& key);

	/**
	 * @brief Gets all settings for a server.
	 *
	 * @param credentials The server credentials.
	 * @returns All settings for the specified server.
	 */
	const std::map<std::string, varconf::Variable>& getServerSettings(const ServerSettingsCredentials& credentials);

	/**
	 * @brief Writes settings to disk.
	 *
	 * Call this whenever you've set any values to make sure that settings are stored.
	 * This method will also automatically be called when the service is shut down.
	 */
	void writeToDisk();

private:

	/**
	 * @brief The config which holds all values.
	 */
	std::auto_ptr<varconf::Config> mConfig;

	/**
	 * @brief Gets the full path to the config file on the file system.
	 *
	 * If the user setting "general:serversettings" is set, the value found there will be used. If not, a file named "serversettings.conf" found in the Ember home directory will be used.
	 *
	 * @returns The full path to the config file on the file system.
	 */
	std::string getFullConfigFilePath() const;

	/**
	 * @brief Reads any saved settings from disk into the config instance.
	 */
	void readFromDisk();

	/**
	 * @brief Gets the name of the section for the supplied server credentials.
	 *
	 * Currently this only composes a name from the host name of the server, but an option in the future would be to expand this with server certificates, so that we can be sure that the right section is matched to the correct server.
	 * @param credentials The server credentials.
	 * @returns The name of the section in the config for the supplied credentials.
	 */
	std::string getSectionForServerCredentials(const ServerSettingsCredentials& credentials) const;
};

}

}

#endif /* EMBER_SERVICES_SERVERSETTINGS_H_ */
