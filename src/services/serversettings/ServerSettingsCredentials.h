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

#ifndef SERVERSETTINGSCREDENTIALS_H_
#define SERVERSETTINGSCREDENTIALS_H_

#include <string>

namespace Eris {
struct ServerInfo;
}

namespace Ember {

/**
 * @brief Server credentials, identifying a server instance.
 *
 * An instance of this class is used to identify a certain server and map this to a section in the server settings configuration.
 *
 * @author Erik Ogenvik <erik@ogenvik.org>
 */
class ServerSettingsCredentials {
public:

	/**
	 * @brief Ctor.
	 *
	 * @param hostName The host name of the server
	 * @param serverName The name of the server instance.
	 */
	ServerSettingsCredentials(std::string hostName, std::string serverName);

	/**
	 * @brief Ctor.
	 *
	 * @param serverInfo A server info object from Eris from which the needed data will be extracted.
	 */
	explicit ServerSettingsCredentials(const Eris::ServerInfo& serverInfo);

	/**
	 * @brief Gets the host name of the server.
	 *
	 * @returns The host name of the server.
	 */
	const std::string& getHostName() const;

	/**
	 * @brief Gets the name of the server instance.
	 *
	 * @returns The name of the server instance.
	 */
	const std::string& getServerName() const;

private:

	/**
	 * @brief The host name of the server.
	 */
	std::string mHostName;

	/**
	 * @brief The name of the server instance.
	 */
	std::string mServerName;
};

}


#endif /* SERVERSETTINGSCREDENTIALS_H_ */
