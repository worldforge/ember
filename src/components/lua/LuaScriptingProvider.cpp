//
// C++ Implementation: LuaScriptingProvider
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LuaScriptingProvider.h"

#include "framework/Exception.h"
#include "framework/LoggingInstance.h"
#include "services/config/ConfigService.h"

#include "LuaScriptingCallContext.h"

#include "components/lua/bindings/lua/BindingsLua.h"
#include "framework/bindings/lua/varconf/BindingsVarconf.h"
#include "framework/bindings/lua/eris/BindingsEris.h"
#include "framework/bindings/lua/atlas/BindingsAtlas.h"
#include "framework/bindings/lua/BindingsFramework.h"
#include "components/ogre/scripting/bindings/lua/BindingsEmberOgre.h"
#include "components/ogre/scripting/bindings/lua/ogre/BindingsOgre.h"
#include "components/ogre/widgets/representations/bindings/lua/BindingsRepresentations.h"
#include "components/ogre/widgets/adapters/atlas/bindings/lua/BindingsAtlasAdapters.h"
#include "domain/bindings/lua/BindingsDomain.h"
#include "services/bindings/lua/BindingsServices.h"
#include "components/cegui/bindings/lua/BindingsCEGUI.h"
#include "framework/bindings/lua/wfmath/BindingsWFMath.h"

namespace Ember::Lua {

LuaScriptingProvider::LuaScriptingProvider() {
	bool loadDebugLib = true;
	if (ConfigService::getSingleton().itemExists("lua", "debug")) {
		loadDebugLib = static_cast<bool>(ConfigService::getSingleton().getValue("lua", "debug"));
	}

	mLua.open_libraries(sol::lib::base);
	//mLua.open_libraries(sol::lib::package);
	mLua.open_libraries(sol::lib::table);
	//mLua.open_libraries(sol::lib::io);
	mLua.open_libraries(sol::lib::string);
	mLua.open_libraries(sol::lib::math);
	if (loadDebugLib) {
		mLua.open_libraries(sol::lib::debug);
	}

	registerBindingsLua(mLua);
	registerBindingsVarconf(mLua);
	registerBindingsEris(mLua);
	registerBindingsAtlas(mLua);
	registerBindingsFramework(mLua);
	registerBindingsEmberOgre(mLua);
	registerBindingsOgre(mLua);
	registerBindingsAtlasAdapters(mLua);
	registerBindingsRepresentations(mLua);
	registerBindingsServices(mLua);
	registerBindingsDomain(mLua);
	registerBindingsCEGUI(mLua);
	registerBindingsWFMath(mLua);

}


LuaScriptingProvider::~LuaScriptingProvider() {
	S_LOG_INFO("Shutting down lua environment.");
}

void LuaScriptingProvider::stop() {
	try {
		//we want to clear up the lua environment without destroying it (lua_close destroys it)
		std::string shutdownScript(R"""(for key,value in pairs(_G) do if key ~= "_G" and key ~= "pairs" then _G[key] = nil end end)""");
		{
			executeScript(shutdownScript, nullptr);
		}
		forceGC();
		mLua = {};
	} catch (...) {
		S_LOG_WARNING("Error when stopping lua.");
	}
}

sol::state& LuaScriptingProvider::getLuaState() {
	return mLua;
}

void LuaScriptingProvider::loadScript(ResourceWrapper& resWrapper, IScriptingCallContext* callContext) {
	executeScriptImpl(std::string(resWrapper.getDataPtr(), resWrapper.getSize()), dynamic_cast<LuaScriptingCallContext*>(callContext), resWrapper.getName());
}

void LuaScriptingProvider::executeScript(const std::string& scriptCode, IScriptingCallContext* callContext) {
	executeScriptImpl(scriptCode, dynamic_cast<LuaScriptingCallContext*>(callContext), "");
}

void LuaScriptingProvider::executeScriptImpl(const std::string& scriptCode, LuaScriptingCallContext* luaCallContext, const std::string& scriptName) {

	auto result = mLua.script(scriptCode);
	if (!result.valid()) {
		throw result.get<sol::error>();
	}
	if (luaCallContext) {
		luaCallContext->ReturnValue = result;
	}

}

bool LuaScriptingProvider::willLoadScript(const std::string& scriptName) {
	size_t pos = scriptName.rfind(".lua");
	return (pos == scriptName.size() - 4);
}

const std::string& LuaScriptingProvider::getName() const {
	static std::string name = "LuaScriptingProvider";
	return name;
}


void LuaScriptingProvider::forceGC() {
	mLua.collect_gc();
}

}
