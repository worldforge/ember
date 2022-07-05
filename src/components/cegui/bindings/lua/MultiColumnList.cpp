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
void registerLua<MultiColumnList>(sol::table& space) {
	space.new_usertype<MCLGridRef>("MCLGridRef",
								   sol::constructors<MCLGridRef(unsigned int, unsigned int)>(),
								   "row", &MCLGridRef::row,
								   "column", &MCLGridRef::column
	);


	auto multiColumnList = space.new_usertype<MultiColumnList>("MultiColumnList",
															   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	multiColumnList["RowSingle"] = sol::var(MultiColumnList::RowSingle);
	multiColumnList["CellSingle"] = sol::var(MultiColumnList::CellSingle);
	multiColumnList["CellMultiple"] = sol::var(MultiColumnList::CellMultiple);
	multiColumnList["NominatedColumnSingle"] = sol::var(MultiColumnList::NominatedColumnSingle);
	multiColumnList["NominatedColumnMultiple"] = sol::var(MultiColumnList::NominatedColumnMultiple);
	multiColumnList["ColumnSingle"] = sol::var(MultiColumnList::ColumnSingle);
	multiColumnList["ColumnMultiple"] = sol::var(MultiColumnList::ColumnMultiple);
	multiColumnList["NominatedRowSingle"] = sol::var(MultiColumnList::NominatedRowSingle);
	multiColumnList["NominatedRowMultiple"] = sol::var(MultiColumnList::NominatedRowMultiple);
	multiColumnList["isUserSortControlEnabled"] = &MultiColumnList::isUserSortControlEnabled;
	multiColumnList["isUserColumnSizingEnabled"] = &MultiColumnList::isUserColumnSizingEnabled;
	multiColumnList["isUserColumnDraggingEnabled"] = &MultiColumnList::isUserColumnDraggingEnabled;
	multiColumnList["getColumnCount"] = &MultiColumnList::getColumnCount;
	multiColumnList["getRowCount"] = &MultiColumnList::getRowCount;
	multiColumnList["getSortColumn"] = &MultiColumnList::getSortColumn;
	multiColumnList["getColumnWithID"] = &MultiColumnList::getColumnWithID;
	multiColumnList["getColumnWithHeaderText"] = string_setter(&MultiColumnList::getColumnWithHeaderText);
	multiColumnList["getTotalColumnHeadersWidth"] = &MultiColumnList::getTotalColumnHeadersWidth;
	multiColumnList["getColumnHeaderWidth"] = &MultiColumnList::getColumnHeaderWidth;
	multiColumnList["getSortDirection"] = &MultiColumnList::getSortDirection;
	multiColumnList["getHeaderSegmentForColumn"] = &MultiColumnList::getHeaderSegmentForColumn;
	multiColumnList["getItemRowIndex"] = &MultiColumnList::getItemRowIndex;
	multiColumnList["getItemColumnIndex"] = &MultiColumnList::getItemColumnIndex;
	multiColumnList["getItemGridReference"] = &MultiColumnList::getItemGridReference;
	multiColumnList["getItemAtGridReference"] = &MultiColumnList::getItemAtGridReference;
	multiColumnList["isListboxItemInColumn"] = &MultiColumnList::isListboxItemInColumn;
	multiColumnList["isListboxItemInRow"] = &MultiColumnList::isListboxItemInRow;
	multiColumnList["isListboxItemInList"] = &MultiColumnList::isListboxItemInList;
//									multiColumnList["findColumnItemWithText"]= string_setter(&MultiColumnList::findColumnItemWithText);
//									multiColumnList["findRowItemWithText"]= string_setter(&MultiColumnList::findRowItemWithText);
//									multiColumnList["findListItemWithText"]= string_setter(&MultiColumnList::findListItemWithText);
	multiColumnList["getFirstSelectedItem"] = &MultiColumnList::getFirstSelectedItem;
	multiColumnList["getNextSelected"] = &MultiColumnList::getNextSelected;
	multiColumnList["getSelectedCount"] = &MultiColumnList::getSelectedCount;
	multiColumnList["isItemSelected"] = &MultiColumnList::isItemSelected;
	multiColumnList["getNominatedSelectionColumnID"] = &MultiColumnList::getNominatedSelectionColumnID;
	multiColumnList["getNominatedSelectionColumn"] = &MultiColumnList::getNominatedSelectionColumn;
	multiColumnList["getNominatedSelectionRow"] = &MultiColumnList::getNominatedSelectionRow;
	multiColumnList["getSelectionMode"] = &MultiColumnList::getSelectionMode;
	multiColumnList["isVertScrollbarAlwaysShown"] = &MultiColumnList::isVertScrollbarAlwaysShown;
	multiColumnList["isHorzScrollbarAlwaysShown"] = &MultiColumnList::isHorzScrollbarAlwaysShown;
	multiColumnList["getColumnID"] = &MultiColumnList::getColumnID;
	multiColumnList["getRowID"] = &MultiColumnList::getRowID;
	multiColumnList["getRowWithID"] = &MultiColumnList::getRowWithID;
	multiColumnList["getListRenderArea"] = &MultiColumnList::getListRenderArea;
	multiColumnList["getVertScrollbar"] = &MultiColumnList::getVertScrollbar;
	multiColumnList["getHorzScrollbar"] = &MultiColumnList::getHorzScrollbar;
	multiColumnList["getListHeader"] = &MultiColumnList::getListHeader;
	multiColumnList["getTotalRowsHeight"] = &MultiColumnList::getTotalRowsHeight;
	multiColumnList["getWidestColumnItemWidth"] = &MultiColumnList::getWidestColumnItemWidth;
	multiColumnList["getHighestRowItemHeight"] = &MultiColumnList::getHighestRowItemHeight;
	multiColumnList["resetList"] = &MultiColumnList::resetList;
	multiColumnList["addColumn"] = [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width) { self->addColumn(text, col_id, width); };
	multiColumnList["insertColumn"] = [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width, unsigned int position) { self->insertColumn(text, col_id, width, position); };
	multiColumnList["removeColumn"] = &MultiColumnList::removeColumn;
	multiColumnList["removeColumnWithID"] = &MultiColumnList::removeColumnWithID;
	multiColumnList["moveColumn"] = &MultiColumnList::moveColumn;
	multiColumnList["moveColumnWithID"] = &MultiColumnList::moveColumnWithID;
	multiColumnList["addRow"] = sol::overload([](MultiColumnList* self) { return self->addRow(); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id) { return self->addRow(item, col_id); });
	multiColumnList["insertRow"] = sol::overload([](MultiColumnList* self, unsigned int row_idx) { return self->insertRow(row_idx); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id, unsigned int row_idx) { return self->insertRow(item, col_id, row_idx); });
	multiColumnList["removeRow"] = &MultiColumnList::removeRow;
	multiColumnList["setItem"] = sol::overload(sol::resolve<ListboxItem*, const MCLGridRef&>(&MultiColumnList::setItem), sol::resolve<ListboxItem*, unsigned int, unsigned int>(&MultiColumnList::setItem));
	multiColumnList["setSelectionMode"] = &MultiColumnList::setSelectionMode;
	multiColumnList["setNominatedSelectionColumnID"] = &MultiColumnList::setNominatedSelectionColumnID;
	multiColumnList["setNominatedSelectionColumn"] = &MultiColumnList::setNominatedSelectionColumn;
	multiColumnList["setNominatedSelectionRow"] = &MultiColumnList::setNominatedSelectionRow;
	multiColumnList["setSortDirection"] = &MultiColumnList::setSortDirection;
	multiColumnList["setSortColumn"] = &MultiColumnList::setSortColumn;
	multiColumnList["setSortColumnByID"] = &MultiColumnList::setSortColumnByID;
	multiColumnList["setShowVertScrollbar"] = &MultiColumnList::setShowVertScrollbar;
	multiColumnList["setShowHorzScrollbar"] = &MultiColumnList::setShowHorzScrollbar;
	multiColumnList["clearAllSelections"] = &MultiColumnList::clearAllSelections;
	multiColumnList["setItemSelectState"] = sol::overload(sol::resolve<ListboxItem*, bool>(&MultiColumnList::setItemSelectState), sol::resolve<const MCLGridRef&, bool>(&MultiColumnList::setItemSelectState));
	multiColumnList["handleUpdatedItemData"] = &MultiColumnList::handleUpdatedItemData;
	multiColumnList["setColumnHeaderWidth"] = &MultiColumnList::setColumnHeaderWidth;
	multiColumnList["setUserSortControlEnabled"] = &MultiColumnList::setUserSortControlEnabled;
	multiColumnList["setUserColumnSizingEnabled"] = &MultiColumnList::setUserColumnSizingEnabled;
	multiColumnList["setUserColumnDraggingEnabled"] = &MultiColumnList::setUserColumnDraggingEnabled;
	multiColumnList["autoSizeColumnHeader"] = &MultiColumnList::autoSizeColumnHeader;
	multiColumnList["setRowID"] = &MultiColumnList::setRowID;
	multiColumnList["ensureRowIsVisible"] = &MultiColumnList::ensureRowIsVisible;
	multiColumnList["ensureColumnIsVisible"] = &MultiColumnList::ensureColumnIsVisible;
	multiColumnList["ensureItemRowIsVisible"] = &MultiColumnList::ensureItemRowIsVisible;
	multiColumnList["ensureItemColumnIsVisible"] = &MultiColumnList::ensureItemColumnIsVisible;
	multiColumnList["ensureItemIsVisible"] = sol::overload(sol::resolve<const ListboxItem*>(&MultiColumnList::ensureItemIsVisible), sol::resolve<const MCLGridRef&>(&MultiColumnList::ensureItemIsVisible));
	multiColumnList["setAutoSizeColumnUsesHeader"] = &MultiColumnList::setAutoSizeColumnUsesHeader;
	multiColumnList["getAutoSizeColumnUsesHeader"] = &MultiColumnList::getAutoSizeColumnUsesHeader;
}