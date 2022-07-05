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
void registerLua<Tree>(sol::table& space) {
	auto tree = space.new_usertype<Tree>("Tree",
										 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tree["getItemCount"] = &Tree::getItemCount;
	tree["getSelectedCount"] = &Tree::getSelectedCount;
	tree["getFirstSelectedItem"] = &Tree::getFirstSelectedItem;
	tree["getLastSelectedItem"] = &Tree::getLastSelectedItem;
	tree["isSortEnabled"] = &Tree::isSortEnabled;
	tree["isMultiselectEnabled"] = &Tree::isMultiselectEnabled;
	tree["isItemTooltipsEnabled"] = &Tree::isItemTooltipsEnabled;
	tree["findFirstItemWithText"] = [](Tree* self, const char* text) { return self->findFirstItemWithText(text); };
	tree["findNextItemWithText"] = [](Tree* self, const char* text, const TreeItem* start_item) { return self->findNextItemWithText(text, start_item); };
	tree["findFirstItemWithID"] = &Tree::findFirstItemWithID;
	tree["findNextItemWithID"] = &Tree::findNextItemWithID;
	tree["isTreeItemInList"] = &Tree::isTreeItemInList;
	tree["resetList"] = &Tree::resetList;
	tree["addItem"] = &Tree::addItem;
	tree["insertItem"] = &Tree::insertItem;
	tree["removeItem"] = &Tree::removeItem;
	tree["clearAllSelections"] = &Tree::clearAllSelections;
	tree["setSortingEnabled"] = &Tree::setSortingEnabled;
	tree["setMultiselectEnabled"] = &Tree::setMultiselectEnabled;
	tree["setItemSelectState"] = sol::overload(sol::resolve<void(TreeItem*, bool)>(&Tree::setItemSelectState),
											   sol::resolve<void(size_t, bool)>(&Tree::setItemSelectState));
	tree["ensureItemIsVisible"] = &Tree::ensureItemIsVisible;
	auto treeItem = space.new_usertype<TreeItem>("TreeItem");

	treeItem["getFont"] = &TreeItem::getFont;
	treeItem["getTextColours"] = &TreeItem::getTextColours;
	treeItem["setFont"] = sol::overload(sol::resolve<void(
	const Font*)>(&TreeItem::setFont),
			[](TreeItem* self, const char* font_name) { self->setFont(font_name); });
	treeItem["setTextColours"] = sol::overload(sol::resolve<void(
	const ColourRect&)>(&TreeItem::setTextColours),
			sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setTextColours),
			sol::resolve<void(Colour)>(&TreeItem::setTextColours));
	treeItem["getText"] = [](TreeItem* self) { return std::string(self->getText().c_str()); };
	treeItem["getTooltipText"] = [](TreeItem* self) { return std::string(self->getTooltipText().c_str()); };
	treeItem["getID"] = &TreeItem::getID;
	treeItem["getUserData"] = &TreeItem::getUserData;
	treeItem["isSelected"] = &TreeItem::isSelected;
	treeItem["isDisabled"] = &TreeItem::isDisabled;
	treeItem["isAutoDeleted"] = &TreeItem::isAutoDeleted;
	treeItem["getOwnerWindow"] = &TreeItem::getOwnerWindow;
	treeItem["getSelectionColours"] = &TreeItem::getSelectionColours;
	treeItem["getSelectionBrushImage"] = &TreeItem::getSelectionBrushImage;
	treeItem["setText"] = string_setter(&TreeItem::setText);
	treeItem["setTooltipText"] = string_setter(&TreeItem::setTooltipText);
	treeItem["setID"] = &TreeItem::setID;
	treeItem["setUserData"] = &TreeItem::setUserData;
	treeItem["setSelected"] = &TreeItem::setSelected;
	treeItem["setDisabled"] = &TreeItem::setDisabled;
	treeItem["setAutoDeleted"] = &TreeItem::setAutoDeleted;
	treeItem["setOwnerWindow"] = &TreeItem::setOwnerWindow;
	treeItem["setSelectionColours"] = sol::overload(sol::resolve<void(
	const ColourRect&)>(&TreeItem::setSelectionColours),
			sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setSelectionColours),
			sol::resolve<void(Colour)>(&TreeItem::setSelectionColours));
	treeItem["setSelectionBrushImage"] = sol::overload(sol::resolve<const Image*>(&TreeItem::setSelectionBrushImage), [](TreeItem* self, const char* name) { self->setSelectionBrushImage(name); });
	treeItem["setButtonLocation"] = &TreeItem::setButtonLocation;
	treeItem["getButtonLocation"] = &TreeItem::getButtonLocation;
	treeItem["getIsOpen"] = &TreeItem::getIsOpen;
	treeItem["toggleIsOpen"] = &TreeItem::toggleIsOpen;
	treeItem["getTreeItemFromIndex"] = &TreeItem::getTreeItemFromIndex;
	treeItem["getItemCount"] = &TreeItem::getItemCount;
	treeItem["addItem"] = &TreeItem::addItem;
	treeItem["removeItem"] = &TreeItem::removeItem;
	treeItem["setIcon"] = &TreeItem::setIcon;

}