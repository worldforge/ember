//
// C++ Interface: OgreSetup
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

struct SDL_Surface;

namespace EmberOgre {

class EmberPagingSceneManager;
class EmberPagingSceneManagerFactory;

/**
	A class used for setting up Ogre. Instead of creating the Ogre root object and the main render window direclty, use this to guarantee that everything is set up correctly.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class OgreSetup{
public:
    OgreSetup();

    ~OgreSetup();
    
    /**
     * Creates the ogre base system.
     * @return 
     */
    Ogre::Root* createOgreSystem();
    
	/**
	 * Configures the application - returns false if the user chooses to abandon configuration.
	 * @param  
	 * @return 
	 */
	bool configure(void);
    
	/**
	 *    Gets the main render window.
	 * @return 
	 */
	inline Ogre::RenderWindow* getRenderWindow() const;
	
    /**
     * Gets the Ogre root object.
     * @return 
     */
    inline Ogre::Root* getRoot() const;
    
    /**
     * chooses and sets up the correct scene manager
     * @param  
     */
    EmberPagingSceneManager* chooseSceneManager();
    
    /**
     * Shuts down the ogre system.
     */
    void shutdown();
    
private:

	/**
	Holds the Ogre root object.
	*/
	Ogre::Root* mRoot;
	
	/**
	Holds the main render window.
	*/
	Ogre::RenderWindow* mRenderWindow;
	
	
	/**
	 *    Attempts to parse out the user selected geometry options for ogre.
	 * @param config The option map, usually found inside the currently selected RenderSystem.
	 * @param width The width of the window in pixels.
	 * @param height The height of the window in pixels.
	 * @param fullscreen Whether fullscreen should be used or not.
	 */
	void parseWindowGeometry(Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen);
	
	/**
	Sets standard values in the ogre environment.
	*/
	void setStandardValues();
	
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

};

Ogre::Root* OgreSetup::getRoot() const { return mRoot;}
Ogre::RenderWindow* OgreSetup::getRenderWindow() const { return mRenderWindow;}

}

#endif
