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
 *      Revision 1.13  2003-10-30 07:37:41  aglanor
 *      2003-10-30 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre:
 *              Makefile.am, DimeOgre.(h|cpp), ConsoleObjectImpl.(h|cpp):
 *              Moved Ogre response to console commands from the app
 *              to its own class, ConsoleObjectImpl.
 *
 *      Revision 1.12  2003/10/29 18:25:25  aglanor
 *      2003-10-29 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * src/services/components/ogre/: DimeOgre.(h|cpp):
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
 *              Format and then to Ogre, all of this outside Dime.
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
 *      	Check screenshot of fully textured pig within dime here:
 *      	http://purple.worldforge.org/~aglanor/screenshots/dime_20030623.png
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
 *      	* DimeOgre.h/cpp: added a Water Plane at height 0.
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
 *      	more relevant here than dime or eris. Now it uses the default namesp
 *

-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
Description: Base class for all the OGRE examples
-----------------------------------------------------------------------------
*/

#ifndef __DimeOgre_H__
#define __DimeOgre_H__

#include <Ogre.h>
#include <OgreConfigFile.h>
#include "OgreFrameListener.h"
#include <framework/ConsoleObject.h> //TODO: this will be included in a different class

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
// Include Eris header files
// ------------------------------
#include <Eris/Entity.h>
#include <Eris/World.h>

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


class TerrainListener;

class CameraRotator;

class CameraFrameListener;

/** Base class which manages the standard startup of an Ogre application.
    Designed to be subclassed for specific examples if required.
*/
class DimeOgre: virtual public SigC::Object //, public Ogre::ActionListener, public Ogre::MouseListener
// TODO: the DimeOgre ConsoleObject will be included in a different class
{
public:
    /// Standard constructor
    DimeOgre()
    {
        mFrameListener = 0;
        mRoot = 0;
    }
    /// Standard destructor
    virtual ~DimeOgre()
    {
        if (mFrameListener)
            delete mFrameListener;
        if (mRoot)
            delete mRoot;
    }

    /// Start the example
    virtual void go(void)
    {
        if (!setup())
            return;

        mRoot->startRendering();
    }

	// Initialize all dime services needed for this application
	void initializeDimeServices(void);

	/* Eris::World entity signals (see eris\src\world.h for more info) */
	// TODO: comment this
	void connectWorldSignals(void);

        /**
         * Called when an entity is created. This connects entity-specific
         * signals to methods in the game view. In the case of Changed and
         * Moved, a pointer to the entity is bound in because these signals
         * do not provide the pointer by themselves.
         *
         * You should add in code that inserts a pointer to the entity's media
         * into your world model.
         *
         * @param e A pointer to the Eris entity that has been created.
         */
        void entityCreate( Eris::Entity *e );

        /**
         * Called on entity deletion. You should remove all information you
         * hold about the entity.
         *
         * @param e A pointer to the Eris entity that has been deleted.
         *
         */
        void entityDelete( Eris::Entity *e );

        /**
         * Called only once, when the player enters the game world. It's
         * possible that you won't need this one.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void entered( Eris::Entity *e );

        /** Called when an entity become visible. You'll probably want to add
         * a media pointer to your world model at this point.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void appearance( Eris::Entity *e );

        /**
         * Called when an entity becomes invisible. You should remove the media
         * pointer corresponding to the entity from your world view, but retain
         * any additional data you're holding about the entity.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void disappearance( Eris::Entity *e );


        /* Eris::Entity signals  (see eris\src\entity.h for more info)*/

        /**
         * Called when an entity changes its container. This may require
         * changes to your world model, but some gameviews can safely ignore
         * this signal.
         *
         * @param e A pointer to the Eris entity that has been recontainered
         * @param c A pointer to the Eris entity that is the new container for e
         */
        void recontainered( Eris::Entity *e, Eris::Entity *c );

        /**
         * I'm not sure what this does. Let's ignore it until I can track down
         * James and bop him on the head for writing unhelpful comments ;)
         * NOTES: I suspect this is when an attribute of the object is changed.
         */
        void changed( const Eris::StringSet &s, Eris::Entity *e );

        /**
         * Called when the entity moves. Here you should alter the position
         * of the media pointer in your world model... this may involve
         * removing it from where it was before the entity moved and
         * placing it in the new position, in which case you'll need
         * a reverse-lookup of some kinda- WFMath::Point<3> is the new
         * entity coordinate, the old one is only known if stored by you.
         *
         * @param c The new coordinates of the entity
         * @param e A pointer to the Eris entity that has moved
         */
        void moved( const WFMath::Point< 3 > &, Eris::Entity *e );

        /**
         * Called when the entity speaks. You'll probably want to display the
         * speech on the screen somehow.
         *
         * @param s A string containing the speech
         * @param e A pointer to the Eris entity
         */
        void say( const std::string &s, Eris::Entity *e );

        /**
         * Sadly undocumented
         */
        void addedMember(Eris::Entity *e);

        /**
         * Also sadly undocumented
         */
        void removedMember(Eris::Entity *e);

	/**
	 * Update the scene animations for all entities
	 */
	void updateAnimations(Ogre::Real time);

	// TODO: these are for tests. Remove them later
	Ogre::Entity* mOgreHead;
	Ogre::SceneNode* mOgreHeadNode;

protected:
	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::Camera* mPlayer1pCamera;
	Ogre::Camera* mPlayer3pCamera;
	Ogre::Camera* mPlayerMapCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::FrameListener* mFrameListener;
	Ogre::RenderWindow* mWindow;

    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    virtual bool setup(void)
    {
        mRoot = new Ogre::Root();

        setupResources();

        bool carryOn = configure();
        if (!carryOn) return false;

        chooseSceneManager();
        createCamera();
        createViewports();

        // Set default mipmap level (NB some APIs ignore this)
        Ogre::TextureManager::getSingleton().setDefaultNumMipMaps(5);

        // Create the scene
        createScene();

        createFrameListener();

        return true;

    }
    /** Configures the application - returns false if the user chooses to abandon configuration. */
    virtual bool configure(void)
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        if(mRoot->showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            mWindow = mRoot->initialise(true);
            //mRoot->showDebugOverlay(true);
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual void chooseSceneManager(void)
    {
        // Get the SceneManager, in this case a generic one
        mSceneMgr = mRoot->getSceneManager(Ogre::ST_EXTERIOR_CLOSE);
    }

	void createCamera(void);

	void createFrameListener(void);

	void createScene(void);

    virtual void createViewports(void)
    {
        // Create one viewport, entire window
        Ogre::Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//float left=0.0f, float top=0.0f, float width=1.0f, float height=1.0f)

	Ogre::Viewport* mapvp = mWindow->addViewport(mPlayerMapCamera,1,0.70,0.05,0.25,0.25);
	mapvp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mapvp->setOverlaysEnabled(false);
	
    }

    /// Method which will define the source of resources (other than current folder)
    virtual void setupResources(void)
    {
        // Load resource paths from config file
        Ogre::ConfigFile cf;
        cf.load("resources.cfg");

        // Go through all settings in the file
        Ogre::ConfigFile::SettingsIterator i = cf.getSettingsIterator();

        Ogre::String typeName, archName;
        while (i.hasMoreElements())
        {
            typeName = i.peekNextKey();
            archName = i.getNext();
            Ogre::ResourceManager::addCommonArchiveEx( archName, typeName );
        }
    }

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
	dime::OgreGameView* mGameview;


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
#endif
