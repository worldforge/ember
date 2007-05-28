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
#include "../EmberOgrePrerequisites.h"
#include "LuaScriptingProvider.h"

#include "bindings/lua/helpers/LuaConnector.h"

#include <CEGUIExceptions.h>

#include "framework/Exception.h"

// include Lua libs and tolua++
#include <lua.hpp>
#include <tolua++.h>

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
	mLuaState = lua_open();

	// init all standard libraries
	luaopen_base(mLuaState);
	luaopen_io(mLuaState);
	luaopen_string(mLuaState);
	luaopen_table(mLuaState);
	luaopen_math(mLuaState);
// #if defined(DEBUG) || defined (_DEBUG)
// 	luaopen_debug(d_state);
// #endif
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
			throw Ember::Exception("Unable to execute Lua script file: '"+resWrapper.getName()+"'\n\n"+errMsg+"\n");
		}
		
		// call it
		if (lua_pcall(mLuaState,0,0,0))
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

void LuaScriptingProvider::executeScript(const std::string& scriptCode)
{
	try {
		int top = lua_gettop(mLuaState);
	
		// load code into lua and call it
		int error =	luaL_loadbuffer(mLuaState, scriptCode.c_str(), scriptCode.length(), scriptCode.c_str()) || lua_pcall(mLuaState,0,0,0);
	
		// handle errors
		if (error)
		{
			std::string errMsg(lua_tostring(mLuaState,-1));
			lua_settop(mLuaState,top);
			throw Ember::Exception("Unable to execute Lua script string: '"+scriptCode+"'\n\n"+errMsg+"\n");
		}
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

void LuaScriptingProvider::forceGC()
{
	lua_setgcthreshold(mLuaState,0);
}


// CEGUI::ScriptModule& LuaScriptingProvider::getScriptModule()
// {
// 	return mLuaScriptModule;
// }

}
