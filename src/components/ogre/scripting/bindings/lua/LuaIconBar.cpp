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
#include "components/ogre/widgets/IconBase.h"
#include "components/ogre/widgets/IconBar.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<IconBar>(sol::table& space) {
	auto iconBar = space.new_usertype<IconBar>("IconBar",
											 sol::constructors<IconBar(const std::string&)>());
	iconBar["addIcon"] = &IconBar::addIcon;
	iconBar["removeIcon"] = &IconBar::removeIcon;
	iconBar["getWindow"] = &IconBar::getWindow;
	iconBar["setIconPadding"] = &IconBar::setIconPadding;
	iconBar["getAbsoluteHeight"] = &IconBar::getAbsoluteHeight;
	iconBar["getAbsoluteWidth"] = &IconBar::getAbsoluteWidth;

	auto iconBase = space.new_usertype<IconBase>("IconBase",
											   sol::constructors<IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*),
													   IconBase(const std::string&, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, const CEGUI::Image*, CEGUI::USize)>());
	iconBase["getContainer"] = &IconBase::getContainer;
	iconBase["getButton"] = &IconBase::getButton;
	iconBase["setForeground"] = &IconBase::setForeground;
	iconBase["loadImageFromImageset"] = &IconBase::loadImageFromImageset;

}