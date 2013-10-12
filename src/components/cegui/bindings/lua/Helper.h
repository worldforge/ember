/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef EMBER_CEGUI_HELPER_H_
#define EMBER_CEGUI_HELPER_H_

#include <CEGUIWindow.h>

namespace Ember
{
namespace Cegui
{

/**
 * @brief A helper class for interacting with CEGUI.
 *
 * This contains functions which makes it easier to work with CEGUI in Lua.
 */
class Helper
{
public:
	/**
	 * Helper method for Lua to allow calling CEGUI::Widget::notifyScreenAreaChanges,
	 * since that's method isn't exposed through the standard Lua bindings.
	 * @param window
	 * @param recursive
	 */
	static void notifyScreenAreaChanged(CEGUI::Window* window, bool recursive);
};
}
}

inline void Ember::Cegui::Helper::notifyScreenAreaChanged(CEGUI::Window* window, bool recursive)
{
	window->notifyScreenAreaChanged(recursive);
}

#endif /* EMBER_CEGUI_HELPER_H_ */
