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

	lua.new_usertype<AtlasObjectDecoder>("Ember::AtlasObjectDecoder",
										 sol::constructors<AtlasObjectDecoder(const Atlas::Objects::Factories&)>(),
										 "getLastObject", &AtlasObjectDecoder::getLastObject,
										 "EventObjectArrived",
										 sol::property([](AtlasObjectDecoder* self) {
											 return Connector::makeConnector<RefValueAdapter<const Atlas::Objects::Root>>(self->EventObjectArrived);
										 })
	);

	lua.new_usertype<AttributeObserver>("Ember::AttributeObserver",
										sol::constructors<
												AttributeObserver(Eris::Entity&, const std::string&),
												AttributeObserver(Eris::Entity&, const std::vector<std::string>&),
												AttributeObserver(Eris::Entity&, const std::string&, const std::string&)>(),
										"forceEvaluation", &AttributeObserver::forceEvaluation,
										"EventChanged",
										sol::property([](AttributeObserver* self) {
											return Connector::makeConnector<RefValueAdapter<const Atlas::Message::Element>>(self->EventChanged);
										}));

	auto consoleBackend = lua.new_usertype<ConsoleBackend>("Ember::ConsoleBackend",
														   "getSingleton", &ConsoleBackend::getSingleton,
														   "runCommand", [](ConsoleBackend* self, const std::string& command, const std::string& args) { self->runCommand(command, args); },
														   "pushMessage", &ConsoleBackend::pushMessage,
														   "GotMessage",
														   sol::property([](ConsoleBackend* self) {
															   return Connector::makeConnector<StringValueAdapter, StringValueAdapter>(self->GotMessage);
														   }));

	lua.new_usertype<EntityExporter>("Ember::EntityExporter",
									 sol::constructors<EntityExporter(Eris::Account&)>(),
									 "start", &EntityExporter::start,
									 "cancel", &EntityExporter::cancel,
									 "setDescription", &EntityExporter::setDescription,
									 "setName", &EntityExporter::setName,
									 "setExportTransient", &EntityExporter::setExportTransient,
									 "setPreserveIds", &EntityExporter::setPreserveIds,
									 "getPreserveIds", &EntityExporter::getPreserveIds,
									 "setExportRules", &EntityExporter::setExportRules,
									 "getExportRules", &EntityExporter::getExportRules,
									 "getStats", &EntityExporter::getStats,
									 "EventCompleted",
									 sol::property([](EntityExporter* self) {
										 return Connector::makeConnector(self->EventCompleted);
									 }),
									 "EventProgress",
									 sol::property([](EntityExporter* self) {
										 return Connector::makeConnector(self->EventProgress);
									 }));


	lua.new_usertype<EntityExporter::Stats>("Ember::EntityExporter::Stats",
											"entitiesQueried", &EntityExporter::Stats::entitiesQueried,
											"entitiesReceived", &EntityExporter::Stats::entitiesReceived,
											"entitiesError", &EntityExporter::Stats::entitiesError,
											"mindsQueried", &EntityExporter::Stats::mindsQueried,
											"mindsReceived", &EntityExporter::Stats::mindsReceived,
											"mindsError", &EntityExporter::Stats::mindsError,
											"rulesQueried", &EntityExporter::Stats::rulesQueried,
											"rulesReceived", &EntityExporter::Stats::rulesReceived,
											"rulesError", &EntityExporter::Stats::rulesError);

	lua.new_usertype<IScriptingProvider>("Ember::IScriptingProvider",
										 "getName", &IScriptingProvider::getName,
										 "forceGC", &IScriptingProvider::forceGC);

	lua.new_usertype<std::stringstream>("std::stringstream",
										sol::constructors<std::stringstream(), std::stringstream(const std::string&)>(),
										"str", [](std::stringstream* self) { return self->str(); },
										sol::base_classes, sol::bases<std::iostream, std::istream, std::ostream>()
	);


	sol::table log;
	log["verbose"] = [](const std::string& message) { S_LOG_VERBOSE(message); };
	log["info"] = [](const std::string& message) { S_LOG_INFO(message); };
	log["warning"] = [](const std::string& message) { S_LOG_WARNING(message); };
	log["failure"] = [](const std::string& message) { S_LOG_FAILURE(message); };
	log["critical"] = [](const std::string& message) { S_LOG_CRITICAL(message); };
	lua["log"] = log;
}