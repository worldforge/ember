//
// C++ Interface: OgreSetup
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREOGRESETUP_H
#define EMBEROGREOGRESETUP_H

#include "EmberOgrePrerequisites.h"
#include "OgreIncludes.h"
#include "OgrePluginLoader.h"

#ifdef BUILD_WEBEMBER
#include "OgreWindowProvider.h"
#endif

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <OgreConfigOptionMap.h>
#include <OgreFrameListener.h>
#include <sigc++/trackable.h>
#include <memory>
namespace varconf {
class Variable;
}

namespace Ogre {
class SceneManagerFactory;
}

namespace Ember {
class ConfigListenerContainer;
namespace OgreView {

class MeshSerializerListener;

/**
 @brief A class used for setting up Ogre.

 Instead of creating the Ogre root object and the main render window directly, use this to guarantee that everything is set up correctly.

 @author Erik Ogenvik <erik@ogenvik.org>
 */
class OgreSetup : public ConsoleObject, public virtual sigc::trackable {
public:
	OgreSetup();

	~OgreSetup() override;

	/**
	 * Creates the Ogre base system.
	 * @return The new Ogre Root object.
	 */
	void createOgreSystem();

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
	 * @brief Gets the ogre overlay system, which is initialized in createOgreSystem.
	 * @return The ogre overlay system, nullptr if not initialized.
	 */
	Ogre::OverlaySystem* getOverlaySystem() const;

	void saveConfig();

	/**
	 * @brief Shuts down the Ogre system.
	 */
	void shutdown();

	void runCommand(const std::string& command, const std::string& args) override;

	/**
	 * @brief Command for simple diagnosis of Ogre.
	 */
	ConsoleCommandWrapper DiagnoseOgre;

private:

	/**
	 * @brief Holds the Ogre root object.
	 */
	std::unique_ptr<Ogre::Root> mRoot;

	/**
	 * @brief Holds the main render window.
	 */
	Ogre::RenderWindow* mRenderWindow;

	/**
	 We'll use our own scene manager factory.
	 */
	std::unique_ptr<Ogre::SceneManagerFactory> mSceneManagerFactory;

	/**
	 * @brief Provides the ability to use relative paths for skeletons in meshes.
	 */
	std::unique_ptr<MeshSerializerListener> mMeshSerializerListener;

	/**
	 * @brief The Ogre overlay system
	 */
	std::unique_ptr<Ogre::OverlaySystem> mOverlaySystem;
	/**
	 * @brief Ogre plugin loader, which handles OS specific and build specific differences.
	 *
	 * Don't destroy it before Ogre::Root, even if it works for you with dynamic builds,
	 * because on static builds it would destroy the plugins!
	 */
	OgrePluginLoader mPluginLoader;

#ifdef BUILD_WEBEMBER
	/**
	 * @brief Handles the window in WebEmber mode.
	 *
	 * When used in WebEmber mode, Ogre will handle the window (instead of SDL).
	 */
	OgreWindowProvider* mOgreWindowProvider;
#endif

	std::unique_ptr<ConfigListenerContainer> mConfigListenerContainer;

	bool mSaveShadersToCache;

	/**
	 * @brief Sets standard values in the Ogre environment.
	 */
	void setStandardValues();

	bool showConfigurationDialog();

	void parseWindowGeometry(const Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen);

	void input_SizeChanged(unsigned int width, unsigned int height);

	/**
	 * @brief Registers the OpenGL Context fix.
	 *
	 * This is needed to combat a bug found at least on KDE 4.14.4 when using OpenGL in the window manager.
	 * For some reason the OpenGL context of the application sometimes is altered when the window is minimized and restored.
	 * This results in segfaults when Ogre then tries to issue OpenGL commands.
	 * The exact cause and reasons for this bug are unknown, but by making sure that the OpenGL context is set each
	 * time the window is resized, minimized or restored we seem to avoid the bug.
	 */
	void registerOpenGLContextFix();

	void Config_ogreLogChanged(const std::string& section, const std::string& key, varconf::Variable& variable);


};

inline Ogre::RenderWindow* OgreSetup::getRenderWindow() const {
	return mRenderWindow;
}

inline Ogre::OverlaySystem* OgreSetup::getOverlaySystem() const {
	return mOverlaySystem.get();
}

}

}

#endif
