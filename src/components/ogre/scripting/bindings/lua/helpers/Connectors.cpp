/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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


#include "Connectors.h"
#include "components/lua/LuaHelper.h"
#include "components/lua/luaobject.h"
#include "services/EmberServices.h"
#include "services/scripting/ScriptingService.h"
#include "framework/LoggingInstance.h"
#include "framework/Exception.h"

#include <tolua++.h>

namespace EmberOgre
{

namespace LuaConnectors
{

ConnectorBase::ConnectorBase() :
	mLuaFunctionIndex(LUA_NOREF), mLuaSelfIndex(LUA_NOREF)
{
}

ConnectorBase::~ConnectorBase()
{
	mConnection.disconnect();
}

void ConnectorBase::disconnect()
{
	mConnection.disconnect();
}

void ConnectorBase::connect(const std::string & luaMethod)
{
	mLuaMethod = luaMethod;
}

void ConnectorBase::connect(int luaMethod)
{
	mLuaFunctionIndex = luaMethod;
}

void ConnectorBase::pushNamedFunction(lua_State* state)
{
	Ember::Lua::LuaHelper::pushNamedFunction(state, mLuaMethod);
}

void ConnectorBase::setSelfIndex(int selfIndex)
{
	mLuaSelfIndex = selfIndex;
}

lua_State* ConnectorBase::sState = 0;

void ConnectorBase::setState(lua_State* state)
{
	sState = state;
}

lua_State* ConnectorBase::getState()
{
	return sState;
}

int ConnectorBase::resolveLuaFunction(lua_State* state)
{
	if (mLuaFunctionIndex == LUA_NOREF || Ember::EmberServices::getSingleton().getScriptingService()->getAlwaysLookup()) {
		pushNamedFunction(state);
		mLuaFunctionIndex = luaL_ref(state, LUA_REGISTRYINDEX);
	}

	///get the lua function
	lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaFunctionIndex);

	//Check if there's a "self" table specified. If so, prepend that as the first argument and increase the number of arguments counter.
	if (mLuaSelfIndex != LUA_NOREF) {
		lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaSelfIndex);
		return 1;
	}
	return 0;
}

void ConnectorBase::callFunction(lua_State* state, int numberOfArguments)
{

	///push our error handling method before calling the code
	int error_index = lua_gettop(state) - numberOfArguments;
#if LUA51
	lua_pushcfunction(state, Ember::Lua::LuaHelper::luaErrorHandler);
#else
	lua_pushliteral(state, "_TRACEBACK");
	lua_rawget(state, LUA_GLOBALSINDEX); /* get traceback function */
#endif
	lua_insert(state, error_index);/* put it under chunk and args */

	luaPop pop(state, 1); // pops error handler on exit

	/// call it
	int error = lua_pcall(state, numberOfArguments, LUA_MULTRET, error_index);

	/// handle errors
	if (error) {
		const std::string& msg = lua_tostring(state,-1);
		// 			lua_pop(state,numberOfArguments);
		throw Ember::Exception(msg);
	}
}

bool StringValueAdapter::pushValue(lua_State* state, StringValueAdapter::value_type value)
{
	tolua_pushstring(state, value.c_str());
	return true;
}


bool EmptyValueAdapter::pushValue(lua_State* state, const Empty& value)
{
	return false;
}
}
}
