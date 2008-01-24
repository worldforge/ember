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
	

#include "framework/Singleton.h"

namespace Eris {
	class Entity;
}


namespace EmberOgre {

namespace Model {
	class Model;
}

class EmberEntity;
class AvatarCamera;
class AvatarController;
class AvatarEmberEntity;
struct AvatarControllerMovement;

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
 * AvatarController to attempt to move or rotate the avatar. After checking 
 * with the world rules if such a thing is allowed, the world node is manipulated.
 * If it's a movement it has to be animated.
 * 
 */
class Avatar : 
public sigc::trackable, 
public Ogre::FrameListener
{
	friend class AvatarController;
	friend class AvatarEmberEntity;

    public:

	Avatar();
	virtual ~Avatar();


	/**
	 *    Gets the avatar camera.
	 * @return 
	 */
	AvatarCamera* getAvatarCamera() const;
	
	/**
	 *    Gets the scene node which the avatar is attached to.
	 * @return 
	 */
	inline Ogre::SceneNode* getAvatarSceneNode() const;
	
	/**
	 *    Called each frame.
	 * @param event 
	 * @return 
	 */
	virtual bool frameStarted(const Ogre::FrameEvent & event);
	
	/**
	 *    Call this when the Eris::Entity representing the avatar has been created.
	 * @param EmberEntity 
	 */
	void createdAvatarEmberEntity(AvatarEmberEntity *EmberEntity);
	
	/**
	 *    Call this when the avatar entity has moved in the world.
	 */
	void movedInWorld();
	
	
	/**
	 *    Called each frame.
	 * @param movement 
	 */
	void updateFrame(AvatarControllerMovement& movement);
	
	/**
	 *    Sets the controller object responsible for controlling the avatar.
	 * @param avatarController 
	 */
	void setAvatarController(AvatarController* avatarController);
	
	/**
	 *    Access for the Eris::Entity which represents the Avatar.
	 * @return 
	 */
	AvatarEmberEntity* getAvatarEmberEntity();
	
	
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
	inline bool isAdmin() const;

protected:
	
	/**
	 * adjust the avatar to the new position in the terrain
	 * for now this means setting the correct heigth 
	 * accoring to mercator terrain, but it will probably
	 * be extended to also include stuff as positioning the avatars feet
	 * right
	 */
	void adjustAvatarToNewPosition(AvatarControllerMovement* movement);
	
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
	void attemptMove(AvatarControllerMovement& movement);
	
	/**
	 * Attempts to rotate the avatar to a certain direction
	 * Note that depending on what the rules allows (i.e. collision detection,
	 * character rules etc.) the outcome of the attempt is uncertain.
	 * 
	 * When standing still one can rotate how much one want.
	 * But when moving, rotation happens in interval
	 * 
	 */
	void attemptRotate(AvatarControllerMovement& movement);
	
	
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
	 * Creates the avatar. We'll have to extend this functionality later on to 
	 * allow for different avatars.
	 */
	void createAvatar();
	
	
	/**
	 * Creates and sets up the different cameras.
	 */
	void createAvatarCameras(Ogre::SceneNode* avatarSceneNode);

	/**
	 * How many meters per second the avatar can walk.
	 * This should be set through some kind of rule checking with the server
	 * depending on the character. To be done later.
	 */
	float mWalkSpeed;

	/**
	 * How many meters per second the avatar can run.
	 * This should be set through some kind of rule checking with the server
	 * depending on the character. To be done later.
	 */
	float mRunSpeed;
	
	/**
	 * The main avatar model
	 */
	Model::Model* mAvatarModel;
	
	/** 
	 * The main avatar scenenode
	 */
	Ogre::SceneNode* mAvatarNode;



	/** node for rotating the model for the entity
	* if it's not looking in the -Z direction (default)
	* To be removed once we've standarized on models
	*/
	Ogre::SceneNode* mAvatarModelNode;

	/**
	The Eris::Entity which represents the Avatar.
	*/
	AvatarEmberEntity* mErisAvatarEntity;

	/**
	* this is used to make sure starts and stops of movement is only sent to the server once
	*/
	AvatarMovementState mCurrentMovementState;
	AvatarMovementState mMovementStateAtBeginningOfMovement; //this is perhaps not needed
	AvatarMovementState mMovementStateAtLastServerMessage;

	/**
	The instance responsible for listening for movement updates and sending those to the server.
	*/
	AvatarController* mAvatarController;

	/**
	Keep a temporary list of entities that needs to be added to the inventory.
	*/
	std::set<Eris::Entity*> mEntitiesToBeAddedToInventory;
	
	/**
	Keep a temporary list of entities that needs to be removed from the inventory.
	*/
	std::set<Eris::Entity*> mEntitiesToBeRemovedFromInventory;
	
	
	/**
	 *    catch changes to the configuration
	 * @param section 
	 * @param key 
	 */
	void ConfigService_EventChangedConfigItem(const std::string& section, const std::string& key);
	
	/**
	 * Listen for location changes, since after a location change we need to honour the onMoved updates even if we're in movement mode.
	 * @param entity 
	 */
	void avatar_LocationChanged(Eris::Entity* entity);

	/**
	*	updates values from the configuration
	*/
	void updateFromConfig();
	
	/**
	True if the current user have admin rights, i.e. is a "creator".
	*/
	bool mIsAdmin;
	
	/**
	If set to true, the avatar has just changed location, so the next onMoved operation will contain the new orientation and position information for the new location.
	*/
	bool mHasChangedLocation;

	
}; //End of class declaration

bool Avatar::isAdmin() const
{
	return mIsAdmin;
}
Ogre::SceneNode* Avatar::getAvatarSceneNode() const
{
	return mAvatarNode;
}

}

#endif // ENTITY_LISTENER_H
