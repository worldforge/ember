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
#include "RegisterLua.h"
#include "components/ogre/widgets/QuickHelp.h"
#include "components/ogre/widgets/QuickHelpCursor.h"

using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

template <>
void registerLua<QuickHelp>(sol::table& space) {
	auto quickHelp = space.new_usertype<QuickHelp>("QuickHelp", sol::no_constructor);
	quickHelp["getSingleton"] = &QuickHelp::getSingleton;
	quickHelp["updateText"] = &QuickHelp::updateText;
	quickHelp["showWidget"] = &QuickHelp::showWidget;
	quickHelp["EventToggleWidgetVisibility"] = LuaConnector::make_property(&QuickHelp::EventToggleWidgetVisibility);

	auto quickHelpCursor = space.new_usertype<QuickHelpCursor>("QuickHelpCursor");
	quickHelpCursor["nextMessage"] = &QuickHelpCursor::nextMessage;
	quickHelpCursor["previousMessage"] = &QuickHelpCursor::previousMessage;
	quickHelpCursor["getSize"] = &QuickHelpCursor::getSize;
	quickHelpCursor["getCursorLocation"] = &QuickHelpCursor::getCursorLocation;
	quickHelpCursor["EventUpdateText"] = LuaConnector::make_property(&QuickHelpCursor::EventUpdateText);

}