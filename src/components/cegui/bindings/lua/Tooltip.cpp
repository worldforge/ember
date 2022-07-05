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
void registerLua<Tooltip>(sol::table& space) {
	auto tooltip = space.new_usertype<Tooltip>("Tooltip",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tooltip["setTargetWindow"] = &Tooltip::setTargetWindow;
	tooltip["getTargetWindow"] = &Tooltip::getTargetWindow;
	tooltip["resetTimer"] = &Tooltip::resetTimer;
	tooltip["setHoverTime"] = &Tooltip::setHoverTime;
	tooltip["getHoverTime"] = &Tooltip::getHoverTime;
	tooltip["setDisplayTime"] = &Tooltip::setDisplayTime;
	tooltip["getDisplayTime"] = &Tooltip::getDisplayTime;
	tooltip["positionSelf"] = &Tooltip::positionSelf;
	tooltip["sizeSelf"] = &Tooltip::sizeSelf;
	tooltip["getTextSize"] = &Tooltip::getTextSize;
}