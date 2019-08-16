/*
 Copyright (C) 2013 Sean Ryan <sryan@evercrack.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef WORLDLOADINGSCREEN_H_
#define WORLDLOADINGSCREEN_H_

#include <sigc++/sigc++.h>
#include "CEGUIUtils.h"

namespace CEGUI {
class Window;

class WindowManager;
}

namespace Ember {
namespace OgreView {
namespace Gui {

/**
 @author Sean Ryan
 @brief This class serves as a static transition that obscures the the 3d
 world as it builds behind the scenes to prevent user disorientation.
 */
class WorldLoadingScreen : public virtual sigc::trackable {
public:
	WorldLoadingScreen();

	~WorldLoadingScreen();

	/**
	 * @brief Returns reference to existing CEGUI window in case GUIManager needs it
	 */
	CEGUI::Window& getWindow();

	/**
	 * @brief Adds the internal CEGUI::Window to the main sheet via the WindowManager
	 */
	void showScreen();

	/**
	 * @breif Removes the internal CEGUI::Window from the main sheet via the WindowManager
	 */
	void hideScreen();

protected:

	/**
	 * @brief Internal CEGUI Widget/Window for the loading screen
	 */
	UniqueWindowPtr <CEGUI::Window> mLoadingWindow;

};

}
}
}
#endif /* WORLDWORLDLOADINGSCREEN_H_ */
