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
#include "EmberOgrePrerequisites.h"

#include <sigc++/trackable.h>
#include <sigc++/signal.h>

#include <wfmath/point.h>
#include <wfmath/quaternion.h>

#include "GUIManager.h"
#include "services/config/ConfigListenerContainer.h"
#include <memory>

namespace Eris {
	class Entity;
}

namespace WFMath {
    class Quaternion;
}


namespace EmberOgre {

namespace Model {
	class Model;
}
namespace Camera {
	class ThirdPersonCameraMount;
}

class EmberEntity;
class AvatarLogger;
class AvatarLoggerParent;
class AvatarAttachmentController;

struct AvatarMovementState
{
public:
	bool isMoving;
	bool isRunning;
	Ogre::Vector3 velocity;
	Ogre::Quaternion orientation;
};

/**
 * This class holds the Avatar. In general it recieves instructions from mainly
 * MovementController to attempt to move or rotate the avatar. After checking
 * with the world rules if such a thing is allowed, the world node is manipulated.
 * If it's a movement it has to be animated.
 *
 */
class Avatar :
public sigc::trackable,
public Ogre::FrameListener,
public Ember::ConfigListenerContainer
{
	friend class MovementController;

    public:

	/**
	 *    Ctor.
	 */
	Avatar(EmberEntity& erisAvatarEntity);

	/**
	 *    Dtor.
	 */
	virtual ~Avatar();


	/**
	 *    Gets the scene node which the avatar is attached to.
	 * @return
	 */
	Ogre::SceneNode* getAvatarSceneNode() const;

	/**
	 *    Called each frame.
	 * @param event
	 * @return
	 */
	virtual bool frameStarted(const Ogre::FrameEvent & event);

	/**
	 *    Call this when the avatar entity has moved in the world.
	 */
	void movedInWorld();


	/**
	 *    Called each frame.
	 * @param movement
	 */
//	void updateFrame(MovementControllerMovement& movement);

	/**
	 *    Access for the Eris::Entity which represents the Avatar.
	 * @return
	 */
	EmberEntity& getEmberEntity();


	/**
	 *    sets the minimum interval to wait before sending new rotation changes to the server
	 *    this is not done instantly to prevent swamping of data to the server
	 *    set this lower if you experience too jerky game play
	 * @param milliseconds
	 */
	void setMinIntervalOfRotationChanges(Ogre::Real milliseconds);

	/**
	Emitted when an entity is added to the inventory.
	*/
	sigc::signal<void, EmberEntity* > EventAddedEntityToInventory;

	/**
	Emitted when an entity is removed from the inventory.
	*/
	sigc::signal<void, EmberEntity* > EventRemovedEntityFromInventory;

	/**
	True if the current user have admin rights, i.e. is a "creator".
	*/
	bool isAdmin() const;

	void moveClientSide(const WFMath::Quaternion& orientation, const WFMath::Vector<3>& movement, float timeslice);


	WFMath::Point<3> getClientSideAvatarPosition() const;

	WFMath::Quaternion getClientSideAvatarOrientation() const;

	Camera::ThirdPersonCameraMount& getCameraMount() const;
protected:

	/**
	 * adjust the avatar to the new position in the terrain
	 * for now this means setting the correct heigth
	 * accoring to mercator terrain, but it will probably
	 * be extended to also include stuff as positioning the avatars feet
	 * right
	 */
//	void adjustAvatarToNewPosition(MovementControllerMovement* movement);

	/**
	 * This method will determine if it's ok to send a small movement change, such as
	 * a small deviation direction during an already begun movement to the server.
	 */
	bool isOkayToSendRotationMovementChangeToServer();

	/**
	Time in milliseconds since we last sent an movement update to the server.
	*/
	Ogre::Real mTimeSinceLastServerMessage;

	/**
	In milliseconds, the minimum time we must wait between sending updates to the server. Set this higher to avoid spamming the server.
	*/
	Ogre::Real mMinIntervalOfRotationChanges;

	/**
	In degrees the minimum amount of degrees we can yaw the camera until we need to send an update to the server of our new position.
	*/
	Ogre::Real mThresholdDegreesOfYawForAvatarRotation;

	/**
	In degrees the accumulated yaw movement since we last sent an update to the server. If this exceeds mThresholdDegreesOfYawForAvatarRotation we need to send an update to the server.
	*/
	float mAccumulatedHorizontalRotation;

	/**
	 * Attempts to move the avatar in a certain direction
	 * Note that depending on what the rules allows (i.e. collision detection,
	 * character rules etc.) the outcome of the attempt is uncertain.
	 *
	 * The parameter timeSlice denotes the slice of time under which the movement
	 * shall take place.
	 */
//	void attemptMove(MovementControllerMovement& movement);

	/**
	 * Attempts to rotate the avatar to a certain direction
	 * Note that depending on what the rules allows (i.e. collision detection,
	 * character rules etc.) the outcome of the attempt is uncertain.
	 *
	 * When standing still one can rotate how much one want.
	 * But when moving, rotation happens in interval
	 *
	 */
//	void attemptRotate(MovementControllerMovement& movement);


	/**
	 * Attempts to stop the avatar.
	 * This should work in most cases.
	 *
	 */
	void attemptStop();

	/**
	 * Attempt to jump.
	 */
	void attemptJump();

	/**
	 * Creates and sets up the different cameras.
	 */
	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);

	/**
	The Eris::Entity which represents the Avatar.
	*/
	EmberEntity& mErisAvatarEntity;

	/**
	 * How many meters per second the avatar can move, at maximum speed.
	 * This should be set through some kind of rule checking with the server
	 * depending on the character. To be done later.
	 */
	float mMaxSpeed;

	AvatarAttachmentController* mAvatarAttachmentController;

	/**
	* this is used to make sure starts and stops of movement is only sent to the server once
	*/
	AvatarMovementState mCurrentMovementState;
	AvatarMovementState mMovementStateAtBeginningOfMovement; //this is perhaps not needed
	AvatarMovementState mMovementStateAtLastServerMessage;

	/**
	Keep a temporary list of entities that needs to be added to the inventory.
	*/
	std::set<Eris::Entity*> mEntitiesToBeAddedToInventory;

	/**
	Keep a temporary list of entities that needs to be removed from the inventory.
	*/
	std::set<Eris::Entity*> mEntitiesToBeRemovedFromInventory;

	std::auto_ptr<Camera::ThirdPersonCameraMount> mCameraMount;

	/**
	 * Listen for location changes, since after a location change we need to honour the onMoved updates even if we're in movement mode.
	 * @param entity
	 */
	void avatar_LocationChanged(Eris::Entity* entity);

	void avatar_Moved();

	/**
	True if the current user have admin rights, i.e. is a "creator".
	*/
	bool mIsAdmin;

	/**
	If set to true, the avatar has just changed location, so the next onMoved operation will contain the new orientation and position information for the new location.
	*/
	bool mHasChangedLocation;


	/**
	 *    Listen for changes of the general:logchatmessages config key and create and destroy an instance of AvatarLogger (actually AvatarLoggerParent) accordingly.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_LogChatMessages(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 *    Bind the frequency of rotation updates being sent to the server when the camera moves to the general:avatarrotationupdatefrequency key
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_AvatarRotationUpdateFrequency(const std::string& section, const std::string& key, varconf::Variable& variable);

	/**
	 *    Bind the max speed to the input:runspeed key. Note that this is capped in the end by the server.
	 * @param section
	 * @param key
	 * @param variable
	 */
	void Config_MaxSpeed(const std::string& section, const std::string& key, varconf::Variable& variable);


	/**
	Holds the objects which logs ingame messages to a file. We don't hold a AvatarLogger instance directly, instead using the AvatarLoggerParent class, since we can't really create an instance of AvatarLogger until we've gotten an AvatarEmberEntity, and the AvatarLoggerParent class will take care of all that.
	*/
	std::auto_ptr<AvatarLoggerParent> mChatLoggerParent;


	/**
	 * @brief We save the five latest orientations that we send to the server.
	 * The reason we do this is to be able to recognize updates sent from the server that's our own updates. Since there might be lag, these updates can arrive half a second or more after they were sent. They should thus be ignored to avoid the camera jumping all over the place.
	 */
	std::list<WFMath::Quaternion> mLastOrientations;

	WFMath::Point<3> mClientSideAvatarPosition;

	WFMath::Quaternion mClientSideAvatarOrientation;


}; //End of class declaration

inline bool Avatar::isAdmin() const
{
	return mIsAdmin;
}

}

#endif // ENTITY_LISTENER_H
