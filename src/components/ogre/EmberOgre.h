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

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.40  2005-02-22 23:51:32  erik
 *      2005-02-23  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Change name of main exec to "ember"
 *      	* Initial checking of Carpenter library which would allow dynamic constructions of buildings. This is very much in a protoyping stage right now. Later on the library will be separated from Ember.
 *
 *      Revision 1.39  2005/02/10 00:54:03  erik
 *      2005-02-10  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* send event on eris polling start and stop
 *
 *      Revision 1.38  2005/01/07 02:42:48  erik
 *      2005-01-07  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* check for, and if not found, copy config files from share to user's home dir
 *
 *      Revision 1.37  2005/01/04 23:02:36  erik
 *      2005-01-04  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* moved all use of terrain coordinates, both in ogre and in atlas, to the common class TerrainPosition
 *      	* (EmberTerrainSceneManager.cpp) increased the page offset
 *      	* ember now loads all terrain at start, this might be slow on older machines. In the future we'll implement a paging mechanism.
 *      	* removed WF2OGRE_VECTOR3 macros
 *      	* moved to CEGUI 0.1.0, which uses ogre's resource system, thus making it easier to manage resources. This prompted some changes to the paths for gui elements.
 *      	* (DebugWidget)minor modifications
 *      	* (AvatarCamera)added method to get the screen coords for a given entity
 *      	* implement the use of EmberOgrePrerequisites.h
 *      	* (EmberEntity)added support for suggested responses from the server when an entity says something
 *      	* (EmberEntityFactory, EmberOgre)added method for accessing the world entity
 *      	* added IngameChatWidget for displaying "bubbles" over NPC's head when they speak
 *      	* (ServerService)better use of the wield op
 *
 *      Revision 1.36  2004/11/17 21:09:11  erik
 *      2004-11-17  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* added output of the eris log to debug widget
 *      	* fixed Wield and Use ops
 *
 *      Revision 1.35  2004/11/13 21:08:01  erik
 *      2004-11-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Removed some bugs which in various ways stopped the main loop.
 *      	* Finalized on a resource structure.
 *      	* Tried to arrange the inludes in a way that made it easy to compile Ember in debug mode without getting bit by the special debug Ogre memory manager. It's not 100% though. For now, change OgreConfig.h, set OGRE_DEBUG_MEMORY_MANAGER  to 0.
 *      	* Fixed a bug which made Ember not release the mouse upon exit.
 *      	* Changed the gui to use png images instead of tga, since apparently some machines can't load the tga in a correct way.
 *
 *      Revision 1.34  2004/11/13 00:48:22  erik
 *      2004-11-13  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	* Changed modeldefintions. Use "Action" instead of "Animation" because an action can contain more than just animations, sounds for example.
 *      	* Fixed wrong references to Ogre::TerrainSceneManager
 *      	* Changed location of config and media files.
 *
 *      Revision 1.33  2004/11/11 23:57:26  erik
 *      2004-11-12  Erik Hjortsberg  <erik@katastrof.nu>
 *
 *      	*  Added environment classes: Sun, Sky, Water, Foliage
 *      	* Rewrote Model handling to use a subclass of Ogre::ResourceManager
 *      	* Various fixes to get the resouce handling to work better for distributions.
 *      	* Upon startup now only a lone avatar is shown. This is for debugging and shall be removed.
 *      	* Removed bug where images were written to a non-existant directory, resulting in strange resource allocation problems. Ugly one.
 *      	* Various small changes to widgets.
 *
 *      Revision 1.32  2004/11/04 21:40:26  erik
 *      2004-11-04 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Moved from dime to ember. Thus the big checkin.
 *      * Added wield action to inventory
 *
 *      Revision 1.31  2004/11/03 02:01:14  erik
 *      2004-11-03 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Added configuration through Varconf. All configuration is now done from the directory ~/.ember
 *      * Added distorted fresnel water. One step closer to FarCry scenery :) This requires a GPU with support for Pixel Shader v. 2.0 (GeForceFX and higher for example)
 *      Screens can be found at the usual place, http://purple.worldforge.org/~erik/ember/screens/?C=M;O=D
 *      * Began work to separate environment into own classes (water, sun, sky etc.)
 *
 *      Revision 1.30  2004/11/02 01:19:55  erik
 *      2004-11-02 Erik Hjortsberg <erik@katastrof.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Added a better placeholder model than the ogre space ship. Ember now no longer use any media linked to the ogre sample media. In preparation for a release all media will reside under ~/.ember/Media . In the future we'll probably use a shared location, but this is only when we do a real release. For the upcoming release, which is more of a development release, ~/.ember/Media will be best. Media can be found at http://purple.worldforge.org/~erik/ember/media/Media.tar.gz
 *      * Removed a silly bug which made Ember segfault on exit.
 *
 *      Revision 1.29  2004/10/30 17:32:38  erik
 *      2004-10-30 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * Fixed the issue with entities not getting correct orientation.
 *      * Added a give action to the EntityPickerWidget
 *      * Fixed creation of entities through MakeEntityWidget
 *
 *      Revision 1.28  2004/10/27 23:45:21  erik
 *      2004-10-28 Erik Hjortsberg <erik@hysteriskt.nu>
 *      http://erikhjortsberg.blogspot.com/
 *
 *      * moved to Eris 1.3
 *      This requires some changes to the way enties are handled etc..
 *      There's a big bug which I can't seem to track down. It manifests itself in that entities attached to other entities (i.e. not directly attached to the root node) can't change their orientation. An interim solution for now is to direct all entities attached to the world to the root node. This works for now, but the effect can be seen when the avatar walk into the sty.
 *      *moved to Ogre 0.15.0
 *      Ember now compiles and works against the latest ogre point release. We'll try to keep it this way.
 *      As a result of the move to 0.15.0 I've updated the meshes (Since there's been changes to the binary .mesh format. New meshes can be downloaded from http://purple.worldforge.org/~erik/ember/media/Media.tar.gz.).
 *      *added some more functionality to ServerBrowserWidget and ServerWidget
 *
 *      Revision 1.27  2004/10/14 00:13:14  erik
 *      2004-10-14 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      * moved EventCreatedAvatarEntity to EmberOgre class and adjusted inventory
 *
 *      Revision 1.26  2004/10/12 23:03:01  erik
 *      2004-10-12 Erik Hjortsberg <erik@katastrof.nu>
 *
 *      * fixed mouse picking
 *      * streamlined inventory a bit more
 *
 *      Revision 1.25  2004/10/12 00:49:55  erik
 *      2004-10-12 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      * added mouse picker menu for touching and taking entities. Right now it's just a placeholder, but it works, can be seen here: http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023002.png
 *      * added a simple inventory. It's just a listbox right now, but it works. The drop button won't do anything yet though. It can be seen here too: http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023044.png
 *      http://purple.worldforge.org/~erik/ember/screens/screenshot_20041012_023052.png
 *
 *      Revision 1.24  2004/10/06 22:32:48  erik
 *      2004-10-07 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      *Moved completely to CEGUI. We now use no overlays.
 *      *Changed the way the input system and the camera works:
 *      	- Normal mode is gui mode where the mouse can be moved around, windows changed and interacted with. If one clicks on the Ogre render window mouse picking in the 3d world will occur (not implemented yet)
 *      	- By pressing and holding the right mouse button one enters Movement mode. Mouse movement here will change the camera, which is centered on the avatar.
 *      	- Pressing keys will move the avatar, UNLESS we're in Normal mode and a CEGUI widget says otherwise (the console for instance). This is not yet fully implemented.
 *
 *      *Implemented a widget system. Each widget is something like a map, an inventory, avatar stats etc.. These should preferrably be loaded through dynamic libs as plugins in the future. We'll also add lua-scripting (which should be easy since it's almost already in CEGUI).
 *
 *      *Removed references to InputManager since it's obsolete now.
 *
 *      Revision 1.23  2004/08/04 23:39:43  aglanor
 *      2004-08-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* /src/components/ogre/GUIManager.(h|cpp) and others related:
 *      	Added basic GUI support, including display of In-Game chat.
 *
 *      Revision 1.22  2004/07/31 11:53:59  erik
 *      2004-07-31 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *cleaned up the class layout and structured it, we're not using that many Singletons anymore
 *      *moved to Ogre CVS in order to get a more decent terrainmanager
 *      *added splatting and nice texturing to the terrain
 *      *added classes TerrainShader and EmberTerrainPageSource which works with the new terrainmanager
 *
 *      Revision 1.21  2004/07/21 00:27:29  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      /src/components/ogre:
 *      *removed references to EntityListener, we'll keep the class however as
 *      we'll pobably use it in the future
 *      *put everything under the namespace EmberOgre
 *
 *      Revision 1.20  2004/07/20 22:52:16  erik
 *      2004-07-21 Erik Hjortsberg <erik@hysteriskt.nu>
 *
 *      in src/components/ogre:
 *
 *      MathConverter.h:
 *      *changed scaling
 *
 *      EmberEntity.*:
 *      *fixed problems with containers
 *      *added more meshes (sty, oak)
 *
 *      Avatar.*:
 *      *moved camera code to AvatarCamera
 *      *better connection to Eris::Avatar and avatar creation
 *
 *      MotionManager.cpp:
 *      *better clean up code
 *
 *      InputManager.cpp:
 *      *removed buggy polling code and replaced it with signals in the ServerService class
 *
 *      EmberEntityFactory.*:
 *      *added support for AvatarEmberEntity
 *
 *      AvatarEmberEntity.*:
 *      New class for handling the Avatar entity
 *
 *      AvatarCamera.*:
 *      New class for handling the Avatar camera
 *
 *      EmberOgre.*:
 *      *cleaned up old code
 *      *moved camera stuff into AvatarCamera
 *      *better connection between Services signals and other objects
 *
 *      EntityListener.cpp:
 *      *removed connection to Avatar, this class is obsolete anyway
 *
 *      AvatarController.*:
 *      *added support for AvatarCamera
 *
 *      Revision 1.19  2004/07/16 21:21:57  erik
 *      2004-07-16 Erik Hjortsberg <erik@hysteriskt.nu>
 *      /src/components/ogre:
 *      *added support for mercator terrain by using a subclass of Ogre's OctreeSceneManager, it includes some preparation in order to access the Ogre headers, I'll add instructions tomorrow
 *      *Ember now depends on Mercator
 *
 *      Revision 1.18  2004/07/12 04:05:48  erik
 *      2004-07-12 Erik Hjortsberg erik by hysteriskt speck org
 *      * src/components/ogre:
 *      Changed the way input is handled through the use of an Ogre::EventProcessor
 *      By default mouse is not grabbed, press F9 to grab and ungrab it.
 *      The mouse now shows a cursor, movement happens when the mouse goes near the edges of the screen.
 *      Various code cleanups, forward declarations and movement of code from .h-files to .cpp-files.
 *      Addition of preprocessor declarations in MathConverter.h to easily convert between WF and Ogre units.
 *      Revamping of how entities are handled though the new class EmberEntity and changes to EntityListener.
 *      RuEmberntary animation of the avatar.
 *      Addition of a debug layer to show triangles, fps etc. Taken from Ogre.
 *      Usage of ogre.cfg for reading configuration values instead of using the console each time.
 *      Some cleanup of the world. The only thing you see now is a ground plane. But it gets populated when connected to a server.
 *      Had some problem with the top cam so I disabled it.
 *
 *      Revision 1.17  2003/12/08 21:25:06  aglanor
 *      2003-11-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: fixed rotation and viewing for the Avatar.
 *
 *      Revision 1.16  2003/11/29 01:25:58  aglanor
 *      2003-11-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added AvatarKeyboardListener.(h|cpp)
 *      	        and restored local avatar movement.
 *
 *      Revision 1.15  2003/11/26 19:03:43  aglanor
 *      2003-11-26 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added Avatar.(h|cpp) to store
 *              the whole scenenode tree for the avatar and to control
 *              its movement, looks, etc.
 *
 *      Revision 1.14  2003/11/25 08:03:05  aglanor
 *      2003-11-25 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/components/ogre: added 3rd person camera and
 *      	        (now working) top view camera. Added another camera
 *      		        ("spycam") for debugging purposes.
 *
 *      Revision 1.13  2003/10/30 07:37:41  aglanor
 *      2003-10-30 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre:
 *              Makefile.am, EmberOgre.(h|cpp), ConsoleObjectImpl.(h|cpp):
 *              Moved Ogre response to console commands from the app
 *              to its own class, ConsoleObjectImpl.
 *
 *      Revision 1.12  2003/10/29 18:25:25  aglanor
 *      2003-10-29 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: EmberOgre.(h|cpp):
 *              Added ConsoleObject inheritance, it runs the Quit command.
 *
 *      Revision 1.11  2003/10/21 20:48:37  aglanor
 *      Restored SkyBox
 *
 *      Revision 1.10  2003/09/28 15:16:06  aglanor
 *      2003-09-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * /src/components/ogre/Makefile.am: removed the cal3d
 *              to Ogre converter, this app will no longer be here.
 *              All models will be converted from Cal3d to Open Model
 *              Format and then to Ogre, all of this outside Ember.
 *              (also, we don't need the cal3d library anymore,
 *              until we add a component that uses it, like Crystalspace).
 *
 *      Revision 1.9  2003/07/03 20:12:25  aglanor
 *      added sample animation to demo mesh
 *
 *      Revision 1.8  2003/06/23 01:20:34  aglanor
 *      2003-06-23 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * Cal3DConverter: converts Cal3D materials to Ogre
 *              materials, and assigns material and texture mapping
 *              to the Ogre mesh accordingly.
 *
 *      	Check screenshot of fully textured pig within Ember here:
 *      	http://purple.worldforge.org/~aglanor/screenshots/Ember_20030623.png
 *
 *      Revision 1.7  2003/05/07 23:28:43  aglanor
 *      Added a little more variety to the media. Now there's different media
 *      for settlers (malebuilder), pig (skeleton), merchant (robot) and
 *      everything else.
 *      The camera is set to the Avatar position/orientation when entering the game world.
 *
 *      Revision 1.6  2003/05/02 12:48:45  aglanor
 *      Cal3D converter half-done. Retrieves the list of vertices, normals, etc from each Submesh. Need still to create a GeometryData and store it all within.
 *
 *      Revision 1.5  2003/04/28 22:42:06  aglanor
 *      Added new viewport for a mini-map and quaternion conversion.
 *      Eris entities are now displayed with orientation.
 *
 *      Revision 1.4  2003/04/28 00:55:06  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* EmberOgre.h/cpp: added a Water Plane at height 0.
 *      		There is also a little ogre head marking (0,0,0).
 *      			The more detailed the scene, the easiest it is
 *      				to spot strange behaviors.
 *      					And it looks cool :)
 *
 *      Revision 1.3  2003/04/24 20:27:28  nikal
 *      Adding Ogre:: to all the variables which are in the Ogre namespace
 *
 *      Revision 1.2  2003/04/24 20:02:08  aglanor
 *      Makefile and include changes after the renaming
 *
 *      Revision 1.1  2003/04/24 19:42:09  aglanor
 *      2003-04-24 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * components/ogre: big cleanup. Files renamed.
 *
 *      Revision 1.8  2003/04/19 21:26:14  nikal
 *      Some changes to get Ogre app to compile with the latest version of OGRE
 *
 *      Revision 1.7  2003/01/05 22:13:46  aglanor
 *      2003-01-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* OgreApp.cpp/h: added basic GUI elements.
 *      	Now OgreApp inherits from ogre::ActionListener
 *      	and ogre::MouseListener
 *
 *      Revision 1.6  2002/12/10 00:02:45  aglanor
 *      2002-12-07 Miguel Guzman (Aglanor) <aglanor@telefonica.net>
 *
 *      	* OgreApp.cpp: meshes are placed into the OGRE scene when an entity is created.
 *      	Currently they're all squirrels of doom.
 *
 *      	* OgreApp.cpp/h: removed "using namespace ogre". OGRE should not be
 *      	more relevant here than Ember or eris. Now it uses the default namesp
 *

-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __EmberOgre_H__
#define __EmberOgre_H__

#include "EmberOgrePrerequisites.h"

//#include <Mercator/Terrain.h>


//#include <OgreFrameListener.h>


// Include OGRE GUI classes (TODO: perhaps in future OGRE releases this will be cleaner)
/*
#include "OgreOverlayManager.h"
#include "OgreCursorGuiElement.h"
#include "OgreBorderButtonGuiElement.h"
#include "OgreListGuiElement.h"
#include "OgreListChanger.h"
#include "OgreEventProcessor.h"
#include "OgreStringResource.h"
*/

// ------------------------------
// Include sigc header files
// ------------------------------
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif

#include "framework/Singleton.h"

#include "carpenter/Carpenter.h"

// #include <OgreConfigFile.h>

namespace Eris {
class View;
class Connection;
}

// namespace Carpenter
// {
// class Carpenter;
// class BluePrint;
// }


namespace EmberOgre {

//class TerrainListener;

class CameraRotator;

class CameraFrameListener;

class Avatar;

class AvatarCamera;

class AvatarController;

class AvatarEmberEntity;

class EmberEntityFactory;

class EmberTerrainSceneManager;

class TerrainGenerator;

class MotionManager;

class InputManager;

class GUIManager;

class ModelDefinitionManager;






/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class EmberOgre : public Ember::Singleton<EmberOgre>, virtual public SigC::Object, public Ogre::FrameListener //, public Ogre::ActionListener, public Ogre::MouseListener
// TODO: the EmberOgre ConsoleObject will be included in a different class
{
public:
 
 	//static EmberOgre & getSingleton(void);
 
    /// Standard constructor
    EmberOgre();
 
    /// Standard destructor
    ~EmberOgre();

	bool frameStarted(const Ogre::FrameEvent & evt);
	bool frameEnded(const Ogre::FrameEvent & evt)
	{
		return true;
	}
	
    virtual void go(void);
	void shutdown();

	// Initialize all Ember services needed for this application
	void initializeEmberServices(void);

	void connectViewSignals(Eris::View* world);
	void connectedToServer(Eris::Connection* connection);


	// TODO: possibly we'd like to do the following in a different way,
	// perhaps refactoring stuff
	Avatar* getAvatar();
	EmberTerrainSceneManager* getSceneManager();
	TerrainGenerator* getTerrainGenerator();
	MotionManager* getMotionManager();
	Ogre::Root* EmberOgre::getOgreRoot();
	EmberEntityFactory* getEntityFactory();
	AvatarCamera* getMainCamera();
	
	inline Ogre::RenderWindow* getRenderWindow() const { return mWindow; }
	
	
	SigC::Signal1<void, EmberEntityFactory*> EventCreatedEmberEntityFactory;
	SigC::Signal1<void, AvatarEmberEntity*> EventCreatedAvatarEntity;
	
	/**
	Emitted before the eris polling is started
	*/
	SigC::Signal0<void> EventStartErisPoll;
	
	/**
	Emitted after the eris polling has finished
	*/
	SigC::Signal0<void> EventEndErisPoll;
	
	//returns the scenenode of the world entity
	//throws en exception if no such node has been created
	Ogre::SceneNode* getWorldSceneNode();
	

//private:
//	static Ogre::SceneManager* sceneMgr;

protected:

	// Avatar setup
	Avatar* mAvatar;
	
	//This class controls the avatar
	AvatarController* mAvatarController;

	/** Instance of EmberOgre */
	//static EmberOgre* _instance;

	Ogre::Root *mRoot;
    EmberTerrainSceneManager* mSceneMgr;
	//Ogre::Camera* mCamera;
//	Ogre::FrameListener* mFrameListener;
	Ogre::RenderWindow* mWindow;
	
	
	
//	TerrainGenerator* mTerrainGenerator;

	
	EmberEntityFactory* mEmberEntityFactory;

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void);
    /** Configures the application - returns false if the user chooses to abandon configuration. */
    virtual bool configure(void);

    virtual void chooseSceneManager(void);

	void createCamera(void);

	void createFrameListener(void);

	void createScene(void);
	
	void preloadMedia(void);

	void getResourceArchiveFromVarconf(Ogre::ResourceManager* manager, std::string variableName, std::string section = "media", std::string type = "FileSystem");

	/**
	
	makes sure that there are files in ~/.ember
	*/
	void checkForConfigFiles();

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources(void);
    
//    EmberTerrainPageSource* mPageSource;
	TerrainGenerator* mTerrainGenerator;
	//InputManager* mInputManager;
	MotionManager* mMotionManager;
	GUIManager* mGUIManager;  
	ModelDefinitionManager* mModelDefinitionManager;
	
	bool mKeepOnRunning;

        /*
	void mouseClicked(Ogre::MouseEvent* e) {}
	void mouseEntered(Ogre::MouseEvent* e) {}
	void mouseExited(Ogre::MouseEvent* e) {}
	void mousePressed(Ogre::MouseEvent* e) {}
	void mouseReleased(Ogre::MouseEvent* e) {}


	void actionPerformed(Ogre::ActionEvent* e)
	{
        // Think about doing something here
        std::string action = e->getActionCommand();
        Ogre::LogManager::getSingleton().logMessage("Got event: " + action);

        if (action == "SS/Setup/HostScreen/Exit")
            Ogre::Root::getSingleton().getRenderSystem()->shutdown();
	}
	*/
	Carpenter::Carpenter mCarpenter;


};

// ----------------------------------------------------------------------------
// Define the application object
// This is derived from BaseApplication which is the class OGRE provides to
// make it easier to set up OGRE without rewriting the same code all the time.
// You can override extra methods of BaseApplication if you want to further
// specialise the setup routine, otherwise the only mandatory override is the
// 'createScene' method which is where you set up your own personal scene.
// ----------------------------------------------------------------------------
/*
class OgreApp : public BaseApplication
{
public:
    // Basic constructor
    OgreApp() {}
	Ember::OgreGameView* mGameview;


protected:

  // Just override the mandatory create scene method
  void createScene(void);

  // Override this too.
  void createFrameListener(void);

  virtual void chooseSceneManager(void)
    {
      // Get the SceneManager
      mSceneMgr = mRoot->getSceneManager( ST_EXTERIOR_CLOSE );
    }

  virtual void createCamera(void);


};
*/

}


#endif
