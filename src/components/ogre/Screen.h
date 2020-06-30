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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "components/ogre/OgreIncludes.h"

#include "framework/ConsoleCommandWrapper.h"

#include <OgrePrerequisites.h>
#include <OgreCommon.h>
#include <OgreRenderTarget.h>

#include <sigc++/trackable.h>

namespace Ember {
namespace OgreView {
namespace Camera {
class Recorder;
}

/**
 * @author Erik Ogenvik
 * @brief Handles the main screen.
 *
 */
class Screen : public ConsoleObject, public virtual sigc::trackable {
public:
	/**
	 * @brief Ctor.
	 * @param window The main render window.
	 */
	explicit Screen(Ogre::RenderWindow& window);

	/**
	 * @brief Dtor.
	 */
	~Screen() override;

	void runCommand(const std::string& command, const std::string& args) override;

	/**
	 * @brief Toggles between wireframe and solid render mode.
	 */
	void toggleRenderMode();

	/**
	 * @brief Takes a screen shot and writes it to disk.
	 */
	void takeScreenshot();

	/**
	 * @brief Get frame stats, compiled from all render targets.
	 *
	 * This will take all compositors into account.
	 * @return Frame stats.
	 */
	const Ogre::RenderTarget::FrameStats& getFrameStats();

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
	std::unique_ptr<Camera::Recorder> mRecorder;

	/**
	 * @brief The current polygon mode used in render targets.
	 */
	Ogre::PolygonMode mPolygonMode;

	/**
	 * @brief Keeps track of the combined frame stats from the various render targets.
	 */
	Ogre::RenderTarget::FrameStats mFrameStats;

	/**
	 * @brief Takes and saves a screenshot to disk.
	 * @return The file name of the new screenshot.
	 */
	std::string _takeScreenshot();

};

}
}
#endif /* SCREEN_H_ */
