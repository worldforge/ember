//
// C++ Implementation: LuaScriptingProvider
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../EmberOgrePrerequisites.h"
#include "LuaScriptingProvider.h"

#include "bindings/lua/helpers/LuaConnector.h"

#include <CEGUIExceptions.h>

#include "framework/Exception.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

// include Lua libs and tolua++
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
//#include <lua.hpp>
#include <tolua++.h>
#include "LuaHelper.h"

TOLUA_API int tolua_Ogre_open (lua_State* tolua_S);
TOLUA_API int tolua_Eris_open (lua_State* tolua_S);
TOLUA_API int tolua_EmberServices_open (lua_State* tolua_S);
TOLUA_API int tolua_EmberOgre_open (lua_State* tolua_S);
TOLUA_API int tolua_Helpers_open (lua_State* tolua_S);
TOLUA_API int tolua_Framework_open (lua_State* tolua_S);
TOLUA_API int tolua_Application_open (lua_State* tolua_S);
TOLUA_API int tolua_atlas_adapters_open (lua_State* tolua_S);
TOLUA_API int tolua_Atlas_open (lua_State* tolua_S);

namespace EmberOgre {

LuaScriptingProvider::LuaScriptingProvider()
: mService(0)
{
	initialize();
}


LuaScriptingProvider::~LuaScriptingProvider()
{
	lua_close(mLuaState);
}

// void LuaScriptingProvider::start()
// {
// 	initialize();
// }

void LuaScriptingProvider::stop()
{
	try {
		///we want to clear up the lua environment without destroying it (lua_close destroys it)
		std::string shutdownScript("for key,value in pairs(_G) do if key ~= \"_G\" and key ~= \"pairs\" then _G[key] = nil end end");
		Ember::IScriptingCallContext callContext(createDefaultContext());
		executeScript(callContext, shutdownScript);
		forceGC();
	} catch (...) {
		S_LOG_WARNING("Error when stopping lua.");
	}
}

void LuaScriptingProvider::initialize() 
{
	createState();
	tolua_Framework_open(mLuaState);
	tolua_EmberOgre_open(mLuaState);
	tolua_Eris_open(mLuaState);
	tolua_EmberServices_open(mLuaState);
	tolua_Helpers_open (mLuaState);
	tolua_Ogre_open(mLuaState);
	tolua_Application_open(mLuaState);
	tolua_atlas_adapters_open(mLuaState);
	tolua_Atlas_open(mLuaState);
	LuaConnector::setState(mLuaState);
}

void LuaScriptingProvider::createState() 
{
	bool loadDebugLib = true;
	if (Ember::EmberServices::getSingletonPtr()->getConfigService()->itemExists("lua", "debug")) {
		loadDebugLib = static_cast<bool>(Ember::EmberServices::getSingletonPtr()->getConfigService()->getValue("lua", "debug"));
	}
	if (loadDebugLib) {
		S_LOG_VERBOSE("Loading lua debug library.");
	}


#ifdef LUA51
	static const luaL_Reg lualibs[] = {
		{"", luaopen_base},
		{LUA_LOADLIBNAME, luaopen_package},
		{LUA_TABLIBNAME, luaopen_table},
		{LUA_IOLIBNAME, luaopen_io},
		{LUA_STRLIBNAME, luaopen_string},
		{LUA_MATHLIBNAME, luaopen_math},
		{NULL, NULL}
		};
	static const luaL_Reg lualibsDebug[] = {
		{"", luaopen_base},
		{LUA_LOADLIBNAME, luaopen_package},
		{LUA_TABLIBNAME, luaopen_table},
		{LUA_IOLIBNAME, luaopen_io},
		{LUA_STRLIBNAME, luaopen_string},
		{LUA_MATHLIBNAME, luaopen_math},
		{LUA_DBLIBNAME, luaopen_debug},
		{NULL, NULL}
		};		
#endif /* LUA51 */

	mLuaState = lua_open();

	// init all standard libraries
#ifdef LUA51
	const luaL_Reg *lib = loadDebugLib ? lualibsDebug : lualibs;
	for (; lib->func; lib++)
	{
		lua_pushcfunction(mLuaState, lib->func);
		lua_pushstring(mLuaState, lib->name);
		lua_call(mLuaState, 1, 0);
	}
#else /* LUA51 */
	luaopen_base(mLuaState);
	luaopen_io(mLuaState);
	luaopen_string(mLuaState);
	luaopen_table(mLuaState);
	luaopen_math(mLuaState);
	if (loadDebugLib) {
		luaopen_debug(mLuaState);
	}
#endif /* LUA51 */

}

lua_State* LuaScriptingProvider::getLuaState()
{
	return mLuaState;
}



void LuaScriptingProvider::loadScript(Ember::ResourceWrapper& resWrapper)
{
	try {
		
		// load code into lua
		int top = lua_gettop(mLuaState);
		int loaderr = luaL_loadbuffer(mLuaState, resWrapper.getDataPtr(), resWrapper.getSize(), resWrapper.getName().c_str());
		
		if (loaderr)
		{
			std::string errMsg(lua_tostring(mLuaState,-1));
			lua_settop(mLuaState,top);
			throw Ember::Exception("Unable to load Lua script file: '"+resWrapper.getName()+"'\n\n"+errMsg+"\n");
		}
		
		///push our error handling method before calling the code
		int error_index = lua_gettop(mLuaState);
		#if LUA51
		lua_pushcfunction(mLuaState, ::EmberOgre::Scripting::LuaHelper::luaErrorHandler);
		#else
		lua_pushliteral(mLuaState, "_TRACEBACK");
		lua_rawget(mLuaState, LUA_GLOBALSINDEX);  /* get traceback function */
		#endif
		lua_insert(mLuaState, error_index);/* put it under chunk and args */


		// call it
		if (lua_pcall(mLuaState,0,0,error_index))
		{
			std::string errMsg(lua_tostring(mLuaState,-1));
			lua_settop(mLuaState,top);
			throw Ember::Exception("Unable to execute Lua script file: '"+resWrapper.getName()+"'\n\n"+errMsg+"\n");
		}
	
		lua_settop(mLuaState,top); // just in case :P
		
		
// 		getScriptModule().executeScriptFile(scriptName);
	} catch (const CEGUI::Exception& ex) {
		throw Ember::Exception(ex.getMessage().c_str());
	} catch( const CEGUI::String& str )
	{
		throw Ember::Exception(str.c_str());
	} catch (const Ember::Exception& ex) {
		throw ex;
	} catch (...) {
		throw Ember::Exception("Unknown error.");
	}
/*	} catch (const Ogre::Exception& ex) {
		throw Ember::Exception("Error when loading script " + scriptName + ". Message: " + ex.get);
	}*/
}

void LuaScriptingProvider::executeScript(Ember::IScriptingCallContext& callContext, const std::string& scriptCode)
{
	try {
		LuaScriptingCallContext& luaCallContext(static_cast<LuaScriptingCallContext&>(callContext));
		int top = lua_gettop(mLuaState);
	
	
		int loaderr = luaL_loadbuffer(mLuaState, scriptCode.c_str(), scriptCode.length(), scriptCode.c_str());
		
		if (loaderr)
		{
			std::string errMsg(lua_tostring(mLuaState,-1));
			lua_settop(mLuaState,top);
			throw Ember::Exception("Unable to load Lua script: '" + scriptCode + "'\n\n"+errMsg+"\n");
		}
		
		///push our error handling method before calling the code
		int error_index = lua_gettop(mLuaState);
		lua_pushcfunction(mLuaState, ::EmberOgre::Scripting::LuaHelper::luaErrorHandler);
		lua_insert(mLuaState, error_index);
		
		/// load code into lua and call it
		int error = lua_pcall(mLuaState,0,0,error_index);
	
		// handle errors
		if (error)
		{
			std::string errMsg(lua_tostring(mLuaState,-1));
			lua_settop(mLuaState,top);
			throw Ember::Exception("Unable to execute Lua script string: '"+scriptCode+"'\n\n"+errMsg+"\n");
		}
		
		fromStack fs(mLuaState);
		LuaRef* luaRef = new LuaRef(fs);
		luaCallContext.returnValue = luaRef;
// 		getScriptModule().executeString(scriptCode);
	} catch (const CEGUI::Exception& ex) {
		throw Ember::Exception(ex.getMessage().c_str());
	} catch( const CEGUI::String& str )
	{
		throw Ember::Exception(str.c_str());
	} catch (const Ember::Exception& ex) {
		throw ex;
	} catch (...) {
		throw Ember::Exception("Unknown error.");
	}

}

bool LuaScriptingProvider::willLoadScript(const std::string& scriptName)
{
	if (Ogre::StringUtil::endsWith(scriptName, ".lua")) {
		return true;
	}
	return false;
}

const std::string& LuaScriptingProvider::getName() const
{
	static std::string name = "LuaScriptingProvider";
	return name;
}

void LuaScriptingProvider::_registerWithService(Ember::ScriptingService* service)
{	
	mService = service;
}

Ember::IScriptingCallContext LuaScriptingProvider::createDefaultContext()
{
	return LuaScriptingCallContext();
}


void LuaScriptingProvider::forceGC()
{
#ifdef LUA51
	lua_gc(mLuaState, LUA_GCCOLLECT, 0);
#else
	lua_setgcthreshold(mLuaState,0);
#endif
}




// int32_t LuaScriptInterface::callFunction(uint32_t nParams)
// {
// 	int32_t result = LUA_NO_ERROR;
// 
// 	int size0 = lua_gettop(m_luaState);
// 	
// 	int error_index = lua_gettop(m_luaState) - nParams;
// 	lua_pushcfunction(m_luaState, luaErrorHandler);
// 	lua_insert(m_luaState, error_index);
// 
// 	if(lua_pcall(m_luaState, nParams, 1, error_index) != 0){
// 		LuaScriptInterface::reportError(NULL, std::string(LuaScriptInterface::popString(m_luaState)));
// 		result = LUA_ERROR;
// 	} else {
// 		result = (int32_t)LuaScriptInterface::popNumber(m_luaState);
// 	}
// 	lua_remove(m_luaState, error_index);
// 
// 	if((lua_gettop(m_luaState) + (int)nParams  + 1) != size0){
// 		LuaScriptInterface::reportError(NULL, "Stack size changed!");
// 	}
// 
// 	return result;
// }



// CEGUI::ScriptModule& LuaScriptingProvider::getScriptModule()
// {
// 	return mLuaScriptModule;
// }

}
