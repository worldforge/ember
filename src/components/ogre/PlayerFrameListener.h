/*
PlayerFrameListener.h by Miguel Guzman (Aglanor)
Based on OGRE's ExampleFrameListener.h
(Follows ExampleFrameListener.h copyright notice)

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.8  2003-09-28 15:16:06  aglanor
 *      2003-09-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * /src/components/ogre/Makefile.am: removed the cal3d
 *              to Ogre converter, this app will no longer be here.
 *              All models will be converted from Cal3d to Open Model
 *              Format and then to Ogre, all of this outside Dime.
 *              (also, we don't need the cal3d library anymore,
 *              until we add a component that uses it, like Crystalspace).
 *
 *      Revision 1.7  2003/07/03 20:12:25  aglanor
 *      added sample animation to demo mesh
 *
 *      Revision 1.6  2003/05/07 23:28:43  aglanor
 *      Added a little more variety to the media. Now there's different media
 *      for settlers (malebuilder), pig (skeleton), merchant (robot) and
 *      everything else.
 *      The camera is set to the Avatar position/orientation when entering the game world.
 *
 *      Revision 1.5  2003/04/28 00:55:06  aglanor
 *      2003-04-28 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *      	* DimeOgre.h/cpp: added a Water Plane at height 0.
 *      		There is also a little ogre head marking (0,0,0).
 *      			The more detailed the scene, the easiest it is
 *      				to spot strange behaviors.
 *      					And it looks cool :)
 *
 *      Revision 1.4  2003/04/24 23:21:09  aglanor
 *      DimeOgre app is again linked back from the FrameListener, so entities sighted
 *      are created in the Ogre SceneManager (as squirrels, of course).
 *      I've also made the sample entity the ogrehead.mesh, you need to have it
 *      on your media dir too.
 *
 *      Revision 1.3  2003/04/24 21:12:49  aglanor
 *      removed ogre namespace
 *
 *      Revision 1.2  2003/04/24 20:02:08  aglanor
 *      Makefile and include changes after the renaming
 *
 *      Revision 1.1  2003/04/24 19:42:09  aglanor
 *      2003-04-24 Miguel Guzman <aglanor [at] telefonica [dot] net>
 *              * components/ogre: big cleanup. Files renamed.
 *

*/

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
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
             This frame listener just moves a specified camera around based on
             keyboard and mouse movements.
             Mouse:    Freelook
             W or Up:  Forward
             S or Down:Backward
             A:           Step left
             D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             O/P:       Yaw the root scene node (and it's children)
             I/K:       Pitch the root scene node (and it's children)
             F:           Toggle frame rate stats on/off
-----------------------------------------------------------------------------
*/

#ifndef __PlayerFrameListener_H__
#define __PlayerFrameListener_H__

#include "Ogre.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"

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


class PlayerFrameListener: public Ogre::FrameListener, public Ogre::KeyListener
{
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	PlayerFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam,
		DimeOgre* theApp,
		bool useBufferedInputKeys = false, bool useBufferedInputMouse = false)
	{
		// point back to the DimeOgre application to which this listener belongs
		mDimeOgre = theApp;

		mUseBufferedInputKeys = useBufferedInputKeys;
		mUseBufferedInputMouse = useBufferedInputMouse;
		mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;

		if (mInputTypeSwitchingOn)
		{
			mEventProcessor = new Ogre::EventProcessor();
			mEventProcessor->initialise(win);
			Ogre::OverlayManager::getSingleton().createCursorOverlay();
			mEventProcessor->startProcessingEvents();
			mEventProcessor->addKeyListener(this);
			mInputDevice = mEventProcessor->getInputReader();

		}
		else
		{
			mInputDevice = Ogre::PlatformManager::getSingleton().createInputReader();
			mInputDevice->initialise(win,true,true);
		}

		mCamera = cam;
		mWindow = win;
		mStatsOn = true;
		mNumScreenShots = 0;
		mTimeUntilNextToggle = 0;
	}

	virtual ~PlayerFrameListener()
	{
		if (mInputTypeSwitchingOn)
		{
			delete mEventProcessor;
		}
		else
		{
			Ogre::PlatformManager::getSingleton().destroyInputReader( mInputDevice );
		}
	}

	bool processUnbufferedKeyInput(const Ogre::FrameEvent& evt)
	{
		if (mInputDevice->isKeyDown(Ogre::KC_A))
		{
			// Move camera left
			mTranslateVector.x = -mMoveScale;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_D))
		{
			// Move camera RIGHT
			mTranslateVector.x = mMoveScale;
		}

		/* Move camera forward by keypress. */
		if (mInputDevice->isKeyDown(Ogre::KC_UP) || mInputDevice->isKeyDown(Ogre::KC_W) )
		{
			mTranslateVector.z = -mMoveScale;
		}

		/* Move camera forward by mousewheel. */
		if( mInputDevice->getMouseRelativeZ() > 0 )
		{
			mTranslateVector.z = -mMoveScale * 8.0;
		}

		/* Move camera backward by keypress. */
		if (mInputDevice->isKeyDown(Ogre::KC_DOWN) || mInputDevice->isKeyDown(Ogre::KC_S) )
		{
			mTranslateVector.z = mMoveScale;
		}

		/* Move camera backward by mouse wheel. */
		if( mInputDevice->getMouseRelativeZ() < 0 )
		{
			mTranslateVector.z = mMoveScale * 8.0;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_PGUP))
		{
			// Move camera up
			mTranslateVector.y = mMoveScale;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_PGDOWN))
		{
			// Move camera down
			mTranslateVector.y = -mMoveScale;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_RIGHT))
		{
			mCamera->yaw(-mRotScale);
		}

		if (mInputDevice->isKeyDown(Ogre::KC_LEFT))
		{
			mCamera->yaw(mRotScale);
		}

		if( mInputDevice->isKeyDown(Ogre::KC_ESCAPE))
		{
			return false;
		}

		// see if switching is on, and you want to toggle
		if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(Ogre::KC_M) && mTimeUntilNextToggle <= 0)
		{
			switchMouseMode();
			mTimeUntilNextToggle = 1;
		}

		if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(Ogre::KC_K) && mTimeUntilNextToggle <= 0)
		{
			// must be going from immediate keyboard to buffered keyboard
			switchKeyMode();
			mTimeUntilNextToggle = 1;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_F) && mTimeUntilNextToggle <= 0)
		{
			mStatsOn = !mStatsOn;
			//Ogre::Root::getSingleton().showDebugOverlay(mStatsOn);
			mTimeUntilNextToggle = 1;
		}

		if (mInputDevice->isKeyDown(Ogre::KC_SYSRQ) && mTimeUntilNextToggle <= 0)
		{
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
			mWindow->writeContentsToFile(tmp);
			mTimeUntilNextToggle = 0.5;
			mWindow->setDebugText(Ogre::String("Wrote ") + tmp);
		}

		/*****************************************
		Eris debug part
		******************************************/

		// Eris polling
		Eris::PollDefault::poll();

		// update animations
		mDimeOgre->updateAnimations(evt.timeSinceLastFrame);

		// Pressing 1 queries the metaserver
		if(mInputDevice->isKeyDown(Ogre::KC_1)) {
			// TODO: use META_REFRESH here, passing a string like this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getMetaserverService()->runCommand("meta_refresh","");
		}

		// Pressing 2 connects to red.worldforge.org
		if(mInputDevice->isKeyDown(Ogre::KC_2) && mTimeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("connect","127.0.0.1");
			std::cout << "Logged to indri" << std::endl;
			mTimeUntilNextToggle = 1;
		}

		// Pressing 3 logs in with the account 'ogretest'
		if(mInputDevice->isKeyDown(Ogre::KC_3) && mTimeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("login","aglanor aglanor");
			mTimeUntilNextToggle = 1;
		}

		// Pressing 4 takes the character ''
		if(mInputDevice->isKeyDown(Ogre::KC_4) && mTimeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			dime::DimeServices::getInstance()->getServerService()->runCommand("takechar","3");
			fprintf(stderr, "TRACE - LOGGED IN - OOOOOOOOOOOOOOOOOOOOOOOOOO");
			mDimeOgre->connectWorldSignals();	// connect Eris signals
			mTimeUntilNextToggle = 1;
		}

		// Pressing 5 creates an idiotic character
		if(mInputDevice->isKeyDown(Ogre::KC_5) && mTimeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			fprintf(stderr, "TRACE - CREATING CHARACTER");
			dime::DimeServices::getInstance()->getServerService()->runCommand("createchar","nada");
			fprintf(stderr, "TRACE - CHARACTER CREATION");
			mDimeOgre->connectWorldSignals();	// connect Eris signals
			mTimeUntilNextToggle = 1;
		}

		// Pressing 5 creates an idiotic character
		if(mInputDevice->isKeyDown(Ogre::KC_6) && mTimeUntilNextToggle <= 0) {
			// TODO: this is an ugly hack (Aglanor)
			fprintf(stderr, "TRACE - TOUCHING");
			dime::DimeServices::getInstance()->getServerService()->runCommand("touch","nada");
			fprintf(stderr, "TRACE - TOUCHED");
			mDimeOgre->connectWorldSignals();	// connect Eris signals
			mTimeUntilNextToggle = 1;
		}


		// Return true to continue rendering
		return true;
	}

	bool processUnbufferedMouseInput(const Ogre::FrameEvent& evt)
	{
		/* Rotation factors, may not be used if the second mouse button is pressed. */

		/* If the second mouse button is pressed, then the mouse movement results in
		sliding the camera, otherwise we rotate. */
		if( mInputDevice->getMouseButton( 1 ) )
		{
			mTranslateVector.x += mInputDevice->getMouseRelativeX() * 0.13;
			mTranslateVector.y -= mInputDevice->getMouseRelativeY() * 0.13;
		}
		else
		{
			mRotX = -mInputDevice->getMouseRelativeX() * 0.13;
			mRotY = -mInputDevice->getMouseRelativeY() * 0.13;
		}

		return true;
	}

	void moveCamera()
	{
		// Make all the changes to the camera
		// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
		mCamera->yaw(mRotX);
		mCamera->pitch(mRotY);
		mCamera->moveRelative(mTranslateVector);
	}

	// Override frameStarted event to process that (don't care about frameEnded)
	bool frameStarted(const Ogre::FrameEvent& evt)
	{

		if (!mInputTypeSwitchingOn)
		{
			mInputDevice->capture();
		}

		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
			if (mTimeUntilNextToggle >= 0)
				mTimeUntilNextToggle -= evt.timeSinceLastFrame;

			// If this is the first frame, pick a speed
			if (evt.timeSinceLastFrame == 0)
			{
				mMoveScale = 1;
				mRotScale = 0.1;
			}
			// Otherwise scale movement units by time passed since last frame
			else
			{
				// Move about 100 units per second,
				mMoveScale = 1.0 * evt.timeSinceLastFrame;
				// Take about 10 seconds for full rotation
				mRotScale = 36 * evt.timeSinceLastFrame;
			}
			mRotX = 0;
			mRotY = 0;
			mTranslateVector = Ogre::Vector3::ZERO;
		}

		if (mUseBufferedInputKeys)
		{
			// no need to do any processing here, it is handled by event processor and
			// you get the results as KeyEvents
		}
		else
		{
			if (processUnbufferedKeyInput(evt) == false)
			{
				return false;
			}
		}

		if (mUseBufferedInputMouse)
		{
			// no need to do any processing here, it is handled by event processor and
			// you get the results as MouseEvents
		}
		else
		{
			if (processUnbufferedMouseInput(evt) == false)
			{
				return false;
			}
		}

		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so update the movement vector
			moveCamera();
		}
		return true;
	}

	void switchMouseMode()
	{
		fprintf(stderr, "TRACE - SWITCHING MOUSE MODE\n");
		mUseBufferedInputMouse = !mUseBufferedInputMouse;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);

	}

	void switchKeyMode()
	{
		fprintf(stderr, "TRACE - SWITCHING KEYBOARD MODE\n");
		mUseBufferedInputKeys = !mUseBufferedInputKeys;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
	}

	void keyClicked(Ogre::KeyEvent* e)
	{
		if (e->getKeyChar() == 'm')
		{
			switchMouseMode();
		}
		else if (e->getKeyChar() == 'k')
		{

			switchKeyMode();
		}

	}
	void keyPressed(Ogre::KeyEvent* e) {}
	void keyReleased(Ogre::KeyEvent* e) {}

protected:
	DimeOgre* mDimeOgre;	// pointer back to the DimeOgre application
	Ogre::EventProcessor* mEventProcessor;
	Ogre::InputReader* mInputDevice;
	Ogre::Camera* mCamera;
	Ogre::Vector3 mTranslateVector;
	Ogre::RenderWindow* mWindow;
	bool mStatsOn;
	bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
	unsigned int mNumScreenShots;
	float mMoveScale;
	float mRotScale;
	// just to stop toggles flipping too fast
	Ogre::Real mTimeUntilNextToggle ;
	float mRotX, mRotY;


};

#endif
