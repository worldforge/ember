/*
PlayerFrameListener.h by Miguel Guzman (Aglanor)
Based on OGRE's ExampleFrameListener.h
(Follows ExampleFrameListener.h copyright notice)

 *  Change History (most recent first):
 *
 *      $Log$
 *      Revision 1.2  2003-04-24 20:02:08  aglanor
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

using namespace Ogre;

class PlayerFrameListener: public FrameListener, public KeyListener
{
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	PlayerFrameListener(RenderWindow* win, Camera* cam, bool useBufferedInputKeys = false, bool useBufferedInputMouse = false)
	{
		mUseBufferedInputKeys = useBufferedInputKeys;
		mUseBufferedInputMouse = useBufferedInputMouse;
		mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;

		if (mInputTypeSwitchingOn)
		{
			mEventProcessor = new EventProcessor();
			mEventProcessor->initialise(win);
			OverlayManager::getSingleton().createCursorOverlay();
			mEventProcessor->startProcessingEvents();
			mEventProcessor->addKeyListener(this);
			mInputDevice = mEventProcessor->getInputReader();

		}
		else
		{
			mInputDevice = PlatformManager::getSingleton().createInputReader();
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
			PlatformManager::getSingleton().destroyInputReader( mInputDevice );
		}
	}

    bool processUnbufferedKeyInput(const FrameEvent& evt)
    {
        if (mInputDevice->isKeyDown(KC_A))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_D))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W) )
        {
            mTranslateVector.z = -mMoveScale;
        }
        /* Move camera forward by mousewheel. */
        if( mInputDevice->getMouseRelativeZ() > 0 )
        {
            mTranslateVector.z = -mMoveScale * 8.0;
        }

        /* Move camera backward by keypress. */
        if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S) )
        {
            mTranslateVector.z = mMoveScale;
        }

        /* Move camera backward by mouse wheel. */
        if( mInputDevice->getMouseRelativeZ() < 0 )
        {
            mTranslateVector.z = mMoveScale * 8.0;
        }

        if (mInputDevice->isKeyDown(KC_PGUP))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGDOWN))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_RIGHT))
        {
            mCamera->yaw(-mRotScale);
        }
		
        if (mInputDevice->isKeyDown(KC_LEFT))
        {
            mCamera->yaw(mRotScale);
        }

        if( mInputDevice->isKeyDown( KC_ESCAPE) )
        {            
            return false;
        }

		// see if switching is on, and you want to toggle
		if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
		{
			switchMouseMode();
			mTimeUntilNextToggle = 1;
		}

		if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
		{
			// must be going from immediate keyboard to buffered keyboard
			switchKeyMode();
			mTimeUntilNextToggle = 1;
		}

		if (mInputDevice->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
		{
			mStatsOn = !mStatsOn;
			Root::getSingleton().showDebugOverlay(mStatsOn);
			mTimeUntilNextToggle = 1;
		}

		if (mInputDevice->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
		{
			char tmp[20];
			sprintf(tmp, "screenshot_%d.png", ++mNumScreenShots);
			mWindow->writeContentsToFile(tmp);
			mTimeUntilNextToggle = 0.5;
			mWindow->setDebugText(String("Wrote ") + tmp);
		}

		// Return true to continue rendering
		return true;
	}

	bool processUnbufferedMouseInput(const FrameEvent& evt)
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
	bool frameStarted(const FrameEvent& evt)
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
				mMoveScale = 100.0 * evt.timeSinceLastFrame;
				// Take about 10 seconds for full rotation
				mRotScale = 36 * evt.timeSinceLastFrame;
			}
			mRotX = 0;
			mRotY = 0;
			mTranslateVector = Vector3::ZERO;
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

	void keyClicked(KeyEvent* e)
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
	void keyPressed(KeyEvent* e) {}
	void keyReleased(KeyEvent* e) {}

protected:
	EventProcessor* mEventProcessor;
	InputReader* mInputDevice;
	Camera* mCamera;
	Vector3 mTranslateVector;
	RenderWindow* mWindow;
	bool mStatsOn;
	bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
	unsigned int mNumScreenShots;
	float mMoveScale;
	float mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	float mRotX, mRotY;

};

#endif
