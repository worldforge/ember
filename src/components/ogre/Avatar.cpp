#include <memory>

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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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
#include "components/ogre/World.h"
#include "EntityWorldPickListener.h"

#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/camera/ThirdPersonCameraMount.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelRepresentation.h"

#include "components/ogre/authoring/EntityMaker.h"
#include "components/ogre/GUIManager.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include "framework/Tokeniser.h"
#include "framework/TimeHelper.h"
#include "framework/MainLoopController.h"

#include "terrain/ITerrainAdapter.h"

#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/TypeInfo.h>
#include <Eris/TypeService.h>
#include <Eris/View.h>
#include <Eris/Task.h>
#include <Eris/Response.h>

#include <wfmath/atlasconv.h>

#include <OgreRoot.h>
#include <OgreCamera.h>
#include <OgreTagPoint.h>

#include <wfmath/stream.h>

namespace Ember {
namespace OgreView {

Avatar::Avatar(Eris::Avatar& erisAvatar,
			   EmberEntity& erisAvatarEntity,
			   Scene& scene,
			   const Camera::CameraSettings& cameraSettings,
			   Terrain::ITerrainAdapter& terrainAdapter) :
		SetAttachedOrientation("setattachedorientation", this, "Sets the orientation of an item attached to the avatar: <attachpointname> <x> <y> <z> <degrees>"),
		AvatarActionDefaultStart("+avatar_action_default", this, "Performs the default action for the avatar."),
		AvatarActionDefaultStop("-avatar_action_default", this, "Stops performing the default action for the avatar."),
		Delete("delete", this, "Deletes an entity."),
		Say("say", this, "Say something."),
		SayTo("sayto", this, "Say something address to one or many entities. Format: '/sayto entityid,entityid,... message"),
		Emote("me", this, "Emotes something."),
		AdminTell("admin_tell", this, "Uses admin mode to directly tell a NPC something. Usage: /admin_tell <entityid> <key> <value>"),
		mMinIntervalOfRotationChanges(1000),
		mErisAvatar(erisAvatar),
		mErisAvatarEntity(erisAvatarEntity),
		mAvatarAttachmentController(new AvatarAttachmentController(*this)),
		mCameraMount(new Camera::ThirdPersonCameraMount(cameraSettings, scene, terrainAdapter)),
		mIsAdmin(false),
		mHasChangedLocation(false),
		mIsMovingServerOnly(false),
		mScene(scene),
		mEntityMaker(new Authoring::EntityMaker(erisAvatar, *EmberServices::getSingleton().getServerService().getConnection())) {

	MainLoopController::getSingleton().EventAfterInputProcessing.connect(sigc::mem_fun(*this, &Avatar::application_AfterInputProcessing));

	registerConfigListener("general", "logchatmessages", sigc::mem_fun(*this, &Avatar::Config_LogChatMessages));
	registerConfigListener("general", "avatarrotationupdatefrequency", sigc::mem_fun(*this, &Avatar::Config_AvatarRotationUpdateFrequency));

	mCurrentMovementState.movement = WFMath::Vector<3>::ZERO();
	mCurrentMovementState.orientation = WFMath::Quaternion().identity();
	mCurrentMovementState.position = erisAvatarEntity.getPredictedPos();

	//check if the user is of type "creator" and thus an admin
	//TODO: get from properties instead
	auto& typeService = EmberServices::getSingleton().getServerService().getConnection()->getTypeService();
	if (mErisAvatarEntity.getType()->isA(typeService.getTypeByName("creator"))) {
		mIsAdmin = true;
		erisAvatar.setIsAdmin(true);
	} else {
		mIsAdmin = false;
	}

	mErisAvatarEntity.EventAttachmentChanged.connect(sigc::mem_fun(*this, &Avatar::attachCameraToEntity));
	mErisAvatarEntity.EventChangedGraphicalRepresentation.connect(sigc::mem_fun(*this, &Avatar::attachCameraToEntity));
	mErisAvatarEntity.Moved.connect(sigc::mem_fun(*this, &Avatar::avatar_Moved));
	mErisAvatarEntity.ChildAdded.connect(sigc::mem_fun(*this, &Avatar::entity_ChildAdded));
	mErisAvatarEntity.ChildRemoved.connect(sigc::mem_fun(*this, &Avatar::entity_ChildRemoved));

	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPosition();

	mErisAvatarEntity.setAttachmentControlDelegate(mAvatarAttachmentController.get());

	mCameraMount->attachToNode(getAvatarSceneNode());

	erisAvatar.CharacterEntityDeleted.connect(sigc::mem_fun(*this, &Avatar::viewEntityDeleted));

	attachCameraToEntity();
}

//The Eris entity is already deleted when this is called.
Avatar::~Avatar() = default;

void Avatar::runCommand(const std::string& command, const std::string& args) {
	if (SetAttachedOrientation == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string attachPointName = tokeniser.nextToken();
		if (!attachPointName.empty()) {
			std::string x = tokeniser.nextToken();
			std::string y = tokeniser.nextToken();
			std::string z = tokeniser.nextToken();
			std::string degrees = tokeniser.nextToken();
			if (!x.empty() && !y.empty() && !z.empty() && !degrees.empty()) {
				Ogre::Degree ogreDegrees(Ogre::StringConverter::parseReal(degrees));
				Ogre::Quaternion rotation(ogreDegrees, Ogre::Vector3(Ogre::StringConverter::parseReal(x), Ogre::StringConverter::parseReal(y), Ogre::StringConverter::parseReal(z)));
				Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentation::getRepresentationForEntity(mErisAvatarEntity);
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
	} else if (AvatarActionDefaultStart == command) {
		performDefaultUsage();
	} else if (Delete == command) {
		Tokeniser tokeniser(args);
		std::string entityId = tokeniser.nextToken();
		if (!entityId.empty()) {
			deleteEntity(entityId);
		}
	} else if (Say == command) {
		mErisAvatar.say(args);
		std::string msg;
		msg = "Saying: [" + args + "]. ";
		S_LOG_VERBOSE(msg);
	} else if (SayTo == command) {
		Tokeniser tokeniser(args);
		std::string entityIdsString = tokeniser.nextToken();
		std::vector<std::string> entityIds = Tokeniser::split(entityIdsString, ",");
		std::string message = tokeniser.remainingTokens();

		mErisAvatar.sayTo(message, entityIds);

		std::string msg;
		if (entityIds.size() > 1) {
			msg = "Saying to many entities: [" + message + "]. ";
		} else if (entityIds.size() == 1) {
			msg = "Saying to entity " + entityIds[0] + ": [" + message + "]. ";
		} else {
			msg = "Saying to no entity: [" + message + "]. ";
		}
		S_LOG_VERBOSE(msg);
	} else if (Emote == command) {
		mErisAvatar.emote(args);

	} else if (AdminTell == command) {
		Tokeniser tokeniser(args);
		std::string entityId = tokeniser.nextToken();
		if (!entityId.empty()) {
			std::string key = tokeniser.nextToken();
			if (!key.empty()) {
				std::string value = tokeniser.nextToken();
				if (!value.empty()) {
					adminTell(entityId, key, value);
				}
			}
		}
	}
}

Camera::ThirdPersonCameraMount& Avatar::getCameraMount() const {
	return *mCameraMount;
}

void Avatar::setMinIntervalOfRotationChanges(Ogre::Real milliseconds) {
	mMinIntervalOfRotationChanges = milliseconds;
}

void Avatar::application_AfterInputProcessing(float timeSinceLastEvent) {
	attemptMove();
}

void Avatar::moveClientSide(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice) {
	//Need to invert movement to fit with models
	mCurrentMovement = WFMath::Vector<3>(-movement.x(), movement.y(), -movement.z());
	if (movement != WFMath::Vector<3>::ZERO()) {

		if (isOkayToSendRotationMovementChangeToServer()) {
			//We need to constraint the orientation to only around the y axis.
			WFMath::Vector<3> rotator(0.0, 0.0, 1.0f);
			rotator.rotate(orientation);
			auto atan = std::atan2(rotator.x(), rotator.z());
			WFMath::Quaternion adjustedOrientation(1, atan);
			//Need to invert movement to fit with models
			adjustedOrientation.rotate(WFMath::Quaternion(1, WFMath::numeric_constants<float>::pi()));

			mClientSideAvatarOrientation = adjustedOrientation;
		}
		mCurrentMovement.rotate(mClientSideAvatarOrientation);
		mClientSideAvatarPosition += mCurrentMovement * timeslice; //TODO: take speed-ground property into account

		if (mErisAvatarEntity.getAttachment()) {
			mErisAvatarEntity.getAttachment()->updatePosition();
		}
	}
}

void Avatar::attemptMove() {

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
		long long currentTime = TimeHelper::currentTimeMillis();
		mLastTransmittedMovements.emplace_back(currentTime, newMovementState);
		if (mLastTransmittedMovements.size() > 10) {
			mLastTransmittedMovements.erase(mLastTransmittedMovements.begin());
		}

		mErisAvatar.moveInDirection(newMovementState.movement, newMovementState.orientation);

	}

	mCurrentMovementState = newMovementState;

	//After we're done, set the current movement to zero. This of course means that you can only call this method once per frame.
	mCurrentMovement = WFMath::Vector<3>::ZERO();

}

EmberEntity& Avatar::getEmberEntity() {
	return mErisAvatarEntity;
}

bool Avatar::isOkayToSendRotationMovementChangeToServer() {
	//Just check if we've recently sent something to the server
	if (mLastTransmittedMovements.empty()) {
		return true;
	}
	long long currentTime = TimeHelper::currentTimeMillis();
	if ((currentTime - mLastTransmittedMovements.rbegin()->first) > mMinIntervalOfRotationChanges) {
		return true;
	}
	return false;
}

Ogre::Node* Avatar::getAvatarSceneNode() const {
	if (mErisAvatarEntity.getAttachment()) {
		auto* attachment = dynamic_cast<NodeAttachment*> (mErisAvatarEntity.getAttachment().get());
		if (attachment) {
			return attachment->getNode();
		}
	}
	return nullptr;
}

const std::string& Avatar::getId() const {
	return mErisAvatar.getId();
}


Scene& Avatar::getScene() const {
	return mScene;
}

void Avatar::setAttributes(const std::string& entityId, Atlas::Message::MapType& elements) {
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entityId);
		//We'll use this flag to make sure that nothing gets sent in the case that the only thing changed was immutable attributes (like "pos").
		bool areAttributesToSend = false;
		Atlas::Message::MapType moveAttributes;
		for (auto& element : elements) {
			if (element.first == "pos" ||
				element.first == "mode" ||
				element.first == "orientation" ||
				element.first == "planted-offset" ||
				element.first == "loc" ||
				element.first == "velocity" ||
				element.first == "propel") {
				moveAttributes.insert(element);
			} else {
				what->setAttr(element.first, element.second);
				areAttributesToSend = true;
			}
		}

		//Some attributes can only be changed through a Move op.
		if (!moveAttributes.empty()) {
			Atlas::Objects::Entity::Anonymous moveArgs;
			for (auto& element : moveAttributes) {
				moveArgs->setAttr(element.first, element.second);
			}

			moveArgs->setId(entityId);

			Atlas::Objects::Operation::Move moveOp;
			moveOp->setFrom(getId());
			moveOp->setArgs1(moveArgs);

			moveOp->setSerialno(Eris::getNewSerialno());
			mErisAvatar.getConnection().getResponder().await(moveOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
				//For now ignore; perhaps we should do some error checking instead?
				return Eris::Router::IGNORED;
			});

			//if the avatar is an admin, we will set the TO property
			//this will bypass all of the server's filtering, allowing us to place any
			//entity, unrelated to if it's too heavy or belong to someone else
			if (isAdmin()) {
				moveOp->setTo(entityId);
			}

			mErisAvatar.getConnection().send(moveOp);
		}


		if (areAttributesToSend) {
			Atlas::Objects::Operation::Set setOp;
			setOp->setFrom(getId());
			if (entityId != getId()) {
				setOp->setTo(entityId);
			}
			setOp->setArgs1(what);

			setOp->setSerialno(Eris::getNewSerialno());
			mErisAvatar.getConnection().getResponder().await(setOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
				//For now ignore; perhaps we should do some error checking instead?
				return Eris::Router::IGNORED;
			});
			S_LOG_INFO("Setting attributes of entity with id " << entityId);
			mErisAvatar.getConnection().send(setOp);
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on setting attributes on entity." << ex);
	}
}

void Avatar::adminTell(const std::string& entityId, const std::string& attribute, const std::string& value) {
	try {

		Atlas::Objects::Entity::Anonymous what;
		what->setAttr(attribute, value);
		Atlas::Objects::Operation::Talk talk;
		talk->setFrom(entityId);
		talk->setTo(entityId);
		talk->setArgs1(what);

		Atlas::Objects::Operation::Sound sound;
		sound->setFrom(getId());
		sound->setTo(entityId);
		sound->setArgs1(talk);

		sound->setSerialno(Eris::getNewSerialno());
		mErisAvatar.getConnection().getResponder().await(sound->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
			//Handle it here, so it does not propagate into the regular system.
			return Eris::Router::HANDLED;
		});
		S_LOG_INFO("Admin telling entity" << entityId << ": " << attribute << ": " << value);
		mErisAvatar.getConnection().send(sound);

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on admin_tell." << ex);
	}
}

void Avatar::deleteEntity(const std::string& entityId) {
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entityId);

		Atlas::Objects::Operation::Delete deleteOp;
		deleteOp->setFrom(getId());
		deleteOp->setTo(entityId);
		deleteOp->setArgs1(what);
		deleteOp->setSerialno(Eris::getNewSerialno());
		mErisAvatar.getConnection().getResponder().await(deleteOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
			//For now ignore; perhaps we should do some error checking instead?
			return Eris::Router::IGNORED;
		});

		S_LOG_INFO("Deleting entity with id " << entityId);
		mErisAvatar.getConnection().send(deleteOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on deleting entity." << ex);
	}
}

void Avatar::movedInWorld() {
	//only snap the avatar to the position and orientation sent from the server if we're not moving or if we're not recently changed location
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

void Avatar::attachCameraToEntity() {
	mCameraMount->attachToNode(getAvatarSceneNode());

	//if we've changed location, we need to update the orientation. This is done on the next onMoved event, which is why we must honour the updated values on next onMoved event, even though we might be moving.
	mHasChangedLocation = true;
	mClientSideAvatarOrientation = mErisAvatarEntity.getOrientation();
	mClientSideAvatarPosition = mErisAvatarEntity.getPredictedPos();


}

void Avatar::avatar_Moved() {
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
		mIsMovingServerOnly = mErisAvatarEntity.getVelocity() != WFMath::Vector<3>::ZERO();
	} else {
		mIsMovingServerOnly = false;
	}
}

void Avatar::entity_ChildAdded(Eris::Entity* childEntity) {
	EventAddedEntityToInventory.emit(dynamic_cast<EmberEntity*> (childEntity));
}

void Avatar::entity_ChildRemoved(Eris::Entity* childEntity) {
	EventRemovedEntityFromInventory.emit(childEntity);
}

void Avatar::Config_AvatarRotationUpdateFrequency(const std::string& section, const std::string& key, varconf::Variable& variable) {
	setMinIntervalOfRotationChanges(static_cast<double>(variable));
}

void Avatar::Config_LogChatMessages(const std::string& section, const std::string& key, varconf::Variable& variable) {
	if (static_cast<bool> (variable)) {
		mChatLogger = std::make_unique<AvatarLogger>(this->getEmberEntity());
	} else {
		mChatLogger.reset();
	}
}

WFMath::Point<3> Avatar::getClientSideAvatarPosition() const {
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

WFMath::Quaternion Avatar::getClientSideAvatarOrientation() const {
	//NOTE: for now we've deactivated the client side prediction as it doesn't really work as it should
	return mErisAvatarEntity.getOrientation().isValid() ? mErisAvatarEntity.getOrientation() : WFMath::Quaternion().identity();
//	if (mIsMovingServerOnly) {
//		return mErisAvatarEntity.getOrientation();
//	} else {
//		return mClientSideAvatarOrientation;
//	}
}

WFMath::Vector<3> Avatar::getClientSideAvatarVelocity() const {
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

void Avatar::useTool(const EmberEntity& tool,
					 const std::string& operation,
					 const Eris::Entity* target,
					 const WFMath::Point<3>& pos,
					 const WFMath::Vector<3>& direction) {

	const Eris::Usage* usage = nullptr;
	auto I = tool.getUsages().find(operation);
	if (I == tool.getUsages().end()) {
		I = tool.getUsagesProtected().find(operation);
		if (I != tool.getUsagesProtected().end()) {
			usage = &I->second;
		}
	} else {
		usage = &I->second;
	}

	if (!usage) {
		return;
	}

	useTool(tool, operation, *usage, target, pos, direction);

}

void Avatar::populateUsageArgs(Atlas::Objects::Entity::RootEntity& entity,
							   const std::map<std::string, Eris::UsageParameter>& params,
							   const Eris::Entity* target,
							   const WFMath::Point<3>& posInWorld,
							   WFMath::Vector<3> direction) {
	for (auto& param : params) {
		Atlas::Message::ListType list;
		if (param.second.type == "entity" || param.second.type == "entity_location") {
			if (target) {
				Atlas::Objects::Entity::RootEntity entityArg;
				entityArg->setId(target->getId());
				if (posInWorld.isValid()) {
					//pos must be relative to the entity
					//TODO: walk up and down the entity hierarchy; the current code only works for entities that share location
					auto localPos = posInWorld.toLocalCoords(target->getPredictedPos(), target->getPredictedOrientation());
					entityArg->setPosAsList(Atlas::Message::Element(localPos.toAtlas()).List());
				}
				list.emplace_back(entityArg->asMessage());
			}
		} else if (param.second.type == "position") {
			if (posInWorld.isValid()) {
				list.emplace_back(posInWorld.toAtlas());
			}
		} else {
			//If there's no direction, calculate it using the position.
			if (!direction.isValid() && posInWorld.isValid()) {
				auto avatarPos = mErisAvatarEntity.getPredictedPos();
				auto size = mErisAvatarEntity.getBBox().highCorner() - mErisAvatarEntity.getBBox().lowCorner();
				//When using something we have our hands at approx. 80% our height. Since the server will take this into account we need to also do it.
				//TODO: put this into a property so we have full sync between server and client.
				avatarPos.y() += size.y() * 0.8;
				direction = posInWorld - avatarPos;
			}
			if (direction.isValid()) {
				direction.normalize();
				list.emplace_back(direction.toAtlas());
			}
		}
		entity->setAttr(param.first, std::move(list));
	}
}


void Avatar::useTool(const EmberEntity& tool,
					 const std::string& operation,
					 const Eris::Usage& usage,
					 const Eris::Entity* target,
					 const WFMath::Point<3>& posInWorld,
					 WFMath::Vector<3> direction) {

	Atlas::Objects::Operation::Use use;
	use->setFrom(mErisAvatar.getId());

	Atlas::Objects::Entity::RootEntity entity;
	entity->setId(tool.getId());

	populateUsageArgs(entity, usage.params, target, posInWorld, direction);

	Atlas::Objects::Operation::RootOperation op;
	op->setParent(operation);
	op->setArgs1(entity);

	use->setArgs1(op);

	mErisAvatar.getConnection().send(use);

}

void Avatar::taskUsage(const std::string& taskId, const std::string& usageName) {
	auto I = mErisAvatarEntity.getTasks().find(taskId);
	if (I == mErisAvatarEntity.getTasks().end()) {
		return;
	}

	auto& usages = I->second->getUsages();
	auto J = std::find_if(usages.begin(), usages.end(), [&](const Eris::TaskUsage& usage) -> bool { return usage.name == usageName; });
	if (J == usages.end()) {
		return;
	}


	taskUsage(taskId, *J);
}


void Avatar::taskUsage(std::string taskId, const Eris::TaskUsage& usage) {

	auto ray = mScene.getMainCamera().getCameraToViewportRay(0.5, 0.5);

	auto& pickedResults = EmberOgre::getSingleton().getWorld()->getEntityPickListener().getPersistentResult();
	Eris::Entity* target = nullptr;
	WFMath::Point<3> posInWorld;
	for (auto& pickedResult : pickedResults) {
		if (pickedResult.entityRef) {
			target = pickedResult.entityRef.get();
			if (!pickedResult.position.isNaN()) {
				posInWorld = Convert::toWF<WFMath::Point<3>>(pickedResult.position);
			}
			break;
		}
	}

	auto direction = Convert::toWF<WFMath::Vector<3>>(ray.getDirection());


	Atlas::Objects::Operation::Use use;
	use->setFrom(mErisAvatar.getId());

	Atlas::Objects::Root task;
	task->setId(std::move(taskId));
	task->setObjtype("task");

	Atlas::Objects::Entity::RootEntity entity;
	entity->setId(usage.name);
	populateUsageArgs(entity, usage.params, target, posInWorld, direction);

	task->setAttr("args", Atlas::Message::ListType{entity->asMessage()});

	use->setArgs1(task);

	mErisAvatar.getConnection().send(use);
}

void Avatar::stopCurrentTask() {
	//Get the first task with a usage, and invoke that. The protocol is that the first usage always should be the "default".
	auto& tasks = mErisAvatarEntity.getTasks();
	for (auto& entry : tasks) {
		auto& task = entry.second;
		if (!task->getUsages().empty()) {
			taskUsage(entry.first, task->getUsages().front());
			break;
		}
	}
}

boost::optional<std::string> Avatar::performDefaultUsage() {
	//Check if there's a tool in our primary hand and use if, otherwise check if we have any default usages.


	auto useWithSelectedEntity = [&](EmberEntity& attachedEntity, const Eris::Usage& usage, const std::string& op) {
		auto ray = mScene.getMainCamera().getCameraToViewportRay(0.5, 0.5);

		auto& pickedResults = EmberOgre::getSingleton().getWorld()->getEntityPickListener().getPersistentResult();
		Eris::Entity* entity = nullptr;
		WFMath::Point<3> pos;
		for (auto& pickedResult : pickedResults) {
			if (pickedResult.entityRef) {
				entity = pickedResult.entityRef.get();
				if (!pickedResult.position.isNaN()) {
					pos = Convert::toWF<WFMath::Point<3>>(pickedResult.position);
				}
				break;
			}
		}

		useTool(attachedEntity, op, usage, entity, pos, Convert::toWF<WFMath::Vector<3>>(ray.getDirection()));
		if (entity) {
			GUIManager::getSingleton().EmitEntityAction("use:" + usage.name, dynamic_cast<EmberEntity*>(entity));
		}

	};

	auto attachedHandPrimaryElement = mErisAvatarEntity.ptrOfProperty("attached_hand_primary");
	if (attachedHandPrimaryElement) {
		auto attachedId = Eris::Entity::extractEntityId(*attachedHandPrimaryElement);
		if (attachedId && !attachedId->empty()) {
			auto attachedEntity = EmberOgre::getSingleton().getWorld()->getEmberEntity(*attachedId);
			if (attachedEntity) {
				auto& usages = attachedEntity->getUsages();
				if (!usages.empty()) {
					auto& op = usages.begin()->first;
					useWithSelectedEntity(*attachedEntity, usages.begin()->second, op);
					return op;
				}
			}
		}
	}
	//Could not find any attached tool with usages, check if there's a default on the character itself
	auto& selfUsages = mErisAvatarEntity.getUsagesProtected();
	if (!selfUsages.empty()) {
		auto& op = selfUsages.begin()->first;
		useWithSelectedEntity(mErisAvatarEntity, selfUsages.begin()->second, selfUsages.begin()->first);
		return op;
	}
	return boost::none;

}


}
}

