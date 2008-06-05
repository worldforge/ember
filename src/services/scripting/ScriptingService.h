//
// C++ Interface: ScriptingService
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
#ifndef EMBERSCRIPTINGSERVICE_H
#define EMBERSCRIPTINGSERVICE_H

#include <framework/Service.h>
#include <framework/ConsoleObject.h>

#include <sigc++/signal.h>
#include <map>
#include <vector>

namespace Ember {
class IResourceProvider;
class IScriptingProvider;
class IScriptingCallContext;
/**
@author Erik Hjortsberg

This service provides scripting support.
In order to use it, an instance implementing IScriptingProvider must be created and registered with the service.
Scripts are then loaded through call to the method loadScript(...). Scripts can also be loaded through the console command /loadscript <path>
*/
class ScriptingService : public Service, public ConsoleObject
{
friend class IScriptingProvider;
public:
    
    ScriptingService();

    virtual ~ScriptingService();

	virtual Service::Status start();
    
    virtual void stop(int code);
	
    /**
    Console command for loading scripts.
    */
	const Ember::ConsoleCommandWrapper LoadScript;
	
	/**
	 *    Registers a new scripting provider.
	 * @param provider 
	 */
	void registerScriptingProvider(IScriptingProvider* provider);
	
	/**
	 *    Loads a new script, if there is an registered scripting provider which will be able to load it.
	 * @param script 
	 */
	void loadScript(const std::string& script);
	
	/**
	 *    Executes the supplied code directly into the provider with the supplied name.
	 * @param scriptCode 
	 * @param scriptType 
	 */
	void executeCode(const std::string& scriptCode, const std::string& scriptType);
	void executeCode(IScriptingCallContext* callContext, const std::string& scriptCode, const std::string& scriptType);
	
	/**
	 *    The EventScriptError signal will be emitted when there is an error in a script.
	 * @return 
	 */
	sigc::signal<void, const std::string&>& getEventScriptError();
	
	/**
	 *    Implement ConsoleObject method.
	 * @param command 
	 * @param args 
	 */
	virtual void runCommand(const std::string &command, const std::string &args);
	
	/**
	 *    Returns the provider with the specified name, or 0 if no can be found.
	 * @param providerName 
	 * @return 
	 */
	IScriptingProvider* getProviderFor(const std::string &providerName);
	
	/**
	 *    Returns a list of the names of all registered scripting providers.
	 * @return 
	 */
	std::vector<std::string> getProviderNames();
	
	IResourceProvider* getResourceProvider();
	
	void setResourceProvider(Ember::IResourceProvider* resourceProvider);
	
	/**
	Returns whether all scripting methods should be looked up at every call. Setting this to true will decrease performance, but allow for dynamic updating of script methods.
	*/
	inline bool getAlwaysLookup() const;
	/**
	Sets whether all scripting methods should be looked up at every call. Setting this to true will decrease performance, but allow for dynamic updating of script methods.
	*/
	inline void setAlwaysLookup(bool alwaysLookup);
private:

	/**
	 *    Call this method when there's an error in a script. This will emit the mEventScriptError signal.
	 * @param error 
	 */
	void scriptError(const std::string& error);
	
	typedef std::map<std::string, IScriptingProvider*> ProviderStore;
	
	/**
	A map of all scripting providers.
	*/
	ProviderStore mProviders;
	
	sigc::signal<void, const std::string&> mEventScriptError;

	IResourceProvider* mResourceProvider;
	
	bool mAlwaysLookup;

};

bool ScriptingService::getAlwaysLookup() const
{
	return mAlwaysLookup;
}

void ScriptingService::setAlwaysLookup(bool alwaysLookup)
{
	mAlwaysLookup = alwaysLookup;
}


}

#endif
