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
-----------------------------------------------------------------------------
*/

/**
    \brief
        This file can be used as a starting point for building an OGRE application.
        I have deliberately compressed both declarations and definitions into 1 file
        to avoid name dependencies here (so you can easily rename this file), if you
        get serious you will definitely want to split all this stuff up into separate
        .h and .cpp files for tidiness, but this is just a jump-start.
*/

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

        // Rotate view by mouse relative position
        float rotX, rotY;
        rotX = -mInputDevice->getMouseRelativeX() * 0.13;
        rotY = -mInputDevice->getMouseRelativeY() * 0.13;


        // Make all the changes to the camera
        // Note that YAW direction is around a fixed axis (freelook stylee) rather than a natural YAW (e.g. airplane)
        mCamera->yaw(rotX);
        mCamera->pitch(rotY);
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

// TODO: find a proper way to do all this stuff (Aglanor)
class DimeFrameListener : public BaseFrameListener
{

public:

	DimeFrameListener(RenderWindow* win, Camera* cam) : BaseFrameListener(win, cam) {}

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
			dime::DimeServices::getInstance()->getServerService()->runCommand("connect","18.224.0.35");
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
		}

		// login ogretest ogretest
		


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
                mRotatingNode = mCentralNode->createChild();

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

void OgreApp::createScene(void)
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
  mShip = mSceneMgr->createEntity("robot", "robot.mesh");

  // create the node
  mShipNode = mSceneMgr->getRootSceneNode()->createChild();
  mShipNode->setPosition(10,25,128);
  mShipNode->setScale(0.2,0.2,0.2);

  // attach the node to the entity
  mShipNode->attachObject(mShip);
//#endif
}

void OgreApp::createFrameListener(void)
{
  mFrameListener= new TerrainListener(mWindow, mCamera);
  mRoot->addFrameListener(mFrameListener);
  CameraFrameListener* cameraFrameListener = new CameraFrameListener(mWindow, mCamera);
  mRoot->addFrameListener(cameraFrameListener);
  DimeFrameListener* dimeFrameListener = new DimeFrameListener(mWindow, mCamera);
  mRoot->addFrameListener(dimeFrameListener);
#if 0
    CameraRotator* cameraRotator = new CameraRotator(mWindow, mCamera, mShipNode, Vector3(0, 0, 100));
    mRoot->addFrameListener(cameraRotator);
#endif
}

void OgreApp::createCamera(void)
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
    OgreApp app;
	
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
