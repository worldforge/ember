/*
    Copyright (C) 2004  Erik Hjortsberg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef AVATARCONTROLLER_H
#define AVATARCONTROLLER_H

//include Ogre stuff
#include <Ogre.h>
//#include <OgreFrameListener.h>
#include <OgreInput.h>
#include <OgreStringConverter.h>

#include "InputManager.h"


namespace DimeOgre {
class Avatar;
class DebugListener;
class DimeEntity;
class AvatarCamera;

class InputManager;
//class InputManager::MouseListener;

struct AvatarControllerMovement
{
	float rotationDegHoriz;
	float rotationDegVert;
	Ogre::Real timeSlice;
	Ogre::Vector3 movementDirection;
	bool isRunning;
	bool isMoving; 
	Ogre::Quaternion cameraOrientation;
};

class AvatarController : public InputManager::MouseListener
{
public:
	//static AvatarController & getSingleton(void);
    
    AvatarController(Avatar* avatar, InputManager* inputManager, Ogre::RenderWindow* window);

	virtual ~AvatarController();


	void frameStarted(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
//	bool frameEnded(const Ogre::FrameEvent & event);

	//
	//InputManager::MouseListener methods
	void mouseMoved(short newX, short newY, short oldX, short oldY);
	void mousePressed(unsigned char button);
	void mouseReleased(unsigned char button);
	
//TODO:this is temporary
//	void setSceneManager(Ogre::SceneManager* sceneManager) {mSceneManager = sceneManager;}

	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);
	
	AvatarCamera* getAvatarCamera() const;


protected:

	Ogre::RenderWindow* mWindow;

	/*
	 * Check what object is under the mouse and puts this in mEntityUnderCursor
	 */
	virtual DimeEntity* doMousePicking(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	virtual void checkMouseMovement(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	virtual void checkMovementKeys(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	virtual void checkMouseClicks(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);

	AvatarCamera* mAvatarCamera;
	void setAvatar(Avatar* avatar);

	

//TODO:this is temporary
	//Ogre::SceneManager* mSceneManager;

	bool mMouseButton0Pressed;
	bool mMouseButton1Pressed;
	bool mMouseButton2Pressed;


	/**
	 * Instance variable for singleton avatar controller implementation.
	 */
    //static AvatarController* _instance;
    
    /**
     * determines how often we should update to the server
     */
	float mUpdateInterval;
	
	/**
	 * rest until next update
	 */
	float mTimeToUpdate;
	
	/**
	 * Avatar
	 */
	Avatar* mAvatar;
	
	InputManager* mInputManager;
	
	//======================================================================
    // Disabled constructors and operators
    //======================================================================
 


    /**
     * Copy constructor not provided.
     */
/*    AvatarController( const AvatarController &source )
    {
    }
*/

    /**
     * Assignment operator not provided.
     */
/*    AvatarController &operator= ( const AvatarController &source )
    {
        return *this;
    }
  */  
    DimeEntity* mEntityUnderCursor;
    DimeEntity* mSelectedEntity;
    
    AvatarControllerMovement movementForFrame;
    
		
};

}

#endif // AvatarController_H
