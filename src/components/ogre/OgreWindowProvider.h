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

#include <OgreRenderWindow.h>

#include <string>

namespace Ember
{
namespace OgreView
{

/**
 * @brief Provides a Window interface for the Input class.
 */
class OgreWindowProvider: public IWindowProvider
{
public:

	explicit OgreWindowProvider(Ogre::RenderWindow& window);

	~OgreWindowProvider() override;

	/**
	 * @brief Returns whether the window is visible.
	 */
	bool isWindowVisible() override;

	/**
	 * @brief Returns the window handle as string.
	 */
	std::string getWindowHandle() override;

	/**
	 * @brief Allows to get the window size.
	 */
	void getWindowSize(unsigned int& width, unsigned int& height) override;

	/**
	 * @brief Processes input for the window.
	 */
	void processInput() override;

private:

	/**
	 * @brief The window used for input events.
	 */
	Ogre::RenderWindow& mWindow;
};

}
}

#endif
