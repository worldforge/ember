/*
 Avatar.h by Miguel Guzman (Aglanor)
 Copyright (C) 2002 Miguel Guzman & The Viewforge Project
 Copyright (C) 2004 Erik Ogenvik

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

#ifndef EMBEROGRE_AVATAR_H
#define EMBEROGRE_AVATAR_H

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
#include <domain/EmberEntity.h>
#include <Eris/Task.h>

namespace Eris {
class Entity;

class Avatar;
}

namespace Ember {
class EmberEntity;
namespace OgreView {

namespace Model {
class Model;
}
namespace Camera {
class ThirdPersonCameraMount;

class CameraSettings;
}
namespace Authoring {
class EntityMaker;
}
namespace Terrain {
struct ITerrainAdapter;
}

class AvatarLogger;

class AvatarLoggerParent;

class AvatarAttachmentController;

class Scene;

struct AvatarMovementState {
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
class Avatar : public virtual sigc::trackable, public ConfigListenerContainer, public ConsoleObject {
	friend class MovementController;

public:

	/**
	 * @brief Ctor.
	 * @param erisAvatarEntity The entity which represents the avatar.
	 * @param scene The scene in which the entities are shown.
	 * @param cameraSettings Camera settings.
	 * @param terrainAdapter The terrain adapter needed for the third person camera.
	 */
	Avatar(Eris::Avatar& erisAvatar, EmberEntity& erisAvatarEntity, Scene& scene, const Camera::CameraSettings& cameraSettings, Terrain::ITerrainAdapter& terrainAdapter);

	/**
	 * @brief Dtor.
	 */
	~Avatar() override;

	const std::string& getId() const;

	/**
	 * @brief Gets the scene node which the avatar is attached to, if any.
	 * @return The scene node to which the avatar entity is attached to, or null if the avatar isn't attached to any scene node.
	 */
	Ogre::Node* getAvatarSceneNode() const;

	/**
	 * @brief Gets the scene in which the entities are shown.
	 * @returns The scene in which entities are shown.
	 */
	Scene& getScene() const;

	/**
	 * @brief Call this when the avatar entity has moved in the world.
	 */
	void movedInWorld();

	Eris::Avatar& getErisAvatar() const;

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
	 * @brief Emitted when the entity the avatar controls is deleted on the server.
	 *
	 * I.e. when a SIGHT of the destruction of the entity is received. This differs from the
	 * Eris entity being deleted in the client as normal shutdown procedure.
	 *
	 * Note that this signal essentially is the same emitted by Eris::View::AvatarEntityDeleted.
	 */
	sigc::signal<void> EventAvatarEntityDestroyed;

	/**
	 * @brief True if the current user have admin rights, i.e. is a "creator".
	 */
	bool isAdmin() const;

	/**
	 * @brief Called when the avatar has moved client side.
	 *
	 * I.e. when the avatar representation on the client has moved.
	 * This movement will be sent to the server, but depending on network time etc. there might be a slight difference in client side and server side position.
	 *
	 * @param orientation The client side orientation.
	 * @param movement The movement, as a normalized vector.
	 * @param timeslice The duration for this movement, in seconds. The actual distance moved will thus be calculated using the movement, along with the max speed and the timeslice.
	 */
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

	void useTool(const EmberEntity& tool,
				 const std::string& operation,
				 const Eris::Entity* target = nullptr,
				 const WFMath::Point<3>& pos = WFMath::Point<3>(),
				 const WFMath::Vector<3>& direction = WFMath::Vector<3>());

	void useTool(const EmberEntity& tool,
				 const std::string& operation,
				 const Eris::Usage& usage,
				 const Eris::Entity* target = nullptr,
				 const WFMath::Point<3>& posInWorld = WFMath::Point<3>(),
				 WFMath::Vector<3> direction = WFMath::Vector<3>());

	void taskUsage(std::string taskId, std::string usage);

	void taskUsage(std::string taskId, const Eris::TaskUsage& usage);

	boost::optional<std::string> performDefaultUsage();

	void stopCurrentTask();

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	void runCommand(const std::string& command, const std::string& args) override;

	void deleteEntity(Eris::Entity* entity);

	void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements);

	void adminTell(const std::string& entityId, const std::string& attribute, const std::string& value);


	/**
	 * @brief Allows setting of the right hand attachment's orientation. This is mainly for debugging purposes and should removed once we get a better editor in place.
	 */
	const ConsoleCommandWrapper SetAttachedOrientation;

	const ConsoleCommandWrapper AvatarActionDefaultStart;
	const ConsoleCommandWrapper AvatarActionDefaultStop;
	const ConsoleCommandWrapper Delete;
	const ConsoleCommandWrapper Say;
	const ConsoleCommandWrapper SayTo;
	const ConsoleCommandWrapper Emote;
	const ConsoleCommandWrapper AdminTell;

protected:

	typedef std::list<std::pair<long long, AvatarMovementState>> TimedMovementStateList;

	/**
	 * @brief In milliseconds, the minimum time we must wait between sending updates to the server. Set this higher to avoid spamming the server.
	 */
	Ogre::Real mMinIntervalOfRotationChanges;

	Eris::Avatar& mErisAvatar;

	/**
	 * @brief The Eris::Entity which represents the Avatar.
	 */
	EmberEntity& mErisAvatarEntity;

	/**
	 * @brief An attachment controller which controls an ingame entity, representing the avatar.
	 * This is the main way the ingame representation is tied to the avatar.
	 */
	std::unique_ptr<AvatarAttachmentController> mAvatarAttachmentController;

	/**
	 * @brief Used to make sure starts and stops of movement is only sent to the server once.
	 */
	AvatarMovementState mCurrentMovementState;

	/**
	 * @brief Keep a list of the last transmitted movements.
	 * So that when we receive a movement update from the server, we can ignore it if it's something we know we sent ourselves.
	 */
	TimedMovementStateList mLastTransmittedMovements;

	/**
	 * @brief A third person camera mount, tracking the avatar entity.
	 * This mount is the default one used when controlling an avatar.
	 */
	std::unique_ptr<Camera::ThirdPersonCameraMount> mCameraMount;

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
	std::unique_ptr<AvatarLogger> mChatLogger;

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
	 * @brief The scene to which the avatar belongs.
	 */
	Scene& mScene;

	/**
	 * @brief An instance of this makes sure that entities can be created with the "/make" or "/createentity" command.
	 *
	 * Note that we'll create an instance of this even if the avatar isn't an admin character. This is to make it easier to do development.
	 * For future versions we might want to only create this instance if the avatar as an admin.
	 */
	std::unique_ptr<Authoring::EntityMaker> mEntityMaker;

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
	 * @brief Hooked up to Eris::View::AvatarEntityDeleted.
	 *
	 * This will emit the EventAvatarEntityDestroyed signal.
	 */
	void viewEntityDeleted();

	/**
	 * @brief Attach the camera to the entity.
	 *
	 * Call this when the location of the entity, or its graphical representation, has changed.
	 */
	void attachCameraToEntity();


	void populateUsageArgs(Atlas::Objects::Entity::RootEntity& entity,
						   const std::map<std::string, Eris::UsageParameter>& params,
						   const Eris::Entity* target,
						   const WFMath::Point<3>& posInWorld,
						   WFMath::Vector<3> direction);
};

inline bool Avatar::isAdmin() const {
	return mIsAdmin;
}

inline Eris::Avatar& Avatar::getErisAvatar() const {
	return mErisAvatar;
}

}

}

#endif // ENTITY_LISTENER_H
