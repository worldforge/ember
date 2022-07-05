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
#include "components/ogre/widgets/ContainerView.h"
#include "components/ogre/widgets/EntityIcon.h"
#include "components/ogre/widgets/EntityIconManager.h"
#include "components/ogre/widgets/icons/IconManager.h"
#include "domain/EmberEntity.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<ContainerView>(sol::table& space) {
	auto containerView = space.new_usertype<ContainerView>("ContainerView",
														 sol::constructors<ContainerView(Ember::OgreView::Gui::EntityIconManager&,
																						 Ember::OgreView::Gui::Icons::IconManager&,
																						 CEGUI::Tooltip&,
																						 CEGUI::Window&,
																						 int)>());
	containerView["showEntityContents"] = &ContainerView::showEntityContents;
	containerView["getEntityIcon"] = &ContainerView::getEntityIcon;
	containerView["addEntityIcon"] = &ContainerView::addEntityIcon;
	containerView["EventEntityPicked"] = LuaConnector::make_property(&ContainerView::EventEntityPicked);
	containerView["EventIconAdded"] = LuaConnector::make_property(&ContainerView::EventIconAdded);
	containerView["EventIconDropped"] = LuaConnector::make_property(&ContainerView::EventIconDropped);

}