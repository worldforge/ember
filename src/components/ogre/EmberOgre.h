/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright © 2000-2002 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/

#ifndef __EmberOgre_H__
#define __EmberOgre_H__

#include "EmberOgrePrerequisites.h"

// ------------------------------
// Include sigc header files
// ------------------------------
#include <sigc++/trackable.h>


#include "framework/Singleton.h"




namespace Eris {
class View;
class Connection;
}

namespace Carpenter
{
class Carpenter;
class BluePrint;
}

namespace Ember
{
class StreamLogObserver;
}


namespace EmberOgre {

namespace Model {
	class ModelDefinitionManager;
}

class CameraRotator;

class CameraFrameListener;

class Avatar;

class AvatarCamera;

class AvatarController;

class AvatarEmberEntity;

class EmberEntityFactory;

class EmberPagingSceneManager;

class TerrainGenerator;

class MotionManager;

class InputManager;

class GUIManager;

class Jesus;

class EmberEntity;

class OgreResourceLoader;

class OgreLogObserver;




/** 

The main class of ember. This functions as a hub for almost all subsystems. (Perhaps this should be refactored?)

*/
class EmberOgre : public Ember::Singleton<EmberOgre>, 
public sigc::trackable,
public Ogre::FrameListener
{
public:
 
 
    /// Standard constructor
    EmberOgre();
 
    /// Standard destructor
    ~EmberOgre();

	virtual bool frameStarted(const Ogre::FrameEvent & evt);
	
    /**
     * starts the main app
     * @param loadOgrePluginsThroughBinreloc if true, plugins will be loaded through binreloc instead of through ~/.ember/plugins.cfg
     */
    virtual void go(bool loadOgrePluginsThroughBinreloc = false);
	void shutdown();

	/**
	 * Initialize all Ember services needed for this application
	 * @param  the prefix for the application, not appliable if running under win32
	 * @param  the an alternative home directory. If the default should be used, send an empty string.
	 */
	void initializeEmberServices(const std::string& prefix, const std::string& homeDir);

	void connectViewSignals(Eris::View* world);
	void connectedToServer(Eris::Connection* connection);


	// TODO: possibly we'd like to do the following in a different way,
	// perhaps refactoring stuff
	Avatar* getAvatar() const;
	Ogre::SceneManager* getSceneManager() const;
	TerrainGenerator* getTerrainGenerator() const;
	MotionManager* getMotionManager() const;
	Ogre::Root* getOgreRoot() const;
	EmberEntityFactory* getEntityFactory() const;
	AvatarCamera* getMainCamera() const;
	AvatarController* getAvatarController() const;
	
	/**
	 * Gets the entity with the supplies id from the world.
	 */
	EmberEntity* getEmberEntity(const std::string & eid) const;
	
	inline Eris::View* const getMainView() { return mWorldView;};
	
	inline Jesus* getJesus() const { return mJesus; }
	
	inline Ogre::RenderWindow* getRenderWindow() const { return mWindow; }
	
	
	sigc::signal<void, EmberEntityFactory*> EventCreatedEmberEntityFactory;
	sigc::signal<void, AvatarEmberEntity*> EventCreatedAvatarEntity;
	sigc::signal<void, Jesus*> EventCreatedJesus;
	/**
	 * Emitted when the use wants to quit the game. Preferrebly the GUI should show some kind of confirmation window.
	 */ 
	sigc::signal<void, bool&> EventRequestQuit;
	
	/**
	Emitted before the eris polling is started
	*/
	sigc::signal<void> EventStartErisPoll;
	
	/**
	Emitted after the eris polling has finished
	*/
	sigc::signal<void> EventEndErisPoll;
	
	/**
	 * returns the scenenode of the world entity
	 * throws en exception if no such node has been created
	 * @return 
	 */
	Ogre::SceneNode* getWorldSceneNode() const;
	
	
	/**
	 *    returns the root scene node
	 * @return 
	 */
	Ogre::SceneNode* getRootSceneNode() const;
	
	
	/**
	Emitted after the GUIManager has been created, but not yet initialized
	*/
	sigc::signal<void, GUIManager&> EventGUIManagerCreated;
	/**
	Emitted after the GUIManager has been initilized
	*/
	sigc::signal<void, GUIManager&> EventGUIManagerInitialized;
	
	/**
	Emitted after the Motion has been created
	*/
	sigc::signal<void, MotionManager&> EventMotionManagerCreated;
	
	
	/**
	Emitted after the TerrainGenerator has been created
	*/
	sigc::signal<void, TerrainGenerator&> EventTerrainGeneratorCreated;
	
	/**
	Emitted after the AvatarController has been created
	*/
	sigc::signal<void, AvatarController&> EventAvatarControllerCreated;
	
	/**
	Emitted after the base Ogre scene has been created
	*/
	sigc::signal<void> EventSceneCreated;
	
	EmberEntity* getEntity(const std::string & id) const;
	
	/**
	 *    Call this to "soft quit" the app. This means that an signal will be emitted, which hopefully will be taken care of by some widget, which will show a confirmation window, asking the user if he/she wants to quit.
	 However, if there is no widget etc. handling the request, the application will instantly quit.
	 */
	void requestQuit();

	/**
	 *    Sets whether eris should be polled each frame. Defaults to true.
	 * @param doPoll 
	 */
	void setErisPolling(bool doPoll);

	/**
	 *    Gets whether eris should be polled each frame.
	 * @return 
	 */
	bool getErisPolling() const;

protected:

	/**
	* The main user avatar
	*/
	Avatar* mAvatar;
	
	//This class controls the avatar
	AvatarController* mAvatarController;


	Ogre::Root *mRoot;
//    EmberTerrainSceneManager* mSceneMgr;
	EmberPagingSceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
	
	
	

	
	EmberEntityFactory* mEmberEntityFactory;

    /**
     * Sets up the application - returns false if the user chooses to abandon configuration.
     * @param loadOgrePluginsThroughBinreloc if true, plugins will be loaded through binreloc instead of through ~/.ember/plugins.cfg
     * @return 
     */
    virtual bool setup(bool loadOgrePluginsThroughBinreloc = false);
  
	/**
	 *    Creates the basic scene with a single avatar, just for testing purpose.
	 * NOTE: remove this when going final
	 * @param  
	 */
	void createScene(void);
	
	/**
	 *    Sets up Jesus. This inialized the mJesus member and loads all building blocks, blueprint and modelblocks etc.
	 */
	void setupJesus();
	
	/**
	 *    Preloads the media, thus avoiding frame rate drops ingame.
	 * @param  
	 */
	void preloadMedia(void);

	/**
	
	makes sure that there are files in ~/.ember
	*/
	void checkForConfigFiles();

   
//    EmberTerrainPageSource* mPageSource;
	TerrainGenerator* mTerrainGenerator;
	
	MotionManager* mMotionManager;
	GUIManager* mGUIManager;  
	Model::ModelDefinitionManager* mModelDefinitionManager;
	
	/**
	when this is fale the app will exit
	*/
	bool mKeepOnRunning;
	
	Jesus* mJesus;
	
	Eris::View* mWorldView;
	
	/**
	Controls whether eris should be polled at each frame update.
	*/
	bool mPollEris;

	/**
	The main log observer used for all logging.
	*/
	OgreLogObserver* mLogObserver;
	
	Ember::StreamLogObserver* mStreamLogObserver;
	
};

}


#endif
