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

#ifndef HAVE_SIGHANDLER_T
typedef void (*sighandler_t)(int);
#endif

#include "EmberOgrePrerequisites.h"
#include <OgreConfigOptionMap.h>
#include <OgreFrameListener.h>

struct SDL_Surface;

namespace EmberOgre {

class EmberPagingSceneManager;
class EmberPagingSceneManagerFactory;
class MeshSerializerListener;

/**
	@brief A class used for setting up Ogre. Instead of creating the Ogre root object and the main render window direclty, use this to guarantee that everything is set up correctly.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class OgreSetup : Ogre::FrameListener
{
public:
	OgreSetup();

	~OgreSetup();

	/**
	* Creates the ogre base system.
	* @return The new Ogre Root object.
	*/
	Ogre::Root* createOgreSystem();

	/**
	 * @brief Configures the application - returns false if the user chooses to abandon configuration.
	 * @return True if everything was correctly set up, else false.
	 */
	bool configure();

	/**
	 * @brief Gets the main render window.
	 * @return The render window.
	 */
	Ogre::RenderWindow* getRenderWindow() const;

	/**
	* @brief Gets the Ogre root object.
	* @return The root object.
	*/
	Ogre::Root* getRoot() const;

	/**
	 * @brief Chooses and sets up the correct scene manager.
	 * @return The newly created scene manager.
	 */
	EmberPagingSceneManager* chooseSceneManager();

	/**
	* @brief Shuts down the ogre system.
	*/
	void shutdown();

	/**
	 * @brief Swap the double buffers every frame.
	 * @param evt The ogre frame event.
	 * @return Always return true.
	 */
	bool frameEnded(const Ogre::FrameEvent & evt);

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
	 * @brief Attempts to parse out the user selected geometry options for ogre.
	 * @param config The option map, usually found inside the currently selected RenderSystem.
	 * @param width The width of the window in pixels.
	 * @param height The height of the window in pixels.
	 * @param fullscreen Whether fullscreen should be used or not.
	 */
	void parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen);

	/**
	 * @brief Sets standard values in the ogre environment.
	 */
	void setStandardValues();

	/**
	 * @brief Checks for the named gl extension.
	 * @param extension The name of the extension to check for.
	 * @return True if the extension was found.
	 */
	int isExtensionSupported(const char *extension);


	/**
	The icon shown in the top of the window.
	*/
	SDL_Surface* mIconSurface;

	/**
	We'll use our own scene manager factory.
	*/
	EmberPagingSceneManagerFactory* mSceneManagerFactory;

	/**
	The main video surface. Since we use SDL for input we need SDL to handle it too.
	*/
	SDL_Surface* mMainVideoSurface;

	/**
	* @brief Provides the ability to use relative paths for skeletons in meshes.
	*/
	MeshSerializerListener* mMeshSerializerListener;

};

inline Ogre::Root* OgreSetup::getRoot() const
{
	return mRoot;
}

inline Ogre::RenderWindow* OgreSetup::getRenderWindow() const
{
	return mRenderWindow;
}

}

#endif
