//
// C++ Implementation: ScriptingService
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ScriptingService.h"


#include "framework/LoggingInstance.h"
#include "framework/IScriptingProvider.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/Exception.h"

namespace Ember {

ScriptingService::ScriptingService()
: LoadScript("loadscript", this, "Loads a script."), mResourceProvider(0), mAlwaysLookup(false)
{
}


ScriptingService::~ScriptingService()
{
	stop(0);
	for(ProviderStore::iterator I = mProviders.begin(); I != mProviders.end(); ++I)
	{
		delete I->second;
	}
}

void ScriptingService::stop(int code)
{
	Service::stop(code);
	for(ProviderStore::iterator I = mProviders.begin(); I != mProviders.end(); ++I)
	{
		I->second->stop();
	}
}

Service::Status ScriptingService::start()
{
	return Service::OK;
}

void ScriptingService::loadScript(const std::string& script)
{
	if (mResourceProvider) {
		Ember::ResourceWrapper resWrapper = mResourceProvider->getResource(script);
		if (!resWrapper.hasData()) {
			scriptError("Unable to find script file " + script + ".");
			S_LOG_FAILURE("Unable to find script file " + script + ".");
			return;
		}

		for(ProviderStore::iterator I = mProviders.begin(); I != mProviders.end(); ++I)
		{
			//check if the provider will load the script
			if (I->second->willLoadScript(script)) {
				S_LOG_INFO("Loading script: " << script << " with scripting provider " << I->second->getName() );
				try {
					I->second->loadScript(resWrapper);
				} catch (const std::exception& ex) {
					S_LOG_WARNING("Error when loading script " << script << " with provider " << I->second->getName() << "." << ex);
					scriptError(ex.what());
				} catch (...) {
					S_LOG_WARNING("Got unknown script error when loading the script " << script);
					scriptError("Unknown error loading script " + script );
				}
				return;
			}
		}
		S_LOG_FAILURE("Could not find a scripting provider which will load the script " << script << ".");
	}
}

void ScriptingService::executeCode(const std::string& scriptCode, const std::string& scriptType, IScriptingCallContext* callContext)
{
	ProviderStore::iterator I = mProviders.find(scriptType);
	if (I == mProviders.end()) {
		S_LOG_FAILURE("There is no scripting provider with the name \"" << scriptType << "\"");
	} else {
		try {
			I->second->executeScript(scriptCode, callContext);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when executing script\n" << scriptCode << "\nwith provider " << I->second->getName() << "." << ex);
			scriptError(ex.what());
		} catch (...) {
			S_LOG_WARNING("Got unknown script error when executing the script " << scriptCode);
			scriptError("Unknown error executing script.");
		}
	}
}

void ScriptingService::callFunction(const std::string& functionName, int narg, const std::string& scriptType, IScriptingCallContext* callContext)
{
	ProviderStore::iterator I = mProviders.find(scriptType);
	if (I == mProviders.end()) {
		S_LOG_FAILURE("There is no scripting provider with the name \"" << scriptType << "\"");
	} else {
		try {
			I->second->callFunction(functionName, narg, callContext);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when executing function '" << functionName << "' with provider " << I->second->getName() << "." << ex);
			scriptError(ex.what());
		} catch (...) {
			S_LOG_WARNING("Got unknown script error when executing the function " << functionName);
			scriptError("Unknown error executing script.");
		}
	}
}

sigc::signal<void, const std::string&>& ScriptingService::getEventScriptError()
{
	return mEventScriptError;
}

void ScriptingService::registerScriptingProvider(IScriptingProvider* provider)
{
	if (mProviders.find(provider->getName()) != mProviders.end()) {
		S_LOG_FAILURE("Could not add alreay existing scripting provider with name " + provider->getName());
	} else {
		mProviders[provider->getName()] = provider;
		provider->_registerWithService(this);
		S_LOG_INFO("Registered scripting provider " << provider->getName());
	}
}

void ScriptingService::scriptError(const std::string& error)
{
	//S_LOG_WARNING(error);
	mEventScriptError.emit(error);
}

IScriptingProvider* ScriptingService::getProviderFor(const std::string &providerName)
{
	ProviderStore::iterator I = mProviders.find(providerName);
	if (I != mProviders.end())
	{
		return I->second;
	}
	return 0;
}

void ScriptingService::runCommand(const std::string &command, const std::string &args)
{
    if (LoadScript == command){
 		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string script = tokeniser.nextToken();
		if (script != "") {
			loadScript(script);
		}
    }

    return;
}

std::vector<std::string> ScriptingService::getProviderNames()
{
	std::vector<std::string> names;
	for(ProviderStore::iterator I = mProviders.begin(); I != mProviders.end(); ++I)
	{
		names.push_back(I->second->getName());
	}
	return names;

}

Ember::IResourceProvider* ScriptingService::getResourceProvider()
{
	return mResourceProvider;
}

void ScriptingService::setResourceProvider(Ember::IResourceProvider* resourceProvider)
{
	mResourceProvider = resourceProvider;
}




}
