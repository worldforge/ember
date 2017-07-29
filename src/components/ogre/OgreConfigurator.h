/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifndef OGRECONFIGURATOR_H_
#define OGRECONFIGURATOR_H_

#include <OgreConfigOptionMap.h>
#include <string>

namespace CEGUI
{
class EventArgs;
class Window;
}

namespace Ember
{
namespace OgreView
{
class OgreResourceLoader;

/**
 * @brief Shows a simple configuration window, allowing for configuring Ogre.
 *
 * The widget is shown using both Ogre and CEGUI, even though it's intended to actually configure Ogre.
 * This shouldn't however be a problem, since we're actually not using much of the features of Ogre; we're basically just using the 2d functions in order to render CEGUI.
 */
class OgreConfigurator
{
public:

	/**
	 * @brief The result of showing the configuration window.
	 */
	enum Result
	{
		/**
		 * @brief Configuration was carried out successfully.
		 */
		OC_OK,

		/**
		 * @brief Configuration was cancelled.
		 */
		OC_CANCEL,

		/**
		 * @brief User requested to use the more complex Ogre configuration.
		 */
		OC_ADVANCED_OPTIONS
	};

	/**
	 * @brief Ctor.
	 */
	OgreConfigurator();

	/**
	 * @brief Dtor.
	 */
	~OgreConfigurator();

	/**
	 * @brief Shows the configuration window.
	 * @return The result of the configuration.
	 */
	Result configure();

	/**
	 * @brief Gets the name of the chosen render system.
	 * @return The name of the chosen render system.
	 */
	std::string getChosenRenderSystemName() const;

	/**
	 * @brief Gets the final configuration options.
	 * @return The final configuration options.
	 */
	Ogre::ConfigOptionMap getConfigOptions() const;

protected:

	bool buttonOkClicked(const CEGUI::EventArgs& args);
	bool buttonCancelClicked(const CEGUI::EventArgs& args);
	bool buttonAdvancedClicked(const CEGUI::EventArgs& args);
	bool renderSystemChanged(const CEGUI::EventArgs& args);
	void updateResolutionList(Ogre::RenderSystem* renderSystem);

	/**
	 * @brief The last frame time.
	 *
	 * Used for providing correct time to CEGUI.
	 */
	long long mLastFrameTime;

	/**
	 * @brief The result of the configuration.
	 */
	Result mResult;

	/**
	 * @brief Whether to continue within the render loop.
	 */
	bool mContinueInLoop;

	/**
	 * @brief The name of the chosen render system.
	 */
	std::string mChosenRenderSystemName;

	/**
	 * @brief The final configuration options.
	 */
	Ogre::ConfigOptionMap mConfigOptions;

	/**
	 * @brief Loads resources into Ogre.
	 *
	 * Since an ArchiveFactory cannot be removed from the Ogre::ArchiveFactoryManager this must be deleted after Ogre has been torn down.
	 * Thus, this is deleted in the destructor.
	 */
	OgreResourceLoader* mLoader;

	/**
	 * @brief The root window of the config dialog.
	 */
	CEGUI::Window* mConfigWindow;
};

}
}
#endif /* OGRECONFIGURATOR_H_ */
