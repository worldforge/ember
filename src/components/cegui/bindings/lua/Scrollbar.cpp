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
void registerLua<Scrollbar>(sol::table& space) {
	auto scrollBar = space.new_usertype<Scrollbar>("Scrollbar",
												   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrollBar["getDocumentSize"] = &Scrollbar::getDocumentSize;
	scrollBar["getPageSize"] = &Scrollbar::getPageSize;
	scrollBar["getStepSize"] = &Scrollbar::getStepSize;
	scrollBar["getOverlapSize"] = &Scrollbar::getOverlapSize;
	scrollBar["getScrollPosition"] = &Scrollbar::getScrollPosition;
	scrollBar["getUnitIntervalScrollPosition"] = &Scrollbar::getUnitIntervalScrollPosition;
	scrollBar["isEndLockEnabled"] = &Scrollbar::isEndLockEnabled;
	scrollBar["setDocumentSize"] = &Scrollbar::setDocumentSize;
	scrollBar["setPageSize"] = &Scrollbar::setPageSize;
	scrollBar["setStepSize"] = &Scrollbar::setStepSize;
	scrollBar["setOverlapSize"] = &Scrollbar::setOverlapSize;
	scrollBar["setScrollPosition"] = &Scrollbar::setScrollPosition;
	scrollBar["setUnitIntervalScrollPosition"] = &Scrollbar::setUnitIntervalScrollPosition;
	scrollBar["setEndLockEnabled"] = &Scrollbar::setEndLockEnabled;
	scrollBar["scrollForwardsByStep"] = &Scrollbar::scrollForwardsByStep;
	scrollBar["scrollBackwardsByStep"] = &Scrollbar::scrollBackwardsByStep;
	scrollBar["scrollForwardsByPage"] = &Scrollbar::scrollForwardsByPage;
	scrollBar["scrollBackwardsByPage"] = &Scrollbar::scrollBackwardsByPage;
}