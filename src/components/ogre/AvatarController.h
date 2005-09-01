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
#include "EmberOgrePrerequisites.h"
//#include <OgreFrameListener.h>
#include <OgreInput.h>
#include <OgreStringConverter.h>

#include <SDL.h>



namespace EmberOgre {
class Avatar;
class EmberEntity;
class AvatarCamera;

class GUIManager;

class InputManager;
class Input;
class AvatarController;

/**
The movement mode of the avatar, run or walk.
*/
class AvatarMovementMode {
public:	
	enum Mode
	{
		MM_WALK = 0,
		MM_RUN = 1
	};
};
	
	
/**
Used for sending the current desired movement to the actual avatar.
*/
struct AvatarControllerMovement
{
	AvatarControllerMovement();
	
	float rotationDegHoriz;
	float rotationDegVert;
	Ogre::Real timeSlice;
	Ogre::Vector3 movementDirection;
	AvatarMovementMode::Mode mode;
	bool isMoving; 
	Ogre::Quaternion cameraOrientation;
};

class AvatarController
: public Ogre::FrameListener
{
public:
    
    

    
    
    AvatarController(Avatar* avatar, Ogre::RenderWindow* window, GUIManager* guiManager);

	virtual ~AvatarController();


	bool frameStarted(const Ogre::FrameEvent & event);
	
	
	/**
	Emitted when the movement mode changes between run and walk.
	*/
	SigC::Signal1<void, AvatarMovementMode::Mode> EventMovementModeChanged;

	

	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);
	
	AvatarCamera* getAvatarCamera() const;

	void detachCamera();
	void attachCamera();
	
	const AvatarControllerMovement& getCurrentMovement() const;


protected:

	Ogre::RenderWindow* mWindow;
	
	GUIManager* mGUIManager;


	void checkMovementKeys(const Ogre::FrameEvent & event, const Input* input);


	AvatarCamera* mAvatarCamera;
	void setAvatar(Avatar* avatar);

	
	/**
	 * Avatar
	 */
	Avatar* mAvatar;
	
    EmberEntity* mEntityUnderCursor;
    EmberEntity* mSelectedEntity;
    
    AvatarControllerMovement movementForFrame, mPreviousMovementForFrame;
	
	SDLKey mKeyCodeForForwardMovement;
	SDLKey mKeyCodeForBackwardsMovement;
	SDLKey mKeyCodeForLeftMovement;
	SDLKey mKeyCodeForRightMovement;
	
	Ogre::SceneNode* mFreeFlyingCameraNode;
	bool mIsAttached;
	
    
		
};



}

#endif // AvatarController_H
