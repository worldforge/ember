/*
-----------------------------------------------------------------------------
OgreApp.cpp by Miguel Guzman Miranda (Aglanor)
Based on OGRE sample applications:
    OGRE (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net
Based on the dime main application by the dime team

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
 *      Revision 1.15  2003-04-23 21:47:33  aglanor
 *      Added a new FrameListener capable of switching between modes.
 *
 *      Revision 1.14  2003/04/22 22:35:33  nikal
 *      changes for aglanor
 *
 *      Revision 1.13  2003/04/19 21:26:14  nikal
 *      Some changes to get Ogre app to compile with the latest version of OGRE
 *
 *      Revision 1.12  2003/01/06 03:48:20  aglanor
 *      2003-01-06 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* OgreApp.cpp/h: dual mode for the GUI: press 'H' to display
 *      	the GUI and use the mouse to click on it.
 *      	Press 'H' again to hide it and return to fullscreen mouseview.
 *
 *      Revision 1.11  2003/01/05 22:13:46  aglanor
 *      2003-01-05 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* OgreApp.cpp/h: added basic GUI elements.
 *      	Now OgreApp inherits from ogre::ActionListener
 *      	and ogre::MouseListener
 *
 *      Revision 1.10  2002/12/10 00:02:45  aglanor
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


// ------------------------------
// Include OGRE dime client files
// ------------------------------
#include "DimeOgreFrameListener.h"
#include "OgreGameView.h"


// ------------------------------
// Include dime header files
// ------------------------------
#include "services/DimeServices.h"
#include "framework/ConsoleBackend.h"

// ------------------------------
// Include Eris header files
// ------------------------------
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#endif

// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>


// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------

// The DEBUG stuff is a workaround for compile errors this causes in OGRE
#ifdef DEBUG
#define DEBUGTEMP
#undef DEBUG
#endif

#include <Ogre.h>

#ifdef DEBUGTEMP
#undef DEBUGTEMP
#define DEBUG
#endif

// ----------------------------------------------------------------------------
// Include the OGRE example framework
// This includes the classes defined to make getting an OGRE application running
// a lot easier. It automatically sets up all the main objects and allows you to
// just override the bits you want to instead of writing it all from scratch.
// ----------------------------------------------------------------------------
#include "OgreApp.h"


// TODO: move CerrLogObserver to its own class (under Logging service, or under Framework)
  class CerrLogObserver: public dime::LoggingService::Observer
    {
    public:
        CerrLogObserver()
        {
        }

        virtual void onNewMessage(const std::string & message, const std::string & file, const int & line,
                                  const dime::LoggingService::MessageImportance & importance, const time_t & timeStamp)
        {
            tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry
		
	    std::cerr.fill('0');
            std::cerr << "[";
	    std::cerr.width(2);
	    std::cerr << (ctm->tm_year/*+1900*/)%100 << "-";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_mon+1 << "-";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_mday << " ";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_hour << ":";
	    std::cerr.width(2);
	    std::cerr <<  ctm->tm_min << ":";
	    std::cerr.width(2);
	    std::cerr << ctm->tm_sec << "] ";
	    std::cerr  << "[File: " << file << ", Line #:" <<  line << "] (";

            if(importance == dime::LoggingService::CRITICAL)
                {
                    std::cerr << "CRITICAL";
                }
            else  if(importance == dime::LoggingService::FAILURE)
                {
                    std::cerr << "FAILURE";
                }
            else if(importance == dime::LoggingService::WARNING)
                {
                    std::cerr << "WARNING";
                }
            else if(importance == dime::LoggingService::INFO)
                {
                    std::cerr << "INFO";
                }
	    else
                {
                    std::cerr << "VERBOSE";
                }
            std::cerr << ") " <<message << std::endl;
        }

    private:

    };


class TerrainListener : public BaseFrameListener
{

	public:
		TerrainListener(RenderWindow* win, Camera* cam) :BaseFrameListener(win, cam) { };

 // Override frameStarted event to process that (don't care about frameEnded)
    bool frameStarted(const FrameEvent& evt)
    {
        float moveScale;
        float rotScale;
        // local just to stop toggles flipping too fast
        static Real timeUntilNextToggle = 0;
        if (timeUntilNextToggle >= 0)
            timeUntilNextToggle -= evt.timeSinceLastFrame;

        // If this is the first frame, pick a speed
        if (evt.timeSinceLastFrame == 0)
        {
            moveScale = 1;
            rotScale = 0.1;
        }
        // Otherwise scale movement units by time passed since last frame
        else
        {
            // Move about 100 units per second,
            moveScale = 10.0 * evt.timeSinceLastFrame;
            // Take about 10 seconds for full rotation
            rotScale = 36 * evt.timeSinceLastFrame;
        }

        // Grab input device state
        mInputDevice->capture();

        static Vector3 vec;

        vec = Vector3::ZERO;

        if (mInputDevice->isKeyDown(KC_A))
        {
            // Move camera left
            vec.x = -moveScale;
        }

        if (mInputDevice->isKeyDown(KC_D))
        {
            // Move camera RIGHT
            vec.x = moveScale;
        }

        if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W))
        {
            // Move camera forward
            vec.z = -moveScale;
        }

        if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S))
        {
            // Move camera backward
            vec.z = moveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGUP))
        {
            // Move camera up
            vec.y = moveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGDOWN))
        {
            // Move camera down
            vec.y = -moveScale;
        }

        if (mInputDevice->isKeyDown(KC_RIGHT))
        {
            mCamera->yaw(-rotScale);
        }
        if (mInputDevice->isKeyDown(KC_LEFT))
        {
            mCamera->yaw(rotScale);
        }

        if( mInputDevice->isKeyDown( KC_ESCAPE) )
        {
            return false;
        }



        // Make position changes to the camera
        mCamera->moveRelative(vec);

        // Rotate scene node if required
        SceneNode* node = mCamera->getSceneManager()->getRootSceneNode();
        if (mInputDevice->isKeyDown(KC_O))
        {
            node->yaw(rotScale);
        }
        if (mInputDevice->isKeyDown(KC_P))
        {
            node->yaw(-rotScale);
        }
        if (mInputDevice->isKeyDown(KC_I))
        {
            node->pitch(rotScale);
        }
        if (mInputDevice->isKeyDown(KC_K))
        {
            node->pitch(-rotScale);
        }

        if (mInputDevice->isKeyDown(KC_F) && timeUntilNextToggle <= 0)
        {
            mStatsOn = !mStatsOn;
            Root::getSingleton().showDebugOverlay(mStatsOn);

            timeUntilNextToggle = 1;
        }


        // Return true to continue rendering
        return true;
    }

};

// Listens to the mouse - can switch between buffered and unbuffered mode
class MouseFrameListener : public BaseFrameListener
{
private:
	bool mBuffered;

	EventProcessor* mEventProcessor;

public:

	MouseFrameListener(RenderWindow* win, Camera* cam, bool buffered = false) : BaseFrameListener(win, cam)
	{
		mBuffered = buffered;
	}

	~MouseFrameListener()
	{
		if(mEventProcessor!=NULL)
			delete mEventProcessor;
	}

	bool frameStarted(const FrameEvent& evt)
	{
		// local just to stop toggles flipping too fast
        static Real timeUntilNextToggle = 0;
        if (timeUntilNextToggle >= 0)
            timeUntilNextToggle -= evt.timeSinceLastFrame;


			// Copy the current state of the input devices
			mInputDevice->capture();

/*
			if(mInputDevice->isKeyDown(Ogre::KC_H) && timeUntilNextToggle <= 0) {
				if(mBuffered) {
					mBuffered = false;
					fprintf(stderr, "TRACE - BUFFERED MODE ON\n");
					timeUntilNextToggle = 1;
				} else {
					mBuffered = true;

					fprintf(stderr, "TRACE - BUFFERED MODE OFF\n");
					timeUntilNextToggle = 1;
				}
			}
*/

		if(mBuffered) {

			if(mInputDevice->isKeyDown(Ogre::KC_H) && timeUntilNextToggle <= 0) {
				Overlay* o = (Overlay*)OverlayManager::getSingleton().getByName("SS/Setup/HostScreen/Overlay");
				o->hide();
				//delete mEventProcessor;
				mBuffered = false;
				fprintf(stderr, "TRACE - BUFFERED MODE OFF\n");
				timeUntilNextToggle = 1;
			}

		} else { // not buffered: we can mouselook

			if(mInputDevice->isKeyDown(Ogre::KC_H) && timeUntilNextToggle <= 0) {
				Overlay* o = (Overlay*)OverlayManager::getSingleton().getByName("SS/Setup/HostScreen/Overlay");
				o->show();
				//OverlayManager::getSingleton().createCursorOverlay();

//            	OverlayManager::getSingleton().createCursorOverlay();

		mEventProcessor = new EventProcessor();
		mEventProcessor->initialise(mWindow);
		OverlayManager::getSingleton().createCursorOverlay();
		mEventProcessor->startProcessingEvents();

				mBuffered = true;
				fprintf(stderr, "TRACE - BUFFERED MODE ON\n");
				timeUntilNextToggle = 1;
			}

			// Rotate view by mouse relative position
        	float rotX, rotY;
        	rotX = -mInputDevice->getMouseRelativeX() * 0.13;
        	rotY = -mInputDevice->getMouseRelativeY() * 0.13;
			mCamera->yaw(rotX);
			mCamera->pitch(rotY);

		}



			return true;
	}

};

// TODO: find a proper way to do all this stuff (Aglanor)
class DimeFrameListener : public BaseFrameListener
{

private:

	OgreApplication* mOgreApplication;


public:

	DimeFrameListener(RenderWindow* win, Camera* cam, OgreApplication* app) : BaseFrameListener(win, cam)
	{
		mOgreApplication = app;
	}

	~DimeFrameListener() {}

	bool frameStarted(const FrameEvent& evt)
	{

		// local just to stop toggles flipping too fast
        static Real timeUntilNextToggle = 0;
        if (timeUntilNextToggle >= 0)
            timeUntilNextToggle -= evt.timeSinceLastFrame;


// Eris polling
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
// GNDN: MSVC < version 7 is broken
#else
		Eris::PollDefault::poll();
#endif

		// Copy the current state of the input devices
		mInputDevice->capture();
		
		// Pressing 1 queries the metaserver
		if(mInputDevice->isKeyDown(Ogre::KC_1)) {
			// TODO: use META_REFRESH here, passing a string like this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getMetaserverService()->runCommand("meta_refresh","");
		}
		
		// Pressing 2 connects to red.worldforge.org
		if(mInputDevice->isKeyDown(Ogre::KC_2) && timeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("connect","65.100.132.92");
			timeUntilNextToggle = 1;
		}

		// Pressing 3 logs in with the account 'ogretest'
		if(mInputDevice->isKeyDown(Ogre::KC_3) && timeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("login","ogretest ogretest");
			timeUntilNextToggle = 1;
		}

		// Pressing 4 takes the character ''
		if(mInputDevice->isKeyDown(Ogre::KC_4) && timeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("takechar","ogre_207");
			timeUntilNextToggle = 1;
			fprintf(stderr, "TRACE - LOGGED IN - OOOOOOOOOOOOOOOOOOOOOOOOOO");
			mOgreApplication->connectWorldSignals();
			timeUntilNextToggle = 1;
		}


		return true;
	}
};

class CameraFrameListener : public BaseFrameListener
{
protected:
	Camera* mCamera;
	SceneDetailLevel normal;
	SceneDetailLevel wireframe;
	SceneDetailLevel sdl;

public:

	CameraFrameListener(RenderWindow* win, Camera* cam) : BaseFrameListener(win, cam)
	{
	                mCamera = cam;
					normal = SDL_WIREFRAME;
					wireframe = SDL_WIREFRAME;
	}

	bool frameStarted(const FrameEvent& evt)
	{
        // local just to stop toggles flipping too fast
        static Real timeUntilNextToggle = 0;
        if (timeUntilNextToggle >= 0)
            timeUntilNextToggle -= evt.timeSinceLastFrame;

		// Copy the current state of the input devices
		mInputDevice->capture();

		if(mInputDevice->isKeyDown(Ogre::KC_Y) && timeUntilNextToggle <= 0) {

			// TODO: this is hackish. Fix it. (Aglanor)
			SceneDetailLevel sdl = mCamera->getDetailLevel();
			if(sdl!=wireframe) {
				normal = sdl;
				mCamera->setDetailLevel(SDL_WIREFRAME);

			} else {
				mCamera->setDetailLevel(normal);
			}

			timeUntilNextToggle = 1;
		}

		return true;
  }

};

class CameraRotator : public BaseFrameListener
{

protected:
        Camera* mCamera;
        SceneNode* mCentralNode;
        SceneNode* mRotatingNode;
        Vector3 mRotationAxis;
        Real mRotationSpeed;

public:
        CameraRotator(RenderWindow* win, Camera* cam, SceneNode* centralNode, Vector3 initialPosition) : BaseFrameListener(win, cam)
        {
                mCamera = cam;
                mCentralNode = centralNode;
                mRotationAxis = Vector3::UNIT_Y;
                mRotationSpeed = 60.0;

                // Create a node to act as the central rotation point
                mRotatingNode = dynamic_cast<SceneNode*>(mCentralNode->createChild());

                mRotatingNode->attachCamera(mCamera);
                mCamera->moveRelative(initialPosition);
                mCamera->lookAt(0, 0, 0);
        }

        ~CameraRotator()
        {
                delete mRotatingNode;
        }

		bool frameStarted(const FrameEvent& evt)
		{
			// Copy the current state of the input devices
			mInputDevice->capture();

			if(mInputDevice->isKeyDown(Ogre::KC_SPACE))
			mRotatingNode->rotate(mRotationAxis, mRotationSpeed * evt.timeSinceLastFrame);

			return true;
        }

};

void OgreApplication::createScene(void)
{
  mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
  // Create a light
  Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20,80,50);

  // create a Skydome
  mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // set the world geometry
  mSceneMgr->setWorldGeometry("terrain.cfg");

//#if 0

  //create the entity
  mShip = mSceneMgr->createEntity("blackdog", "squirel_of_doom.mesh");

  // create the node
  mShipNode = dynamic_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
  mShipNode->setPosition(10,25,128);
  mShipNode->setScale(0.01,0.01,0.01);

  // attach the node to the entity
  mShipNode->attachObject(mShip);
//#endif

	// a hack from the OGRE GUI sample
	Overlay* o = (Overlay*)OverlayManager::getSingleton().getByName("SS/Setup/HostScreen/Overlay");
	ActionTarget* at = static_cast<BorderButtonGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Join"));


	at->addActionListener(this);
	at = static_cast<BorderButtonGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/Exit"));
	at->addActionListener(this);


	ListChanger* list = static_cast<ListGuiElement*>(GuiManager::getSingleton().getGuiElement("SS/Setup/HostScreen/AvailableGamesList"));

	list->addListItem(new StringResource("test1"));
	list->addListItem(new StringResource("test2"));

	(GuiManager::getSingleton().getGuiElement("Core/CurrFps"))->addMouseListener(this);

	GuiContainer* pCursorGui = OverlayManager::getSingleton().getCursorGui();
	pCursorGui->setMaterialName("Cursor/default");
}

void OgreApplication::createFrameListener(void)
{

	DimeOgreFrameListener* dimeOgreFrameListener = new DimeOgreFrameListener(mWindow, mCamera, false, false);
	mRoot->addFrameListener(dimeOgreFrameListener);

	/*
	MouseFrameListener* mouseFrameListener = new MouseFrameListener(mWindow, mCamera, false);
	mRoot->addFrameListener(mouseFrameListener);
	mFrameListener= new TerrainListener(mWindow, mCamera);
	mRoot->addFrameListener(mFrameListener);
	CameraFrameListener* cameraFrameListener = new CameraFrameListener(mWindow, mCamera);
	mRoot->addFrameListener(cameraFrameListener);
	DimeFrameListener* dimeFrameListener = new DimeFrameListener(mWindow, mCamera, this);
	mRoot->addFrameListener(dimeFrameListener);*/

#if 0
    CameraRotator* cameraRotator = new CameraRotator(mWindow, mCamera, mShipNode, Vector3(0, 0, 100));
    mRoot->addFrameListener(cameraRotator);
#endif
}

void OgreApplication::createCamera(void)
{
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");

  // Position it at 500 in Z direction
  mCamera->setPosition(Vector3(128,25,75));

  // Look back along -Z
  mCamera->lookAt(Vector3(0,0,-300));
  mCamera->setNearClipDistance( 1 );
  // do not clip at far distance
  // so we can see the skydome
  //mCamera->setFarClipDistance( 384 );

}

void OgreApplication::initializeDimeServices(void)
{
	// Initialize dime services

	// Initialize the Logging service and an error observer
	dime::LoggingService *logging = dime::DimeServices::getInstance()->getLoggingService();
	CerrLogObserver* obs = new CerrLogObserver();
	obs->setFilter(dime::LoggingService::VERBOSE);
	logging->addObserver(obs);


	// Initialize the Configuration Service
	dime::DimeServices::getInstance()->getConfigService()->start();

	// Initialize the Sound Service
#ifndef WIN32
	// Test that /dev/dsp is availible
	FILE *temp = fopen("/dev/dsp","w");
	if (temp) {
	  fclose(temp);
#endif
	// Initialize the SoundService
	dime::DimeServices::getInstance()->getSoundService()->start();
#ifndef WIN32
	}
#endif

	// Initialize and start the Metaserver Service.
#if defined( _MSC_VER ) && ( _MSC_VER < 1300 )
	// GNDN: MSVC < version 7 is broken
#else
	// Set Eris Logging Level
	Eris::setLogLevel(Eris::LOG_DEBUG);

	dime::DimeServices::getInstance()->getMetaserverService()->start();

	// Initialize the Server Service
	dime::DimeServices::getInstance()->getServerService()->start();
#endif



}


void OgreApplication::connectWorldSignals(void) {

    /* Find out where the Eris world instance resides... */
    Eris::World *w = dime::DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &OgreApplication::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &OgreApplication::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &OgreApplication::entered ) );

    w->Appearance.connect( SigC::slot( *this, &OgreApplication::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &OgreApplication::disappearance ) );

}

/* Eris::World entity signals */

void OgreApplication::entityCreate( Eris::Entity *e )
{

	// create the ogre entity
	Entity* ogreEntity = mSceneMgr->createEntity(e->getID(), "squirel_of_doom.mesh");

	// create the ogre node
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	SceneNode* ogreNode = dynamic_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());

	// set the node position based on eris entity position
	WFMath::Point<3> position = e->getPosition();
	ogreNode->setPosition(position.x(),position.y(),position.z());
	ogreNode->setScale(0.05,0.05,0.05);

	// attach the node to the entity
	ogreNode->attachObject(ogreEntity);

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");

    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &OgreApplication::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &OgreApplication::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &OgreApplication::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &OgreApplication::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &OgreApplication::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &OgreApplication::say ), e ) );
}


void OgreApplication::entityDelete( Eris::Entity *e )
{}

void OgreApplication::entered( Eris::Entity *e )
{}

void OgreApplication::appearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - APPEARANCE - AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa");
}

void OgreApplication::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void OgreApplication::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void OgreApplication::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void OgreApplication::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void OgreApplication::say( const std::string &s, Eris::Entity *e )
{
	// TODO: fix this one
	/*
    dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
	*/
}

void OgreApplication::addedMember(Eris::Entity *e)
{}

void OgreApplication::removedMember(Eris::Entity *e)
{}

void OgreApplication::runCommand(const std::string &command, const std::string &args)
{
}




// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------
#if OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"


INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    OgreApplication app;

	// Initialize all dime services needed for this application
	app.initializeDimeServices();

    try {
        app.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occured: %s\n",
                e.getFullDescription().c_str());
#endif
    }


    return 0;
}
