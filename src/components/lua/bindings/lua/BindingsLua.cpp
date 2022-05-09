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

void registerBindingsLua(sol::state_view& lua) {

	lua.new_usertype<Ember::Lua::Connector>(
			"Ember::Lua::Connector",
			"connect",
			[](Ember::Lua::Connector* self, sol::function luaMethod) {
				return self->connect(std::move(luaMethod));
			},
			"disconnect", &Ember::Lua::Connector::disconnect);
	lua.new_usertype<Ember::Lua::LuaConsoleObject>(
			"Ember::Lua::LuaConsoleObject",
			sol::constructors<Ember::Lua::LuaConsoleObject(const std::string&, sol::function)>()
	);

}