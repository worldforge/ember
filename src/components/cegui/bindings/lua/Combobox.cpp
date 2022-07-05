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
void registerLua<Combobox>(sol::table& space) {
	auto combobox = space.new_usertype<Combobox>("Combobox",
												 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	combobox["getSingleClickEnabled"] = &Combobox::getSingleClickEnabled;
	combobox["isDropDownListVisible"] = &Combobox::isDropDownListVisible;
	combobox["getEditbox"] = &Combobox::getEditbox;
	combobox["getPushButton"] = &Combobox::getPushButton;
	combobox["getDropList"] = &Combobox::getDropList;
	combobox["hasInputFocus"] = &Combobox::hasInputFocus;
	combobox["isReadOnly"] = &Combobox::isReadOnly;
	combobox["getTextMatchState"] = &Combobox::getTextMatchState;
	combobox["getValidationString"] = string_getter(&Combobox::getValidationString);
	combobox["getCaretIndex"] = &Combobox::getCaretIndex;
	combobox["getSelectionStartIndex"] = &Combobox::getSelectionStartIndex;
	combobox["getSelectionEndIndex"] = &Combobox::getSelectionEndIndex;
	combobox["getSelectionLength"] = &Combobox::getSelectionLength;
	combobox["getMaxTextLength"] = &Combobox::getMaxTextLength;
	combobox["activateEditbox"] = &Combobox::activateEditbox;
	combobox["getItemCount"] = &Combobox::getItemCount;
	combobox["getSelectedItem"] = &Combobox::getSelectedItem;
	combobox["getListboxItemFromIndex"] = &Combobox::getListboxItemFromIndex;
	combobox["getItemIndex"] = &Combobox::getItemIndex;
	combobox["isItemSelected"] = &Combobox::isItemSelected;
	combobox["findItemWithText"] = [](Combobox* self, const char* text, const ListboxItem* start_item) { return self->findItemWithText(text, start_item); };
	combobox["isListboxItemInList"] = &Combobox::isListboxItemInList;
	combobox["resetList"] = &Combobox::resetList;
	combobox["addItem"] = &Combobox::addItem;
	combobox["insertItem"] = &Combobox::insertItem;
	combobox["removeItem"] = &Combobox::removeItem;
	combobox["clearAllSelections"] = &Combobox::clearAllSelections;
	combobox["setSortingEnabled"] = &Combobox::setSortingEnabled;
	combobox["setShowVertScrollbar"] = &Combobox::setShowVertScrollbar;
	combobox["setShowHorzScrollbar"] = &Combobox::setShowHorzScrollbar;
	combobox["setItemSelectState"] = sol::overload(sol::resolve<void(ListboxItem*, bool)>(&Combobox::setItemSelectState), sol::resolve<void(size_t, bool)>(&Combobox::setItemSelectState));
	combobox["handleUpdatedListItemData"] = &Combobox::handleUpdatedListItemData;
	combobox["isSortEnabled"] = &Combobox::isSortEnabled;
	combobox["isVertScrollbarAlwaysShown"] = &Combobox::isVertScrollbarAlwaysShown;
	combobox["isHorzScrollbarAlwaysShown"] = &Combobox::isHorzScrollbarAlwaysShown;
	combobox["showDropList"] = &Combobox::showDropList;
	combobox["hideDropList"] = &Combobox::hideDropList;
	combobox["setSingleClickEnabled"] = &Combobox::setSingleClickEnabled;
	combobox["setReadOnly"] = &Combobox::setReadOnly;
	combobox["setValidationString"] = string_setter(&Combobox::setValidationString);
	combobox["setCaretIndex"] = &Combobox::setCaretIndex;
	combobox["setSelection"] = &Combobox::setSelection;
	combobox["setMaxTextLength"] = &Combobox::setMaxTextLength;
	combobox["getAutoSizeListHeightToContent"] = &Combobox::getAutoSizeListHeightToContent;
	combobox["getAutoSizeListWidthToContent"] = &Combobox::getAutoSizeListWidthToContent;
	combobox["setAutoSizeListHeightToContent"] = &Combobox::setAutoSizeListHeightToContent;
	combobox["setAutoSizeListWidthToContent"] = &Combobox::setAutoSizeListWidthToContent;
	combobox["updateAutoSizedDropList"] = &Combobox::updateAutoSizedDropList;

	auto comboDropList = space.new_usertype<ComboDropList>("ComboDropList",
														   sol::base_classes, sol::bases<Listbox, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	comboDropList["setArmed"] = &ComboDropList::setArmed;
	comboDropList["isArmed"] = &ComboDropList::isArmed;
	comboDropList["setAutoArmEnabled"] = &ComboDropList::setAutoArmEnabled;
	comboDropList["isAutoArmEnabled"] = &ComboDropList::isAutoArmEnabled;
	comboDropList["resizeToContent"] = &ComboDropList::resizeToContent;
}