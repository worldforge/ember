/*
	Avatar.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge

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

//TODO: reorder headers, etc


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <Ogre.h>
#include <typeinfo>

#include "services/server/ServerService.h"
#include "services/DimeServices.h"
/*
#include "EntityListener.h"
#include <Ogre.h>

#include <Eris/PollDefault.h>
#include <Eris/Log.h>
#include <Eris/TypeInfo.h>

#include "MathConverter.h"
*/


#include "DimeEntity.h"
#include "AvatarController.h"
#include "Avatar.h"


Avatar::Avatar()
{
}

Avatar::Avatar(Ogre::SceneManager* sceneManager)  	
{
	mTimeSinceLastServerMessage = 0;
	mMinIntervalOfTrivialChanges = 0.2; //seconds
	mAccumulatedHorizontalRotation = 0;

	mAvatar1pCameraNode = NULL;

	mWalkSpeed = WF2OGRE(2.0);
	mRunSpeed = WF2OGRE(5.0);

	mSceneMgr = sceneManager; // TODO: assert it's not null

	// Create the Avatar tree of nodes, with a base entity
	// and attach all the needed cameras
	
	createAvatar();
	createAvatarCameras(mAvatarNode);


}

Avatar::~Avatar()
{
}

void Avatar::createAvatar()
{
	// The avatar itself
	mAvatarNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	mAvatarNode->setPosition(WF2OGRE_VECTOR3(0,0,5));
	//mAvatarNode->setOrientation(0,1,0,0);
	//mAvatarNode->setScale(OGRESCALER);
	// TODO: very important! When the scale is set to default (1,1,1)
	// and a mesh with normal scale is put here (like 2 meters high and things like that)
	// we'll need to rescale the local space position for the nodes (shown below)
	// - will be like dividing by 100 or something

	// Model Node and Entity for display
	// TODO: do also the scaling here! That way the other nodes can be positioned in their real places
	mAvatarModelNode = dynamic_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarModelNode"));
	mAvatarEntity = mSceneMgr->createEntity("AvatarEntity", "robot.mesh");
	mAvatarModelNode->attachObject(mAvatarEntity);
//	mAvatarModelNode->rotate(Ogre::Vector3::UNIT_Y,90);

	//let's set the current animation state to walking
	mAnimStateWalk = mAvatarEntity->getAnimationState("Walk");	
	
	Ogre::ControllerManager* controllerManager = &Ogre::ControllerManager::getSingleton();
	mAvatarAnimationControllerFunction = new Ogre::AnimationControllerFunction(mAnimStateWalk->getLength());
	mAvatarAnimationController = controllerManager->createController(controllerManager->getFrameTimeSource(), mAnimStateWalk, mAvatarAnimationControllerFunction);
}



void Avatar::createAvatarCameras(Ogre::SceneNode* avatarSceneNode)
{
	if (mAvatar1pCameraNode != NULL) {
		Ogre::String cameraNodeName = mAvatar1pCameraNode->getName();
		mAvatar1pCameraNode->getParent()->removeChild(cameraNodeName);
		avatarSceneNode->addChild(mAvatar1pCameraNode);
	} else {
			
		
		// 1st person, 3rd person and Top camera nodes
		mAvatar1pCameraNode = dynamic_cast<Ogre::SceneNode*>(avatarSceneNode->createChild("Avatar1pCameraNode"));
		mAvatar1pCameraNode->setPosition(WF2OGRE_VECTOR3(0,0.8,-0.1));
		//rotate to sync with WF world
	    mAvatar1pCameraNode->rotate(Ogre::Vector3::UNIT_Y,-90);
		//mAvatar1pCameraNode->showBoundingBox(true);
		mAvatar3pCameraNode = dynamic_cast<Ogre::SceneNode*>(mAvatar1pCameraNode->createChild("Avatar3pCameraNode"));
		Ogre::Vector3 pos = WF2OGRE_VECTOR3(0,0,5);
		//mAvatar3pCameraNode->setPosition(15000,2500,0);
		mAvatar3pCameraNode->setPosition(pos);
		mAvatarTopCameraNode = dynamic_cast<Ogre::SceneNode*>(avatarSceneNode->createChild("AvatarTopCameraNode"));
		mAvatarTopCameraNode->setPosition(WF2OGRE_VECTOR3(2,200,0));
	
		// Create the camera
		mAvatar1pCamera = mSceneMgr->createCamera("Avatar1pCam");
		mAvatar1pCameraNode->attachObject(mAvatar1pCamera);
	
		// Look back along x axis
		//mAvatar1pCamera->lookAt(mOgreHeadNode->getPosition());
		mAvatar1pCamera->setNearClipDistance(1);
	
		// do not clip at far distance
		// so we can see the skydome
		//mCamera->setFarClipDistance( 384 );
	
		//3rd person camera
		mAvatar3pCamera = mSceneMgr->createCamera("Avatar3pCamera");
		mAvatar3pCameraNode->attachObject(mAvatar3pCamera);
		// Look to the Avatar's head
		//mAvatar3pCamera->setAutoTracking(true, mAvatar1pCameraNode);
		mAvatar3pCamera->setNearClipDistance(1);
	
		// Create the Top camera
		mAvatarTopCamera = mSceneMgr->createCamera("AvatarTopCamera");
		mAvatarTopCameraNode->attachObject(mAvatarTopCamera);
		// Look to the Avatar's head
		mAvatarTopCamera->setAutoTracking(true, avatarSceneNode);
		mAvatarTopCamera->setNearClipDistance(1);
		mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-90.0);
	}	
}

void Avatar::updateFrame(AvatarControllerMovement movement)
{
	
	//for now we'll just rotate without notifying the server
	//except when moving!
	attemptRotate(movement);
	//this next method will however send send stuff to the server
	attemptMove(movement);
	/*
	//for now we'll just rotate without notifying the server
	attemptRotate(movement.rotationDegHoriz, movement.rotationDegVert, movement.timeSlice);
	//this next method will however send send stuff to the server
	attemptMove(movement.movementDirection, movement.isRunning, movement.timeSlice);
	*/
}

void Avatar::attemptMove(AvatarControllerMovement movement)
{
	Ogre::Vector3 move = movement.movementDirection;
	bool isRunning = movement.isRunning;
	Ogre::Real timeSlice = movement.timeSlice;
	float speed = isRunning ? mRunSpeed : mWalkSpeed;
	Vector3 rawVelocity = move * speed;
	
	
	//first we'll register the current state in newMovementState and compare to mCurrentMovementState
	//that way we'll only send stuff to the server if our movement changes
	AvatarMovementState newMovementState;
	newMovementState.orientation = mAvatarNode->getOrientation();
	newMovementState.velocity = rawVelocity;// * newMovementState.orientation.xAxis();
	
	if (move != Vector3::ZERO) {
		newMovementState.isMoving = true;
		newMovementState.isRunning = isRunning;
	} else {
		newMovementState.isMoving = false;
		newMovementState.isRunning = false;
	}		
	bool sendToServer = false;
	
	//first we check if we are already moving 
	if (!mCurrentMovementState.isMoving) {
		//we are not moving. Should we start to move?
		if (newMovementState.isMoving) {
			//we'll start moving
			//let's send the movement command to the server
			sendToServer = true;		

			//we'll also start the animation of the avatar's movement animation
			mAnimStateWalk->setEnabled(true);
			//fprintf(stderr, "TRACE - AVATAR START WALKING ANIMATION\n");
		} else if (!(newMovementState.orientation == mMovementStateAtLastServerMessage.orientation)) {
			//we have rotated since last server update
			//let's see if it's ok to send server message
			//if not we have to wait some more frames until we'll send an update
			if (isOkayToSendTrivialMovementChangeToServer()) {
				sendToServer = true;
			}
		}
	} else {
		//we are already moving
		//let's see if we've changed speed or direction or even stopped
		if (!newMovementState.isMoving) {
			fprintf(stderr, "TRACE - STOPPED\n");
			//we have stopped; we must alert the server
			sendToServer = true;
			//plus stop the animation of the avatar
			mAnimStateWalk->setEnabled(false);
		} else if (newMovementState.velocity != mCurrentMovementState.velocity || !(newMovementState.orientation == mCurrentMovementState.orientation)){
			//either the speed or the direction has changed
			sendToServer = true;
		}
	}
		
	if (sendToServer) {
		fprintf(stderr, "TRACE - SEND MOVE OPS TO SERVER\n");
		mMovementStateAtBeginningOfMovement = newMovementState;
		mMovementStateAtLastServerMessage = newMovementState;
		mTimeSinceLastServerMessage = 0;
		

		//for now we'll only send velocity
		dime::DimeServices::getInstance()->getServerService()->moveInDirection(Ogre2Atlas_Vector3(OGRE2WF(newMovementState.orientation * newMovementState.velocity)), Ogre2Atlas(newMovementState.orientation));

//		dime::DimeServices::getInstance()->getServerService()->moveInDirection(Ogre2Atlas(mCurrentMovementState.velocity), Ogre2Atlas(mCurrentMovementState.orientation));

	} else {
		mTimeSinceLastServerMessage += timeSlice;
		//fprintf(stderr, "TRACE - DONT SEND MOVE OPS TO SERVER\n");
	}

	if (newMovementState.isMoving) {
		//do the actual movement of the avatar node
		mAvatarNode->translate(mAvatarNode->getOrientation() * (rawVelocity * timeSlice));
	}
	mCurrentMovementState = newMovementState;
}

void Avatar::attemptJump() {}


void Avatar::attemptRotate(AvatarControllerMovement movement)
{
	float degHoriz = movement.rotationDegHoriz;
	float degVert = movement.rotationDegVert;
	Ogre::Real timeSlice = movement.timeSlice;
	
	mAccumulatedHorizontalRotation += (degHoriz * timeSlice);

	//if we're moving we must sync the rotation with messages sent to the server
	if (!movement.isMoving) {
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,mAccumulatedHorizontalRotation);
		mAccumulatedHorizontalRotation = 0;
	} else if (isOkayToSendTrivialMovementChangeToServer()) {
		// rotate the Avatar Node only in X position (no vertical rotation)
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,mAccumulatedHorizontalRotation);
		mAccumulatedHorizontalRotation = 0;
	}

	// pitch the 1p Camera
	//mAvatar1pCameraNode->yaw(degHoriz);
	mAvatar1pCameraNode->pitch(degVert * timeSlice);
	// TODO: rotate 3p cam and rotate *back* top camera

	// Rotate top camera *back*
	// The mini-map will always be in the same position
	// (Up is North)
	// TODO: study why do I need twice the angle!
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
}

bool Avatar::isOkayToSendTrivialMovementChangeToServer()
{
	return mTimeSinceLastServerMessage > mMinIntervalOfTrivialChanges;
	
}

Ogre::Camera* Avatar::getAvatar1pCamera()
{
	return mAvatar1pCamera;
}

Ogre::Camera* Avatar::getAvatar3pCamera()
{
	return mAvatar3pCamera;
}

Ogre::Camera* Avatar::getAvatarTopCamera()
{
	return mAvatarTopCamera;
}

void Avatar::movedInWorld( const WFMath::Point< 3 > &p )
{
	mAvatarNode->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(mErisAvatarEntity->getPosition()));
	mAvatarNode->setOrientation(Atlas2Ogre(mErisAvatarEntity->getOrientation()));
	
}


void Avatar::enteredWorld(Eris::Entity *e)
{
	
	/*
	 * sometimes, for no apparent reason, the avatar get's created as a 
	 * normal entity
	 * And sometimes not.
	 * Thus we need to check for this and act appropriatly
	 */
	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(e);
	if (dimeEntity != 0) {
		//it got created as a DimeEntity, do some reshuffeling
		//dimeEntity->markAsMainAvatar(mSceneMgr);
		createAvatarCameras(dimeEntity->getSceneNode());
		delete mAvatarNode;
		mAvatarNode = dimeEntity->getSceneNode();
		//here we should disconnect signals, if I only found out how...
		//TODO: disconnect signals
		//e->Moved.
	} 

	mAvatarNode->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(e->getPosition()));
	mAvatarNode->setOrientation(Atlas2Ogre(e->getOrientation()));

	mErisAvatarEntity = e;
	e->Moved.connect(SigC::slot( *this, &Avatar::movedInWorld ));

	
}

void Avatar::touch(DimeEntity* entity)
{
	dime::DimeServices::getInstance()->getServerService()->touch(entity);
}


