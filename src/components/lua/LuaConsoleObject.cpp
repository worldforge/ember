//
// C++ Implementation: LuaConsoleObject
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LuaConsoleObject.h"
#include "Connectors_impl.h"

namespace Ember {
namespace Lua {

LuaConsoleObject::LuaConsoleObject(const std::string& command, const std::string& luaMethod, const std::string& description) :
		mConnector(std::make_unique<TemplatedConnectorBase<StringValueAdapter, StringValueAdapter>>(StringValueAdapter(), StringValueAdapter())),
		mCommandWrapper(command, this, description) {
	mConnector->connect(luaMethod);
}

LuaConsoleObject::LuaConsoleObject(const std::string& command, lua_Object /*luaMethod */, const std::string& description) :
		mConnector(std::make_unique<TemplatedConnectorBase<StringValueAdapter, StringValueAdapter>>(StringValueAdapter(), StringValueAdapter())),
		mCommandWrapper(command, this, description) {
	//we need to get the correct lua function
	lua_State* state = ConnectorBase::getState();
	int luaType = lua_type(state, -1);
	int index = luaL_ref(state, LUA_REGISTRYINDEX);
	if (luaType == LUA_TFUNCTION) {
		mConnector->connect(index);
	} else {
		S_LOG_WARNING("No valid lua function sent as argument to LuaConsoleObject::LuaConsoleObject");
	}
}

LuaConsoleObject::~LuaConsoleObject() = default;

void LuaConsoleObject::runCommand(const std::string& command, const std::string& args) {
	mConnector->callLuaMethod<std::string, std::string>(command, args);
}

LuaConsoleObject* LuaConsoleObject::setSelf(lua_Object selfIndex) {
	if (mConnector) {
		if (selfIndex != LUA_NOREF) {
			//we need to get the correct lua table
			lua_State* state = ConnectorBase::getState();
			int luaType = lua_type(state, -1);
			int index = luaL_ref(state, LUA_REGISTRYINDEX);
			if (luaType == LUA_TTABLE) {
				mConnector->setSelfIndex(index);
			}
		}
	}
	return this;
}

}
}
