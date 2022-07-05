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
void registerLua<LayoutContainer>(sol::table& space) {
	auto layoutContainer = space.new_usertype<LayoutContainer>("LayoutContainer", sol::no_constructor,
															   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	layoutContainer["markNeedsLayouting"] = &LayoutContainer::markNeedsLayouting;
	layoutContainer["needsLayouting"] = &LayoutContainer::needsLayouting;
	layoutContainer["layout"] = &LayoutContainer::layout;
	layoutContainer["layoutIfNecessary"] = &LayoutContainer::layoutIfNecessary;

	space.new_usertype<HorizontalLayoutContainer>("HorizontalLayoutContainer",
												  sol::base_classes, sol::bases<SequentialLayoutContainer, LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	auto gridLayoutContainer = space.new_usertype<GridLayoutContainer>("GridLayoutContainer",
																	   sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
//											"AP_Disabled", GridLayoutContainer::AP_Disabled,
//											"AP_LeftToRight", GridLayoutContainer::AP_LeftToRight,
//											"AP_TopToBottom", GridLayoutContainer::AP_TopToBottom,
	gridLayoutContainer["setGridDimensions"] = &GridLayoutContainer::setGridDimensions;
	gridLayoutContainer["getGridWidth"] = &GridLayoutContainer::getGridWidth;
	gridLayoutContainer["getGridHeight"] = &GridLayoutContainer::getGridHeight;
	gridLayoutContainer["setAutoPositioning"] = &GridLayoutContainer::setAutoPositioning;
	gridLayoutContainer["getAutoPositioning"] = &GridLayoutContainer::getAutoPositioning;
	gridLayoutContainer["setNextAutoPositioningIdx"] = &GridLayoutContainer::setNextAutoPositioningIdx;
	gridLayoutContainer["getNextAutoPositioningIdx"] = &GridLayoutContainer::getNextAutoPositioningIdx;
	gridLayoutContainer["autoPositioningSkipCells"] = &GridLayoutContainer::autoPositioningSkipCells;
	gridLayoutContainer["addChildToPosition"] = &GridLayoutContainer::addChildToPosition;
	gridLayoutContainer["getChildAtPosition"] = &GridLayoutContainer::getChildAtPosition;
	gridLayoutContainer["removeChildFromPosition"] = &GridLayoutContainer::removeChildFromPosition;
	gridLayoutContainer["swapChildPositions"] = sol::resolve<size_t, size_t, size_t, size_t>(&GridLayoutContainer::swapChildPositions);
	gridLayoutContainer["swapChildren"] = sol::resolve<Window*, Window*>(&GridLayoutContainer::swapChildren);
	gridLayoutContainer["moveChildToPosition"] = sol::resolve<Window*, size_t, size_t>(&GridLayoutContainer::moveChildToPosition);


	auto sequentialLayoutContainer = space.new_usertype<SequentialLayoutContainer>("SequentialLayoutContainer", sol::no_constructor,
																				   sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	sequentialLayoutContainer["getPositionOfChild"] = sol::overload(
			sol::resolve<size_t(Window*) const>(&SequentialLayoutContainer::getPositionOfChild),
			[](SequentialLayoutContainer* self, const char* wnd) { return self->getPositionOfChild(wnd); });
	sequentialLayoutContainer["getChildAtPosition"] = &SequentialLayoutContainer::getChildAtPosition;
	sequentialLayoutContainer["swapChildPositions"] = &SequentialLayoutContainer::swapChildPositions;
	sequentialLayoutContainer["swapChildren"] = sol::overload(sol::resolve<void(Window*, Window*)>(&SequentialLayoutContainer::swapChildren),
															  [](SequentialLayoutContainer* self, const char* wnd1, Window* wnd2) { self->swapChildren(wnd1, wnd2); },
															  [](SequentialLayoutContainer* self, const char* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); },
															  [](SequentialLayoutContainer* self, Window* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); });
	sequentialLayoutContainer["moveChildToPosition"] = sol::overload(sol::resolve<void(Window*, size_t)>(&SequentialLayoutContainer::moveChildToPosition),
																	 [](SequentialLayoutContainer* self, const char* wnd, size_t position) { self->moveChildToPosition(wnd, position); });
	sequentialLayoutContainer["addChildToPosition"] = &SequentialLayoutContainer::addChildToPosition;
	sequentialLayoutContainer["removeChildFromPosition"] = &SequentialLayoutContainer::removeChildFromPosition;

	space.new_usertype<VerticalLayoutContainer>("VerticalLayoutContainer",
												sol::base_classes, sol::bases<SequentialLayoutContainer, LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
}