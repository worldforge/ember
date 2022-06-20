//
// C++ Interface: ScriptingService
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
#ifndef EMBERSCRIPTINGSERVICE_H
#define EMBERSCRIPTINGSERVICE_H

#include <framework/Service.h>
#include <framework/ConsoleCommandWrapper.h>

#include <sigc++/signal.h>
#include <map>
#include <vector>
#include <memory>

namespace Ember {
struct IResourceProvider;

struct IScriptingProvider;

struct IScriptingCallContext;

/**
@author Erik Ogenvik

This service provides scripting support.
In order to use it, an instance implementing IScriptingProvider must be created and registered with the service.
Scripts are then loaded through call to the method loadScript(...). Scripts can also be loaded through the console command /loadscript <path>
*/
class ScriptingService : public Service, public ConsoleObject {
	friend struct IScriptingProvider;

public:

	ScriptingService();

	~ScriptingService() override;

	/**
	Console command for loading scripts.
	*/
	const ConsoleCommandWrapper LoadScript;

	/**
	 *    Registers a new scripting provider.
	 * @param provider 
	 */
	void registerScriptingProvider(std::unique_ptr<IScriptingProvider> provider);

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
	void executeCode(const std::string& scriptCode, const std::string& scriptType, IScriptingCallContext* callContext = nullptr);

	/**
	 *    The EventScriptError signal will be emitted when there is an error in a script.
	 * @return 
	 */
	sigc::signal<void, const std::string&> EventScriptError;

	/**
	 *    Implement ConsoleObject method.
	 * @param command 
	 * @param args 
	 */
	void runCommand(const std::string& command, const std::string& args) override;

	/**
	 *    Returns the provider with the specified name, or 0 if no can be found.
	 * @param providerName 
	 * @return 
	 */
	IScriptingProvider* getProviderFor(const std::string& providerName);

	/**
	 *    Returns a list of the names of all registered scripting providers.
	 * @return 
	 */
	std::vector<std::string> getProviderNames();

	IResourceProvider* getResourceProvider();

	void setResourceProvider(IResourceProvider* resourceProvider);

	/**
	 * @brief Forces garbage collection for all scripting providers.
	 */
	void forceGCForAllProviders();

	void stop();

private:

	/**
	 *    Call this method when there's an error in a script. This will emit the mEventScriptError signal.
	 * @param error 
	 */
	void scriptError(const std::string& error);

	typedef std::map<std::string, std::unique_ptr<IScriptingProvider>> ProviderStore;

	/**
	A map of all scripting providers.
	*/
	std::map<std::string, std::unique_ptr<IScriptingProvider>> mProviders;


	IResourceProvider* mResourceProvider;


};


}

#endif
