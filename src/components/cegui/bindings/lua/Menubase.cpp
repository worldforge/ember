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
void registerLua<MenuBase>(sol::table& space) {
	space.new_usertype<Menubar>("Menubar",
								sol::base_classes, sol::bases<MenuBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	auto menuBase = space.new_usertype<MenuBase>("MenuBase", sol::no_constructor,
												 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	menuBase["getItemSpacing"] = &MenuBase::getItemSpacing;
	menuBase["isMultiplePopupsAllowed"] = &MenuBase::isMultiplePopupsAllowed;
	menuBase["getPopupMenuItem"] = &MenuBase::getPopupMenuItem;
	menuBase["setItemSpacing"] = &MenuBase::setItemSpacing;
	menuBase["changePopupMenuItem"] = &MenuBase::changePopupMenuItem;
	menuBase["setAllowMultiplePopups"] = &MenuBase::setAllowMultiplePopups;
	menuBase["getAutoCloseNestedPopups"] = &MenuBase::getAutoCloseNestedPopups;
	menuBase["getPopupMenuItem"] = &MenuBase::getPopupMenuItem;

	auto menuItem = space.new_usertype<MenuItem>("MenuItem",
												 sol::base_classes, sol::bases<ItemEntry, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	menuItem["isHovering"] = &MenuItem::isHovering;
	menuItem["isPushed"] = &MenuItem::isPushed;
	menuItem["getPopupMenu"] = &MenuItem::getPopupMenu;
	menuItem["setPopupMenu"] = &MenuItem::setPopupMenu;
	menuItem["openPopupMenu"] = sol::overload(&MenuItem::openPopupMenu, [](MenuItem* self) { self->openPopupMenu(); });
	menuItem["closePopupMenu"] = sol::overload(&MenuItem::closePopupMenu, [](MenuItem* self) { self->closePopupMenu(); });
	menuItem["togglePopupMenu"] = &MenuItem::togglePopupMenu;
	menuItem["hasAutoPopup"] = &MenuItem::hasAutoPopup;
	menuItem["getAutoPopupTimeout"] = &MenuItem::getAutoPopupTimeout;
	menuItem["setAutoPopupTimeout"] = &MenuItem::setAutoPopupTimeout;
	menuItem["getPopupOffset"] = &MenuItem::getPopupOffset;
	menuItem["setPopupOffset"] = &MenuItem::setPopupOffset;

	auto popupMenu = space.new_usertype<PopupMenu>("PopupMenu",
												   sol::base_classes, sol::bases<MenuBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	popupMenu["getFadeInTime"] = &PopupMenu::getFadeInTime;
	popupMenu["getFadeOutTime"] = &PopupMenu::getFadeOutTime;
	popupMenu["setFadeInTime"] = &PopupMenu::setFadeInTime;
	popupMenu["setFadeOutTime"] = &PopupMenu::setFadeOutTime;
	popupMenu["openPopupMenu"] = sol::overload(&PopupMenu::openPopupMenu, [](PopupMenu* self) { self->openPopupMenu(); });
	popupMenu["closePopupMenu"] = sol::overload(&PopupMenu::closePopupMenu, [](PopupMenu* self) { self->closePopupMenu(); });
}