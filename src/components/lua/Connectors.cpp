/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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


#include "Connectors.h"
#include "LuaHelper.h"
#include "luaobject.h"
#include "services/EmberServices.h"
#include "services/scripting/ScriptingService.h"
#include "framework/LoggingInstance.h"
#include "framework/Exception.h"

#include <tolua++.h>

#include <utility>

namespace Ember {

namespace Lua {

ConnectorBase::ConnectorBase() = default;

ConnectorBase::~ConnectorBase() {
	mConnection.disconnect();
}

void ConnectorBase::disconnect() {
	mConnection.disconnect();
}

void ConnectorBase::connect(sol::function luaMethod) {
	mLuaFunction = std::move(luaMethod);
}

void ConnectorBase::setSelf(sol::table self) {
	mLuaSelf = std::move(self);
}

lua_State* ConnectorBase::sState = nullptr;

void ConnectorBase::setState(lua_State* state) {
	sState = state;
}

lua_State* ConnectorBase::getState() {
	return sState;
}

int ConnectorBase::resolveLuaFunction() {
	//get the lua function
	lua_rawgeti(mLuaFunction.lua_state(), LUA_REGISTRYINDEX, mLuaFunction.registry_index());

	//Check if there's a "self" table specified. If so, prepend that as the first argument and increase the number of arguments counter.
	if (mLuaSelf.registry_index() != LUA_NOREF) {
		lua_rawgeti(mLuaFunction.lua_state(), LUA_REGISTRYINDEX, mLuaSelf.registry_index());
		return 1;
	}
	return 0;
}


void ConnectorBase::callFunction(int numberOfArguments) {

	auto state = mLuaFunction.lua_state();
	//push our error handling method before calling the code
	int error_index = lua_gettop(state) - numberOfArguments;
	lua_pushcfunction(state, LuaHelper::luaErrorHandler);
	lua_insert(state, error_index);/* put it under chunk and args */

	luaPop pop(state, 1); // pops error handler on exit

	// call it
	int error = 0;

	try {
		error = lua_pcall(state, numberOfArguments, LUA_MULTRET, error_index);
	} catch (const std::exception& ex) {
		const std::string& msg = lua_tostring(state, -1);
		throw Exception(msg);
	}

	// handle errors
	if (error) {
		const std::string& msg = lua_tostring(state, -1);
		S_LOG_FAILURE("(LuaScriptModule) Lua error: " << msg);
		return;
	}
}


template<>
bool ConnectorBase::returnValueFromLua() {
	lua_State* state = mLuaFunction.lua_state();
	bool vale = lua_isboolean(state, -1) ? lua_toboolean(state, -1) : true;
	lua_pop(state, 1);
	return vale;
}


bool StringValueAdapter::pushValue(lua_State* state, StringValueAdapter::value_type value) {
	tolua_pushstring(state, value.c_str());
	return true;
}

bool BooleanValueAdapter::pushValue(lua_State* state, const bool& value) {
	tolua_pushboolean(state, value);
	return true;
}

bool EmptyValueAdapter::pushValue(lua_State*, const Empty&) {
	return false;
}
}
}
