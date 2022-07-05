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
void registerLua<TabControl>(sol::table& space) {
	auto tabButton = space.new_usertype<TabButton>("TabButton",
												   sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tabButton["isSelected"] = &TabButton::isSelected;
	tabButton["setSelected"] = &TabButton::setSelected;
	tabButton["setTargetWindow"] = &TabButton::setTargetWindow;
	tabButton["getTargetWindow"] = &TabButton::getTargetWindow;

	auto tabControl = space.new_usertype<TabControl>("TabControl",
													 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tabControl["getTabCount"] = &TabControl::getTabCount;
	tabControl["setSelectedTab"] = sol::overload([](TabControl* self, const char* name) { self->setSelectedTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::setSelectedTab));
	tabControl["setSelectedTabAtIndex"] = &TabControl::setSelectedTabAtIndex;
	tabControl["getTabContentsAtIndex"] = &TabControl::getTabContentsAtIndex;
	tabControl["getTabContents"] = sol::overload([](TabControl* self, const char* name) { return self->getTabContents(name); }, sol::resolve<Window*(unsigned int) const>(&TabControl::getTabContents));
	tabControl["isTabContentsSelected"] = &TabControl::isTabContentsSelected;
	tabControl["getSelectedTabIndex"] = &TabControl::getSelectedTabIndex;
	tabControl["getTabHeight"] = &TabControl::getTabHeight;
	tabControl["getTabTextPadding"] = &TabControl::getTabTextPadding;
	tabControl["setTabHeight"] = &TabControl::setTabHeight;
	tabControl["setTabTextPadding"] = &TabControl::setTabTextPadding;
	tabControl["addTab"] = &TabControl::addTab;
	tabControl["removeTab"] = sol::overload([](TabControl* self, const char* name) { self->removeTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::removeTab));
}