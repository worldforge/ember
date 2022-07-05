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
void registerLua<ItemListBase>(sol::table& space) {
	auto itemEntry = space.new_usertype<ItemEntry>("ItemEntry",
												   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemEntry["getItemPixelSize"] = &ItemEntry::getItemPixelSize;
	itemEntry["getOwnerList"] = &ItemEntry::getOwnerList;
	itemEntry["isSelected"] = &ItemEntry::isSelected;
	itemEntry["isSelectable"] = &ItemEntry::isSelectable;
	itemEntry["setSelected"] = &ItemEntry::setSelected;
	itemEntry["select"] = &ItemEntry::select;
	itemEntry["deselect"] = &ItemEntry::deselect;
	itemEntry["setSelectable"] = &ItemEntry::setSelectable;


	auto itemListBase = space.new_usertype<ItemListBase>("ItemListBase", sol::no_constructor,
														 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemListBase["Ascending"] = sol::var(ItemListBase::Ascending);
	itemListBase["Descending"] = sol::var(ItemListBase::Descending);
	itemListBase["UserSort"] = sol::var(ItemListBase::UserSort);
	itemListBase["getItemCount"] = &ItemListBase::getItemCount;
	itemListBase["getItemFromIndex"] = &ItemListBase::getItemFromIndex;
	itemListBase["getItemIndex"] = &ItemListBase::getItemIndex;
	itemListBase["findItemWithText"] = [](ItemListBase* self, const char* text, const ItemEntry* start_item) { return self->findItemWithText(text, start_item); };
	itemListBase["isItemInList"] = &ItemListBase::isItemInList;
	itemListBase["isAutoResizeEnabled"] = &ItemListBase::isAutoResizeEnabled;
	itemListBase["isSortEnabled"] = &ItemListBase::isSortEnabled;
	itemListBase["getSortMode"] = &ItemListBase::getSortMode;
	itemListBase["getItemRenderArea"] = &ItemListBase::getItemRenderArea;
	itemListBase["getContentPane"] = &ItemListBase::getContentPane;
	itemListBase["resetList"] = &ItemListBase::resetList;
	itemListBase["addItem"] = &ItemListBase::addItem;
	itemListBase["insertItem"] = &ItemListBase::insertItem;
	itemListBase["removeItem"] = &ItemListBase::removeItem;
	itemListBase["handleUpdatedItemData"] = &ItemListBase::handleUpdatedItemData;
	itemListBase["setAutoResizeEnabled"] = &ItemListBase::setAutoResizeEnabled;
	itemListBase["sizeToContent"] = &ItemListBase::sizeToContent;
	itemListBase["setSortEnabled"] = &ItemListBase::setSortEnabled;
	itemListBase["setSortMode"] = &ItemListBase::setSortMode;
	itemListBase["sortList"] = &ItemListBase::sortList;

	auto itemListBox = space.new_usertype<ItemListbox>("ItemListbox",
													   sol::base_classes, sol::bases<ScrolledItemListBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemListBox["getSelectedCount"] = &ItemListbox::getSelectedCount;
	itemListBox["getLastSelectedItem"] = &ItemListbox::getLastSelectedItem;
	itemListBox["getFirstSelectedItem"] = &ItemListbox::getFirstSelectedItem;
	itemListBox["getNextSelectedItem"] = &ItemListbox::getNextSelectedItem;
	itemListBox["getNextSelectedItemAfter"] = &ItemListbox::getNextSelectedItemAfter;
	itemListBox["isMultiSelectEnabled"] = &ItemListbox::isMultiSelectEnabled;
	itemListBox["isItemSelected"] = &ItemListbox::isItemSelected;
	itemListBox["setMultiSelectEnabled"] = &ItemListbox::setMultiSelectEnabled;
	itemListBox["clearAllSelections"] = &ItemListbox::clearAllSelections;
	itemListBox["selectRange"] = &ItemListbox::selectRange;
	itemListBox["selectAllItems"] = &ItemListbox::selectAllItems;

	auto scrolledItemListBase = space.new_usertype<ScrolledItemListBase>("ScrolledItemListBase", sol::no_constructor,
																		 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrolledItemListBase["isVertScrollbarAlwaysShown"] = &ScrolledItemListBase::isVertScrollbarAlwaysShown;
	scrolledItemListBase["isHorzScrollbarAlwaysShown"] = &ScrolledItemListBase::isHorzScrollbarAlwaysShown;
	scrolledItemListBase["getVertScrollbar"] = &ScrolledItemListBase::getVertScrollbar;
	scrolledItemListBase["getHorzScrollbar"] = &ScrolledItemListBase::getHorzScrollbar;
	scrolledItemListBase["setShowVertScrollbar"] = &ScrolledItemListBase::setShowVertScrollbar;
	scrolledItemListBase["setShowHorzScrollbar"] = &ScrolledItemListBase::setShowHorzScrollbar;
}