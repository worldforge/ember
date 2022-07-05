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
void registerLua<Editbox>(sol::table& space) {
	auto editBox = space.new_usertype<Editbox>("Editbox",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	editBox["hasInputFocus"] = &Editbox::hasInputFocus;
	editBox["isReadOnly"] = &Editbox::isReadOnly;
	editBox["isTextMasked"] = &Editbox::isTextMasked;
	editBox["getTextMatchState"] = &Editbox::getTextMatchState;
	editBox["getValidationString"] = string_getter(&Editbox::getValidationString);
	editBox["getCaretIndex"] = &Editbox::getCaretIndex;
	editBox["getSelectionStartIndex"] = &Editbox::getSelectionStartIndex;
	editBox["getSelectionEndIndex"] = &Editbox::getSelectionEndIndex;
	editBox["getSelectionLength"] = &Editbox::getSelectionLength;
	editBox["getMaskCodePoint"] = &Editbox::getMaskCodePoint;
	editBox["getMaxTextLength"] = &Editbox::getMaxTextLength;
	editBox["setReadOnly"] = &Editbox::setReadOnly;
	editBox["setTextMasked"] = &Editbox::setTextMasked;
	editBox["setValidationString"] = string_setter(&Editbox::setValidationString);
	editBox["setCaretIndex"] = &Editbox::setCaretIndex;
	editBox["setSelection"] = &Editbox::setSelection;
	editBox["setMaskCodePoint"] = &Editbox::setMaskCodePoint;
	editBox["setMaxTextLength"] = &Editbox::setMaxTextLength;
}