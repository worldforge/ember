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

#include "framework/Tokeniser.h"


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
, mMovementCommandMapper("movement", "key_bindings_movement")
, RunToggle("+run", this, "Toggle running mode.")
, ToggleCameraAttached("toggle_cameraattached", this, "Toggle between the camera being attached to the avatar and free flying.")
, CharacterMoveForward("+character_move_forward", this, "Move the avatar forward.")
, CharacterMoveBackward("+character_move_backward", this, "Move the avatar backward.")
, CharacterStrafeLeft("+character_strafe_left", this, "Strafe left.")
, CharacterStrafeRight("+character_strafe_right", this, "Strafe right.")
, CharacterMoveUpwards("+character_move_upwards", this, "Move the avatar upwards.")
, CharacterMoveDownwards("+character_move_downwards", this, "Move the avatar downwards.")
, mMovementDirection(Ogre::Vector3::ZERO)
{

	mMovementCommandMapper.restrictToInputMode(Input::IM_MOVEMENT );
	
	setAvatar(avatar);
	
	
	mAvatar->setAvatarController(this);
	
	Ogre::Root::getSingleton().addFrameListener(this);
	
	
	mFreeFlyingCameraNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mFreeFlyingCameraNode->setPosition(0,30,0);
	detachCamera();
	
	
	mMovementCommandMapper.bindToInput(EmberOgre::getSingleton().getInput());
}
AvatarController::~AvatarController()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

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



void AvatarController::runCommand(const std::string &command, const std::string &args)
{
	if (RunToggle == command) {
		mIsRunning = true;
	} else if (RunToggle.getInverseCommand() == command) {
		mIsRunning = false;
	} else if (ToggleCameraAttached == command)
	{
		if (mIsAttached) {
			detachCamera();
		} else {
			attachCamera();
		}
	} else if (CharacterMoveForward == command) {
		mMovementDirection.x = 1;
	} else if (CharacterMoveForward.getInverseCommand() == command) {
		mMovementDirection.x = 0;
	} else if (CharacterMoveBackward == command) {
		mMovementDirection.x = -1;
	} else if (CharacterMoveBackward.getInverseCommand() == command) {
		mMovementDirection.x = 0;
	} else if (CharacterStrafeLeft == command) {
		mMovementDirection.z = -1;
	} else if (CharacterStrafeLeft.getInverseCommand() == command) {
		mMovementDirection.z = 0;
	} else if (CharacterStrafeRight == command) {
		mMovementDirection.z = 1;
	} else if (CharacterStrafeRight.getInverseCommand() == command) {
		mMovementDirection.z = 0;
	} else if (CharacterMoveUpwards == command) {
		mMovementDirection.y = 1;
	} else if (CharacterMoveUpwards.getInverseCommand() == command) {
		mMovementDirection.y = 0;
	} else if (CharacterMoveDownwards == command) {
		mMovementDirection.y = -1;
	} else if (CharacterMoveDownwards.getInverseCommand() == command) {
		mMovementDirection.y = 0;
	}
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
	
/*	if (mMovementDirection != Ogre::Vector3::ZERO) {*/
	movementForFrame.mode = mIsRunning ? AvatarMovementMode::MM_RUN : AvatarMovementMode::MM_WALK;
	movementForFrame.isMoving = true;
	movementForFrame.movementDirection = mMovementDirection;
	movementForFrame.timeSlice = event.timeSinceLastFrame;
	if (movementForFrame.mode != mPreviousMovementForFrame.mode) {
		EventMovementModeChanged.emit(movementForFrame.mode);
	}
// 	} else {
// 	}

// 	if (mGUIManager->isInMovementKeysMode()) {
//		movementForFrame.movementDirection = Ogre::Vector3::ZERO;
//		movementForFrame.mIsRunning = false;
//		movementForFrame.isMoving = false;	
		
/*		checkMovementKeys(event, EmberOgre::getSingleton().getInput());
		
		
		movementForFrame.timeSlice = event.timeSinceLastFrame;
	}	*/
		
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


const AvatarControllerMovement & AvatarController::getCurrentMovement() const
{
	return movementForFrame;
}


AvatarCamera* AvatarController::getAvatarCamera() const
{
	return mAvatarCamera;	
}


}

