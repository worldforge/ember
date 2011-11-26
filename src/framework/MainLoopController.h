/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifndef MAINLOOPCONTROLLER_H_
#define MAINLOOPCONTROLLER_H_

#include <sigc++/signal.h>
#include "Singleton.h"

namespace Ember
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 *
 * @brief Singleton instance representing the main loop.
 *
 * This is mainly used to request that the application quits.
 */
class MainLoopController: public Singleton<MainLoopController>
{
public:

	/**
	 * @brief Ctor.
	 * @param shouldQuit A reference to a boolean which represents whether the application should quit.
	 */
	MainLoopController(bool& shouldQuit);

	/**
	 * @brief Return true if application has received an "exit" command else false.
	 *
	 * @return true if "shouldquit" else false
	 */
	bool shouldQuit();

	/**
	 * @brief Causes the application to quit.
	 * This will instantly shut down the application, in contrast to requestQuit which will try to show a confirmation dialog to the user.
	 */
	void quit();

	/**
	 * @brief Call this to "soft quit" the app. This means that an signal will be emitted, which hopefully will be taken care of by some widget, which will show a confirmation window, asking the user if he/she wants to quit.
	 * However, if there is no widget etc. handling the request, the application will instantly quit.
	 */
	void requestQuit();

	/**
	 * @brief Emitted when the use wants to quit the game. Preferrebly the GUI should show some kind of confirmation window.
	 */
	sigc::signal<void, bool&> EventRequestQuit;

private:

	/**
	 * @brief Set this to true when the application should quit.
	 */
	bool& mShouldQuit;
};

}
#endif /* MAINLOOPCONTROLLER_H_ */
