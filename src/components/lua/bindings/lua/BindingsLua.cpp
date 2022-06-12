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

#include "BindingsLua.h"
#include "../../Connector.h"
#include "../../LuaConsoleObject.h"


using namespace Ember::Lua;

void registerBindingsLua(sol::state_view& lua) {

	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto Lua = Ember["Lua"].get_or_create<sol::table>();

	auto luaConnector = Lua.new_usertype<LuaConnector>("LuaConnector");
	luaConnector["connect"] = sol::overload(sol::resolve<std::unique_ptr<LuaConnection>(sol::function) const>(&LuaConnector::connect),
											sol::resolve<std::unique_ptr<LuaConnection>(sol::function, sol::object) const>(&LuaConnector::connect));
	auto luaConnection = Lua.new_usertype<LuaConnection>("LuaConnection");
	luaConnection["disconnect"] = &LuaConnection::disconnect;


	Lua.new_usertype<LuaConsoleObject>(
			"LuaConsoleObject",
			sol::constructors<Ember::Lua::LuaConsoleObject(const std::string&, sol::function), Ember::Lua::LuaConsoleObject(const std::string&, sol::function, const std::string& description)>()
	);

}