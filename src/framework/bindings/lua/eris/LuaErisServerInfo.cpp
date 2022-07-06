/*
 Copyright (C) 2022 Erik Ogenvik

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
#include "RegisterLua.h"
#include <Eris/ServerInfo.h>


using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<ServerInfo>(sol::table& space) {

	auto serverInfo = space.new_usertype<ServerInfo>("ServerInfo");
	serverInfo["host"] = &ServerInfo::host;
	serverInfo["status"] = &ServerInfo::status;
	serverInfo["name"] = &ServerInfo::name;
	serverInfo["ruleset"] = &ServerInfo::ruleset;
	serverInfo["server"] = &ServerInfo::server;
	serverInfo["protocol_version"] = &ServerInfo::protocol_version;
	serverInfo["clients"] = &ServerInfo::clients;
	serverInfo["ping"] = &ServerInfo::ping;
	serverInfo["entities"] = &ServerInfo::entities;
	serverInfo["ruleset"] = &ServerInfo::ruleset;
	serverInfo["uptime"] = &ServerInfo::uptime;
	serverInfo["version"] = &ServerInfo::version;
	serverInfo["buildDate"] = &ServerInfo::buildDate;
	serverInfo["INVALID"] = sol::var(Eris::ServerInfo::Status::INVALID);
	serverInfo["QUERYING"] = sol::var(Eris::ServerInfo::Status::QUERYING);
	serverInfo["VALID"] = sol::var(Eris::ServerInfo::Status::VALID);
	serverInfo["TIMEOUT"] = sol::var(Eris::ServerInfo::Status::TIMEOUT);
}