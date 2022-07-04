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

#include "BindingsServices.h"
#include "components/lua/Connector.h"
#include "domain/EmberEntity.h"
#include "domain/EntityTalk.h"
#include "domain/IGraphicalRepresentation.h"
#include "services/server/AvatarTransferInfo.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/EmberServices.h"
#include "services/input/Input.h"
#include "services/metaserver/MetaserverService.h"
#include "services/scripting/ScriptingService.h"
#include "framework/IScriptingProvider.h"
#include "services/serversettings/ServerSettings.h"
#include "services/serversettings/ServerSettingsCredentials.h"
#include "services/server/LocalServerAdminCreator.h"

#include <varconf/variable.h>

#include <Eris/Account.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/BaseConnection.h>
#include <Eris/View.h>

#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/Entity.h>

#include <sigc++/sigc++.h>

using namespace Ember;
using namespace Ember::Lua;
using namespace Ember::Services;

void registerBindingsServices(sol::state_view& lua) {

	auto Ember = lua["Ember"].get_or_create<sol::table>();


	auto avatarTransferInfo = Ember.new_usertype<AvatarTransferInfo>("AvatarTransferInfo", sol::no_constructor);
	avatarTransferInfo["getAvatarName"] = &AvatarTransferInfo::getAvatarName;
	avatarTransferInfo["getTransferInfo"] = &AvatarTransferInfo::getTransferInfo;

	Ember["BaseDirType_DATA"] = Ember::BaseDirType_DATA;
	Ember["BaseDirType_CONFIG"] = Ember::BaseDirType_CONFIG;
	Ember["BaseDirType_CACHE"] = Ember::BaseDirType_CACHE;
	Ember["BaseDirType_RUNTIME"] = Ember::BaseDirType_RUNTIME;

	auto configService = Ember.new_usertype<ConfigService>("ConfigService", sol::no_constructor);
	configService["getValue"] = sol::resolve<varconf::Variable(const std::string&, const std::string&) const>(&ConfigService::getValue);
	configService["setValue"] = &ConfigService::setValue;
	configService["isItemSet"] = &ConfigService::isItemSet;
	configService["itemExists"] = &ConfigService::itemExists;
	configService["getHomeDirectory"] = [](ConfigService* self, BaseDirType baseDirType) { return self->getHomeDirectory(baseDirType).string(); };
	configService["EventChangedConfigItem"] = LuaConnector::make_property(&ConfigService::EventChangedConfigItem);


	auto emberServices = Ember.new_usertype<EmberServices>("EmberServices", sol::no_constructor);
	emberServices["getSingleton"] = &EmberServices::getSingleton;
	emberServices["getConfigService"] = &EmberServices::getConfigService;
	emberServices["getMetaserverService"] = &EmberServices::getMetaserverService;
	emberServices["getServerService"] = &EmberServices::getServerService;
	emberServices["getScriptingService"] = &EmberServices::getScriptingService;
	emberServices["getServerSettingsService"] = &EmberServices::getServerSettingsService;

	auto input = Ember.new_usertype<Input>("Input", sol::no_constructor);
	input["getSingleton"] = &Input::getSingleton;
	input["IM_GUI"] = sol::var(Input::InputMode::IM_GUI);
	input["IM_MOVEMENT"] = sol::var(Input::InputMode::IM_MOVEMENT);
	input["isKeyDown"] = [](Input* self, int keyCode) { return self->isKeyDown(static_cast<SDL_Scancode>(keyCode)); };
	input["setInputMode"] = &Input::setInputMode;
	input["getInputMode"] = &Input::getInputMode;
	input["toggleInputMode"] = &Input::toggleInputMode;
	input["EventMouseMoved"] = LuaConnector::make_property(&Input::EventMouseMoved);
	input["EventMouseButtonPressed"] = LuaConnector::make_property(&Input::EventMouseButtonPressed);
	input["EventMouseButtonReleased"] = LuaConnector::make_property(&Input::EventMouseButtonReleased);
	input["EventChangedInputMode"] = LuaConnector::make_property(&Input::EventChangedInputMode);

	Ember.new_usertype<LocalServerAdminCreator>("LocalServerAdminCreator",
												sol::constructors<LocalServerAdminCreator(Ember::ServerService&)>()
	);
	auto metaServerService = Ember.new_usertype<MetaserverService>("MetaserverService", sol::no_constructor);
	metaServerService["getMetaServer"] = &MetaserverService::getMetaServer;
	metaServerService["compareVersions"] = &MetaserverService::compareVersions;

	auto scriptingService = Ember.new_usertype<ScriptingService>("ScriptingService", sol::no_constructor);
	scriptingService["loadScript"] = &ScriptingService::loadScript;
	scriptingService["executeCode"] = sol::overload(&ScriptingService::executeCode,
													[](ScriptingService* self, const std::string& scriptCode, const std::string& scriptType) { self->executeCode(scriptCode, scriptType); });
	scriptingService["getProviderFor"] = &ScriptingService::getProviderFor;
	scriptingService["getProviderNames"] = &ScriptingService::getProviderNames;
	scriptingService["EventScriptError"] = LuaConnector::make_property(&ScriptingService::EventScriptError);

	auto serverService = Ember.new_usertype<ServerService>("ServerService", sol::no_constructor);
	serverService["getAccount"] = &ServerService::getAccount;
	serverService["connect"] = &ServerService::connect;
	serverService["connectLocal"] = &ServerService::connectLocal;
	serverService["hasLocalSocket"] = &ServerService::hasLocalSocket;
	serverService["disconnect"] = &ServerService::disconnect;
	serverService["GotAvatar"] = LuaConnector::make_property(&ServerService::GotAvatar);
	serverService["GotView"] = LuaConnector::make_property(&ServerService::GotView);
	serverService["GotConnection"] = LuaConnector::make_property(&ServerService::GotConnection);
	serverService["GotAccount"] = LuaConnector::make_property(&ServerService::GotAccount);
	serverService["LoginSuccess"] = LuaConnector::make_property(&ServerService::LoginSuccess);
	serverService["LoginFailure"] = LuaConnector::make_property(&ServerService::LoginFailure);
	serverService["GotAllCharacters"] = LuaConnector::make_property(&ServerService::GotAllCharacters);
	serverService["DestroyedAccount"] = LuaConnector::make_property(&ServerService::DestroyedAccount);
	serverService["DestroyedAvatar"] = LuaConnector::make_property(&ServerService::DestroyedAvatar);
	serverService["DestroyedView"] = LuaConnector::make_property(&ServerService::DestroyedView);
	serverService["EventSendingObject"] = LuaConnector::make_property(&ServerService::EventSendingObject);
	serverService["EventReceivedObject"] = LuaConnector::make_property(&ServerService::EventReceivedObject);
	serverService["EventStatusChanged"] = LuaConnector::make_property(&ServerService::EventStatusChanged);
	serverService["EventLocalSocketChanged"] = LuaConnector::make_property(&ServerService::EventLocalSocketChanged);

	auto serverSettings = Ember.new_usertype<ServerSettings>("ServerSettings", sol::no_constructor);
	serverSettings["getItem"] = &ServerSettings::getItem;

	Ember.new_usertype<ServerSettingsCredentials>("ServerSettingsCredentials",
												  sol::constructors<ServerSettingsCredentials(const std::string&, const std::string&)>()
	);


}