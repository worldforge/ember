/*
 * Copyright (C) 2012 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef EMBEROGREWINDOWPROVIDER_H
#define EMBEROGREWINDOWPROVIDER_H

#include "services/input/IWindowProvider.h"

#include <OgreWindowEventUtilities.h>

#include <string>

namespace Ember
{
namespace OgreView
{

/**
 * @brief Provides a Window interface for the Input class.
 */
class OgreWindowProvider: public IWindowProvider, public Ogre::WindowEventListener
{
public:

	OgreWindowProvider(Ogre::RenderWindow& window);

	~OgreWindowProvider();

	/**
	 * @brief Returns whether the window is visible.
	 */
	bool isWindowVisible();

	/**
	 * @brief Returns the window handle as string.
	 */
	std::string getWindowHandle();

	/**
	 * @brief Allows to get the window size.
	 */
	void getWindowSize(unsigned int& width, unsigned int& height);

	/**
	 * @brief Processes input for the window.
	 */
	void processInput();

	/**
	 * @brief Ogre will call this automatically, when the window is resized.
	 * 
	 * This will update the window size for mouse calculations.
	 */
	void windowResized(Ogre::RenderWindow* rw);

	/**
	 * @brief Ogre will call this automatically, when the close (X) button is pressed by the user.
	 * 
	 * Used to choose whether you want from Ogre to destroy the window automatically.
	 * By default its true, but we don't want Ogre to destroy it, since Ember will ask for confirmation first.
	 * 
	 * @return Whether you want from Ogre to destroy the window.
	 */
	bool windowClosing(Ogre::RenderWindow* rw);

	/**
	 * @brief Ogre will call this automatically before the window is destroyed.
	 * 
	 * This is the last chance to detach OIS.
	 */
	void windowClosed(Ogre::RenderWindow* rw);

	/**
	 * @brief Ogre will call this automatically when the window changes focus.
	 * 
	 * This is usually requested by the user with alt+tab key combination.
	 */
	void windowFocusChange(Ogre::RenderWindow* rw);
private:

	/**
	 * @brief The window used for input events.
	 */
	Ogre::RenderWindow& mWindow;
};

}
}

#endif
