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
void registerLua<Spinner>(sol::table& space) {
	auto spinner = space.new_usertype<Spinner>("Spinner",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	spinner["FloatingPoint"] = sol::var(Spinner::FloatingPoint);
	spinner["Integer"] = sol::var(Spinner::Integer);
	spinner["Hexadecimal"] = sol::var(Spinner::Hexadecimal);
	spinner["Octal"] = sol::var(Spinner::Octal);
	spinner["getCurrentValue"] = &Spinner::getCurrentValue;
	spinner["getStepSize"] = &Spinner::getStepSize;
	spinner["getMaximumValue"] = &Spinner::getMaximumValue;
	spinner["getMinimumValue"] = &Spinner::getMinimumValue;
	spinner["getTextInputMode"] = &Spinner::getTextInputMode;
	spinner["setCurrentValue"] = &Spinner::setCurrentValue;
	spinner["setStepSize"] = &Spinner::setStepSize;
	spinner["setMaximumValue"] = &Spinner::setMaximumValue;
	spinner["setMinimumValue"] = &Spinner::setMinimumValue;
	spinner["setTextInputMode"] = &Spinner::setTextInputMode;
}