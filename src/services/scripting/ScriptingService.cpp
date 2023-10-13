//
// C++ Implementation: ScriptingService
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
#include "ScriptingService.h"


#include "framework/LoggingInstance.h"
#include "framework/IScriptingProvider.h"
#include "framework/Tokeniser.h"

#include <vector>

namespace Ember {

ScriptingService::ScriptingService()
		: Service("Scripting"),
		  LoadScript("loadscript", this, "Loads a script."),
		  mResourceProvider(nullptr) {
}


ScriptingService::~ScriptingService() {
	for (auto& entry: mProviders) {
		entry.second->stop();
	}
}

void ScriptingService::stop() {
	for (auto& entry: mProviders) {
		entry.second->stop();
	}
}

void ScriptingService::loadScript(const std::string& script) {
	if (mResourceProvider) {
		ResourceWrapper resWrapper = mResourceProvider->getResource(script);
		if (!resWrapper.hasData()) {
			scriptError("Unable to find script file " + script + ".");
			S_LOG_FAILURE("Unable to find script file " + script + ".");
			return;
		}

		for (auto& entry: mProviders) {
			//check if the provider will load the script
			if (entry.second->willLoadScript(script)) {
				S_LOG_INFO("Loading script: " << script << " with scripting provider " << entry.second->getName());
				try {
					entry.second->loadScript(resWrapper, nullptr);
				} catch (const std::exception& ex) {
					S_LOG_WARNING("Error when loading script " << script << " with provider " << entry.second->getName() << "." << ex);
					scriptError(ex.what());
				} catch (...) {
					S_LOG_WARNING("Got unknown script error when loading the script " << script);
					scriptError("Unknown error loading script " + script);
				}
				return;
			}
		}
		S_LOG_FAILURE("Could not find a scripting provider which will load the script " << script << ".");
	}
}

void ScriptingService::executeCode(const std::string& scriptCode, const std::string& scriptType, IScriptingCallContext* callContext) {
	auto I = mProviders.find(scriptType);
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

void ScriptingService::registerScriptingProvider(std::unique_ptr<IScriptingProvider> provider) {
	if (mProviders.find(provider->getName()) != mProviders.end()) {
		S_LOG_FAILURE("Could not add already existing scripting provider with name " + provider->getName());
	} else {
		S_LOG_INFO("Registering scripting provider " << provider->getName());
		mProviders[provider->getName()] = std::move(provider);
	}
}

void ScriptingService::scriptError(const std::string& error) {
	//S_LOG_WARNING(error);
	EventScriptError.emit(error);
}

IScriptingProvider* ScriptingService::getProviderFor(const std::string& providerName) {
	auto I = mProviders.find(providerName);
	if (I != mProviders.end()) {
		return I->second.get();
	}
	return nullptr;
}

void ScriptingService::forceGCForAllProviders() {
	for (auto& provider: mProviders) {
		provider.second->forceGC();
	}
}

void ScriptingService::runCommand(const std::string& command, const std::string& args) {
	if (LoadScript == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string script = tokeniser.nextToken();
		if (!script.empty()) {
			loadScript(script);
		}
	}
}

std::vector<std::string> ScriptingService::getProviderNames() {
	std::vector<std::string> names;
	for (auto& entry: mProviders) {
		names.push_back(entry.second->getName());
	}
	return names;

}

IResourceProvider* ScriptingService::getResourceProvider() {
	return mResourceProvider;
}

void ScriptingService::setResourceProvider(IResourceProvider* resourceProvider) {
	mResourceProvider = resourceProvider;
}


}
