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

#include "tolua++.h"
#include "bindings/lua/helpers/LuaConnector.h"

#include <OgreString.h>

#include <CEGUIExceptions.h>

#include "framework/Exception.h"

TOLUA_API int tolua_Ogre_open (lua_State* tolua_S);
TOLUA_API int tolua_Eris_open (lua_State* tolua_S);
TOLUA_API int tolua_EmberServices_open (lua_State* tolua_S);
TOLUA_API int tolua_EmberOgre_open (lua_State* tolua_S);
TOLUA_API int tolua_Helpers_open (lua_State* tolua_S);
TOLUA_API int tolua_Framework_open (lua_State* tolua_S);

namespace EmberOgre {

LuaScriptingProvider::LuaScriptingProvider()
{
	tolua_Framework_open(mLuaScriptModule.getLuaState());
	tolua_EmberOgre_open(mLuaScriptModule.getLuaState());
	tolua_Eris_open(mLuaScriptModule.getLuaState());
	tolua_EmberServices_open(mLuaScriptModule.getLuaState());
	tolua_Helpers_open (mLuaScriptModule.getLuaState());
	tolua_Ogre_open(mLuaScriptModule.getLuaState());
	LuaConnector::setState(mLuaScriptModule.getLuaState());
}


LuaScriptingProvider::~LuaScriptingProvider()
{
}

void LuaScriptingProvider::loadScript(const std::string& scriptName)
{
	try {
		getScriptModule().executeScriptFile(scriptName);
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
		getScriptModule().executeString(scriptCode);
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
	this->service = service;
}

CEGUI::ScriptModule& LuaScriptingProvider::getScriptModule()
{
	return mLuaScriptModule;
}

}
