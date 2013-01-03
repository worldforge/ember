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

#ifndef SCREEN_H_
#define SCREEN_H_

#include "components/ogre/OgreIncludes.h"

#include "framework/ConsoleObject.h"

#include <OgreCommon.h>

#include <sigc++/trackable.h>

namespace Ember
{
namespace OgreView
{
namespace Camera
{
class Recorder;
}

/**
 * @author Erik Ogenvik
 * @brief Handles the main screen.
 *
 */
class Screen: public ConsoleObject, public virtual sigc::trackable
{
public:
	/**
	 * @brief Ctor.
	 * @param window The main render window.
	 */
	Screen(Ogre::RenderWindow& window);

	/**
	 * @brief Dtor.
	 */
	virtual ~Screen();

	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 * @brief Toggles between wireframe and solid render mode.
	 */
	void toggleRenderMode();

	/**
	 * @brief Takes a screen shot and writes it to disk.
	 */
	void takeScreenshot();

	const ConsoleCommandWrapper ToggleRendermode;
	const ConsoleCommandWrapper Screenshot;
	const ConsoleCommandWrapper Record;

protected:

	/*'
	 * @brief The main render window.
	 */
	Ogre::RenderWindow& mWindow;

	/**
	 * @brief A recorder which can record frames to disk.
	 */
	Camera::Recorder* mRecorder;

	/**
	 * @brief The current polygon mode used in render targets.
	 */
	Ogre::PolygonMode mPolygonMode;

	/**
	 * @brief Takes and saves a screenshot to disk.
	 * @return The file name of the new screenshot.
	 */
	const std::string _takeScreenshot();

};

}
}
#endif /* SCREEN_H_ */
