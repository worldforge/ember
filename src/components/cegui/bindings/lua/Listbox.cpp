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
void registerLua<Listbox>(sol::table& space) {
	auto listbox = space.new_usertype<Listbox>("Listbox",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	listbox["getItemCount"] = &Listbox::getItemCount;
	listbox["getSelectedCount"] = &Listbox::getSelectedCount;
	listbox["getFirstSelectedItem"] = &Listbox::getFirstSelectedItem;
	listbox["getNextSelected"] = &Listbox::getNextSelected;
	listbox["getListboxItemFromIndex"] = &Listbox::getListboxItemFromIndex;
	listbox["getItemIndex"] = &Listbox::getItemIndex;
	listbox["findItemWithText"] = [](Listbox* self, const char* text, const ListboxItem* start_item) { return self->findItemWithText(text, start_item); };
	listbox["isSortEnabled"] = &Listbox::isSortEnabled;
	listbox["isMultiselectEnabled"] = &Listbox::isMultiselectEnabled;
	listbox["isItemSelected"] = &Listbox::isItemSelected;
	listbox["isListboxItemInList"] = &Listbox::isListboxItemInList;
	listbox["isVertScrollbarAlwaysShown"] = &Listbox::isVertScrollbarAlwaysShown;
	listbox["isHorzScrollbarAlwaysShown"] = &Listbox::isHorzScrollbarAlwaysShown;
	listbox["resetList"] = &Listbox::resetList;
	listbox["addItem"] = &Listbox::addItem;
	listbox["insertItem"] = &Listbox::insertItem;
	listbox["removeItem"] = &Listbox::removeItem;
	listbox["clearAllSelections"] = &Listbox::clearAllSelections;
	listbox["setSortingEnabled"] = &Listbox::setSortingEnabled;
	listbox["setMultiselectEnabled"] = &Listbox::setMultiselectEnabled;
	listbox["setShowVertScrollbar"] = &Listbox::setShowVertScrollbar;
	listbox["setShowHorzScrollbar"] = &Listbox::setShowHorzScrollbar;
	listbox["setItemSelectState"] = sol::overload(sol::resolve<ListboxItem*, bool>(&Listbox::setItemSelectState),
												  sol::resolve<size_t, bool>(&Listbox::setItemSelectState));
	listbox["handleUpdatedItemData"] = &Listbox::handleUpdatedItemData;
	listbox["ensureItemIsVisible"] = sol::overload(sol::resolve<void(
	const ListboxItem*)>(&Listbox::ensureItemIsVisible),
			sol::resolve<void(size_t)>(&Listbox::ensureItemIsVisible));
	listbox["getListRenderArea"] = &Listbox::getListRenderArea;
	listbox["getVertScrollbar"] = &Listbox::getVertScrollbar;
	listbox["getHorzScrollbar"] = &Listbox::getHorzScrollbar;
	listbox["getWidestItemWidth"] = &Listbox::getWidestItemWidth;
	listbox["getTotalItemsHeight"] = &Listbox::getTotalItemsHeight;
	listbox["getItemAtPoint"] = &Listbox::getItemAtPoint;

	auto listboxItem = space.new_usertype<ListboxItem>("ListboxItem");


	listboxItem["getText"] = string_getter(&ListboxItem::getText);
	listboxItem["getTooltipText"] = string_getter(&ListboxItem::getTooltipText);
	listboxItem["getID"] = &ListboxItem::getID;
	listboxItem["getUserData"] = &ListboxItem::getUserData;
	listboxItem["isSelected"] = &ListboxItem::isSelected;
	listboxItem["isDisabled"] = &ListboxItem::isDisabled;
	listboxItem["isAutoDeleted"] = &ListboxItem::isAutoDeleted;
	listboxItem["getOwnerWindow"] = &ListboxItem::getOwnerWindow;
	listboxItem["getSelectionColours"] = &ListboxItem::getSelectionColours;
	listboxItem["getSelectionBrushImage"] = &ListboxItem::getSelectionBrushImage;
	listboxItem["setText"] = string_setter(&ListboxItem::setText);
	listboxItem["setTooltipText"] = string_setter(&ListboxItem::setTooltipText);
	listboxItem["setID"] = &ListboxItem::setID;
	listboxItem["setUserData"] = &ListboxItem::setUserData;
	listboxItem["setSelected"] = &ListboxItem::setSelected;
	listboxItem["setAutoDeleted"] = &ListboxItem::setAutoDeleted;
	listboxItem["setSelectionColours"] = sol::overload(sol::resolve<void(
	const ColourRect&)>(&ListboxItem::setSelectionColours),
			sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxItem::setSelectionColours),
			sol::resolve<void(Colour)>(&ListboxItem::setSelectionColours));
	listboxItem["setSelectionBrushImage"] = sol::overload(sol::resolve<void(
	const Image*)>(&ListboxItem::setSelectionBrushImage),
			[](ListboxItem* self, const char* name) { self->setSelectionBrushImage(name); });
	listboxItem["getPixelSize"] = &ListboxItem::getPixelSize;

	auto listboxTextItem = space.new_usertype<ListboxTextItem>("ListboxTextItem",
															   sol::base_classes, sol::bases<ListboxItem>()
	);
	listboxTextItem["getFont"] = &ListboxTextItem::getFont;
	listboxTextItem["getTextColours"] = &ListboxTextItem::getTextColours;
	listboxTextItem["setFont"] = sol::overload(sol::resolve<void(Font*)>(&ListboxTextItem::setFont),
											   [](ListboxTextItem* self, const char* font) { self->setFont(font); });
	listboxTextItem["setTextColours"] = sol::overload(sol::resolve<void(
	const ColourRect&)>(&ListboxTextItem::setTextColours),
			sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxTextItem::setTextColours),
			sol::resolve<void(Colour)>(&ListboxTextItem::setTextColours));
	listboxTextItem["setTextParsingEnabled"] = &ListboxTextItem::setTextParsingEnabled;
	listboxTextItem["isTextParsingEnabled"] = &ListboxTextItem::isTextParsingEnabled;
}