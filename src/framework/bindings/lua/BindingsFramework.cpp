/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "BindingsFramework.h"

#include "components/lua/Connector.h"
#include "framework/AtlasObjectDecoder.h"
#include "framework/AttributeObserver.h"
#include "framework/ConsoleBackend.h"
#include "framework/EntityExporter.h"
#include "framework/EntityImporter.h"
#include "framework/IScriptingProvider.h"
#include <Atlas/Objects/Factories.h>
#include <Eris/Entity.h>
#include <Eris/Account.h>

#include <varconf/variable.h>

#include <iostream>
#include <istream>
#include <ostream>

using namespace Ember;
using namespace Ember::Lua;

void registerBindingsFramework(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto std = lua["std"].get_or_create<sol::table>();
	auto OgreView = Ember["OgreView"].get_or_create<sol::table>();
	auto Gui = OgreView["Gui"].get_or_create<sol::table>();
	auto Adapters = Gui["Adapters"].get_or_create<sol::table>();


	auto atlasObjectDecoder = Ember.new_usertype<AtlasObjectDecoder>("AtlasObjectDecoder",
																	 sol::constructors<AtlasObjectDecoder(const Atlas::Objects::Factories&)>()
	);
	atlasObjectDecoder["getLastObject"] = &AtlasObjectDecoder::getLastObject;
	atlasObjectDecoder["EventObjectArrived"] = LuaConnector::make_property(&AtlasObjectDecoder::EventObjectArrived);
	auto attributeObserver = Ember.new_usertype<AttributeObserver>("AttributeObserver",
																   sol::constructors<
																		   AttributeObserver(Eris::Entity&, const std::string&),
																		   AttributeObserver(Eris::Entity&, const std::vector<std::string>&),
																		   AttributeObserver(Eris::Entity&, const std::string&, const std::string&)>()
	);
	attributeObserver["forceEvaluation"] = &AttributeObserver::forceEvaluation;
	attributeObserver["EventChanged"] = LuaConnector::make_property(&AttributeObserver::EventChanged);

	auto consoleBackend = Ember.new_usertype<ConsoleBackend>("ConsoleBackend", sol::no_constructor);
	consoleBackend["getSingleton"] = &ConsoleBackend::getSingleton;
	consoleBackend["runCommand"] = sol::overload([](ConsoleBackend* self, const std::string& command, const std::string& args) { self->runCommand(command, args); },
												 [](ConsoleBackend* self, const std::string& command) { self->runCommand(command); });
	consoleBackend["pushMessage"] = &ConsoleBackend::pushMessage;
	consoleBackend["GotMessage"] = LuaConnector::make_property(&ConsoleBackend::GotMessage);

	auto entityExporter = Ember.new_usertype<EntityExporter>("EntityExporter",
															 sol::constructors<EntityExporter(Eris::Account&)>());
	entityExporter["start"] = &EntityExporter::start;
	entityExporter["cancel"] = &EntityExporter::cancel;
	entityExporter["setDescription"] = &EntityExporter::setDescription;
	entityExporter["setName"] = &EntityExporter::setName;
	entityExporter["setExportTransient"] = &EntityExporter::setExportTransient;
	entityExporter["setPreserveIds"] = &EntityExporter::setPreserveIds;
	entityExporter["getPreserveIds"] = &EntityExporter::getPreserveIds;
	entityExporter["setExportRules"] = &EntityExporter::setExportRules;
	entityExporter["getExportRules"] = &EntityExporter::getExportRules;
	entityExporter["getStats"] = &EntityExporter::getStats;
	entityExporter["EventCompleted"] = LuaConnector::make_property(&EntityExporter::EventCompleted);
	entityExporter["EventProgress"] = LuaConnector::make_property(&EntityExporter::EventCompleted);


	entityExporter["Stats"] = Ember.new_usertype<EntityExporter::Stats>("Stats", sol::no_constructor,
																		"entitiesQueried", &EntityExporter::Stats::entitiesQueried,
																		"entitiesReceived", &EntityExporter::Stats::entitiesReceived,
																		"entitiesError", &EntityExporter::Stats::entitiesError,
																		"mindsQueried", &EntityExporter::Stats::mindsQueried,
																		"mindsReceived", &EntityExporter::Stats::mindsReceived,
																		"mindsError", &EntityExporter::Stats::mindsError,
																		"rulesQueried", &EntityExporter::Stats::rulesQueried,
																		"rulesReceived", &EntityExporter::Stats::rulesReceived,
																		"rulesError", &EntityExporter::Stats::rulesError);

	Ember.new_usertype<IScriptingProvider>("IScriptingProvider", sol::no_constructor,
										   "getName", &IScriptingProvider::getName,
										   "forceGC", &IScriptingProvider::forceGC);

	std.new_usertype<std::stringstream>("std::stringstream",
										sol::constructors<std::stringstream(), std::stringstream(const std::string&)>(),
										"str", [](std::stringstream* self) { return self->str(); },
										sol::base_classes, sol::bases<std::iostream, std::istream, std::ostream>()
	);


	auto log = lua.create_table();
	log["verbose"] = [](const std::string& message) { S_LOG_VERBOSE(message); };
	log["info"] = [](const std::string& message) { S_LOG_INFO(message); };
	log["warning"] = [](const std::string& message) { S_LOG_WARNING(message); };
	log["failure"] = [](const std::string& message) {
		S_LOG_FAILURE(message);
	};
	log["critical"] = [](const std::string& message) { S_LOG_CRITICAL(message); };
	log["foo"] = [](const std::string& message) { S_LOG_CRITICAL(message); };

	lua["log"] = log;
}