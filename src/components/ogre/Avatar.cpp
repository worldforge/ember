/*
 Avatar.cpp by Erik Ogenvik
 Copyright (C) 2002 Miguel Guzman, Erik Ogenvik & The Worldforge

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
#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/AvatarLogger.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/AvatarAttachmentController.h"
#include "components/ogre/Scene.h"

#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/camera/ThirdPersonCameraMount.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"

#include "components/ogre/authoring/EntityMaker.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "framework/Tokeniser.h"
#include "framework/Time.h"
#include "framework/MainLoopController.h"

#include "terrain/ITerrainAdapter.h"

#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>

#include <OgreRoot.h>

#include <OgreTagPoint.h>

#include <wfmath/stream.h>

namespace Ember
{
namespace OgreView
{

Avatar::Avatar(EmberEntity& erisAvatarEntity, Scene& scene, const Camera::CameraSettings& cameraSettings, Terrain::ITerrainAdapter& terrainAdapter) :
	SetAttachedOrientation("setattachedorientation", this, "Sets the orientation of an item attached to the avatar: <attachpointname> <x> <y> <z> <degrees>"), mErisAvatarEntity(erisAvatarEntity), mMaxSpeed(5), mAvatarAttachmentController(new AvatarAttachmentController(*this)), mCameraMount(new Camera::ThirdPersonCameraMount(cameraSettings, scene.getSceneManager(), terrainAdapter)), mIsAdmin(false), mHasChangedLocation(false), mChatLoggerParent(nullptr), mIsMovingServerOnly(false), mScene(scene), mEntityMaker(new Authoring::EntityMaker(erisAvatarEntity, *EmberServices::getSingleton().getServerService().getConnection()))
{
	setMinIntervalOfRotationChanges(1000); //milliseconds

	MainLoopController::getSingleton().EventAfterInputProcessing.connect(sigc::mem_fun(*this, &Avatar::application_AfterInputProcessing));

	registerConfigListener("general", "logchatmessages", sigc::mem_fun(*this, &Avatar::Config_LogChatMessages));
	registerConfigListener("general", "avatarrotationupdatefrequency", sigc::mem_fun(*this, &Avatar::Config_AvatarRotationUpdateFrequency));
	registerConfigListener("input", "maxspeed", sigc::mem_fun(*this, &Avatar::Config_MaxSpeed));

	mCurrentMovementState.movement = WFMath::Vector<3>::ZERO();
	mCurrentMovementState.orientation = WFMath::Quaternion().identity();
	mCurrentMovementState.position = erisAvatarEntity.getPredictedPos();

	//check if the user is of type "creator" and thus an admin
	Eris::TypeService* typeService = EmberServices::getSingleton().getServerService().getConnection()->getTypeService();
	if (mErisAvatarEntity.getType()->isA(typeService->getTypeByName("creator"))) {
		mIsAdmin = true;
		 EmberServices::getSingleton().getServerService().getAvatar()->setIsAdmin(true);
	} else {
		mIsAdmin = false;
	}

	mErisAvatarEntity.LocationChanged.connect(sigc::mem_fun(*this, &Avatar::avatar_LocationChanged));
	mErisAvatarEntity.EventChangedGraphicalRepresentation.connect(sigc::mem_fun(*this, &Avatar::attachCameraToEntity));
	mErisAvatarEntity.Moved.connect(sigc::mem_fun(*this, &Avatar::avatar_Moved));
	mErisAvatarEntity.ChildAdded.connect(sigc::mem_fun(*this, &Avatar::entity_ChildAdded));
	mErisAvatarEntity.ChildRemoved.connect(sigc::mem_fun(*this, &Avatar::entity_ChildRemoved));

	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPosition();

	mErisAvatarEntity.setAttachmentControlDelegate(mAvatarAttachmentController);

	mCameraMount->attachToNode(getAvatarSceneNode());

	mErisAvatarEntity.getView()->AvatarEntityDeleted.connect(sigc::mem_fun(*this, &Avatar::viewEntityDeleted));

	attachCameraToEntity();
}

Avatar::~Avatar()
{
	//The Eris entity is already deleted when this is called.
	//mErisAvatarEntity.setAttachmentControlDelegate(0);
	delete mAvatarAttachmentController;
	delete mEntityMaker;
}

void Avatar::runCommand(const std::string &command, const std::string &args)
{
	if (SetAttachedOrientation == command) {
		Tokeniser tokeniser;
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
					const auto& attachPoints = model.getAttachedPoints();
					if (attachPoints) {
						for (const auto& attachPoint : *attachPoints) {
							if (attachPoint.Definition.Name == attachPointName) {
								attachPoint.TagPoint->setOrientation(rotation);
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
	attemptMove();
}

void Avatar::moveClientSide(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice)
{
	mCurrentMovement = movement * mMaxSpeed;
	if (movement != WFMath::Vector<3>::ZERO()) {

		if (isOkayToSendRotationMovementChangeToServer()) {
			//We need to constraint the orientation to only around the z axis.
			WFMath::Vector<3> rotator(1.0, 0.0, 0.0);
			rotator.rotate(orientation);
			WFMath::Quaternion adjustedOrientation;
			adjustedOrientation.fromRotMatrix(WFMath::RotMatrix<3>().rotationZ(atan2(rotator.y(), rotator.x())));

			mClientSideAvatarOrientation = adjustedOrientation;
			//For some not quite explained reason we need to rotate the orientation 90 degrees around the z axis for the orientation to be correct.
			mClientSideAvatarOrientation.rotate(WFMath::Quaternion(WFMath::Vector<3>(0, 0, 1), WFMath::numeric_constants<WFMath::CoordType>::pi() / 2));
		}
		//...and then adjust the rotation 90 degrees in the other direction when calculating how to rotate the movement direction
		WFMath::Quaternion adjustedOrientation = mClientSideAvatarOrientation;
		adjustedOrientation.rotate(WFMath::Quaternion(WFMath::Vector<3>(0, 0, -1), WFMath::numeric_constants<WFMath::CoordType>::pi() / 2));
		mCurrentMovement.rotate(adjustedOrientation);
		mClientSideAvatarPosition += mCurrentMovement * timeslice;

		if (mErisAvatarEntity.getAttachment()) {
			mErisAvatarEntity.getAttachment()->updatePosition();
		}
	}
}

void Avatar::attemptMove()
{

	//first we'll register the current state in newMovementState and compare to mCurrentMovementState
	//that way we'll only send stuff to the server if our movement changes
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
			//we have stopped; we must alert the server
			sendToServer = true;
		} else if (newMovementState.movement != mCurrentMovementState.movement || newMovementState.orientation != mCurrentMovementState.orientation) {
			//either the speed or the direction has changed
			sendToServer = true;
		}
	}

	if (sendToServer) {
		std::stringstream ss;
		ss << "Sending move op to server, direction: " << newMovementState.movement << ", orientation: " << newMovementState.orientation << ", speed: " << sqrt(newMovementState.movement.sqrMag()) << ".";
		S_LOG_VERBOSE(ss.str());

		//Save the ten latest orientations sent to the server, so we can later when we receive an update from the server we can recognize that it's our own updates and ignore them.
		long long currentTime = Time::currentTimeMillis();
		mLastTransmittedMovements.push_back(TimedMovementStateList::value_type(currentTime, newMovementState));
		if (mLastTransmittedMovements.size() > 10) {
			mLastTransmittedMovements.erase(mLastTransmittedMovements.begin());
		}
		EmberServices::getSingleton().getServerService().moveInDirection(newMovementState.movement, newMovementState.orientation);

	}

	mCurrentMovementState = newMovementState;

	//After we're done, set the current movement to zero. This of course means that you can only call this method once per frame.
	mCurrentMovement = WFMath::Vector<3>::ZERO();

}

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
	long long currentTime = Time::currentTimeMillis();
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

Scene& Avatar::getScene() const
{
	return mScene;
}


void Avatar::movedInWorld()
{
	//only snap the avatar to the postition and orientation sent from the server if we're not moving or if we're not recently changed location
	//The main reason when moving is that we don't want to have the avatar snapping back in the case of lag
	//However, if we've just recently changed location, we need to also update the orientation to work with the new location.
	//	if (!mCurrentMovementState.isMoving || mHasChangedLocation)
	//	{
	//		const WFMath::Quaternion& orient = mErisAvatarEntity->getOrientation();
	//		bool isOwnRotation = false;
	//		//Check if the new orientation is one that we sent ourself, and if so ignore it
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
	//		//we must set this, else ember will think that we've rotated the avatar ourselves and try to send an update to the server
	//		mMovementStateAtLastServerMessage.orientation = mAvatarNode->getOrientation();
	//		mHasChangedLocation = false;
	//	}
}

void Avatar::avatar_LocationChanged(Eris::Entity* entity)
{
	attachCameraToEntity();
}

void Avatar::attachCameraToEntity()
{
	mCameraMount->attachToNode(getAvatarSceneNode());

	//if we've changed location, we need to update the orientation. This is done on the next onMoved event, which is why we must honour the updated values on next onMoved event, even though we might be moving.
	mHasChangedLocation = true;
	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPredictedPos();


}

void Avatar::avatar_Moved()
{
	//For now this is disabled, since there are a couple of issues with it
	//	for (TimedMovementStateList::iterator I = mLastTransmittedMovements.begin(); I != mLastTransmittedMovements.end(); ++I) {
	//		float distance = WFMath::Distance(I->second.position, mErisAvatarEntity.getPosition());
	//		if (I->second.movement != WFMath::Vector<3>::ZERO() && distance < 0.3) {
	//			mLastTransmittedMovements.erase(I);
	//			S_LOG_VERBOSE("Ignoring server movement update since it's sent by us.");
	//			return;
	//		}
	//	}
	if (mCurrentMovementState.movement == WFMath::Vector<3>::ZERO()) {
		mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
		mClientSideAvatarPosition = mErisAvatarEntity.getPosition();
		if (mErisAvatarEntity.getVelocity() != WFMath::Vector<3>::ZERO()) {
			mIsMovingServerOnly = true;
		} else {
			mIsMovingServerOnly = false;
		}
	} else {
		mIsMovingServerOnly = false;
	}
}

void Avatar::entity_ChildAdded(Eris::Entity* childEntity)
{
	EventAddedEntityToInventory.emit(static_cast<EmberEntity*> (childEntity));
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
		mChatLoggerParent = std::unique_ptr<AvatarLoggerParent>(new AvatarLoggerParent(*this));
	} else {
		mChatLoggerParent.reset();
	}
}

WFMath::Point<3> Avatar::getClientSideAvatarPosition() const
{
	//NOTE: for now we've deactivated the client side prediction as it doesn't really work as it should
	WFMath::Point<3> pos = mErisAvatarEntity.getPredictedPos();
	return pos.isValid() ? pos : WFMath::Point<3>::ZERO();
//	//If the avatar entity is moving, we're note moving on the client side, and we haven't sent something to the server lately, we should assume that we're moving as a result of server side actions, and therefore use the server side position
//	//	if (mCurrentMovement == WFMath::Vector<3>::ZERO() && mErisAvatarEntity.isMoving()) {
//	//		bool clientSideMovement = false;
//	//		if (mLastTransmittedMovements.size()) {
//	//			long long currentTime = EmberServices::getSingleton().getTimeService().currentTimeMillis();
//	//			if ((currentTime - mLastTransmittedMovements.rbegin()->first) < 1000) {
//	//				clientSideMovement = true;
//	//			}
//	//		}
//	//		if (!clientSideMovement) {
//	//			return mErisAvatarEntity.getPredictedPos();
//	//		}
//	//
//	//	}
//	if (mIsMovingServerOnly) {
//		return mErisAvatarEntity.getPredictedPos();
//	} else {
//		return mClientSideAvatarPosition;
//	}
}

WFMath::Quaternion Avatar::getClientSideAvatarOrientation() const
{
	//NOTE: for now we've deactivated the client side prediction as it doesn't really work as it should
	return mErisAvatarEntity.getOrientation().isValid() ? mErisAvatarEntity.getOrientation() : WFMath::Quaternion().identity();
//	if (mIsMovingServerOnly) {
//		return mErisAvatarEntity.getOrientation();
//	} else {
//		return mClientSideAvatarOrientation;
//	}
}

WFMath::Vector<3> Avatar::getClientSideAvatarVelocity() const
{
	//NOTE: for now we've deactivated the client side prediction as it doesn't really work as it should
	return mErisAvatarEntity.getVelocity().isValid() ? mErisAvatarEntity.getVelocity() : WFMath::Vector<3>::ZERO();
//	if (mIsMovingServerOnly) {
//		return mErisAvatarEntity.getVelocity();
//	} else {
//		return mCurrentMovementState.movement;
//	}
}

void Avatar::viewEntityDeleted() {
	EventAvatarEntityDestroyed();
}



}
}

