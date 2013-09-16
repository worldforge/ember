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

#include <CEGUI/Window.h>
#include <CEGUI/ImageManager.h>

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

	/**
	 * Workaround for an issue in the CEGUI 0.8.2 lua bindings where Element::getChildCount returns size_t, which tolua++ can't understand
	 * FIXME: remove once CEGUI has fixed the issue with the lua binding
	 * @param window
	 * @return
	 */
	static unsigned int Window_getChildCount(CEGUI::Window* window);

	/**
	 * Expose the ImageManager::getSingleton method.
	 * FIXME: remove once CEGUI has fixed the issue with the lua binding
	 * @return
	 */
	static CEGUI::ImageManager& ImageManager_getSingleton();

	/**
	 * Expose the Window::getChildRecursive(string) method
	 *
	 * FIXME: remove once CEGUI has fixed the issue with the lua binding
	 *
	 * @param window
	 * @param name
	 * @return
	 */
	static CEGUI::Window* Window_getChildRecursive(CEGUI::Window* window, const std::string& name);

	/**
	 * Expose the Window::removeChild(Element) method
	 *
	 * FIXME: remove once CEGUI has fixed the issue with the lua binding
	 *
	 * @param window
	 * @param name
	 * @return
	 */
	static void Window_removeChild(CEGUI::Window* window, CEGUI::Window* child);
};
}
}

inline void Ember::Cegui::Helper::notifyScreenAreaChanged(CEGUI::Window* window, bool recursive)
{
	window->notifyScreenAreaChanged(recursive);
}

inline unsigned int Ember::Cegui::Helper::Window_getChildCount(CEGUI::Window* window)
{
	return window->getChildCount();
}

inline CEGUI::ImageManager& Ember::Cegui::Helper::ImageManager_getSingleton()
{
	return CEGUI::ImageManager::getSingleton();
}

inline CEGUI::Window* Ember::Cegui::Helper::Window_getChildRecursive(CEGUI::Window* window, const std::string& name)
{
	return window->getChildRecursive(name);
}

inline void Ember::Cegui::Helper::Window_removeChild(CEGUI::Window* window, CEGUI::Window* child)
{
	window->removeChild(child);
}



#endif /* EMBER_CEGUI_HELPER_H_ */
