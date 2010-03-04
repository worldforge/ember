/*
 Avatar.h by Miguel Guzman (Aglanor)
 Copyright (C) 2002 Miguel Guzman & The Viewforge Project
 Copyright (C) 2004 Erik Hjortsberg

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

#ifndef AVATAR_H
#define AVATAR_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "services/config/ConfigListenerContainer.h"
#include "framework/ConsoleObject.h"

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <wfmath/point.h>
#include <wfmath/quaternion.h>
#include <wfmath/vector.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include <memory>
#include <set>

namespace Eris
{
class Entity;
}

namespace EmberOgre
{

namespace Model
{
class Model;
}
namespace Camera
{
class ThirdPersonCameraMount;
}

class EmberEntity;
class AvatarLogger;
class AvatarLoggerParent;
class AvatarAttachmentController;

struct AvatarMovementState
{
public:
	WFMath::Vector<3> movement;
	WFMath::Quaternion orientation;
	WFMath::Point<3> position;
};

/**
 * @brief Represents the avatar used by the client on the server.
 *
 * In general it receives instructions from mainly an instance of MovementController to attempt to move or rotate the avatar.
 * This class is also responsible for making sure that the client local representation of the avatar is kept up to date with the server side representation. This can involve some client side prediction with regards to movement.
 */
class Avatar: public sigc::trackable, public Ember::ConfigListenerContainer, public Ember::ConsoleObject
{
	friend class MovementController;

public:

	/**
	 * @brief Ctor.
	 * @param erisAvatarEntity The entity which represents the avatar.
	 */
	Avatar(EmberEntity& erisAvatarEntity);

	/**
	 * @brief Dtor.
	 */
	virtual ~Avatar();

	/**
	 * @brief Gets the scene node which the avatar is attached to, if any.
	 * @return The scene node to which the avatar entity is attached to, or null if the avatar isn't attached to any scene node.
	 */
	Ogre::Node* getAvatarSceneNode() const;

	/**
	 * @brief Call this when the avatar entity has moved in the world.
	 */
	void movedInWorld();

	/**
	 * @brief Accessor for the Eris::Entity which represents the Avatar.
	 * @return
	 */
	EmberEntity& getEmberEntity();

	/**
	 * @brief Sets the minimum interval to wait before sending new rotation changes to the server. This is not done instantly to prevent swamping of data to the server. Set this lower if you experience too jerky game play
	 * @param milliseconds The amount of milliseconds to wait between sending rotation updates to the server.
	 */
	void setMinIntervalOfRotationChanges(Ogre::Real milliseconds);

	/**
	 * @brief Emitted when an entity is added to the inventory.
	 */
	sigc::signal<void, EmberEntity*> EventAddedEntityToInventory;

	/**
	 * @brief Emitted when an entity is removed from the inventory.
	 */
	sigc::signal<void, EmberEntity*> EventRemovedEntityFromInventory;

	/**
	 * @brief True if the current user have admin rights, i.e. is a "creator".
	 */
	bool isAdmin() const;

	void moveClientSide(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice);

	/**
	 * @brief Gets the client side position of the avatar.
	 * This can differ from the position of the entity which represents the avatar, since the player can move the avatar before the server receives updates.
	 * @return The position of the avatar, as the client sees it.
	 */
	WFMath::Point<3> getClientSideAvatarPosition() const;

	/**
	 * @brief Gets the client side orientation of the avatar.
	 * This can differ from the orientation of the entity which represents the avatar, since the player can move the avatar before the server receives updates.
	 * @return The orientation of the avatar, as the client sees it.
	 */
	WFMath::Quaternion getClientSideAvatarOrientation() const;

	/**
	 * @brief Gets the client side velocity of the avatar.
	 * This can differ from the velocity of the entity which represents the avatar, since the player can move the avatar before the server receives updates.
	 * @return The velocity of the avatar, as the client sees it.
	 */
	WFMath::Vector<3> getClientSideAvatarVelocity() const;

	/**
	 * @brief Gets the camera mount attached to the avatar.
	 * @return The third person camera mount attached to the avatar.
	 */
	Camera::ThirdPersonCameraMount& getCameraMount() const;

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 * @brief Allows setting of the right hand attachment's orientation. This is mainly for debugging purposes and should removed once we get a better editor in place.
	 */
	const Ember::ConsoleCommandWrapper SetAttachedOrientation;

protected:

	typedef std::list<std::pair<long, AvatarMovementState> > TimedMovementStateList;

	/**
	 * @brief In milliseconds, the minimum time we must wait between sending updates to the server. Set this higher to avoid spamming the server.
	 */
	Ogre::Real mMinIntervalOfRotationChanges;

	/**
	 * @brief The Eris::Entity which represents the Avatar.
	 */
	EmberEntity& mErisAvatarEntity;

	/**
	 * @brief How many meters per second the avatar can move, at maximum speed.
	 * This should be set through some kind of rule checking with the server
	 * depending on the character. To be done later.
	 */
	float mMaxSpeed;

	AvatarAttachmentController* mAvatarAttachmentController;

	/**
	 * @brief Used to make sure starts and stops of movement is only sent to the server once.
	 */
	AvatarMovementState mCurrentMovementState;

	/**
	 * @brief Keep a list of the last transmitted movements, so that when we receive a movement update from the server, we can ignore it if it's something we know we sent ourselves.
	 */
	TimedMovementStateList mLastTransmittedMovements;

	/**
	 * @brief Keep a temporary list of entities that needs to be added to the inventory.
	 */
	std::set<Eris::Entity*> mEntitiesToBeAddedToInventory;

	/**
	 * @brief Keep a temporary list of entities that needs to be removed from the inventory.
	 */
	std::set<Eris::Entity*> mEntitiesToBeRemovedFromInventory;

	/**
	 * @brief A third person camera mount, tracking the avatar entity.
	 * This mount is the default one used when controlling an avatar.
	 */
	std::auto_ptr<Camera::ThirdPersonCameraMount> mCameraMount;

	/**
	 * @brief True if the current user have admin rights, i.e. is a "creator".
	 */
	bool mIsAdmin;

	/**
	 * @brief If set to true, the avatar has just changed location, so the next onMoved operation will contain the new orientation and position information for the new location.
	 */
	bool mHasChangedLocation;

	/**
	 * @brief Holds the objects which logs ingame messages to a file. We don't hold a AvatarLogger instance directly, instead using the AvatarLoggerParent class, since we can't really create an instance of AvatarLogger until we've gotten an AvatarEmberEntity, and the AvatarLoggerParent class will take care of all that.
	 */
	std::auto_ptr<AvatarLoggerParent> mChatLoggerParent;

	/**
	 * @brief We save the five latest orientations that we send to the server.
	 * The reason we do this is to be able to recognize updates sent from the server that's our own updates. Since there might be lag, these updates can arrive half a second or more after they were sent. They should thus be ignored to avoid the camera jumping all over the place.
	 */
	std::list<WFMath::Quaternion> mLastOrientations;

	/**
	 * @brief The avatar entity position, as the client sees it.
	 * This might differ from the position reported by the entity, as we allow the client side avatar entity to move in advance of getting updates from the server.
	 */
	WFMath::Point<3> mClientSideAvatarPosition;

	/**
	 * @brief The avatar entity orientation, as the client sees it.
	 * This might differ from the orientation reported by the entity, as we allow the client side avatar entity to move in advance of getting updates from the server.
	 */
	WFMath::Quaternion mClientSideAvatarOrientation;

	/**
	 * @brief The avatar entity velocity, as the client sees it.
	 * This might differ from the velocity reported by the entity, as we allow the client side avatar entity to move in advance of getting updates from the server.
	 */
	WFMath::Vector<3> mCurrentMovement;

	/**
	 * @brief Set to true if the movement of the avatar only occurred on the server (i.e. the user did not initiate the movement).
	 * Normally we'll ignore most of the movement updates sent from the server, since we want to provide client side prediction. But if the server is controlling the movement we instead want to use the server side position updates directly.
	 */
	bool mIsMovingServerOnly;

	/**
	 * @brief This method will determine if it's ok to send a small movement change, such as a small deviation direction during an already begun movement to the server.
	 */
	bool isOkayToSendRotationMovementChangeToServer();

	/**
	 * @brief Attempts to move the avatar in a certain direction.
	 * Note that depending on what the rules allows (i.e. collision detection, character rules etc.) the outcome of the attempt is uncertain.
	 */
	void attemptMove();

	/**
	 * @brief Listen for location changes, since after a location change we need to honour the onMoved updates even if we're in movement mode.
	 * @param entity The new parent entity.
	 */
	void avatar_LocationChanged(Eris::Entity* entity);

	/**
	 * @brief Listen to avatar movement and update the internal representation of the position and orientation of the avatar if suitable.
	 */
	void avatar_Moved();

	/**
	 * @brief Listen to child entities being added to the avatar entity and add the entity to the queue of entities which should be emitted as added to the inventory at the next frame.
	 */
	void entity_ChildAdded(Eris::Entity* childEntity);

	/**
	 * @brief Listen to child entities being removed from the avatar entity and emit the inventory removal event.
	 */
	void entity_ChildRemoved(Eris::Entity* childEntity);

	/**
	 * @brief After input has been processed we'll do a check to see if we've changed velocity or orientation, and therefore needs to send an update to the server.
	 */
	void application_AfterInputProcessing(float timeSinceLastEvent);

	/**
	 * @brief Listen for changes of the general:logchatmessages config key and create and destroy an instance of AvatarLogger (actually AvatarLoggerParent) accordingly.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_LogChatMessages(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Bind the frequency of rotation updates being sent to the server when the camera moves to the general:avatarrotationupdatefrequency key
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_AvatarRotationUpdateFrequency(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 * @brief Bind the max speed to the input:runspeed key. Note that this is capped in the end by the server.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_MaxSpeed(const std::string& section, const std::string& key, varconf::Variable& variable);

};

inline bool Avatar::isAdmin() const
{
	return mIsAdmin;
}

}

#endif // ENTITY_LISTENER_H
