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
#include "InputManager.h"
#include "DimeOgre.h"

#include "DimeEntity.h"
#include "DimePhysicalEntity.h"
#include "AvatarDimeEntity.h"
#include "AvatarCamera.h"
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

AvatarController::AvatarController(Avatar* avatar, InputManager* inputManager, Ogre::RenderWindow* window) 
: mEntityUnderCursor(NULL) 
, mSelectedEntity(NULL)
, mInputManager(inputManager)
, mWindow(window)
, mAvatarCamera(NULL)
{
	setAvatar(avatar);
	mUpdateInterval = 1.0;
	mTimeToUpdate = mUpdateInterval;
	mMouseButton0Pressed = false;
	mMouseButton1Pressed = false;
	mMouseButton2Pressed = false;
	//AvatarController* avatarController = this;
	//DimeOgre::getSingletonPtr()->getOgreRoot()->addFrameListener(avatarController);
	createAvatarCameras(avatar->getAvatarSceneNode());
	mInputManager->addMouseListener(this);
	mInputManager->setAvatarController(this);
	mAvatar->setAvatarController(this);

}
AvatarController::~AvatarController()
{}

void AvatarController::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatarCamera == NULL) {
		mAvatarCamera = new AvatarCamera(avatarSceneNode, DimeOgre::getSingletonPtr()->getSceneManager(), mWindow);
	} else {
		mAvatarCamera->setAvatarNode(avatarSceneNode);
	}
}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}





void AvatarController::frameStarted(const FrameEvent & event, InputReader* inputReader) {
		
	//is this the correct order to check things?
	if (mInputManager->isMouseUsed()) {
		checkMouseClicks(event, inputReader);
		checkMouseMovement(event, inputReader);
		
	}
	checkMovementKeys(event, inputReader);
	movementForFrame.timeSlice = event.timeSinceLastFrame;

	if (movementForFrame.isMoving) {
		std::string aString;
		Ogre::Vector3 pos = mAvatar->mAvatarNode->getPosition();
		aString += "X: " + StringConverter::toString((long)OGRE2WF(pos.x));
		aString += " Y: " + StringConverter::toString((long)OGRE2WF(pos.y));
		aString += " Z: " + StringConverter::toString((long)OGRE2WF(pos.z));
	
		Root::getSingleton().getAutoCreatedWindow()->setDebugText(aString);
	}

	
	mAvatar->updateFrame(movementForFrame);
	
	// check if a second has passed, then check for entity equailty, then make Avatar update the server
	// or something!
	mTimeToUpdate = mTimeToUpdate - event.timeSinceLastFrame;
	if(mTimeToUpdate<=0) {
		//std::cout << "	UPDATING INTO SERVER" << std::endl;
		mTimeToUpdate = mUpdateInterval;
	}


	
}

/*
 * Part of this is taken from code posted by bad_camel at the Ogre3d forums 2004
 */
DimeEntity* AvatarController::doMousePicking(const FrameEvent & event, InputReader* inputReader) 
{
	Real mouseX = mInputManager->getMouseX();
	Real mouseY = mInputManager->getMouseY();
	
	// Start a new ray query 
	Ray cameraRay = getAvatarCamera()->getCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	RaySceneQuery *raySceneQuery = DimeOgre::getSingletonPtr()->getSceneManager()->createRayQuery( cameraRay ); 
	raySceneQuery->execute(); 
	RaySceneQueryResult result = raySceneQuery->getLastResults(); 
	   
	MovableObject *closestObject = NULL; 
	//in meters how far we can pick objects
	Real closestDistance = WF2OGRE(10000.0); //TODO:set this to something closer, 10000m is for debugging
	 
	std::list< RaySceneQueryResultEntry >::iterator rayIterator; 
	   
	Ogre::UserDefinedObject* avatarObject = dynamic_cast<Ogre::UserDefinedObject*>(mAvatar->mErisAvatarEntity);
	for ( rayIterator = result.begin( ); 
		rayIterator != result.end( ); 
		rayIterator++ ) {
		//only pick entities that have a userobject attached
		if (( *rayIterator ).movable->getUserObject() != NULL) {
			//we don't want to pick ourselves
			if ( ( *rayIterator ).movable->getUserObject() != avatarObject) { 
				if ( ( *rayIterator ).distance < closestDistance ) { 
					closestObject = ( *rayIterator ).movable; 
					closestDistance = ( *rayIterator ).distance; 
				}
			} 
		}
	} 
 
   // No movable clicked 
	if ( closestObject == NULL ) {    
     // Nada! 
	     mEntityUnderCursor == NULL;
	     return NULL;
		Root::getSingleton().getAutoCreatedWindow()->setDebugText("none");
	} else { 
		mEntityUnderCursor = dynamic_cast<DimeEntity*>(closestObject->getUserObject());
		return  mEntityUnderCursor;
     // Yippee! 
	} 		
	
}

void AvatarController::checkMouseClicks(const FrameEvent & event, InputReader* inputReader) {

	if (mMouseButton0Pressed) {
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
	} else if (mMouseButton1Pressed) {
		//we should expand this into some kind of chooser of what action will be taken
		//perhaps by using a small dropdownlist
		//waiting for CEGUI...
		DimeEntity* dimeEntity = doMousePicking(event, inputReader);
		if (dimeEntity != NULL) {
			mAvatar->touch(dimeEntity);
		}
	}
	
}

void AvatarController::checkMouseMovement(const FrameEvent & event, InputReader* inputReader) {
 	//TODO refactor into a 3d person view class thing
 	
 	/*this is in percent how much of the border areas that are "hot", i.e. makes the 
 	 * view rotate when the mouse moves over them.
 	 * Think of it as a picture frame.
 	 */
	const float sizeOfHotBorder = .25;

	/*this is in percent how much of the border that makes the movement max out
	 */										
	const float sizeOfMaxHotBorder = .05;
	
	/* Max movement per second. I guess this is in degrees, but I'm not sure. 
	 * Seemed as a nice value though
	 */
	const float maxMovement = 50; 
	
	int screenX = Root::getSingleton().getAutoCreatedWindow()->getWidth();
	int screenY = Root::getSingleton().getAutoCreatedWindow()->getHeight();
	
	//we must use this obscure method, else all other overlays would "obstruct"
	//the mouse pointer, giving us no data
	float mouseX = mInputManager->getMouseX();
	float mouseY = mInputManager->getMouseY();
	
	float diffX = 0.0, diffY = 0.0;
	
	
	//this calculates how close the pointer is to the border and determines how
	//much we should move
	if (mouseX <= sizeOfHotBorder) {
		diffX = (mouseX <= sizeOfMaxHotBorder) ? (maxMovement) : maxMovement * ((sizeOfHotBorder - mouseX) / sizeOfHotBorder);
	} else if (mouseX >= 1 - sizeOfHotBorder) {
		diffX = (1.0 - mouseX <= sizeOfMaxHotBorder) ? (-maxMovement) : -((sizeOfHotBorder + (mouseX - 1.0)) / sizeOfHotBorder) * maxMovement;
	}	
	if (mouseY <= sizeOfHotBorder) {
		diffY = (mouseY <= sizeOfMaxHotBorder) ? (maxMovement) : maxMovement * ((sizeOfHotBorder - mouseY) / sizeOfHotBorder);
	} else if (mouseY >= 1 - sizeOfHotBorder) {
		diffY = (1.0 - mouseY <= sizeOfMaxHotBorder) ? (-maxMovement) : -((sizeOfHotBorder + (mouseY - 1.0)) / sizeOfHotBorder) * maxMovement;
	}	
			
//	movementForFrame.rotationDegHoriz = diffX;
//	movementForFrame.rotationDegVert = diffY;
//	mAvatar->attemptRotate(diffX * event.timeSinceLastFrame ,diffY * event.timeSinceLastFrame, event.timeSinceLastFrame);
	
	//we do the camera pitch instantly and correct the avatar to the new position 
	//when it's suitable
	mAvatar->getAvatarCamera()->yaw(diffX * event.timeSinceLastFrame);
	mAvatar->getAvatarCamera()->pitch(diffY * event.timeSinceLastFrame);
//	movementForFrame.cameraOrientation = mAvatar->getAvatarCamera()->getOrienation();
}

void AvatarController::checkMovementKeys(const FrameEvent & event, InputReader* inputReader)
{
		//Real timePassed = event.timeSinceLastFrame;
		bool isRunning = mInputManager->isKeyDown(KC_LSHIFT);

		Ogre::Vector3 movement = Ogre::Vector3::ZERO;
		bool isMovement = false; 

		// forwards / backwards
		if(mInputManager->isKeyDown(KC_UP))  // W also, and same for the rest
		{
			movement.x = 1; 	//scale this
			isMovement = true;
		}
		else if(mInputManager->isKeyDown(KC_DOWN))
		{
			movement.x = -1;		//scale
			isMovement = true;
		}

		// strafe
		if(mInputManager->isKeyDown(KC_LEFT))
		{
			movement.z = -1;
			isMovement = true;
		}
		else if(mInputManager->isKeyDown(KC_RIGHT))
		{
			movement.z = 1;
			isMovement = true;
		}

		// up down
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

AvatarCamera* AvatarController::getAvatarCamera() const
{
	return mAvatarCamera;	
}


}

