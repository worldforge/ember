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
void registerLua<ScrollablePane>(sol::table& space) {
	auto scrollablePane = space.new_usertype<ScrollablePane>("ScrollablePane",
															 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrollablePane["getContentPane"] = &ScrollablePane::getContentPane;
	scrollablePane["isVertScrollbarAlwaysShown"] = &ScrollablePane::isVertScrollbarAlwaysShown;
	scrollablePane["setShowVertScrollbar"] = &ScrollablePane::setShowVertScrollbar;
	scrollablePane["isHorzScrollbarAlwaysShown"] = &ScrollablePane::isHorzScrollbarAlwaysShown;
	scrollablePane["setShowHorzScrollbar"] = &ScrollablePane::setShowHorzScrollbar;
	scrollablePane["isContentPaneAutoSized"] = &ScrollablePane::isContentPaneAutoSized;
	scrollablePane["setContentPaneAutoSized"] = &ScrollablePane::setContentPaneAutoSized;
	scrollablePane["getContentPaneArea"] = &ScrollablePane::getContentPaneArea;
	scrollablePane["setContentPaneArea"] = &ScrollablePane::setContentPaneArea;
	scrollablePane["getHorizontalStepSize"] = &ScrollablePane::getHorizontalStepSize;
	scrollablePane["setHorizontalStepSize"] = &ScrollablePane::setHorizontalStepSize;
	scrollablePane["getHorizontalOverlapSize"] = &ScrollablePane::getHorizontalOverlapSize;
	scrollablePane["setHorizontalOverlapSize"] = &ScrollablePane::setHorizontalOverlapSize;
	scrollablePane["getHorizontalScrollPosition"] = &ScrollablePane::getHorizontalScrollPosition;
	scrollablePane["setHorizontalScrollPosition"] = &ScrollablePane::setHorizontalScrollPosition;
	scrollablePane["getVerticalStepSize"] = &ScrollablePane::getVerticalStepSize;
	scrollablePane["setVerticalStepSize"] = &ScrollablePane::setVerticalStepSize;
	scrollablePane["getVerticalOverlapSize"] = &ScrollablePane::getVerticalOverlapSize;
	scrollablePane["setVerticalOverlapSize"] = &ScrollablePane::setVerticalOverlapSize;
	scrollablePane["getVerticalScrollPosition"] = &ScrollablePane::getVerticalScrollPosition;
	scrollablePane["setVerticalScrollPosition"] = &ScrollablePane::setVerticalScrollPosition;
	scrollablePane["getViewableArea"] = &ScrollablePane::getViewableArea;
	scrollablePane["getVertScrollbar"] = &ScrollablePane::getVertScrollbar;
	scrollablePane["getHorzScrollbar"] = &ScrollablePane::getHorzScrollbar;
}