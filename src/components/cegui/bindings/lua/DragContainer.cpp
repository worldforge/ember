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
void registerLua<DragContainer>(sol::table& space) {
	auto dragContainer = space.new_usertype<DragContainer>("DragContainer",
														   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	dragContainer["isDraggingEnabled"] = &DragContainer::isDraggingEnabled;
	dragContainer["setDraggingEnabled"] = &DragContainer::setDraggingEnabled;
	dragContainer["isBeingDragged"] = &DragContainer::isBeingDragged;
	dragContainer["getPixelDragThreshold"] = &DragContainer::getPixelDragThreshold;
	dragContainer["setPixelDragThreshold"] = &DragContainer::setPixelDragThreshold;
	dragContainer["getDragAlpha"] = &DragContainer::getDragAlpha;
	dragContainer["setDragAlpha"] = &DragContainer::setDragAlpha;
	dragContainer["getDragCursorImage"] = &DragContainer::getDragCursorImage;
	dragContainer["setDragCursorImage"] = sol::overload([](DragContainer* self, const char* name) { self->setDragCursorImage(name); }, sol::resolve<const Image*>(&DragContainer::setDragCursorImage));
	dragContainer["getCurrentDropTarget"] = &DragContainer::getCurrentDropTarget;
	dragContainer["isStickyModeEnabled"] = &DragContainer::isStickyModeEnabled;
	dragContainer["setStickyModeEnabled"] = &DragContainer::setStickyModeEnabled;
	dragContainer["pickUp"] = &DragContainer::pickUp;
	dragContainer["setFixedDragOffset"] = &DragContainer::setFixedDragOffset;
	dragContainer["getFixedDragOffset"] = &DragContainer::getFixedDragOffset;
	dragContainer["setUsingFixedDragOffset"] = &DragContainer::setUsingFixedDragOffset;
	dragContainer["isUsingFixedDragOffset"] = &DragContainer::isUsingFixedDragOffset;
}