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
#include "EntityListener.h"

#include "DimeEntity.h"

#include "AvatarController.h"

using namespace Ogre;

AvatarController* AvatarController::_instance = 0;

AvatarController& AvatarController::getSingleton(void)
{
	if(_instance == 0)
		_instance = new AvatarController;
	return *_instance;
}

AvatarController::AvatarController() : mEntityUnderCursor(NULL) , mSelectedEntity(NULL)
{
	mUpdateInterval = 1.0;
	mTimeToUpdate = mUpdateInterval;
	mMouseButton0Pressed = false;
	mMouseButton1Pressed = false;
	mMouseButton2Pressed = false;
}
AvatarController::~AvatarController()
{}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
}





void AvatarController::frameStarted(const FrameEvent & event, InputReader* inputReader) {
		
	//is this the correct order to check things?
	if (InputManager::getSingleton().isMouseUsed()) {
		checkMouseClicks(event, inputReader);
		checkMouseMovement(event, inputReader);
		
	}
	checkMovementKeys(event, inputReader);
	movementForFrame.timeSlice = event.timeSinceLastFrame;

	if (movementForFrame.isMoving) {
		std::string aString;
		Vector3 pos = mAvatar->mAvatarNode->getPosition();
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
	Real mouseX = InputManager::getSingleton().getMouseX();
	Real mouseY = InputManager::getSingleton().getMouseY();
	
	SceneManager* sceneManager = EntityListener::getSingleton().getSceneManager();
	// Start a new ray query 
	Ray cameraRay = mAvatar->getAvatar3pCamera()->getCameraToViewportRay( mouseX, mouseY ); 
	RaySceneQuery *raySceneQuery = sceneManager->createRayQuery( cameraRay ); 
	raySceneQuery->execute(); 
	RaySceneQueryResult result = raySceneQuery->getLastResults(); 
	   
	MovableObject *closestObject = NULL; 
	//in meters how far we can pick objects
	Real closestDistance = WF2OGRE(10000.0); //TODO:set this to something closer, 10000m is for debugging
	 
	std::list< RaySceneQueryResultEntry >::iterator rayIterator; 
	   
	for ( rayIterator = result.begin( ); 
		rayIterator != result.end( ); 
		rayIterator++ ) {
		//only pick entities that have a userobject attached
		if (( *rayIterator ).movable->getUserObject() != NULL) {
			//we don't want to pick ourselves
			if ( ( *rayIterator ).movable != mAvatar->mAvatarEntity) { 
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
			aString = "OgreID: " + dimeEntity->getOgreEntity()->getName();
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
	float mouseX = InputManager::getSingleton().getMouseX();
	float mouseY = InputManager::getSingleton().getMouseY();
	
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
			
	movementForFrame.rotationDegHoriz = diffX;
	movementForFrame.rotationDegVert = diffY;
//	mAvatar->attemptRotate(diffX * event.timeSinceLastFrame ,diffY * event.timeSinceLastFrame, event.timeSinceLastFrame);

}

void AvatarController::checkMovementKeys(const FrameEvent & event, InputReader* inputReader)
{
		//Real timePassed = event.timeSinceLastFrame;
		bool isRunning = InputManager::getSingleton().isKeyDown(KC_LSHIFT);

		Vector3 movement = Vector3::ZERO;
		bool isMovement = false; 

		// forwards / backwards
		if(InputManager::getSingleton().isKeyDown(KC_UP))  // W also, and same for the rest
		{
			movement.x = 1; 	//scale this
			isMovement = true;
		}
		else if(InputManager::getSingleton().isKeyDown(KC_DOWN))
		{
			movement.x = -1;		//scale
			isMovement = true;
		}

		// strafe
		if(InputManager::getSingleton().isKeyDown(KC_LEFT))
		{
			movement.z = -1;
			isMovement = true;
		}
		else if(InputManager::getSingleton().isKeyDown(KC_RIGHT))
		{
			movement.z = 1;
			isMovement = true;
		}

		// up down
		if(InputManager::getSingleton().isKeyDown(KC_PGDOWN))
		{
			movement.y = -1;
			isMovement = true;
		}
		else if(InputManager::getSingleton().isKeyDown(KC_PGUP))
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
			movementForFrame.movementDirection = Vector3::ZERO;
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

