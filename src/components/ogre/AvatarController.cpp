/*
    Copyright (C) 2004  Erik Hjortsberg
    some parts Copyright (C) 2004 bad_camel at Ogre3d forums

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



#include "AvatarController.h"

#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarEmberEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
#include "OgreStringConverter.h"
#include "Avatar.h"
#include "DebugListener.h"
#include "EmberOgre.h"

#include "EmberSceneManager/include/EmberTerrainSceneManager.h"

#include "input/Input.h"

using namespace Ogre;
namespace EmberOgre {

/*
AvatarController* AvatarController::_instance = 0;

AvatarController& AvatarController::getSingleton(void)
{
	if(_instance == 0)
		_instance = new AvatarController;
	return *_instance;
}

*/

AvatarController::AvatarController(Avatar* avatar, Ogre::RenderWindow* window, GUIManager* guiManager) 
: mEntityUnderCursor(NULL) 
, mSelectedEntity(NULL)
, mGUIManager(guiManager)
, mWindow(window)
, mAvatarCamera(NULL)
{
	setAvatar(avatar);
	
	//AvatarController* avatarController = this;
	//EmberOgre::getSingletonPtr()->getOgreRoot()->addFrameListener(avatarController);
	createAvatarCameras(avatar->getAvatarSceneNode());
	mAvatar->setAvatarController(this);
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	
	mKeyCodeForForwardMovement = SDLK_w;
	mKeyCodeForBackwardsMovement = SDLK_s;
	mKeyCodeForLeftMovement = SDLK_a;
	mKeyCodeForRightMovement = SDLK_d;

}
AvatarController::~AvatarController()
{}

void AvatarController::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatarCamera == NULL) {
		mAvatarCamera = new AvatarCamera(avatarSceneNode, EmberOgre::getSingletonPtr()->getSceneManager(), mWindow, mGUIManager);
	} else {
		mAvatarCamera->setAvatarNode(avatarSceneNode);
	}
}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}













bool AvatarController::frameStarted(const Ogre::FrameEvent& event)
{

	//getAvatarCamera()->yaw(Ogre::Degree(1.0f));
	movementForFrame.movementDirection = Ogre::Vector3::ZERO;
	movementForFrame.isRunning = false;
	movementForFrame.isMoving = false;	
	
	if (mGUIManager->isInMovementKeysMode()) {
		checkMovementKeys(event, mGUIManager->getInput());
	}	
	movementForFrame.timeSlice = event.timeSinceLastFrame;
	
	mAvatar->updateFrame(movementForFrame);
	

	return true;
}



void AvatarController::checkMovementKeys(const FrameEvent & event, const Input* input)
{
		//Real timePassed = event.timeSinceLastFrame;
		bool isRunning = input->isKeyDown(SDLK_RSHIFT) || input->isKeyDown(SDLK_LSHIFT);

		Ogre::Vector3 movement = Ogre::Vector3::ZERO;
		bool isMovement = false; 

		// forwards / backwards
		if(input->isKeyDown(mKeyCodeForForwardMovement))  // W also, and same for the rest
		{
			movement.x = 1; 	//scale this
			isMovement = true;
		}
		else if(input->isKeyDown(mKeyCodeForBackwardsMovement))
		{
			movement.x = -1;		//scale
			isMovement = true;
		}

		// strafe
		if(input->isKeyDown(mKeyCodeForLeftMovement))
		{
			movement.z = -1;
			isMovement = true;
		}
		else if(input->isKeyDown(mKeyCodeForRightMovement))
		{
			movement.z = 1;
			isMovement = true;
		}

/*		// up down
		if(mInputManager->isKeyDown(KC_PGDOWN))
		{
			movement.y = -1;
			isMovement = true;
		}
		else if(mInputManager->isKeyDown(KC_PGUP))
		{
			movement.y = 1;
			isMovement = true;
		}
*/		
		if(isMovement)
		{
			movementForFrame.movementDirection = movement;
			movementForFrame.isRunning = isRunning;
			movementForFrame.isMoving = true;
			
//			mAvatar->attemptMove(movement, isRunning);
		} else {
			movementForFrame.movementDirection = Ogre::Vector3::ZERO;
			movementForFrame.isRunning = false;
			movementForFrame.isMoving = false;

//			mAvatar->attemptStop();
		}
}

/*
void AvatarController::mouseMoved(short newX, short newY, short oldX, short oldY)
{
	//we don't need this
}

void AvatarController::mousePressed(unsigned char button)
{
	if (button == 0) {
		mMouseButton0Pressed = true;
	} else if (button == 1) {
		mMouseButton1Pressed = true;
	} else {
		mMouseButton2Pressed = true;
	}
		
//	fprintf(stderr, "PRESSED A MOUSE BUTTOn\n");
}

void AvatarController::mouseReleased(unsigned char button)
{
	if (button == 0) {
		mMouseButton0Pressed = false;
	} else if (button == 1) {
		mMouseButton1Pressed = false;
	} else {
		mMouseButton2Pressed = false;
	}
//	fprintf(stderr, "RELEASED A MOUSE BUTTOn\n");
}
*/
AvatarCamera* AvatarController::getAvatarCamera() const
{
	return mAvatarCamera;	
}


}

