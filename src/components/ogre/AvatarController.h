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

class GUIManager;

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

class AvatarController 
: public Ogre::KeyListener, public Ogre::FrameListener
//: public InputManager::MouseListener
{
public:
	//static AvatarController & getSingleton(void);
    
    AvatarController(Avatar* avatar, Ogre::RenderWindow* window, GUIManager* guiManager);

	virtual ~AvatarController();


	void frameStarted(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);
	
	
	bool frameStarted(const Ogre::FrameEvent & event);

	

	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);
	
	AvatarCamera* getAvatarCamera() const;

	
	virtual void 	keyPressed (Ogre::KeyEvent *e);
	virtual void 	keyReleased (Ogre::KeyEvent *e);
	// do-nothing events
	virtual void 	keyClicked (Ogre::KeyEvent *e) {}
	

protected:

	Ogre::RenderWindow* mWindow;
	
	GUIManager* mGUIManager;


	void checkMovementKeys(const Ogre::FrameEvent & event, Ogre::InputReader* inputReader);


	AvatarCamera* mAvatarCamera;
	void setAvatar(Avatar* avatar);

	
	/**
	 * Avatar
	 */
	Avatar* mAvatar;
	
    DimeEntity* mEntityUnderCursor;
    DimeEntity* mSelectedEntity;
    
    AvatarControllerMovement movementForFrame;
	
	Ogre::KeyCode mKeyCodeForForwardMovement;
	Ogre::KeyCode mKeyCodeForBackwardsMovement;
	Ogre::KeyCode mKeyCodeForLeftMovement;
	Ogre::KeyCode mKeyCodeForRightMovement;
	
	
	
    
		
};

}

#endif // AvatarController_H
