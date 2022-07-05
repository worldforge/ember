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
#include "components/ogre/widgets/Widget.h"


using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<Widget>(sol::table& space) {
	auto widget = space.new_usertype<Widget>("Widget", sol::no_constructor);
	widget["show"] = &Widget::show;
	widget["hide"] = &Widget::hide;
	widget["isVisible"] = &Widget::isVisible;
	widget["isActive"] = &Widget::isActive;
	widget["getWindow"] = [](Widget* self, const std::string& name) { return &self->getWindow<CEGUI::Window>(name); };
	widget["enableCloseButton"] = &Widget::enableCloseButton;
	widget["registerConsoleVisibilityToggleCommand"] = &Widget::registerConsoleVisibilityToggleCommand;
	widget["getMainSheet"] = &Widget::getMainSheet;
	widget["getMainWindow"] = &Widget::getMainWindow;
	widget["createWindow"] = sol::resolve<CEGUI::Window*(const std::string&)>(&Widget::createWindow);
	widget["loadMainSheet"] = &Widget::loadMainSheet;
	widget["getPrefix"] = &Widget::getPrefix;
	widget["getIsActiveWindowOpaque"] = &Widget::getIsActiveWindowOpaque;
	widget["setIsActiveWindowOpaque"] = &Widget::setIsActiveWindowOpaque;
	widget["getDefaultScheme"] = &Widget::getDefaultScheme;
	widget["addTabbableWindow"] = &Widget::addTabbableWindow;
	widget["closeTabGroup"] = &Widget::closeTabGroup;
	widget["EventFrameStarted"] = LuaConnector::make_property(&Widget::EventFrameStarted);
	widget["EventFirstTimeShown"] = LuaConnector::make_property(&Widget::EventFirstTimeShown);
}