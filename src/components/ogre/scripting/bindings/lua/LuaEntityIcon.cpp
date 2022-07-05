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
#include "components/ogre/widgets/EntityIcon.h"
#include "components/ogre/widgets/EntityIconSlot.h"
#include "components/ogre/widgets/EntityIconManager.h"
#include "components/ogre/widgets/EntityIconDragDropPreview.h"
#include "components/ogre/widgets/icons/Icon.h"
#include "components/ogre/World.h"
#include "domain/EmberEntity.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;
using namespace Ember;

template <>
void registerLua<EntityIcon>(sol::table& space) {
	auto entityIconDragDropTarget = space.new_usertype<EntityIconDragDropTarget>("EntityIconDragDropTarget",
																			   sol::constructors<EntityIconDragDropTarget(CEGUI::Window*)>());
	entityIconDragDropTarget["EventIconEntered"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconEntered);
	entityIconDragDropTarget["EventIconLeaves"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconLeaves);
	entityIconDragDropTarget["EventIconDropped"] = LuaConnector::make_property(&EntityIconDragDropTarget::EventIconDropped);
	//TODO: check if we still need to == operator with sol
	auto entityIcon = space.new_usertype<EntityIcon>("EntityIcon", sol::no_constructor,
												   sol::base_classes, sol::bases<EntityIconDragDropTarget>());
	entityIcon["getImage"] = &EntityIcon::getImage;
	entityIcon["getDragContainer"] = &EntityIcon::getDragContainer;
	entityIcon["getIcon"] = &EntityIcon::getIcon;
	entityIcon["setSlot"] = &EntityIcon::setSlot;
	entityIcon["getSlot"] = &EntityIcon::getSlot;
	entityIcon["setTooltipText"] = &EntityIcon::setTooltipText;
	entityIcon["getEntity"] = &EntityIcon::getEntity;
	entityIcon["setTag"] = &EntityIcon::setTag;
	entityIcon["getTag"] = &EntityIcon::getTag;

	auto entityIconDragDropPreview = space.new_usertype<EntityIconDragDropPreview>("EntityIconDragDropPreview",
																				 sol::constructors<EntityIconDragDropPreview(Ember::OgreView::World&)>());
	entityIconDragDropPreview["createPreview"] = &EntityIconDragDropPreview::createPreview;
	entityIconDragDropPreview["cleanupCreation"] = &EntityIconDragDropPreview::cleanupCreation;
	entityIconDragDropPreview["getDropPosition"] = &EntityIconDragDropPreview::getDropPosition;
	entityIconDragDropPreview["getDropOrientation"] = &EntityIconDragDropPreview::getDropOrientation;
	entityIconDragDropPreview["EventEntityFinalized"] = LuaConnector::make_property(&EntityIconDragDropPreview::EventEntityFinalized);

	auto entityIconManager = space.new_usertype<EntityIconManager>("EntityIconManager", sol::no_constructor);
	entityIconManager["createSlot"] = sol::overload(&EntityIconManager::createSlot,
													[](EntityIconManager* self) { return self->createSlot(); });
	entityIconManager["destroySlot"] = &EntityIconManager::destroySlot;
	entityIconManager["createIcon"] = sol::overload(&EntityIconManager::createIcon,
													[](EntityIconManager* self, Icons::Icon* icon, EmberEntity* entity) { return self->createIcon(icon, entity); });
	entityIconManager["destroyIcon"] = &EntityIconManager::destroyIcon;
	entityIconManager["EventIconDragStart"] = LuaConnector::make_property(&EntityIconManager::EventIconDragStart);
	entityIconManager["EventIconDragStop"] = LuaConnector::make_property(&EntityIconManager::EventIconDragStop);

	auto entityIconSlot = space.new_usertype<EntityIconSlot>("EntityIconSlot", sol::no_constructor,
														   sol::base_classes, sol::bases<EntityIconDragDropTarget>());
	entityIconSlot["addEntityIcon"] = &EntityIconSlot::addEntityIcon;
	entityIconSlot["removeEntityIcon"] = &EntityIconSlot::removeEntityIcon;
	entityIconSlot["getEntityIcon"] = &EntityIconSlot::getEntityIcon;
	entityIconSlot["getWindow"] = &EntityIconSlot::getWindow;
	entityIconSlot["notifyIconDraggedOff"] = &EntityIconSlot::notifyIconDraggedOff;
	entityIconSlot["EventIconDraggedOff"] = LuaConnector::make_property(&EntityIconSlot::EventIconDraggedOff);

}