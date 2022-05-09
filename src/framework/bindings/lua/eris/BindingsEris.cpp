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

	lua.new_usertype<Account>(
			"Eris::Account",
			"getConnection", &Account::getConnection);
	lua.new_usertype<Avatar>(
			"Eris::Avatar",
			"getId", &Avatar::getId,
			"getEntityId", &Avatar::getEntityId,
			"getEntity", &Avatar::getEntity,
			"getView", &Avatar::getView,
			"getConnection", &Avatar::getConnection,
			"wield", &Avatar::wield,
			"place", [](Avatar* self,
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
			},
			"take", [](Avatar* self,
					   const Eris::Entity* entity,
					   int amount = 1) {
				self->place(entity, self->getEntity(), {}, {}, {}, amount);
			},
			"ContainerOpened", sol::property([](Avatar* self) {
				return Connector::makeConnector<RefValueAdapter<Eris::Entity>>(self->ContainerOpened);
			}),
			"ContainerClosed", sol::property([](Avatar* self) {
				return Connector::makeConnector<RefValueAdapter<Eris::Entity>>(self->ContainerClosed);
			})
	);
	lua.new_usertype<Connection>("Eris::Connection",
								 "Disconnected", sol::property([](Connection* self) {
				return std::make_shared<Connector>(self->Disconnected);
			}));

	lua.new_usertype<Entity>("Eris::Entity",
							 "numContained", &Entity::numContained,
							 "getContained", &Entity::getContained,
							 "valueOfProperty", &Entity::valueOfProperty,
							 "hasProperty", &Entity::hasProperty,
							 "getId", &Entity::getId,
							 "getName", &Entity::getName,
							 "getStamp", &Entity::getStamp,
							 "getType", &Entity::getType,
							 "getLocation", &Entity::getLocation,
							 "getPosition", &Entity::getPosition,
							 "getProperties", &Entity::getProperties,
							 "isMoving", &Entity::isMoving,
							 "getBBox", &Entity::getBBox,
							 "hasBBox", &Entity::hasBBox,
							 "hasChild", &Entity::hasChild,
							 "extractEntityId", [](const Atlas::Message::Element& element) {
				auto extractedId = Eris::Entity::extractEntityId(element);
				if (extractedId) {
					return *extractedId;
				} else {
					return std::string("");
				}
			},
							 "ChildAdded",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<PtrValueAdapter<Eris::Entity>>(self->ChildAdded);
							 }),
							 "ChildRemoved",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<PtrValueAdapter<Eris::Entity>>(self->ChildRemoved);
							 }),
							 "LocationChanged",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<PtrValueAdapter<Eris::Entity>>(self->LocationChanged);
							 }),
							 "Changed",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<RefValueAdapter<const std::set<std::string>>>(self->Changed);
							 }),
							 "Moved",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector(self->Moved);
							 }),
							 "Say",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<RefValueAdapter<const Atlas::Objects::Root>>(self->Say);
							 }),
							 "Emote",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<StringValueAdapter>(self->Emote);
							 }),
							 "BeingDeleted",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector(self->BeingDeleted);
							 }),

							 "TaskAdded",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<StringValueAdapter, PtrValueAdapter<Eris::Task>>(self->TaskAdded);
							 }),
							 "TaskRemoved",
							 sol::property([](Entity* self) {
								 return Connector::makeConnector<StringValueAdapter, PtrValueAdapter<Eris::Task>>(self->TaskRemoved);
							 }),
							 "getTasksSize", [](Entity* self) { return (int) self->getTasks().size(); },
							 "getTask",
							 [](Entity* self, const std::string& id) -> Eris::Task* {
								 auto I = self->getTasks().find(id);
								 if (I != self->getTasks().end()) {
									 return I->second.get();
								 }
								 return nullptr;
							 },
							 "getTaskFirst",
							 [](Entity* self) -> Eris::Task* {
								 if (!self->getTasks().empty()) {
									 return self->getTasks().begin()->second.get();
								 }
								 return nullptr;
							 },
							 "getTaskIdFirst",
							 [](Entity* self) {
								 if (!self->getTasks().empty()) {
									 return self->getTasks().begin()->first;
								 }
								 return std::string("");
							 }
	);

	lua.new_usertype<Meta>("Eris::Meta",
						   "getGameServerCount", &Meta::getGameServerCount,
						   "getInfoForServer", &Meta::getInfoForServer,
						   "queryServerByIndex", &Meta::queryServerByIndex,
						   "refresh", &Meta::refresh,
						   "cancel", &Meta::cancel,
						   "ReceivedServerInfo",
						   sol::property([](Meta* self) {
							   return Connector::makeConnector<RefValueAdapter<const ServerInfo>>(self->ReceivedServerInfo);
						   }),
						   "Failure",
						   sol::property([](Meta* self) {
							   return Connector::makeConnector<StringValueAdapter>(self->Failure);
						   })

	);

	lua.new_enum("Eris::ServerInfo::Status",
				 "INVALID", Eris::ServerInfo::Status::INVALID,
				 "QUERYING", Eris::ServerInfo::Status::QUERYING,
				 "VALID", Eris::ServerInfo::Status::VALID,
				 "TIMEOUT", Eris::ServerInfo::Status::TIMEOUT
	);

	lua.new_usertype<ServerInfo>("Eris::ServerInfo",
								 "host", &ServerInfo::host,
								 "status", &ServerInfo::status,
								 "name", &ServerInfo::name,
								 "ruleset", &ServerInfo::ruleset,
								 "server", &ServerInfo::server,
								 "protocol_version", &ServerInfo::protocol_version,
								 "clients", &ServerInfo::clients,
								 "ping", &ServerInfo::ping,
								 "entities", &ServerInfo::entities,
								 "ruleset", &ServerInfo::ruleset,
								 "uptime", &ServerInfo::uptime,
								 "version", &ServerInfo::version,
								 "buildDate", &ServerInfo::buildDate
	);

	lua.new_usertype<Task>("Eris::Task",
						   "name", &Task::name,
						   "progress", &Task::progress,
						   "progressRate", &Task::progressRate,
						   "isComplete", &Task::isComplete,
						   "Completed",
						   sol::property([](Task* self) {
							   return Connector::makeConnector(self->Completed);
						   }),
						   "Progressed",
						   sol::property([](Task* self) {
							   return Connector::makeConnector(self->Progressed);
						   }),
						   "UsagesChanged",
						   sol::property([](Task* self) {
							   return Connector::makeConnector(self->Completed);
						   }),
						   "usagesSize", [](Task* self) { return static_cast<int>(self->getUsages().size()); },
						   "usageName",
						   [](Task* self, int index) {
							   if (index < static_cast<int>(self->getUsages().size())) {
								   return self->getUsages()[index].name;
							   }
							   return std::string("");
						   }
	);

	lua.new_usertype<TypeInfo>("Eris::TypeInfo",
							   "isBound", &TypeInfo::isBound,
							   "hasUnresolvedChildren", &TypeInfo::hasUnresolvedChildren,
							   "resolveChildren", &TypeInfo::resolveChildren,
							   "getName", &TypeInfo::getName,
							   "getChildren", &TypeInfo::getChildren
	);

	lua.new_usertype<TypeService>("Eris::TypeService",
								  "getTypeByName", &TypeService::getTypeByName,
								  "getTypeForAtlas", &TypeService::getTypeForAtlas,
								  "findTypeByName", &TypeService::findTypeByName,
								  "BoundType",
								  sol::property([](TypeService* self) {
									  return Connector::makeConnector<PtrValueAdapter<TypeInfo>>(self->BoundType);
								  }),
								  "BadType",
								  sol::property([](TypeService* self) {
									  return Connector::makeConnector<PtrValueAdapter<TypeInfo>>(self->BadType);
								  })
	);

	lua.new_usertype<Usage>("Eris::Usage",
							"name", &Usage::name,
							"constraint", &Usage::constraint,
							"description", &Usage::description
	);

	lua.new_usertype<View>("Eris::View",
						   "getAvatar", &View::getAvatar,
						   "lookQueueSize", &View::lookQueueSize,
						   "getTopLevel", &View::getTopLevel,
						   "getEntity", [](View* self, std::string eid) { return static_cast<Ember::EmberEntity*>(self->getEntity(eid)); },
						   "TopLevelEntityChanged",
						   sol::property([](View* self) {
							   return Connector::makeConnector(self->TopLevelEntityChanged);
						   })
	);


}