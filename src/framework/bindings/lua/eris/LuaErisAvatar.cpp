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
#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/Entity.h>
#include <Eris/View.h>


using namespace Eris;
using namespace Ember::Lua;

template <>
void registerLua<Avatar>(sol::table& space) {
	auto avatar = space.new_usertype<Avatar>("Avatar", sol::no_constructor);
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


}