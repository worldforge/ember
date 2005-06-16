/*
    Copyright (C) 2004  Erik Hjortsberg

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

#ifndef MOTIONMANAGER_H
#define MOTIONMANAGER_H

#include "EmberOgrePrerequisites.h"
#include <OgrePredefinedControllers.h> 
#include "framework/Singleton.h"

namespace EmberOgre {

class EmberEntity;
class EmberPhysicalEntity;
class TerrainGenerator;

/*
 * This class will be responsible for making sure that entites moves
 * in a nice and fluid way. Eventually we'll also implement physics, perhaps it 
 * will go into here.
 * The manager also takes care of keeping tabs on all animations.
 */
class MotionManager : public Ogre::FrameListener, public Ember::Singleton<MotionManager> {
public:

	MotionManager();
	virtual ~MotionManager();
	//static MotionManager & getSingleton(void);
	
	/*
	 * Adds a EmberEntity to the movement list.
	 * That means that until removeEntity is called for the specific entity
	 * new positions for the entity will be calculated for each frame.
	 */
	void addEntity(EmberEntity* entity);
	void removeEntity(EmberEntity* entity);
	
	/*
	 * Registers an animationState. After registration it will be enough to use
	 * AnimationState::enabled(bool) to toggle the animation on and off
	 */
	void addAnimation(Ogre::AnimationState* animationState);
	/*
	 * Deregisters an animationState
	 */
	void removeAnimation(Ogre::AnimationState* animationState);
	/*
	 * Pauses the supplies animationState
	 */
	void pauseAnimation(Ogre::AnimationState* animationState);
	/*
	 * Unpauses (starts) an already paused animationState
	 */
	void unpauseAnimation(Ogre::AnimationState* animationState);
	
	void addAnimatedEntity(EmberPhysicalEntity* entity);
	void removeAnimatedEntity(EmberPhysicalEntity* entity);
	
/*	void addAction(Action* action);
	void removeAction(Action* action);*/
	


	/*
	 * Methods from Ogre::FrameListener
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	bool frameEnded(const Ogre::FrameEvent& event);
	
	/*
	 * Adjusts the height of the supplied node
	 */
	//void adjustHeightPositionForNode(Ogre::SceneNode* sceneNode);

	void setTerrainGenerator(TerrainGenerator* generator);
	
	
private:

	typedef std::map<std::string , EmberPhysicalEntity*> EntityStore;
	EntityStore mAnimatedEntities;

// 	typedef std::set<Action*> ActionStore;
// 	ActionStore mActions;

	/**
	 * A pointer to the applications ControllerManager. This will take care of 
	 * keeping tabs on all controllers.
	 */
	Ogre::ControllerManager* mControllerManager;
	typedef Ogre::Controller<Ogre::Real> animationControllerType;
	typedef std::map<Ogre::AnimationState*, animationControllerType*> animationStateMap;
	typedef std::set<Ogre::AnimationState*> AnimationStateSet;
	/*
	 * A map of AnimationState's and their corresponding Controllers
	 */
	AnimationStateSet mAnimations;


	//static MotionManager* _instance;

	/* This method will iterate over all registered moving entities and update
	 * their positions.
	 */
	void doMotionUpdate(Ogre::Real timeSlice);

	/* This method will iterate over all registered animationStates and update
	 * those that are enabled
	 */
	void doAnimationUpdate(Ogre::Real timeSlice);


	/* 
	 * Update the motion for a single EmberEntity
	 */
	void updateMotionForEntity(EmberEntity* entity, Ogre::Real timeSlice);

	/*
	 * This contains all of the entities that will be moved each frame
	 */
	std::set<EmberEntity*> mMotionSet;
	
	TerrainGenerator* mTerrainGenerator;
	
};
}


#endif // MOTIONMANAGER_H
