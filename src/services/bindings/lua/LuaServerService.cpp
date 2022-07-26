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
#include "RegisterLua.h"
#include "services/server/ServerService.h"
#include "services/server/AvatarTransferInfo.h"
#include "services/server/LocalServerAdminCreator.h"
#include <Eris/Account.h>
#include <Eris/Connection.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>

using namespace Ember;
using namespace Ember::Lua;

template<>
void registerLua<ServerService>(sol::table& space) {
	auto serverService = space.new_usertype<ServerService>("ServerService", sol::no_constructor);
	serverService["getAccount"] = &ServerService::getAccount;
	serverService["connect"] = sol::overload([](ServerService* self, const std::string& host) { return self->connect(host); },
											 [](ServerService* self, const std::string& host, short port) { return self->connect(host, port); });
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


	auto avatarTransferInfo = space.new_usertype<AvatarTransferInfo>("AvatarTransferInfo", sol::no_constructor);
	avatarTransferInfo["getAvatarName"] = &AvatarTransferInfo::getAvatarName;
	avatarTransferInfo["getTransferInfo"] = &AvatarTransferInfo::getTransferInfo;

	space.new_usertype<LocalServerAdminCreator>("LocalServerAdminCreator",
												sol::constructors<LocalServerAdminCreator(Ember::ServerService&)>()
	);
}