//
// C++ Interface: IScriptingProvider
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
#ifndef EMBERISCRIPTINGPROVIDER_H
#define EMBERISCRIPTINGPROVIDER_H

#include <string>

#include "IResourceProvider.h"

namespace Ember {

class ScriptingService;

// class IScriptingCallReturnValue
// {
// public:
// 
// 	virtual operator double() = 0;
// 	virtual operator std::string() = 0;
// 
// 	template<typename Treturn> 
// 	Treturn asObject();
// protected:
// };


/**
@brief Base interface for a call into a scripting environment.
Sometimes when you make a call into a scripting environment, from the C++ code, you want to be able to inspect and act upon the return values. However, different scripting languages handles these things differently, so instead of a common class we provide this very bare bone interface. It's then up to to each implementation of IScriptingProvider to make sure that it also provides a concrete implementation of this class and knows how to handle it.

Whenever you then want to act on values returned from a call into a specific scripting environment you have to first create the suitable instance of a subclass of this, and then pass that onto a call to the ScriptingService::executeScript method.
@author Erik Hjortsberg <erik.hjortsber@iteam.se>
*/
class IScriptingCallContext
{
public:
	virtual ~IScriptingCallContext() {}
// 	IScriptingCallReturnValue* getReturnValue() = 0;

protected:
};

/**
@brief A generic scripting provider.
We want to support many different scripting environments, so the ScriptingService isn't bound to any one type and insteads deals with abstract implementations of this interface. Any implementation is then bound to a specific scripting environment, such as Lua or Python.

Any implementation of this interface is expected to handle setup and teardown of the underlying scripting environment/virtual machine.
Often you want to do it in two separate steps, where a call to stop() forces the scripting environment to unload all allocated objects and structs, and the desctructor then handles destroying the scripting environment completely.
@author Erik Hjortsberg <erik.hjortsber@iteam.se>
*/
class IScriptingProvider
{
public:
	
	virtual ~IScriptingProvider(){}
	
	/**
	 *    @brief Loads the script from the wrapper.
	 * @param resourceWrapper A resource wrapper pointing to a valid resource which can be loaded. This should contain a text file with the script contents.
	 */
	virtual void loadScript(ResourceWrapper& resourceWrapper) = 0;
	
	/**
	 *    @brief Executes the supplied string directly into the scripting environment.
	 * Optionally a pointer to a scripting call context can be submitted too, which will then be populated with return values and other scripting environment specific info.
	 * @param scriptCode The code to excute.
	 * @param callContext An optional pointer to a scripting call context. This will be populated with return values and other info. If you don't have any need for such info, leave this empty.
	 */
	virtual void executeScript(const std::string& scriptCode, Ember::IScriptingCallContext* callContext) = 0;
	
	virtual void callFunction(const std::string& functionName, int narg, Ember::IScriptingCallContext* callContext) = 0;
	
	/**
	 *   @brief Returns true if the provider will load the supplied script name. This is in most cases decided from the filename suffix.
	 * @param scriptName The name of the script.
	 * @return True if the script can be loaded, else false.
	 */
	virtual bool willLoadScript(const std::string& scriptName) = 0;
	
	/**
	 *   @brief Gets the unique name of the scripting provider.
	 * @return The name of the scripting provider.
	 */
	virtual const std::string& getName() const = 0;
	
	/**
	 *   @brief Register with  a service to allow for callbacks etc.
	 * @param service The service to register with.
	 */
	virtual void _registerWithService(ScriptingService* service) = 0;
	
	/**
	 *   @brief Forces a full garbage collection.
	 */
	virtual void forceGC() = 0;

// 	virtual void start() = 0;
	/**
	* @brief  Stops the scripting provider. 
	* This involves releasing all of the objects held in scripts, but not destroying the actual scripting environment (so that callbacks and similiar objects still can use it).
	*/
	virtual void stop() = 0;

};

}

#endif
