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
Filename:    BaseFrameListener.h
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

#include "Ogre.h"

using namespace Ogre;

class BaseFrameListener: public FrameListener
{
public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    BaseFrameListener(RenderWindow* win, Camera* cam)
    {
        mInputDevice = PlatformManager::getSingleton().createInputReader();
        mInputDevice->initialise(win,true, true);
        mCamera = cam;
        mWindow = win;
        mStatsOn = true;
    }
    virtual ~BaseFrameListener()
    {
        PlatformManager::getSingleton().destroyInputReader( mInputDevice );
    }

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
            moveScale = 100.0 * evt.timeSinceLastFrame;
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

protected:
    InputReader* mInputDevice;
    Camera* mCamera;
    RenderWindow* mWindow;
    bool mStatsOn;

};

