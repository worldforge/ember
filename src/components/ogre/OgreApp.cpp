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

#include "services/DimeServices.h"


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
#include "ExampleApplication.h"



class TerrainListener : public ExampleFrameListener
{
  public:
    TerrainListener(RenderWindow* win, Camera* cam) :ExampleFrameListener(win, cam) { };

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

class CameraRotator : public ExampleFrameListener
{

protected:
        Camera* mCamera;
        SceneNode* mCentralNode;
        SceneNode* mRotatingNode;
        Vector3 mRotationAxis;
        Real mRotationSpeed;

public:
        CameraRotator(RenderWindow* win, Camera* cam, SceneNode* centralNode, Vector3 initialPosition) : ExampleFrameListener(win, cam)
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


// ----------------------------------------------------------------------------
// Define the application object
// This is derived from ExampleApplication which is the class OGRE provides to
// make it easier to set up OGRE without rewriting the same code all the time.
// You can override extra methods of ExampleApplication if you want to further
// specialise the setup routine, otherwise the only mandatory override is the
// 'createScene' method which is where you set up your own personal scene.
// ----------------------------------------------------------------------------
class OgreApp : public ExampleApplication
{
public:
    // Basic constructor
    OgreApp() {}
	Entity* mShip;
	SceneNode* mShipNode;

protected:

    // Just override the mandatory create scene method
    void createScene(void)
    {


		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
		// Create a light
        Light* l = mSceneMgr->createLight("MainLight");
        l->setPosition(20,80,50);

		// create a Skydome
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

		// set the world geometry
		mSceneMgr->setWorldGeometry("terrain.cfg");
/*

		// create the entity
		mShip = mSceneMgr->createEntity("razor", "razor.mesh");

		// create the node
		mShipNode = mSceneMgr->getRootSceneNode()->createChild();

		// attach the node to the entity
		mShipNode->attachObject(mShip);
		*/

    }

	void createFrameListener(void)
    {
		mFrameListener= new TerrainListener(mWindow, mCamera);
		mRoot->addFrameListener(mFrameListener);
	/*
		CameraRotator* cameraRotator = new CameraRotator(mWindow, mCamera, mShipNode, Vector3(0, 0, 100));
        mRoot->addFrameListener(cameraRotator);
		*/

    }
	
	virtual void chooseSceneManager(void)
    {
        // Get the SceneManager
        mSceneMgr = mRoot->getSceneManager( ST_EXTERIOR_CLOSE );
    }

    virtual void createCamera(void)
    {
        // Create the camera
        mCamera = mSceneMgr->createCamera("PlayerCam");

        // Position it at 500 in Z direction
        mCamera->setPosition(Vector3(128,25,128));

        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance( 1 );
		// do not clip at far distance
		// so we can see the skydome
        //mCamera->setFarClipDistance( 384 );

    }



};








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
	
	// Initialize dime stuff
	dime::LoggingService *logging = dime::DimeServices::getInstance()->getLoggingService();
	


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
