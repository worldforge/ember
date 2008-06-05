//
// C++ Interface: LuaScriptingProvider
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
#ifndef EMBEROGRELUASCRIPTINGPROVIDER_H
#define EMBEROGRELUASCRIPTINGPROVIDER_H

#include "framework/IScriptingProvider.h"

#include <CEGUIScriptModule.h>
#include <CEGUILua.h>

#include "luaobject.h"

struct lua_State;

namespace Ember
{
	class ScriptingService;
}

namespace EmberOgre {

class LuaScriptingCallContext : public Ember::IScriptingCallContext
{
public:
	inline LuaScriptingCallContext();
	inline ~LuaScriptingCallContext();
	inline void setReturnValue(LuaRef* returnValue);
	inline LuaRef* getReturnValue();
protected:
	LuaRef* mReturnValue;
};

void LuaScriptingCallContext::setReturnValue(LuaRef* returnValue)
{
	delete returnValue;
	mReturnValue = returnValue;
}

LuaRef* LuaScriptingCallContext::getReturnValue()
{
	return mReturnValue;
}


/**
@brief A scripting provider for Lua.

This acts as a bridge between Ember and the Lua scripting environment. Opon creation and destruction it will take care of setting up and tearing down the lua virtual machine. Remember to call @see stop() before deleting an instance of this to make sure that everyting is properly cleaned up.
@author Erik Hjortsberg
*/
class LuaScriptingProvider : public Ember::IScriptingProvider
{
public:
    LuaScriptingProvider();

    virtual ~LuaScriptingProvider();
    
    virtual Ember::IScriptingCallContext createDefaultContext();
    
	/**
	 *    Loads the script.
	 * @param scriptName 
	 */
	virtual void loadScript(Ember::ResourceWrapper& resWrapper);
	
	/**
	 *    Executes the supplied string directly into the scripting environment.
	 * @param scriptCode 
	 */
	virtual void executeScript(Ember::IScriptingCallContext* callContext, const std::string& scriptCode, const std::string& scriptName = std::string(""));
	
	/**
	 *    Returns true if the provider will load the supplied script name. This is in most cases decided from the filename suffix.
	 * @param scriptName 
	 * @return 
	 */
	virtual bool willLoadScript(const std::string& scriptName);
	
	/**
	 *    Gets the unique name of the scripting provider.
	 * @return 
	 */
	virtual const std::string& getName() const;
	
	/**
	 *    Register with  a service to allow for callbacks etc.
	 * @param service 
	 */
	virtual void _registerWithService(Ember::ScriptingService* service);
	
	/**
	 *    Forces a full garbage collection.
	 */
	virtual void forceGC();
    
// 	virtual void start();


	/**
	 *    @brief Stops the lua environment, which mainly means that all objects are destroyed.
	 * Call this before this object is destroyed to make sure that all held objects and references are properly released. If not, there's a risk of dangling pointers.
	 */
	virtual void stop();
    
    
	/**
	 *    Gets the current lua state. This will always return a valid lua virtual machine, but note that if @see stop() already has been called it will porbably be in an invalid state.
	 * @return The current lua environment.
	 */
	lua_State* getLuaState();
	

private:

	void initialize();
	void createState();
// 	std::auto_ptr<CEGUI::LuaScriptModule> mLuaScriptModule;
	Ember::ScriptingService* mService;
	lua_State* mLuaState;
	
};

}

#endif
