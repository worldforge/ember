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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "Avatar.h"


#include <typeinfo>


#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/config/ConfigService.h"
#include "services/time/TimeService.h"
#include "services/sound/SoundService.h"


#include "EmberEntity.h"
#include "EmberPhysicalEntity.h"
#include "AvatarController.h"
#include "AvatarCamera.h"
#include "AvatarLogger.h"

#include "AvatarEmberEntity.h"
#include "model/Model.h"
#include "model/SubModel.h"
#include "EmberOgre.h"
#include "MathConverter.h"
#include "services/input/Input.h"

#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>

#include <OgreRoot.h>

namespace EmberOgre {


Avatar::Avatar()
: mErisAvatarEntity(0)
, mHasChangedLocation(false)
, mChatLoggerParent(0)
{
	mTimeSinceLastServerMessage = 0;
	setMinIntervalOfRotationChanges(1000); //milliseconds
	mAccumulatedHorizontalRotation = 0;
	mThresholdDegreesOfYawForAvatarRotation = 100;


	///these are the defaults, but if anything is set in the config file that will override these values as updateFromConfig is called
	mWalkSpeed = 2.47;
	mRunSpeed = 5; ///5 seems to be the max speed in cyphesis


	/// Create the Avatar tree of nodes, with a base entity
	/// and attach all the needed cameras

	createAvatar();

	Ogre::Root::getSingleton().addFrameListener(this);

	registerConfigListener("general","logchatmessages", sigc::mem_fun(*this, &Avatar::Config_LogChatMessages));
	registerConfigListener("general","avatarrotationupdatefrequency", sigc::mem_fun(*this, &Avatar::Config_AvatarRotationUpdateFrequency));
	registerConfigListener("input","walkspeed", sigc::mem_fun(*this, &Avatar::Config_WalkSpeed));
	registerConfigListener("input","runspeed", sigc::mem_fun(*this, &Avatar::Config_RunSpeed));

	mCurrentMovementState.isMoving = false;
	mCurrentMovementState.velocity = Ogre::Vector3::ZERO;
	mCurrentMovementState.orientation = Ogre::Quaternion::IDENTITY;
	mCurrentMovementState.isRunning = false;

}

Avatar::~Avatar()
{

}

void Avatar::setMinIntervalOfRotationChanges(Ogre::Real milliseconds)
{
	mMinIntervalOfRotationChanges = milliseconds;
}

void Avatar::createAvatar()
{
	mAvatarNode = static_cast<Ogre::SceneNode*>(EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChild());
	mAvatarNode->setPosition(Ogre::Vector3(0,0,0));
}

bool Avatar::frameStarted(const Ogre::FrameEvent & event)
{

	if (mEntitiesToBeAddedToInventory.size() > 0) {
		std::set<Eris::Entity*>::iterator I = mEntitiesToBeAddedToInventory.begin();
		std::set<Eris::Entity*>::iterator I_end = mEntitiesToBeAddedToInventory.end();

		for (; I != I_end; ++I) {
			///no need to do dynamic cast here
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

	///for now we'll just rotate without notifying the server
	///except when moving!
	attemptRotate(movement);


	///this next method will however send send stuff to the server
	attemptMove(movement);

	///only adjust if there is actual movement. If we adjust when there's only rotation we'll get a strange jerky effect (some bug I guess)
	if (movement.isMoving) {
		adjustAvatarToNewPosition(&movement);
	}

}

void Avatar::attemptMove(AvatarControllerMovement& movement)
{
	Ogre::Vector3 move = movement.movementDirection;
	bool isRunning = movement.mode == AvatarMovementMode::MM_RUN;
	Ogre::Real timeSlice = movement.timeSlice;
	float speed = isRunning ? mRunSpeed : mWalkSpeed;
	Ogre::Vector3 rawVelocity = move * speed;


	///first we'll register the current state in newMovementState and compare to mCurrentMovementState
	///that way we'll only send stuff to the server if our movement changes
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
			S_LOG_VERBOSE( "Avatar stopped moving.");
			//we have stopped; we must alert the server
			sendToServer = true;
		} else if (newMovementState.velocity != mCurrentMovementState.velocity || !(newMovementState.orientation == mCurrentMovementState.orientation)){
			//either the speed or the direction has changed
			sendToServer = true;
		}
	}


	if (sendToServer) {
		S_LOG_VERBOSE("Sending move op to server.");
		mMovementStateAtBeginningOfMovement = newMovementState;
		mMovementStateAtLastServerMessage = newMovementState;
		mTimeSinceLastServerMessage = 0;


		///Save the five latest orientations sent to the server, so we can later when we recieve an update from the server we can recognize that it's our own updates and ignore them.
		mLastOrientations.push_back(Ogre2Atlas(newMovementState.orientation));
		if (mLastOrientations.size() > 5) {
			mLastOrientations.erase(mLastOrientations.begin());
		}
		//for now we'll only send velocity
		Ember::EmberServices::getSingletonPtr()->getServerService()->moveInDirection(Ogre2Atlas_Vector3(newMovementState.orientation * newMovementState.velocity), Ogre2Atlas(newMovementState.orientation));

//		Ember::EmberServices::getSingletonPtr()->getServerService()->moveInDirection(Ogre2Atlas(mCurrentMovementState.velocity), Ogre2Atlas(mCurrentMovementState.orientation));

	} else {
		mTimeSinceLastServerMessage += timeSlice * 1000;
	}

	if (newMovementState.isMoving) {
		//do the actual movement of the avatar node
		mAvatarNode->translate(mAvatarNode->getOrientation() * (rawVelocity * timeSlice));
	}
	mCurrentMovementState = newMovementState;

}

void Avatar::adjustAvatarToNewPosition(AvatarControllerMovement* movement)
{
	///allow the eris entity to adjust to the containing node
	if (mErisAvatarEntity) {
		mErisAvatarEntity->adjustPosition();
	}
//	MotionManager::getSingleton().adjustHeightPositionForNode(mAvatarNode);
}


void Avatar::attemptJump() {}


void Avatar::attemptRotate(AvatarControllerMovement& movement)
{
	//TODO: remove the direct references to AvatarCamera
/*	float degHoriz = movement.rotationDegHoriz;
	float degVert = movement.rotationDegVert;
	Ogre::Real timeSlice = movement.timeSlice;*/

//	mAccumulatedHorizontalRotation += (degHoriz * timeSlice);

	//if we're moving we must sync the rotation with messages sent to the server
	if (!movement.isMoving && fabs(getAvatarCamera()->getYaw().valueDegrees()) > mThresholdDegreesOfYawForAvatarRotation) {
//		mAvatarNode->setOrientation(movement.cameraOrientation);
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
		Ogre::Degree yaw = getAvatarCamera()->getYaw();
		getAvatarCamera()->yaw(-yaw);
//		mAccumulatedHorizontalRotation = 0;
	} else if (isOkayToSendRotationMovementChangeToServer() && (getAvatarCamera()->getYaw().valueDegrees())) {
		// rotate the Avatar Node only in X position (no vertical rotation)
//		mAvatarNode->setOrientation(movement.cameraOrientation);
		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
		Ogre::Degree yaw = getAvatarCamera()->getYaw();
		getAvatarCamera()->yaw(-yaw);

//		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,mAccumulatedHorizontalRotation);
//		mAccumulatedHorizontalRotation = 0;
	}

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
	///only snap the avatar to the postition and orientation sent from the server if we're not moving or if we're not recently changed location
	///The main reason when moving is that we don't want to have the avatar snapping back in the case of lag
	///However, if we've just recently changed location, we need to also update the orientation to work with the new location.
	if (!mCurrentMovementState.isMoving || mHasChangedLocation)
	{
		const WFMath::Quaternion& orient = mErisAvatarEntity->getOrientation();
		bool isOwnRotation = false;
		///Check if the new orientation is one that we sent ourself, and if so ignore it
		for (std::list<WFMath::Quaternion>::const_iterator I = mLastOrientations.begin(); I != mLastOrientations.end(); ++I) {
			if (orient == *I) {
				isOwnRotation = true;
				break;
			}
		}
		if (!isOwnRotation) {
			mAvatarNode->setOrientation(Atlas2Ogre(orient));
		}
		mAvatarNode->setPosition(Atlas2Ogre(mErisAvatarEntity->getPosition()));
		///we must set this, else ember will think that we've rotated the avatar ourselves and try to send an update to the server
		mMovementStateAtLastServerMessage.orientation = mAvatarNode->getOrientation();
		mHasChangedLocation = false;
	}
}

void Avatar::avatar_LocationChanged(Eris::Entity* entity)
{
	///if we've changed location, we need to update the orientation. This is done on the next onMoved event, which is why we must honour the updated values on next onMoved event, even though we might be moving.
	mHasChangedLocation = true;
}


void Avatar::createdAvatarEmberEntity(AvatarEmberEntity *emberEntity)
{
	Ogre::SceneNode* oldAvatar = mAvatarNode;

	///check if the user is of type "creator" and thus an admin
	Eris::TypeService* typeService = emberEntity->getErisAvatar()->getConnection()->getTypeService();
	if (emberEntity->getType()->isA(typeService->getTypeByName("creator"))) {
		mIsAdmin = true;
	} else {
		mIsAdmin = false;
	}

	mAvatarNode = emberEntity->getSceneNode();
	mAvatarModel = emberEntity->getModel();

	mErisAvatarEntity = emberEntity;
	emberEntity->setAvatar(this);
	mAvatarController->createAvatarCameras(emberEntity->getSceneNode());

	EmberOgre::getSingleton().getSceneManager()->destroySceneNode(oldAvatar->getName());
	Ember::Input::getSingleton().setMovementModeEnabled(true);

	mErisAvatarEntity->LocationChanged.connect(sigc::mem_fun(*this, &Avatar::avatar_LocationChanged));

	EventCreatedAvatarEntity.emit(emberEntity);
}

void Avatar::Config_AvatarRotationUpdateFrequency(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	setMinIntervalOfRotationChanges(static_cast<double>(variable));
}

void Avatar::Config_WalkSpeed(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mWalkSpeed = static_cast<double>(variable);
}

void Avatar::Config_RunSpeed(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mRunSpeed = static_cast<double>(variable);
}

void Avatar::Config_LogChatMessages(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (static_cast<bool>(variable)) {
		mChatLoggerParent = std::auto_ptr<AvatarLoggerParent>(new AvatarLoggerParent(*this));
	} else {
		mChatLoggerParent.reset();
	}
}


// void Avatar::touch(EmberEntity* entity)
// {
// 	Ember::EmberServices::getSingletonPtr()->getServerService()->touch(entity);
// }

/*
Ogre::Camera* Avatar::getCamera() const
{
	return mAvatarController->getCamera();
}
*/
}


