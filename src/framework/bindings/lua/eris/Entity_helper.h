/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_ENTITY_HELPER_H
#define EMBER_ENTITY_HELPER_H

#include <Eris/Entity.h>

namespace {
int _Entity_getTasksSize(Eris::Entity* entity) {
	return entity->getTasks().size();
}

Eris::Task* _Entity_getTask(Eris::Entity* entity, std::string name) {
	auto I = entity->getTasks().find(name);
	if (I != entity->getTasks().end()) {
		return I->second;
	}
	return nullptr;
}

Eris::Task* _Entity_getTaskFirst(Eris::Entity* entity) {
	if (!entity->getTasks().empty()) {
		return entity->getTasks().begin()->second;
	}
	return nullptr;
}

std::string _Entity_getTaskIdFirst(Eris::Entity* entity) {
	if (!entity->getTasks().empty()) {
		return entity->getTasks().begin()->first;
	}
	return "";
}

bool _Entity_extractEntityId(const Atlas::Message::Element& element, std::string& id) {
	auto extractedId = Eris::Entity::extractEntityId(element);
	if (extractedId) {
		id = *extractedId;
		return true;
	}
	return false;
}

}

#endif //EMBER_ENTITY_HELPER_H
