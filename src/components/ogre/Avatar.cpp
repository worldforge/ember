/*
	Avatar.cpp by Erik Hjortsberg
	Copyright (C) 2002 Miguel Guzman, Erik Hjortsberg & The Worldforge

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
#include <typeinfo>

#include "EmberOgrePrerequisites.h"

#include "services/server/ServerService.h"
#include "services/EmberServices.h"



#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "PersonEmberEntity.h"
#include "AvatarController.h"
#include "AvatarCamera.h"
//#include "MotionManager.h"

#include "AvatarEmberEntity.h"
#include "model/Model.h"
#include "model/SubModel.h"
#include "EmberOgre.h"

#include "GUIManager.h"

#include "Avatar.h"

namespace EmberOgre {



Avatar::Avatar()  
: mErisAvatarEntity(0)
{
	mTimeSinceLastServerMessage = 0;
	mMinIntervalOfRotationChanges = 1; //seconds
	mAccumulatedHorizontalRotation = 0;
	mThresholdDegreesOfYawForAvatarRotation = 100;


	mWalkSpeed = 3;
	mRunSpeed = 5; //5 seems to be the max speed in cyphesis


	// Create the Avatar tree of nodes, with a base entity
	// and attach all the needed cameras
	
	createAvatar();

	Ogre::Root::getSingleton().addFrameListener(this);

}

Avatar::~Avatar()
{
}

void Avatar::createAvatar()
{
	// The avatar itself
	mAvatarNode = static_cast<Ogre::SceneNode*>(EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChild());
	mAvatarNode->setPosition(Ogre::Vector3(0,0,0));
	//mAvatarNode->setOrientation(0,1,0,0);
	//mAvatarNode->setScale(Ogre::Vector3(0.01,0.01,0.01));
	
/*	// Model Node and Entity for display
	// TODO: do also the scaling here! That way the other nodes can be positioned in their real places
	mAvatarModelNode = static_cast<Ogre::SceneNode*>(mAvatarNode->createChild("AvatarModelNode"));
	
	Model* model = new Model("AvatarEntity");
	model->create("settler");
	
// 	Model* model = Model::Create("spider.modeldef.xml", "AvatarEntity");
	//Model::Create("malebuilder.modeldef.xml", "AvatarEntity1");


	mAvatarModel = model;
	
	mAvatarModelNode->attachObject(mAvatarModel);
	mAvatarModelNode->rotate(Ogre::Vector3::UNIT_Y,(Ogre::Degree)90);*/
  
  
/*  Ogre::Light* light = mSceneMgr->createLight("AvatarLight__");
           light->setType(Ogre::Light::LT_POINT);
            light->setCastShadows(true);
            light->setDiffuseColour(0.6,0.6,0.6);
            //light->setSpecularColour(1, 1, 1);
            light->setAttenuation(50,1,0.0005,0);
	mAvatarModelNode->attachObject(light);*/
// 
// 	mAvatarModelNode->showBoundingBox(true);

}

bool Avatar::frameStarted(const Ogre::FrameEvent & event)
{
// 	if (mErisAvatarEntity) {
// 		std::stringstream ss;
// 		ss << "X: " << mErisAvatarEntity->getPredictedPos().x() << " Y: " << mErisAvatarEntity->getPredictedPos().y();
// 		GUIManager::getSingleton().setDebugText(ss.str());
// 	}

	if (mEntitiesToBeAddedToInventory.size() > 0) {
		std::set<Eris::Entity*>::iterator I = mEntitiesToBeAddedToInventory.begin();
		std::set<Eris::Entity*>::iterator I_end = mEntitiesToBeAddedToInventory.end();
		
		for (; I != I_end; ++I) {
			//no need to do dynamic cast here
			EmberEntity* emberEntity = static_cast<EmberEntity*>(*I);
			EventAddedEntityToInventory.emit(emberEntity);
		}
		
		mEntitiesToBeAddedToInventory.clear();
	}	

	
/*	if (mEntitiesToBeRemovedFromInventory.size() > 0) {
		std::set<Eris::Entity*>::iterator J = mEntitiesToBeRemovedFromInventory.begin();
		std::set<Eris::Entity*>::iterator J_end = mEntitiesToBeRemovedFromInventory.end();
		
		for (; J != J_end; ++J) {
			EmberEntity* emberEntity = dynamic_cast<EmberEntity*>(*J);
			if (emberEntity)
				EventRemovedEntityFromInventory.emit(emberEntity);
		}
		
		mEntitiesToBeRemovedFromInventory.clear();
	}*/
	return true;
}



void Avatar::updateFrame(AvatarControllerMovement& movement)
{
	
	//for now we'll just rotate without notifying the server
	//except when moving!
	attemptRotate(movement);
	
	
	//this next method will however send send stuff to the server
	attemptMove(movement);
	
	adjustAvatarToNewPosition(&movement);
	
	/*
	//for now we'll just rotate without notifying the server
	attemptRotate(movement.rotationDegHoriz, movement.rotationDegVert, movement.timeSlice);
	//this next method will however send send stuff to the server
	attemptMove(movement.movementDirection, movement.isRunning, movement.timeSlice);
	*/
	
}

void Avatar::attemptMove(AvatarControllerMovement& movement)
{
	Ogre::Vector3 move = movement.movementDirection;
	bool isRunning = movement.mode == AvatarMovementMode::MM_RUN;
	Ogre::Real timeSlice = movement.timeSlice;
	float speed = isRunning ? mRunSpeed : mWalkSpeed;
	Ogre::Vector3 rawVelocity = move * speed;
	
	
	//first we'll register the current state in newMovementState and compare to mCurrentMovementState
	//that way we'll only send stuff to the server if our movement changes
	AvatarMovementState newMovementState;
	newMovementState.orientation = mAvatarNode->getOrientation();
	newMovementState.velocity = rawVelocity;// * newMovementState.orientation.xAxis();
	
	if (move != Ogre::Vector3::ZERO) {
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
//			mAvatarModel->startAnimation("walk");
			//fprintf(stderr, "TRACE - AVATAR START WALKING ANIMATION\n");
		} else if (!(newMovementState.orientation == mMovementStateAtLastServerMessage.orientation)) {
			//we have rotated since last server update
			//let's see if it's ok to send server message
			//if not we have to wait some more frames until we'll send an update
			if (isOkayToSendRotationMovementChangeToServer()) {
				sendToServer = true;
			}
		}
	} else {
		//we are already moving
		//let's see if we've changed speed or direction or even stopped
		if (!newMovementState.isMoving) {
			S_LOG_INFO( "TRACE - STOPPED");
			//we have stopped; we must alert the server
			sendToServer = true;
			//plus stop the animation of the avatar
//			mAvatarModel->stopAnimation("walk");
		} else if (newMovementState.velocity != mCurrentMovementState.velocity || !(newMovementState.orientation == mCurrentMovementState.orientation)){
			//either the speed or the direction has changed
			sendToServer = true;
		}
	}
		
	
/*	if (mErisAvatarEntity) {
		std::stringstream ss;
		
 		ss << newMovementState.orientation * newMovementState.velocity << "  ---  " << WFMath::Vector<3>(mErisAvatarEntity->getPredictedVelocity()).rotate((mErisAvatarEntity->getOrientation().inverse()));
		GUIManager::getSingleton().setDebugText(ss.str());
	}	*/
	
	
	if (sendToServer) {
		S_LOG_INFO("TRACE - SEND MOVE OPS TO SERVER");
		mMovementStateAtBeginningOfMovement = newMovementState;
		mMovementStateAtLastServerMessage = newMovementState;
		mTimeSinceLastServerMessage = 0;
		

		//for now we'll only send velocity
		Ember::EmberServices::getInstance()->getServerService()->moveInDirection(Ogre2Atlas_Vector3(newMovementState.orientation * newMovementState.velocity), Ogre2Atlas(newMovementState.orientation));

//		Ember::EmberServices::getInstance()->getServerService()->moveInDirection(Ogre2Atlas(mCurrentMovementState.velocity), Ogre2Atlas(mCurrentMovementState.orientation));

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

void Avatar::adjustAvatarToNewPosition(AvatarControllerMovement* movement)
{
	//get the new coordinates of the avatar and check with mercator to adjust for
	//height
	if (mErisAvatarEntity) {
		mErisAvatarEntity->adjustHeightPosition();
	}
//	MotionManager::getSingleton().adjustHeightPositionForNode(mAvatarNode);
}


void Avatar::attemptJump() {}


void Avatar::attemptRotate(AvatarControllerMovement& movement)
{
	//TODO: remove the direct references to AvatarCamera
	float degHoriz = movement.rotationDegHoriz;
	float degVert = movement.rotationDegVert;
	Ogre::Real timeSlice = movement.timeSlice;
	
//	mAccumulatedHorizontalRotation += (degHoriz * timeSlice);

	//if we're moving we must sync the rotation with messages sent to the server
	if (!movement.isMoving && fabs(getAvatarCamera()->getYaw().valueDegrees()) > mThresholdDegreesOfYawForAvatarRotation) {
//		mAvatarNode->setOrientation(movement.cameraOrientation);
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
		getAvatarCamera()->yaw(-getAvatarCamera()->getYaw());
//		mAccumulatedHorizontalRotation = 0;
	} else if (isOkayToSendRotationMovementChangeToServer() && (getAvatarCamera()->getYaw().valueDegrees())) {
		// rotate the Avatar Node only in X position (no vertical rotation)
//		mAvatarNode->setOrientation(movement.cameraOrientation);
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
		getAvatarCamera()->yaw(-getAvatarCamera()->getYaw());

//		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,mAccumulatedHorizontalRotation);
//		mAccumulatedHorizontalRotation = 0;
	}

	// pitch the 1p Camera
	//mAvatar1pCameraNode->yaw(degHoriz);
	// TODO: rotate 3p cam and rotate *back* top camera

	// Rotate top camera *back*
	// The mini-map will always be in the same position
	// (Up is North)
	// TODO: study why do I need twice the angle!
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
	//mAvatarTopCameraNode->rotate(Ogre::Vector3::UNIT_Y,-degHoriz);
}

bool Avatar::isOkayToSendRotationMovementChangeToServer()
{
	return mTimeSinceLastServerMessage > mMinIntervalOfRotationChanges;
	
}

AvatarCamera* Avatar::getAvatarCamera() const
{
	return mAvatarController->getAvatarCamera();
}

AvatarEmberEntity* Avatar::getAvatarEmberEntity()
{
	return mErisAvatarEntity;
}


void Avatar::setAvatarController(AvatarController* avatarController)
{
	mAvatarController = avatarController;	
}

void Avatar::movedInWorld()
{
	if (!mCurrentMovementState.isMoving) 
	{
		mAvatarNode->setPosition(Atlas2Ogre(mErisAvatarEntity->getPosition()));
	}
	
}


void Avatar::createdAvatarEmberEntity(AvatarEmberEntity *EmberEntity)
{
	Ogre::SceneNode* oldAvatar = mAvatarNode;
	
	
	//mAvatarNode->getParent()->removeChild(mAvatarNode->getName());
	
	//HACK!!! DEBUG!!
	//mAvatarNode->getParent()->removeChild(mAvatarNode->getName());
	//EmberEntity->getSceneNode()->addChild(mAvatarNode);
//	mSceneMgr->getRootSceneNode()->addChild(mAvatarNode);
	//HACK!!! DEBUG!!
	
	mAvatarNode = EmberEntity->getSceneNode();
	mAvatarModel = EmberEntity->getModel();

	mErisAvatarEntity = EmberEntity;
	EmberEntity->setAvatar(this);

	
	
	mAvatarController->createAvatarCameras(EmberEntity->getSceneNode());
		
	EmberOgre::getSingleton().getSceneManager()->destroySceneNode(oldAvatar->getName());

}

// void Avatar::touch(EmberEntity* entity)
// {
// 	Ember::EmberServices::getInstance()->getServerService()->touch(entity);
// }

/*
Ogre::Camera* Avatar::getCamera() const
{
	return mAvatarController->getCamera();
}
*/
}


