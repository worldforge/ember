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
#include <Eris/Entity.h>
#include <Eris/Task.h>
#include <Eris/TypeInfo.h>
#include <Atlas/Objects/SmartPtr.h>
#include <Atlas/Objects/Root.h>

using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<Entity>(sol::table& space) {
	auto entity = space.new_usertype<Entity>("Entity", sol::no_constructor);

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

}