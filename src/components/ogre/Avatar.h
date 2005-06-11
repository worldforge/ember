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

// TODO: use forward declarations here

#ifndef AVATAR_H
#define AVATAR_H
#include "EmberOgrePrerequisites.h"

//#include <OgreNoMemoryMacros.h>

	#include <Eris/Entity.h>
	#include <Eris/View.h>
	#include <Eris/PollDefault.h>
	#include <Eris/Log.h>
	#include <Eris/TypeInfo.h>
	
	#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
	#include <sigc++/signal_system.h>
	#else
	#include <sigc++/object.h>
	#include <sigc++/signal.h>
	#include <sigc++/slot.h>
	#include <sigc++/bind.h>
	#include <sigc++/object_slot.h>
	#endif
//#include <OgreMemoryMacros.h>

#include <OgrePredefinedControllers.h> 
#include "framework/Singleton.h"



namespace EmberOgre {
class EmberEntity;
class AvatarCamera;
class AvatarController;
class AvatarEmberEntity;
struct AvatarControllerMovement;
class Model;

struct AvatarMovementState
{
public:
	bool isMoving;
	bool isRunning;
	Ogre::Vector3 velocity;
	Ogre::Quaternion orientation;
};
	
/*
 * This class holds the Avatar. In general it recieves instructions from mainly 
 * AvatarController to attempt to move or rotate the avatar. After checking 
 * with the world rules if such a thing is allowed, the world node is manipulated.
 * If it's a movement it has to be animated.
 * 
 *  
 * TODO: create facilities for animation of the avatar, walking, running, jumping etc.
 */
class Avatar : virtual public SigC::Object, public Ogre::FrameListener
{
	friend class AvatarController;
	friend class AvatarEmberEntity;

    public:

	Avatar();
	Avatar(Ogre::SceneManager* sceneManager);
	~Avatar();

	Ogre::Camera* getAvatar1pCamera(void);
	Ogre::Camera* getAvatar3pCamera(void);
	Ogre::Camera* getAvatarTopCamera(void);
	
	AvatarCamera* getAvatarCamera() const;
	
	Ogre::SceneNode* getAvatarSceneNode() const
	{
		return mAvatarNode;
	}

	//Ogre::Camera* getCamera() const;
	
	virtual bool frameStarted(const Ogre::FrameEvent & event);

	
	
	void createdAvatarEmberEntity(AvatarEmberEntity *EmberEntity);
	
	void movedInWorld();
	
	//void touch(EmberEntity* entity);
	
	void updateFrame(AvatarControllerMovement& movement);
	
	void setAvatarController(AvatarController* avatarController);
	
	AvatarEmberEntity* getAvatarEmberEntity();

	SigC::Signal1<void, EmberEntity* > EventAddedEntityToInventory;
	SigC::Signal1<void, EmberEntity* > EventRemovedEntityFromInventory;
// 	SigC::Signal1<void, Eris::Entity* > EventAddedEntityToInventory;
// 	SigC::Signal1<void, Eris::Entity* > EventRemovedEntityFromInventory;

protected:
	
	/*
	 * adjust the avatar to the new position in the terrain
	 * for now this means setting the correct heigth 
	 * accoring to mercator terrain, but it will probably
	 * be extended to also include stuff as positioning the avatars feet
	 * right
	 */
	void Avatar::adjustAvatarToNewPosition(AvatarControllerMovement* movement);
	
	/*
	 * This method will determine if it's ok to send a small movement change, such as
	 * a small deviation direction during an already begun movement to the server.
	 */
	bool isOkayToSendRotationMovementChangeToServer();
	Ogre::Real mTimeSinceLastServerMessage;
	Ogre::Real mMinIntervalOfRotationChanges;
	Ogre::Real mThresholdDegreesOfYawForAvatarRotation;
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
	
	
	Ogre::SceneManager* mSceneMgr;
	/*
	 * The main avatar entity
	 */
	Model* mAvatarModel;
	
	/* 
	 * The main avatar scenenode
	 */
	Ogre::SceneNode* mAvatarNode;



	// node for rotating the model for the entity
	// if it's not looking in the -Z direction (default)
	// To be removed once we've standarized on models
	Ogre::SceneNode* mAvatarModelNode;

	AvatarEmberEntity* mErisAvatarEntity;
	//Eris::Avatar* mErisAvatar;

	//this is used to make sure starts and stops of movement is only sent to the server once
	AvatarMovementState mCurrentMovementState;
	AvatarMovementState mMovementStateAtBeginningOfMovement; //this is perhaps not needed
	AvatarMovementState mMovementStateAtLastServerMessage;

	AvatarController* mAvatarController;

	std::set<Eris::Entity*> mEntitiesToBeAddedToInventory;
	std::set<Eris::Entity*> mEntitiesToBeRemovedFromInventory;

	
}; //End of class declaration

}

#endif // ENTITY_LISTENER_H
