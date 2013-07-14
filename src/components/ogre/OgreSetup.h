//
// C++ Interface: OgreSetup
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREOGRESETUP_H
#define EMBEROGREOGRESETUP_H

#include "EmberOgrePrerequisites.h"
#include "OgreIncludes.h"

#ifdef BUILD_WEBEMBER
#include "OgreWindowProvider.h"
#endif

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <OgreConfigOptionMap.h>
#include <OgreFrameListener.h>
#include <sigc++/trackable.h>

namespace Ember
{
namespace OgreView
{

class EmberPagingSceneManager;
class EmberPagingSceneManagerFactory;
class MeshSerializerListener;

/**
 @brief A class used for setting up Ogre.

 Instead of creating the Ogre root object and the main render window directly, use this to guarantee that everything is set up correctly.

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class OgreSetup: public ConsoleObject, public virtual sigc::trackable
{
public:
	OgreSetup();

	virtual ~OgreSetup();

	/**
	 * Creates the Ogre base system.
	 * @return The new Ogre Root object.
	 */
	Ogre::Root* createOgreSystem();

	/**
	 * @brief Configures the application - returns false if the user chooses to abandon configuration.
	 * @return True if everything was correctly set up, else false.
	 */
	Ogre::Root* configure();

	/**
	 * @brief Gets the main render window.
	 * @return The render window.
	 */
	Ogre::RenderWindow* getRenderWindow() const;

	/**
	 * @brief Chooses and sets up the correct scene manager.
	 * @return The newly created scene manager.
	 */
	Ogre::SceneManager* chooseSceneManager();

	/**
	 * @brief Gets the ogre overlay system, which is initialized in createOgreSystem.
	 * @return The ogre overlay system, nullptr if not initialized.
	 */
	Ogre::OverlaySystem* getOverlaySystem() const;

	/**
	 * @brief Shuts down the Ogre system.
	 */
	void shutdown();

	virtual void runCommand(const std::string& command, const std::string& args);

	/**
	 * @brief Command for simple diagnosis of Ogre.
	 */
	ConsoleCommandWrapper DiagnoseOgre;

private:

	/**
	 * @brief Holds the Ogre root object.
	 */
	Ogre::Root* mRoot;

	/**
	 * @brief Holds the main render window.
	 */
	Ogre::RenderWindow* mRenderWindow;

	/**
	 We'll use our own scene manager factory.
	 */
	EmberPagingSceneManagerFactory* mSceneManagerFactory;

	/**
	 * @brief Provides the ability to use relative paths for skeletons in meshes.
	 */
	MeshSerializerListener* mMeshSerializerListener;

	/**
	 * @brief The Ogre overlay system
	 */
	Ogre::OverlaySystem* mOverlaySystem;

#ifdef BUILD_WEBEMBER
	/**
	 * @brief Handles the window in WebEmber mode.
	 *
	 * When used in WebEmber mode, Ogre will handle the window (instead of SDL).
	 */
	OgreWindowProvider* mOgreWindowProvider;
#endif

	/**
	 * @brief Sets standard values in the Ogre environment.
	 */
	void setStandardValues();

	/**
	 * @brief Checks for the named gl extension.
	 * @param extension The name of the extension to check for.
	 * @return True if the extension was found.
	 */
	int isExtensionSupported(const char *extension);

	bool showConfigurationDialog();

	void parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen);

	void input_SizeChanged(unsigned int width, unsigned int height);

};

inline Ogre::RenderWindow* OgreSetup::getRenderWindow() const
{
	return mRenderWindow;
}

inline Ogre::OverlaySystem* OgreSetup::getOverlaySystem() const
{
	return mOverlaySystem;
}

}

}

#endif
