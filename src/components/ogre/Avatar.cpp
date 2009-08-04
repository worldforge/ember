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

#include "components/ogre/EmberOgre.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/AvatarLogger.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/AvatarAttachmentController.h"

#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/camera/ThirdPersonCameraMount.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/time/TimeService.h"
#include "services/input/Input.h"

#include "framework/Tokeniser.h"

#include "main/Application.h"

#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>



#include <OgreRoot.h>

#include <OgreTagPoint.h>

#include <typeinfo>

namespace EmberOgre
{

Avatar::Avatar(EmberEntity& erisAvatarEntity) :
SetAttachedOrientation("setattachedorientation", this, "Sets the orientation of an item attached to the avatar: <attachpointname> <x> <y> <z> <degrees>")
, mErisAvatarEntity(erisAvatarEntity)
, mMaxSpeed(5)
, mAvatarAttachmentController(new AvatarAttachmentController(*this))
, mCameraMount(new Camera::ThirdPersonCameraMount(*EmberOgre::getSingleton().getSceneManager()))
, mIsAdmin(false)
, mHasChangedLocation(false)
, mChatLoggerParent(0)
{
	setMinIntervalOfRotationChanges(1000); //milliseconds

	Ember::Application::getSingleton().EventAfterInputProcessing.connect(sigc::mem_fun(*this, &Avatar::application_AfterInputProcessing));

	registerConfigListener("general", "logchatmessages", sigc::mem_fun(*this, &Avatar::Config_LogChatMessages));
	registerConfigListener("general", "avatarrotationupdatefrequency", sigc::mem_fun(*this, &Avatar::Config_AvatarRotationUpdateFrequency));
	registerConfigListener("input", "maxspeed", sigc::mem_fun(*this, &Avatar::Config_MaxSpeed));

	mCurrentMovementState.movement = WFMath::Vector<3>::ZERO();
	mCurrentMovementState.orientation = WFMath::Quaternion().identity();
	mCurrentMovementState.position = erisAvatarEntity.getPredictedPos();

	///check if the user is of type "creator" and thus an admin
	Eris::TypeService* typeService = Ember::EmberServices::getSingleton().getServerService()->getConnection()->getTypeService();
	if (mErisAvatarEntity.getType()->isA(typeService->getTypeByName("creator"))) {
		mIsAdmin = true;
	} else {
		mIsAdmin = false;
	}

	mErisAvatarEntity.LocationChanged.connect(sigc::mem_fun(*this, &Avatar::avatar_LocationChanged));
	mErisAvatarEntity.Moved.connect(sigc::mem_fun(*this, &Avatar::avatar_Moved));
	mErisAvatarEntity.ChildAdded.connect(sigc::mem_fun(*this, &Avatar::entity_ChildAdded));
	mErisAvatarEntity.ChildRemoved.connect(sigc::mem_fun(*this, &Avatar::entity_ChildRemoved));

	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPredictedPos();

	mErisAvatarEntity.setAttachmentControlDelegate(mAvatarAttachmentController);

	mCameraMount->attachToNode(getAvatarSceneNode());
}

Avatar::~Avatar()
{

}

void Avatar::runCommand(const std::string &command, const std::string &args)
{
	if (SetAttachedOrientation == command) {
		Ember::Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string attachPointName = tokeniser.nextToken();
		if (attachPointName != "") {
			std::string x = tokeniser.nextToken();
			std::string y = tokeniser.nextToken();
			std::string z = tokeniser.nextToken();
			std::string degrees = tokeniser.nextToken();
			if (x != "" && y != "" && z != "" && degrees != "") {
				Ogre::Degree ogreDegrees(Ogre::StringConverter::parseReal(degrees));
				Ogre::Quaternion rotation(ogreDegrees, Ogre::Vector3(Ogre::StringConverter::parseReal( x), Ogre::StringConverter::parseReal( y), Ogre::StringConverter::parseReal( z)));
				Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(mErisAvatarEntity);
				if (modelRepresentation) {
					Model::Model& model = modelRepresentation->getModel();
					const Model::Model::AttachPointWrapperStore* attachPoints = model.getAttachedPoints();
					if (attachPoints) {
						for (Model::Model::AttachPointWrapperStore::const_iterator I = attachPoints->begin(); I != attachPoints->end(); ++I) {
							if (I->Definition.Name == attachPointName) {
								I->TagPoint->setOrientation(rotation);
							}
						}
					}
				}
			}
		}
	}
}

Camera::ThirdPersonCameraMount& Avatar::getCameraMount() const
{
	return *mCameraMount;
}

void Avatar::setMinIntervalOfRotationChanges(Ogre::Real milliseconds)
{
	mMinIntervalOfRotationChanges = milliseconds;
}

void Avatar::application_AfterInputProcessing(float timeSinceLastEvent)
{
	if (mEntitiesToBeAddedToInventory.size() > 0) {
		std::set<Eris::Entity*>::iterator I = mEntitiesToBeAddedToInventory.begin();
		std::set<Eris::Entity*>::iterator I_end = mEntitiesToBeAddedToInventory.end();

		for (; I != I_end; ++I) {
			EmberEntity* emberEntity = static_cast<EmberEntity*> (*I);
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

	attemptMove();
}

void Avatar::moveClientSide(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice)
{
	mCurrentMovement = movement * mMaxSpeed;
	if (movement != WFMath::Vector<3>::ZERO()) {

		//We need to constraint the orientation to only around the z axis.
		WFMath::Vector<3> rotator(1.0, 0.0, 0.0);
		rotator.rotate(orientation);
		WFMath::Quaternion adjustedOrientation;
		adjustedOrientation.fromRotMatrix(WFMath::RotMatrix<3>().rotationZ(atan2(rotator.y(), rotator.x())));

		mClientSideAvatarOrientation = adjustedOrientation;
		//For some not quite explained reason we need to rotate the orientation 90 degrees around the z axis for the orientation to be correct.
		mClientSideAvatarOrientation.rotate(WFMath::Quaternion(WFMath::Vector<3>(0, 0, 1), WFMath::Pi / 2));
		mCurrentMovement.rotate(adjustedOrientation);
		WFMath::Vector<3> adjustedMovement(movement * timeslice * mMaxSpeed);
		mClientSideAvatarPosition += adjustedMovement.rotate(adjustedOrientation);

		if (mErisAvatarEntity.getAttachment()) {
			mErisAvatarEntity.getAttachment()->updatePosition();
		}
	}
}

void Avatar::attemptMove()
{

	///first we'll register the current state in newMovementState and compare to mCurrentMovementState
	///that way we'll only send stuff to the server if our movement changes
	AvatarMovementState newMovementState;
	newMovementState.orientation = mClientSideAvatarOrientation;
	newMovementState.movement = mCurrentMovement;
	newMovementState.position = mClientSideAvatarPosition;

	bool isMoving = mCurrentMovement.isValid() && mCurrentMovement != WFMath::Vector<3>::ZERO();
	bool wasMoving = mCurrentMovementState.movement.isValid() && mCurrentMovementState.movement != WFMath::Vector<3>::ZERO();
	bool sendToServer = false;

	//first we check if we are already moving
	if (!wasMoving) {
		//we are not moving. Should we start to move?
		if (isMoving) {
			//we'll start moving
			//let's send the movement command to the server
			sendToServer = true;

		}
	} else {
		//we are already moving
		//let's see if we've changed speed or direction or even stopped
		if (!isMoving) {
			S_LOG_VERBOSE( "Avatar stopped moving.");
			//we have stopped; we must alert the server
			sendToServer = true;
		} else if (newMovementState.movement != mCurrentMovementState.movement || !(newMovementState.orientation == mCurrentMovementState.orientation)) {
			//either the speed or the direction has changed
			sendToServer = true;
		}
	}

	if (sendToServer) {
		S_LOG_VERBOSE("Sending move op to server.");

		///Save the five latest orientations sent to the server, so we can later when we recieve an update from the server we can recognize that it's our own updates and ignore them.
		long currentTime = Ember::EmberServices::getSingleton().getTimeService()->currentTimeMillis();
		mLastTransmittedMovements.push_back(TimedMovementStateList::value_type(currentTime, newMovementState));
		if (mLastTransmittedMovements.size() > 5) {
			mLastTransmittedMovements.erase(mLastTransmittedMovements.begin());
		}
		Ember::EmberServices::getSingletonPtr()->getServerService()->moveInDirection(newMovementState.movement, newMovementState.orientation);

	}

	mCurrentMovementState = newMovementState;

	//After we're done, set the current movement to zero. This of course means that you can only call this method once per frame.
	mCurrentMovement = WFMath::Vector<3>::ZERO();

}

//void Avatar::attemptRotate(MovementControllerMovement& movement)
//{
//	//TODO: remove the direct references to AvatarCamera
///*	float degHoriz = movement.rotationDegHoriz;
//	float degVert = movement.rotationDegVert;
//	Ogre::Real timeSlice = movement.timeSlice;*/
//
////	mAccumulatedHorizontalRotation += (degHoriz * timeSlice);
//
//	//if we're moving we must sync the rotation with messages sent to the server
//	if (!movement.isMoving && fabs(getAvatarCamera()->getYaw().valueDegrees()) > mThresholdDegreesOfYawForAvatarRotation) {
////		mAvatarNode->setOrientation(movement.cameraOrientation);
////		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
//		Ogre::Degree yaw = getAvatarCamera()->getYaw();
//		getAvatarCamera()->yaw(-yaw);
////		mAccumulatedHorizontalRotation = 0;
//	} else if (isOkayToSendRotationMovementChangeToServer() && (getAvatarCamera()->getYaw().valueDegrees())) {
//		// rotate the Avatar Node only in X position (no vertical rotation)
////		mAvatarNode->setOrientation(movement.cameraOrientation);
////		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,getAvatarCamera()->getYaw());
//		Ogre::Degree yaw = getAvatarCamera()->getYaw();
//		getAvatarCamera()->yaw(-yaw);
//
////		mAvatarNode->rotate(Ogre::Vector3::UNIT_Y,mAccumulatedHorizontalRotation);
////		mAccumulatedHorizontalRotation = 0;
//	}

//}

EmberEntity& Avatar::getEmberEntity()
{
	return mErisAvatarEntity;
}

bool Avatar::isOkayToSendRotationMovementChangeToServer()
{
	//Just check if we've recently sent something to the server
	if (!mLastTransmittedMovements.size()) {
		return true;
	}
	long currentTime = Ember::EmberServices::getSingleton().getTimeService()->currentTimeMillis();
	if ((currentTime - mLastTransmittedMovements.rbegin()->first) > mMinIntervalOfRotationChanges) {
		return true;
	}
	return false;
}

Ogre::Node* Avatar::getAvatarSceneNode() const
{
	NodeAttachment* attachment = dynamic_cast<NodeAttachment*> (mErisAvatarEntity.getAttachment());
	if (attachment) {
		return attachment->getNode();
	}
	return 0;
}

void Avatar::movedInWorld()
{
	///only snap the avatar to the postition and orientation sent from the server if we're not moving or if we're not recently changed location
	///The main reason when moving is that we don't want to have the avatar snapping back in the case of lag
	///However, if we've just recently changed location, we need to also update the orientation to work with the new location.
	//	if (!mCurrentMovementState.isMoving || mHasChangedLocation)
	//	{
	//		const WFMath::Quaternion& orient = mErisAvatarEntity->getOrientation();
	//		bool isOwnRotation = false;
	//		///Check if the new orientation is one that we sent ourself, and if so ignore it
	//		for (std::list<WFMath::Quaternion>::const_iterator I = mLastOrientations.begin(); I != mLastOrientations.end(); ++I) {
	//			if (orient == *I) {
	//				isOwnRotation = true;
	//				break;
	//			}
	//		}
	//		if (!isOwnRotation) {
	//			mAvatarNode->setOrientation(Convert::toOgre(orient));
	//		}
	//		mAvatarNode->setPosition(Convert::toOgre(mErisAvatarEntity->getPosition()));
	//		///we must set this, else ember will think that we've rotated the avatar ourselves and try to send an update to the server
	//		mMovementStateAtLastServerMessage.orientation = mAvatarNode->getOrientation();
	//		mHasChangedLocation = false;
	//	}
}

void Avatar::avatar_LocationChanged(Eris::Entity* entity)
{
	///if we've changed location, we need to update the orientation. This is done on the next onMoved event, which is why we must honour the updated values on next onMoved event, even though we might be moving.
	mHasChangedLocation = true;
	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPredictedPos();
}

void Avatar::avatar_Moved()
{
	for (TimedMovementStateList::const_reverse_iterator I = mLastTransmittedMovements.rbegin(); I != mLastTransmittedMovements.rend(); ++I) {
		if (WFMath::Distance(I->second.position, mErisAvatarEntity.getPosition()) < 2) {
			return;
		}
	}
	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPredictedPos();
}

void Avatar::entity_ChildAdded(Eris::Entity* childEntity)
{
	mEntitiesToBeAddedToInventory.insert(childEntity);
}

void Avatar::entity_ChildRemoved(Eris::Entity* childEntity)
{
	EventRemovedEntityFromInventory.emit(static_cast<EmberEntity*> (childEntity));
}

void Avatar::Config_AvatarRotationUpdateFrequency(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	setMinIntervalOfRotationChanges(static_cast<double> (variable));
}

void Avatar::Config_MaxSpeed(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	mMaxSpeed = static_cast<double> (variable);
}

void Avatar::Config_LogChatMessages(const std::string& section, const std::string& key, varconf::Variable& variable)
{
	if (static_cast<bool> (variable)) {
		mChatLoggerParent = std::auto_ptr<AvatarLoggerParent>(new AvatarLoggerParent(*this));
	} else {
		mChatLoggerParent.reset();
	}
}

WFMath::Point<3> Avatar::getClientSideAvatarPosition() const
{
	//If the avatar entity is moving, we should prefer that position instead
	//TODO: alter this so that we can override client movement ourselves, but only when we're moving on the client.
	if (mCurrentMovement == WFMath::Vector<3>::ZERO() && mErisAvatarEntity.isMoving()) {
		return mErisAvatarEntity.getPredictedPos();
	}
	return mClientSideAvatarPosition;
}

WFMath::Quaternion Avatar::getClientSideAvatarOrientation() const
{
	if (mCurrentMovement == WFMath::Vector<3>::ZERO() && mErisAvatarEntity.isMoving()) {
		return mErisAvatarEntity.getOrientation();
	}
	return mClientSideAvatarOrientation;
}

}

