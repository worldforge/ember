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
#include "LuaFunctor.h"

using namespace CEGUI;
template <>
void registerLua<ButtonBase>(sol::table& space) {
	auto buttonBase = space.new_usertype<ButtonBase>("ButtonBase", sol::no_constructor,
													 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	buttonBase["isHovering"] = &ButtonBase::isHovering;
	buttonBase["isPushed"] = &ButtonBase::isPushed;
	buttonBase["setPushedState"] = &ButtonBase::setPushedState;

	space.new_usertype<PushButton>("PushButton",
								   sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	auto toggleButton = space.new_usertype<ToggleButton>("ToggleButton",
														 sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	toggleButton["isSelected"] = &ToggleButton::isSelected;
	toggleButton["setSelected"] = &ToggleButton::setSelected;

	auto radioButton = space.new_usertype<RadioButton>("RadioButton",
													   sol::base_classes, sol::bases<ToggleButton, ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	radioButton["getGroupID"] = &RadioButton::getGroupID;
	radioButton["setGroupID"] = &RadioButton::setGroupID;
	radioButton["getSelectedButtonInGroup"] = &RadioButton::getSelectedButtonInGroup;
}