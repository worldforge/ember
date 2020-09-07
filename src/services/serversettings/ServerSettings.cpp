//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2010
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//


#include "ServerSettings.h"
#include "ServerSettingsCredentials.h"
#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"
#include <fstream>
#include <sstream>

namespace Ember {

namespace Services {

ServerSettings::ServerSettings() :
		Service("Server settings"),
		mConfig(new varconf::Config()) {
}

ServerSettings::~ServerSettings() = default;

bool ServerSettings::start() {
	readFromDisk();
	setRunning(true);
	return true;
}

void ServerSettings::stop() {
	Service::stop();
	writeToDisk();
}

std::string ServerSettings::getSectionForServerCredentials(const ServerSettingsCredentials& credentials) const {
	//This should be expanded to a more complex way of assuring that the section returned is correct for the server credentials.
	std::stringstream ss;
	ss << "hostname_" << credentials.getHostName() << "_servername_" << credentials.getServerName();
	std::string sectionName(ss.str());
	mConfig->clean(sectionName);
	return sectionName;
}

void ServerSettings::writeToDisk() {
	auto filePath = getFullConfigFilePath();

	mConfig->writeToFile(filePath.string()); //calling this without any scope arguments makes it write all settings in all scopes
}

void ServerSettings::readFromDisk() {
	auto filePath = getFullConfigFilePath();

	std::ifstream file(filePath.string().c_str());

	// If an existing server settings is present, then read it in.
	if (!file.fail()) {
		// read in file
		// 		serverCache.readFromFile(cacheFile.c_str(),varconf::GLOBAL);
		S_LOG_VERBOSE("Loading existing server settings [ " << filePath.string() << " ]");
		try {
			// make sure it is well formed
			mConfig->parseStream(file, varconf::GLOBAL);
		} catch (varconf::ParseError& ex) {
			S_LOG_FAILURE("Error loading server settings file: " << ex);
			return;
		}
		file.close();
	}
}

void ServerSettings::setItem(const ServerSettingsCredentials& credentials, const std::string& key, const varconf::Variable& item) {
	mConfig->setItem(getSectionForServerCredentials(credentials), key, item);
}

bool ServerSettings::findItem(const ServerSettingsCredentials& credentials, const std::string& key) const {
	std::string cleanKey(key);
	mConfig->clean(cleanKey);
	return mConfig->findItem(getSectionForServerCredentials(credentials), cleanKey);
}

varconf::Variable ServerSettings::getItem(const ServerSettingsCredentials& credentials, const std::string& key) const {
	std::string cleanKey(key);
	mConfig->clean(cleanKey);
	return mConfig->getItem(getSectionForServerCredentials(credentials), cleanKey);
}

bool ServerSettings::eraseItem(const ServerSettingsCredentials& credentials, const std::string& key) {
	std::string cleanKey(key);
	mConfig->clean(cleanKey);
	return mConfig->erase(getSectionForServerCredentials(credentials), cleanKey);
}

const varconf::sec_map& ServerSettings::getServerSettings(const ServerSettingsCredentials& credentials) {
	return mConfig->getSection(getSectionForServerCredentials(credentials));
}

boost::filesystem::path ServerSettings::getFullConfigFilePath() const {
	ConfigService& cfgService = EmberServices::getSingleton().getConfigService();
	// fetch the configuration file
	if (cfgService.hasItem("general", "serversettings")) {
		auto value = cfgService.getValue("general", "serversettings");
		if (value.is_string()) {
			return cfgService.getHomeDirectory(BaseDirType_CONFIG) / value.as_string();
		}
	}
	// default fallback value
	return cfgService.getHomeDirectory(BaseDirType_CONFIG) / "serversettings.conf";

}
}
}
