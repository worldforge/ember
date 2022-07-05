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
void registerLua<MultiLineEditbox>(sol::table& space) {
	auto multiLineEditbox = space.new_usertype<MultiLineEditbox>("MultiLineEditbox",
																 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	multiLineEditbox["hasInputFocus"] = &MultiLineEditbox::hasInputFocus;
	multiLineEditbox["isReadOnly"] = &MultiLineEditbox::isReadOnly;
	multiLineEditbox["getCaretIndex"] = &MultiLineEditbox::getCaretIndex;
	multiLineEditbox["getSelectionStartIndex"] = &MultiLineEditbox::getSelectionStartIndex;
	multiLineEditbox["getSelectionEndIndex"] = &MultiLineEditbox::getSelectionEndIndex;
	multiLineEditbox["getSelectionLength"] = &MultiLineEditbox::getSelectionLength;
	multiLineEditbox["getMaxTextLength"] = &MultiLineEditbox::getMaxTextLength;
	multiLineEditbox["isWordWrapped"] = &MultiLineEditbox::isWordWrapped;
	multiLineEditbox["getVertScrollbar"] = &MultiLineEditbox::getVertScrollbar;
	multiLineEditbox["isVertScrollbarAlwaysShown"] = &MultiLineEditbox::isVertScrollbarAlwaysShown;
	multiLineEditbox["getHorzScrollbar"] = &MultiLineEditbox::getHorzScrollbar;
	multiLineEditbox["getTextRenderArea"] = &MultiLineEditbox::getTextRenderArea;
	multiLineEditbox["getLineNumberFromIndex"] = &MultiLineEditbox::getLineNumberFromIndex;
	multiLineEditbox["setReadOnly"] = &MultiLineEditbox::setReadOnly;
	multiLineEditbox["setCaretIndex"] = &MultiLineEditbox::setCaretIndex;
	multiLineEditbox["setSelection"] = &MultiLineEditbox::setSelection;
	multiLineEditbox["setMaxTextLength"] = &MultiLineEditbox::setMaxTextLength;
	multiLineEditbox["ensureCaretIsVisible"] = &MultiLineEditbox::ensureCaretIsVisible;
	multiLineEditbox["setWordWrapping"] = &MultiLineEditbox::setWordWrapping;
	multiLineEditbox["setShowVertScrollbar"] = &MultiLineEditbox::setShowVertScrollbar;
	multiLineEditbox["setSelectionBrushImage"] = &MultiLineEditbox::setSelectionBrushImage;
	multiLineEditbox["getSelectionBrushImage"] = &MultiLineEditbox::getSelectionBrushImage;
}