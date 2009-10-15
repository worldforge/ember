//
// C++ Interface: LuaScriptingProvider
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

struct lua_State;

namespace Ember
{
	class ScriptingService;
}

namespace EmberOgre {

class LuaScriptingCallContext;

/**
@brief A scripting provider for Lua.

This acts as a bridge between Ember and the Lua scripting environment. Opon creation and destruction it will take care of setting up and tearing down the lua virtual machine. Remember to call stop() before deleting an instance of this to make sure that everything is properly cleaned up.

If you want to inspect the return values from calls to lua scripts, pass a pointer to LuaScriptingCallContext to the executeScript methods.
@author Erik Hjortsberg
*/
class LuaScriptingProvider : public Ember::IScriptingProvider
{
public:
    LuaScriptingProvider();

    virtual ~LuaScriptingProvider();

	/**
	 *    @brief Loads the script from the wrapper.
	 * @param resourceWrapper A resource wrapper pointing to a valid resource which can be loaded. This should contain a text file with the script contents.
	 */
	virtual void loadScript(Ember::ResourceWrapper& resWrapper);

	/**
	 *    @brief Executes the supplied string directly into the scripting environment.
	 * Optionally a pointer to a scripting call context can be submitted too, which will then be populated with return values and other scripting environment specific info.
	 * @param scriptCode The code to excute.
	 * @param callContext An optional pointer to a scripting call context. This will be populated with return values and other info. If you don't have any need for such info, leave this empty.
	 */
	virtual void executeScript(const std::string& scriptCode, Ember::IScriptingCallContext* callContext);

	virtual void callFunction(const std::string& functionName, int narg, Ember::IScriptingCallContext* callContext);

	/**
	 *   @brief Returns true if the provider will load the supplied script name. This is in most cases decided from the filename suffix.
	 * @param scriptName The name of the script.
	 * @return True if the script can be loaded, else false.
	 */
	virtual bool willLoadScript(const std::string& scriptName);

	/**
	 *   @brief Gets the unique name of the scripting provider.
	 * @return The name of the scripting provider.
	 */
	virtual const std::string& getName() const;

	/**
	 *   @brief Register with  a service to allow for callbacks etc.
	 * @param service The service to register with.
	 */
	virtual void _registerWithService(Ember::ScriptingService* service);

	/**
	 *   @brief Forces a full garbage collection.
	 */
	virtual void forceGC();

// 	virtual void start();


	/**
	 *    @brief Stops the lua environment, which mainly means that all objects are destroyed.
	 * Call this before this object is destroyed to make sure that all held objects and references are properly released. If not, there's a risk of dangling pointers.
	 */
	virtual void stop();


	/**
	 *    @brief Gets the current lua state.
	 * This will always return a valid lua virtual machine, but note that if @see stop() already has been called it will porbably be in an invalid state.
	 * @return The current lua environment.
	 */
	lua_State* getLuaState();


// 	int getErrorHandlingFunctionIndex() const;

	/**
	 * @brief Gets the name of the error handling function, if available.
	 * @return The error handling function name (i.e. a lua function).
	 */
	const std::string& getErrorHandlingFunctionName() const;


private:

	/**
	 *    @brief Executes the supplied script code.
	 * @param scriptCode The code to execute.
	 * @param luaCallContext An optional lua call context, which if present will contain any return values.
	 * @param scriptName The name of the script, mainly used for debugging purpose.
	 */
	void executeScriptImpl(const std::string& scriptCode, LuaScriptingCallContext* luaCallContext, const std::string& scriptName = std::string(""));

	void callFunctionImpl(const std::string& functionName, int narg, LuaScriptingCallContext* callContext);

	/**
	 *    Initializes the lua scripting environment. This entails creating a new Lua virtual machine/state,  making sure that the correct lua libraries are loaded and a calling tolua bindings registering hooks.
	 If you add a new tolua bindings class, don't forget to alter this method to include a call to the method which registers all classes and structs.
	 */
	void initialize();


	/**
	 *    Creates a new Lua virtual machine/state.
	 */
	void createState();
// 	std::auto_ptr<CEGUI::LuaScriptModule> mLuaScriptModule;

	/**
	The main scripting service instance.
	*/
	Ember::ScriptingService* mService;

	/**
	The main lua state. This is the sole entry into the lua virtual machine.
	*/
	lua_State* mLuaState;

// 	int mErrorHandlingFunctionIndex;

	/**
	 * @brief The name of the error handling function, if available.
	 */
	std::string mErrorHandlingFunctionName;

};

}

#endif
