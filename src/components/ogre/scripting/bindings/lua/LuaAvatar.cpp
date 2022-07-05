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
#include "components/ogre/Avatar.h"
#include <Eris/Avatar.h>


using namespace Ember::OgreView;
using namespace Ember::Lua;

template <>
void registerLua<Avatar>(sol::table& space) {
	auto avatar = space.new_usertype<Avatar>("Avatar", sol::no_constructor);
	avatar["getAvatarSceneNode"] = &Avatar::getAvatarSceneNode;
	avatar["getEmberEntity"] = &Avatar::getEmberEntity;
	avatar["getErisAvatar"] = &Avatar::getErisAvatar;
	avatar["deleteEntity"] = &Avatar::deleteEntity;
	avatar["setMinIntervalOfRotationChanges"] = &Avatar::setMinIntervalOfRotationChanges;
	avatar["isAdmin"] = &Avatar::isAdmin;
	avatar["getClientSideAvatarPosition"] = &Avatar::getClientSideAvatarPosition;
	avatar["getClientSideAvatarOrientation"] = &Avatar::getClientSideAvatarOrientation;
	avatar["useTool"] = sol::overload(
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target,
			   const WFMath::Point<3>& pos,
			   const WFMath::Vector<3>& direction) {
				self->useTool(tool, operation, target, pos, direction);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target,
			   const WFMath::Point<3>& pos) {
				self->useTool(tool, operation, target, pos);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation,
			   const Eris::Entity* target) {
				self->useTool(tool, operation, target);
			},
			[](Avatar* self,
			   const Ember::EmberEntity& tool,
			   const std::string& operation) {
				self->useTool(tool, operation);
			});
	avatar["taskUsage"] = sol::overload(sol::resolve<void(const std::string&, const std::string&)>(&Avatar::taskUsage),
										sol::resolve<void(std::string, const Eris::TaskUsage&)>(&Avatar::taskUsage));

	avatar["EventAddedEntityToInventory"] = LuaConnector::make_property(&Avatar::EventAddedEntityToInventory);
	avatar["EventRemovedEntityFromInventory"] = LuaConnector::make_property(&Avatar::EventRemovedEntityFromInventory);
	avatar["EventAvatarEntityDestroyed"] = LuaConnector::make_property(&Avatar::EventAvatarEntityDestroyed);

}