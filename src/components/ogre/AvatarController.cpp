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

#include "OgreStringConverter.h"


#include "Avatar.h"
#include "DebugListener.h"
#include "DimeOgre.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "PersonDimeEntity.h"
#include "AvatarDimeEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
#include "AvatarController.h"

using namespace Ogre;
namespace DimeOgre {

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
	//DimeOgre::getSingletonPtr()->getOgreRoot()->addFrameListener(avatarController);
	createAvatarCameras(avatar->getAvatarSceneNode());
	mAvatar->setAvatarController(this);
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	
	mKeyCodeForForwardMovement = Ogre::KC_W;
	mKeyCodeForBackwardsMovement = Ogre::KC_S;
	mKeyCodeForLeftMovement = Ogre::KC_A;
	mKeyCodeForRightMovement = Ogre::KC_D;

}
AvatarController::~AvatarController()
{}

void AvatarController::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatarCamera == NULL) {
		mAvatarCamera = new AvatarCamera(avatarSceneNode, DimeOgre::getSingletonPtr()->getSceneManager(), mWindow, mGUIManager);
	} else {
		mAvatarCamera->setAvatarNode(avatarSceneNode);
	}
}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}





void AvatarController::frameStarted(const FrameEvent & event, InputReader* inputReader) {
		
/*
 * 	if (inputReader->isKeyDown(KC_F7)) {
		DimeEntity* dimeEntity = doMousePicking(event, inputReader);
		if ( dimeEntity == NULL ) {    
	     	// Nada! 
			Root::getSingleton().getAutoCreatedWindow()->setDebugText("none");
		} else { 
			std::string aString;
			aString = "OgreID: " + dimeEntity->getSceneNode()->getName();
			aString += " ErisID: " + dimeEntity->getID();
			aString += " ErisName: " + dimeEntity->getName();
			Root::getSingleton().getAutoCreatedWindow()->setDebugText(aString);
		} 	
	}
	
	*/
	//is this the correct order to check things?
/*	if (mInputManager->isMouseUsed()) {
		checkMouseClicks(event, inputReader);
		//mAvatarCamera->updateFromMouseMovement(event, mInputManager);
		
	}
	*/
//	checkMovementKeys(event, inputReader);
	//movementForFrame.timeSlice = event.timeSinceLastFrame;
/*
	if (movementForFrame.isMoving) {
		std::string aString;
		Ogre::Vector3 pos = mAvatar->mAvatarNode->getPosition();
		aString += "X: " + StringConverter::toString((long)OGRE2WF(pos.x));
		aString += " Y: " + StringConverter::toString((long)OGRE2WF(pos.y));
		aString += " Z: " + StringConverter::toString((long)OGRE2WF(pos.z));
	
		Root::getSingleton().getAutoCreatedWindow()->setDebugText(aString);
	}

	
	//mAvatar->updateFrame(movementForFrame);
	
	// check if a second has passed, then check for entity equailty, then make Avatar update the server
	// or something!
	mTimeToUpdate = mTimeToUpdate - event.timeSinceLastFrame;
	if(mTimeToUpdate<=0) {
		//std::cout << "	UPDATING INTO SERVER" << std::endl;
		mTimeToUpdate = mUpdateInterval;
	}

*/
	
}





void AvatarController::keyPressed (Ogre::KeyEvent *e)
{
/*	if (mKeyCodeForForwardMovement)
		mIsForwardMovement = true;
		
	if (mKeyCodeForBackwardsMovement)
		mIsBackwardsMovement = true;
		
	if (mKeyCodeForLeftMovement)
		mIsLeftMovement = true;
		
	if (mKeyCodeForRightMovement)
		mIsRightMovement = true;
	*/
}


void AvatarController::keyReleased (Ogre::KeyEvent *e)
{
// 	if (mKeyCodeForForwardMovement)
// 		mIsForwardMovement = false;
// 		
// 	if (mKeyCodeForBackwardsMovement)
// 		mIsBackwardsMovement = false;
// 		
// 	if (mKeyCodeForLeftMovement)
// 		mIsLeftMovement = false;
// 		
// 	if (mKeyCodeForRightMovement)
// 		mIsRightMovement = false;

}


bool AvatarController::frameStarted(const Ogre::FrameEvent& event)
{
	movementForFrame.movementDirection = Ogre::Vector3::ZERO;
	movementForFrame.isRunning = false;
	movementForFrame.isMoving = false;	
	
	if (mGUIManager->isInMovementKeysMode()) {
		checkMovementKeys(event, mGUIManager->getEventProcessor()->getInputReader());
	}	
	movementForFrame.timeSlice = event.timeSinceLastFrame;
	
	mAvatar->updateFrame(movementForFrame);
	

	return true;
}



void AvatarController::checkMovementKeys(const FrameEvent & event, InputReader* inputReader)
{
		//Real timePassed = event.timeSinceLastFrame;
		bool isRunning = inputReader->isKeyDown(KC_LSHIFT);

		Ogre::Vector3 movement = Ogre::Vector3::ZERO;
		bool isMovement = false; 

		// forwards / backwards
		if(inputReader->isKeyDown(mKeyCodeForForwardMovement))  // W also, and same for the rest
		{
			movement.x = 1; 	//scale this
			isMovement = true;
		}
		else if(inputReader->isKeyDown(mKeyCodeForBackwardsMovement))
		{
			movement.x = -1;		//scale
			isMovement = true;
		}

		// strafe
		if(inputReader->isKeyDown(mKeyCodeForLeftMovement))
		{
			movement.z = -1;
			isMovement = true;
		}
		else if(inputReader->isKeyDown(mKeyCodeForRightMovement))
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

