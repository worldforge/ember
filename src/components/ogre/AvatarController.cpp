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
#include "AvatarEmberEntity.h"
#include "AvatarCamera.h"
#include "GUIManager.h"
#include "Avatar.h"
#include "EmberOgre.h"
#include "SceneManagers/EmberPagingSceneManager/include/EmberPagingSceneManager.h"


#include "input/Input.h"

using namespace Ogre;
namespace EmberOgre {


AvatarControllerMovement::AvatarControllerMovement() :	rotationDegHoriz(0),
	rotationDegVert(0),
	timeSlice(0),
	movementDirection(Ogre::Vector3::ZERO),
	isMoving(false)
{
}

AvatarController::AvatarController(Avatar* avatar, Ogre::RenderWindow* window, GUIManager* guiManager, Ogre::Camera* camera) 
: mEntityUnderCursor(0) 
, mSelectedEntity(0)
, mGUIManager(guiManager)
, mWindow(window)
, mAvatarCamera(0)
, mCamera(camera)
, mMovementCommandMapper("gui", "key_bindings_key")
{
	
	setAvatar(avatar);
	
	
	mAvatar->setAvatarController(this);
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	
	mKeyCodeForForwardMovement = SDLK_w;
	mKeyCodeForBackwardsMovement = SDLK_s;
	mKeyCodeForLeftMovement = SDLK_a;
	mKeyCodeForRightMovement = SDLK_d;
	mFreeFlyingCameraNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mFreeFlyingCameraNode->setPosition(0,30,0);
	detachCamera();
	
	EmberOgre::getSingleton().getInput().EventKeyReleased.connect(sigc::mem_fun(*this, &AvatarController::input_KeyReleased));
	
	mMovementCommandMapper.bindToInput(EmberOgre::getSingleton().getInput());
}
AvatarController::~AvatarController()
{}

void AvatarController::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatarCamera == 0) {
		mAvatarCamera = new AvatarCamera(avatarSceneNode, EmberOgre::getSingletonPtr()->getSceneManager(), mWindow, mGUIManager, mCamera);
	} else {
		attachCamera();
	}
}

void AvatarController::setAvatar(Avatar* avatar)
{
	mAvatar = avatar;
	createAvatarCameras(avatar->getAvatarSceneNode());
	attachCamera();
}





void AvatarController::detachCamera() 
{
	mIsAttached = false;
	mAvatarCamera->attach(mFreeFlyingCameraNode);
	//mAvatarCamera->setMode(AvatarCamera::MODE_FIRST_PERSON);
}


void AvatarController::attachCamera() 
{
	mIsAttached = true;
	mAvatarCamera->attach(mAvatar->getAvatarSceneNode());
	//mAvatarCamera->setMode(AvatarCamera::MODE_FIRST_PERSON);
}

void  AvatarController::input_KeyReleased(const SDL_keysym& keysym, Input::InputMode mode)
{
	if (keysym.sym == SDLK_F6) {
		if (mIsAttached) {
			detachCamera();
		} else {
			attachCamera();
		}
	}
}


bool AvatarController::frameStarted(const Ogre::FrameEvent& event)
{

// 	EmberPagingSceneManager* mScnMgr = static_cast<EmberPagingSceneManager*>(EmberOgre::getSingleton().getSceneManager());
// 	if (mGUIManager->getInput()->isKeyDown(SDLK_F4)) {
// /*		Real change;
// 		mScnMgr->getOption( "MaxPixelError", &change );
// 		change -= 0.5f;
// 		mScnMgr->setOption( "MaxPixelError", &change ); */
// 		--Ogre::PagingLandScapeOptions::getSingleton().maxPixelError;
// 		Ogre::PagingLandScapeOptions::getSingleton().calculateCFactor();
// 		mScnMgr->WorldDimensionChange();
// 	}
// 	if (mGUIManager->getInput()->isKeyDown(SDLK_F5)) {
// /*		Real change;
// 		mScnMgr->getOption( "MaxPixelError", &change );
// 		change += 0.5f;
// 		mScnMgr->setOption( "MaxPixelError", &change ); */
// 		++Ogre::PagingLandScapeOptions::getSingleton().maxPixelError; 
// 		Ogre::PagingLandScapeOptions::getSingleton().calculateCFactor();
// 		mScnMgr->WorldDimensionChange();
// 	}
// 	

	movementForFrame = AvatarControllerMovement();
	

	if (mGUIManager->isInMovementKeysMode()) {
//		movementForFrame.movementDirection = Ogre::Vector3::ZERO;
//		movementForFrame.isRunning = false;
//		movementForFrame.isMoving = false;	
		
		checkMovementKeys(event, EmberOgre::getSingleton().getInput());
		
		if (movementForFrame.mode != mPreviousMovementForFrame.mode) {
			EventMovementModeChanged.emit(movementForFrame.mode);
		}
		
		movementForFrame.timeSlice = event.timeSinceLastFrame;
	}	
		
	if (mIsAttached) {
//		mAvatarCamera->adjustForTerrain();
		mAvatar->updateFrame(movementForFrame);
	} else {
		Ogre::Real scaler = 50;
		//make this inverse, so that when the run key is pressed, the free flying camera goes slower
		//this is since we assume that one wants to go fast when in free flying mode
		if (movementForFrame.mode == AvatarMovementMode::MM_RUN) {
			scaler = 10;
		}
		Ogre::Vector3 correctDirection(movementForFrame.movementDirection.z, movementForFrame.movementDirection.y, -movementForFrame.movementDirection.x);
		mFreeFlyingCameraNode->translate(mAvatarCamera->getOrientation(false) * (correctDirection * movementForFrame.timeSlice * scaler));

	}
	mPreviousMovementForFrame = movementForFrame;
		
	

	return true;
}



void AvatarController::checkMovementKeys(const FrameEvent & event, const Input& input)
{
		//Real timePassed = event.timeSinceLastFrame;
		bool isRunning = input.isKeyDown(SDLK_RSHIFT) || input.isKeyDown(SDLK_LSHIFT);

		Ogre::Vector3 movement = Ogre::Vector3::ZERO;
		bool isMovement = false; 

		// forwards / backwards
		if(input.isKeyDown(mKeyCodeForForwardMovement))  // W also, and same for the rest
		{
			movement.x = 1; 	//scale this
			isMovement = true;
		}
		else if(input.isKeyDown(mKeyCodeForBackwardsMovement))
		{
			movement.x = -1;		//scale
			isMovement = true;
		}

		// strafe
		if(input.isKeyDown(mKeyCodeForLeftMovement))
		{
			movement.z = -1;
			isMovement = true;
		}
		else if(input.isKeyDown(mKeyCodeForRightMovement))
		{
			movement.z = 1;
			isMovement = true;
		}

		// up down
		if(input.isKeyDown(SDLK_PAGEUP))
		{
			movement.y = 1;
			isMovement = true;
		}
		else if(input.isKeyDown(SDLK_PAGEDOWN))
		{
			movement.y = -1;
			isMovement = true;
		}
		
		movementForFrame.mode = isRunning ? AvatarMovementMode::MM_RUN : AvatarMovementMode::MM_WALK;
		movementForFrame.isMoving = isMovement;
		if(isMovement)
		{
			movementForFrame.movementDirection = movement;
			
		} else {
			movementForFrame.movementDirection = Ogre::Vector3::ZERO;
		}
}

const AvatarControllerMovement & AvatarController::getCurrentMovement() const
{
	return movementForFrame;
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

