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
void registerLua<FrameWindow>(sol::table& space) {
	auto frameWindow = space.new_usertype<FrameWindow>("FrameWindow",
													   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	frameWindow["isSizingEnabled"] = &FrameWindow::isSizingEnabled;
	frameWindow["isFrameEnabled"] = &FrameWindow::isFrameEnabled;
	frameWindow["isTitleBarEnabled"] = &FrameWindow::isTitleBarEnabled;
	frameWindow["isCloseButtonEnabled"] = &FrameWindow::isCloseButtonEnabled;
	frameWindow["isRollupEnabled"] = &FrameWindow::isRollupEnabled;
	frameWindow["isRolledup"] = &FrameWindow::isRolledup;
	frameWindow["isDragMovingEnabled"] = &FrameWindow::isDragMovingEnabled;
	frameWindow["setSizingEnabled"] = &FrameWindow::setSizingEnabled;
	frameWindow["setFrameEnabled"] = &FrameWindow::setFrameEnabled;
	frameWindow["setTitleBarEnabled"] = &FrameWindow::setTitleBarEnabled;
	frameWindow["setCloseButtonEnabled"] = &FrameWindow::setCloseButtonEnabled;
	frameWindow["setRollupEnabled"] = &FrameWindow::setRollupEnabled;
	frameWindow["setDragMovingEnabled"] = &FrameWindow::setDragMovingEnabled;
	frameWindow["getSizingBorderThickness"] = &FrameWindow::getSizingBorderThickness;
	frameWindow["setSizingBorderThickness"] = &FrameWindow::setSizingBorderThickness;
	frameWindow["toggleRollup"] = &FrameWindow::toggleRollup;
	frameWindow["offsetPixelPosition"] = &FrameWindow::offsetPixelPosition;
}