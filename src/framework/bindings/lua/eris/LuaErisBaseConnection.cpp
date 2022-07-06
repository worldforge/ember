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
#include <Eris/Connection.h>
#include <Eris/TypeService.h>


using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<BaseConnection>(sol::table& space) {
	auto baseConnection = space.new_usertype<BaseConnection>("BaseConnection", sol::no_constructor);
	baseConnection["getStatus"] = &BaseConnection::getStatus;
	baseConnection["isConnected"] = &BaseConnection::isConnected;
	baseConnection["getHost"] = &BaseConnection::getHost;
	baseConnection["getPort"] = &BaseConnection::getPort;
	baseConnection["getFactories"] = sol::resolve<Atlas::Objects::Factories&()>(&BaseConnection::getFactories);
	baseConnection["INVALID_STATUS"] = sol::var(BaseConnection::INVALID_STATUS);
	baseConnection["NEGOTIATE"] = sol::var(BaseConnection::NEGOTIATE);
	baseConnection["CONNECTING"] = sol::var(BaseConnection::CONNECTING);
	baseConnection["CONNECTED"] = sol::var(BaseConnection::CONNECTED);
	baseConnection["CONNECTED"] = sol::var(BaseConnection::CONNECTED);
	baseConnection["DISCONNECTED"] = sol::var(BaseConnection::DISCONNECTED);
	baseConnection["DISCONNECTING"] = sol::var(BaseConnection::DISCONNECTING);
	baseConnection["QUERY_GET"] = sol::var(BaseConnection::QUERY_GET);
	baseConnection["Connected"] = LuaConnector::make_property(&BaseConnection::Connected);
	baseConnection["Disconnected"] = LuaConnector::make_property(&BaseConnection::Disconnected);

	auto connection = space.new_usertype<Connection>("Connection", sol::no_constructor,
													 sol::base_classes, sol::bases<BaseConnection>());
	connection["getTypeService"] = &Connection::getTypeService;
	connection["getServerInfo"] = &Connection::getServerInfo;
	connection["GotServerInfo"] = LuaConnector::make_property(&Connection::GotServerInfo);
	connection["Failure"] = LuaConnector::make_property(&Connection::Failure);

}