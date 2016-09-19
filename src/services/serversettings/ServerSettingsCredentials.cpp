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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#include "ServerSettingsCredentials.h"

#include <Eris/ServerInfo.h>

namespace Ember
{

namespace Services
{

ServerSettingsCredentials::ServerSettingsCredentials(const std::string& hostName, const std::string& serverName) :
	mHostName(hostName), mServerName(serverName)
{
}

ServerSettingsCredentials::ServerSettingsCredentials(const Eris::ServerInfo& serverInfo) :
	mHostName(serverInfo.getHostname()), mServerName(serverInfo.getServername())
{
}

const std::string& ServerSettingsCredentials::getHostName() const
{
	return mHostName;
}

const std::string& ServerSettingsCredentials::getServerName() const
{
	return mServerName;
}
}
}
