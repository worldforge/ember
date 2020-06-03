/*
 Copyright (C) 2019 Erik Ogenvik

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

#ifndef EMBER_CEGUIUTILS_H
#define EMBER_CEGUIUTILS_H

#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <memory>

namespace Ember {
namespace OgreView {

namespace Gui {

template<typename T>
struct WindowDestroyer {
	void operator()(T* window) {
		CEGUI::WindowManager::getSingleton().destroyWindow(window);
		CEGUI::WindowManager::getSingleton().cleanDeadPool(); //Need to make sure that it's really destroyed.
	}
};

/**
 * A specialization of std::unique_ptr which will destroy the contained window through the CEGUI::WindowManager upon destruction,
 * as well as clean the CEGUI dead pool.
 */
template<typename T>
using UniqueWindowPtr= std::unique_ptr<T, WindowDestroyer<T>>;

}
}
}
#endif //EMBER_CEGUIUTILS_H
