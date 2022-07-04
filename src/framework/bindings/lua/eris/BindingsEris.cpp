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

#include <Eris/Account.h>
#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/Entity.h>
#include <Eris/View.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/Task.h>
#include <Eris/Metaserver.h>

#include "domain/EmberEntity.h"
#include "components/lua/Connector.h"

#include "BindingsEris.h"

using namespace Eris;
using namespace Ember::Lua;

void registerBindingsEris(sol::state_view& lua) {
	auto Eris = lua["Eris"].get_or_create<sol::table>();

	auto account = Eris.new_usertype<Account>("Account", sol::no_constructor);
	account["getConnection"] = &Account::getConnection;
	auto avatar = Eris.new_usertype<Avatar>(			"Avatar", sol::no_constructor);
	avatar["getId"] = &Avatar::getId;
	avatar["getEntityId"] = &Avatar::getEntityId;
	avatar["getEntity"] = &Avatar::getEntity;
	avatar["getView"] = &Avatar::getView;
	avatar["getConnection"] = &Avatar::getConnection;
	avatar["wield"] = &Avatar::wield;
	auto place = [](Avatar* self,
					const Eris::Entity* entity,
					const Eris::Entity* container,
					const WFMath::Point<3>& pos = WFMath::Point<3>(),
					const WFMath::Quaternion& orientation = WFMath::Quaternion(),
					float offset = 0,
					int amount = 1) {
		boost::optional<float> offsetOptional;
		if (offset != 0) {
			offsetOptional = offset;
		}
		self->place(entity, container, pos, orientation, offsetOptional, amount);
	};
	avatar["place"] = sol::overload(
			[=](Avatar* self,
				const Eris::Entity* entity,
				const Eris::Entity* container,
				const WFMath::Point<3>& pos = WFMath::Point<3>(),
				const WFMath::Quaternion& orientation = WFMath::Quaternion(),
				float offset = 0,
				int amount = 1) {
				place(self, entity, container, pos, orientation, offset, amount);
			},
			[=](Avatar* self,
				const Eris::Entity* entity,
				const Eris::Entity* container,
				const WFMath::Point<3>& pos = WFMath::Point<3>(),
				const WFMath::Quaternion& orientation = WFMath::Quaternion(),
				float offset = 0) {
				place(self, entity, container, pos, orientation, offset, 1);
			},
			[=](Avatar* self,
				const Eris::Entity* entity,
				const Eris::Entity* container,
				const WFMath::Point<3>& pos = WFMath::Point<3>(),
				const WFMath::Quaternion& orientation = WFMath::Quaternion()) {
				place(self, entity, container, pos, orientation, 0, 1);
			},
			[=](Avatar* self,
				const Eris::Entity* entity,
				const Eris::Entity* container,
				const WFMath::Point<3>& pos = WFMath::Point<3>()) {
				place(self, entity, container, pos, {}, 0, 1);
			},
			[=](Avatar* self,
				const Eris::Entity* entity,
				const Eris::Entity* container) {
				place(self, entity, container, {}, {}, 0, 1);
			}
	);

	avatar["take"] = sol::overload([](Avatar* self,
									  const Eris::Entity* entity,
									  int amount) {
		self->place(entity, self->getEntity(), {}, {}, {}, amount);
	}, [](Avatar* self,
		  const Eris::Entity* entity) {
		self->place(entity, self->getEntity(), {}, {}, {}, 1);
	});
	avatar["ContainerOpened"] = LuaConnector::make_property(&Avatar::ContainerOpened);
	avatar["ContainerClosed"] = LuaConnector::make_property(&Avatar::ContainerClosed);

	auto baseConnection = Eris.new_usertype<BaseConnection>("BaseConnection", sol::no_constructor);
	baseConnection["getStatus"] = &BaseConnection::getStatus;
	baseConnection["isConnected"] = &BaseConnection::isConnected;
	baseConnection["getHost"] = &BaseConnection::getHost;
	baseConnection["getPort"] = &BaseConnection::getPort;
	baseConnection["getFactories"] = sol::resolve<Atlas::Objects::Factories&()>(&BaseConnection::getFactories);
	baseConnection["INVALID_STATUS"] = sol::var(BaseConnection::INVALID_STATUS);
	baseConnection["NEGOTIATE"] = sol::var(BaseConnection::NEGOTIATE);
	baseConnection["CONNECTING"] = sol::var(BaseConnection::CONNECTING);
	baseConnection["CONNECTED"] = sol::var(BaseConnection::CONNECTED);
	baseConnection["CONNECTED"] = sol::var(BaseConnection::CONNECTED);
	baseConnection["DISCONNECTED"] = sol::var(BaseConnection::DISCONNECTED);
	baseConnection["DISCONNECTING"] = sol::var(BaseConnection::DISCONNECTING);
	baseConnection["QUERY_GET"] = sol::var(BaseConnection::QUERY_GET);
	baseConnection["Connected"] = LuaConnector::make_property(&BaseConnection::Connected);
	baseConnection["Disconnected"] = LuaConnector::make_property(&BaseConnection::Disconnected);

	auto connection = Eris.new_usertype<Connection>("Connection", sol::no_constructor,
													sol::base_classes, sol::bases<BaseConnection>());
	connection["getTypeService"] = &Connection::getTypeService;
	connection["getServerInfo"] = &Connection::getServerInfo;
	connection["GotServerInfo"] = LuaConnector::make_property(&Connection::GotServerInfo);
	connection["Failure"] = LuaConnector::make_property(&Connection::Failure);

	auto entity = Eris.new_usertype<Entity>("Entity", sol::no_constructor);

	entity["numContained"] = &Entity::numContained;
	entity["getContained"] = &Entity::getContained;
	entity["valueOfProperty"] = &Entity::valueOfProperty;
	entity["hasProperty"] = &Entity::hasProperty;
	entity["getId"] = &Entity::getId;
	entity["getName"] = &Entity::getName;
	entity["getStamp"] = &Entity::getStamp;
	entity["getType"] = &Entity::getType;
	entity["getLocation"] = &Entity::getLocation;
	entity["getPosition"] = &Entity::getPosition;
	entity["getProperties"] = &Entity::getProperties;
	entity["isMoving"] = &Entity::isMoving;
	entity["getBBox"] = &Entity::getBBox;
	entity["hasBBox"] = &Entity::hasBBox;
	entity["hasChild"] = &Entity::hasChild;
	entity["extractEntityId"] = [](const Atlas::Message::Element& element) {
		auto extractedId = Eris::Entity::extractEntityId(element);
		if (extractedId) {
			return *extractedId;
		} else {
			return std::string("");
		}
	};
	entity["ChildAdded"] = LuaConnector::make_property(&Entity::ChildAdded);
	entity["ChildRemoved"] = LuaConnector::make_property(&Entity::ChildRemoved);
	entity["LocationChanged"] = LuaConnector::make_property(&Entity::LocationChanged);
	entity["Changed"] = LuaConnector::make_property(&Entity::Changed);
	entity["Moved"] = LuaConnector::make_property(&Entity::Moved);
	entity["Say"] = LuaConnector::make_property(&Entity::Say);
	entity["Emote"] = LuaConnector::make_property(&Entity::Emote);
	entity["BeingDeleted"] = LuaConnector::make_property(&Entity::BeingDeleted);

	entity["TaskAdded"] = LuaConnector::make_property(&Entity::TaskAdded);
	entity["TaskRemoved"] = LuaConnector::make_property(&Entity::TaskRemoved);
	entity["getTasksSize"] = [](Entity* self) { return (int) self->getTasks().size(); };
	entity["getTask"] =
			[](Entity* self, const std::string& id) -> Eris::Task* {
				auto I = self->getTasks().find(id);
				if (I != self->getTasks().end()) {
					return I->second.get();
				}
				return nullptr;
			};
	entity["getTaskFirst"] =
			[](Entity* self) -> Eris::Task* {
				if (!self->getTasks().empty()) {
					return self->getTasks().begin()->second.get();
				}
				return nullptr;
			};
	entity["getTaskIdFirst"] =
			[](Entity* self) {
				if (!self->getTasks().empty()) {
					return self->getTasks().begin()->first;
				}
				return std::string("");
			};

	auto meta = Eris.new_usertype<Meta>("Meta");
	meta["getGameServerCount"] = &Meta::getGameServerCount;
	meta["getInfoForServer"] = &Meta::getInfoForServer;
	meta["queryServerByIndex"] = &Meta::queryServerByIndex;
	meta["refresh"] = &Meta::refresh;
	meta["cancel"] = &Meta::cancel;
	meta["ReceivedServerInfo"] = LuaConnector::make_property(&Meta::ReceivedServerInfo);
	meta["Failure"] = LuaConnector::make_property(&Meta::Failure);


	auto serverInfo = Eris.new_usertype<ServerInfo>("ServerInfo");
	serverInfo["host"] = &ServerInfo::host;
	serverInfo["status"] = &ServerInfo::status;
	serverInfo["name"] = &ServerInfo::name;
	serverInfo["ruleset"] = &ServerInfo::ruleset;
	serverInfo["server"] = &ServerInfo::server;
	serverInfo["protocol_version"] = &ServerInfo::protocol_version;
	serverInfo["clients"] = &ServerInfo::clients;
	serverInfo["ping"] = &ServerInfo::ping;
	serverInfo["entities"] = &ServerInfo::entities;
	serverInfo["ruleset"] = &ServerInfo::ruleset;
	serverInfo["uptime"] = &ServerInfo::uptime;
	serverInfo["version"] = &ServerInfo::version;
	serverInfo["buildDate"] = &ServerInfo::buildDate;
	serverInfo["INVALID"] = sol::var(Eris::ServerInfo::Status::INVALID);
	serverInfo["QUERYING"] = sol::var(Eris::ServerInfo::Status::QUERYING);
	serverInfo["VALID"] = sol::var(Eris::ServerInfo::Status::VALID);
	serverInfo["TIMEOUT"] = sol::var(Eris::ServerInfo::Status::TIMEOUT);

	auto task = Eris.new_usertype<Task>("Task", sol::no_constructor);
	task["name"] = &Task::name;
	task["progress"] = &Task::progress;
	task["progressRate"] = &Task::progressRate;
	task["isComplete"] = &Task::isComplete;
	task["Completed"] = LuaConnector::make_property(&Task::Completed);
	task["Progressed"] = LuaConnector::make_property(&Task::Progressed);
	task["UsagesChanged"] = LuaConnector::make_property(&Task::UsagesChanged);
	task["usagesSize"] = [](Task* self) { return static_cast<int>(self->getUsages().size()); };
	task["usageName"] = [](Task* self, int index) {
		if (index < static_cast<int>(self->getUsages().size())) {
			return self->getUsages()[index].name;
		}
		return std::string("");
	};


	Eris.new_usertype<TypeInfo>("TypeInfo", sol::no_constructor,
								"isBound", &TypeInfo::isBound,
								"hasUnresolvedChildren", &TypeInfo::hasUnresolvedChildren,
								"resolveChildren", &TypeInfo::resolveChildren,
								"getName", &TypeInfo::getName,
								"getChildren", &TypeInfo::getChildren
	);

	auto typeService = Eris.new_usertype<TypeService>("TypeService", sol::no_constructor);

	typeService["getTypeByName"] = &TypeService::getTypeByName;
	typeService["getTypeForAtlas"] = &TypeService::getTypeForAtlas;
	typeService["findTypeByName"] = &TypeService::findTypeByName;
	typeService["BoundType"] = LuaConnector::make_property(&TypeService::BoundType);
	typeService["BadType"] = LuaConnector::make_property(&TypeService::BadType);

	Eris.new_usertype<Usage>("Usage", sol::no_constructor,
							 "name", &Usage::name,
							 "constraint", &Usage::constraint,
							 "description", &Usage::description
	);

	auto view = Eris.new_usertype<View>("View", sol::no_constructor);
	view["getAvatar"] = &View::getAvatar;
	view["lookQueueSize"] = &View::lookQueueSize;
	view["getTopLevel"] = &View::getTopLevel;
	view["getEntity"] = [](View* self, std::string eid) { return static_cast<Ember::EmberEntity*>(self->getEntity(eid)); };
	view["TopLevelEntityChanged"] = LuaConnector::make_property(&View::TopLevelEntityChanged);


}