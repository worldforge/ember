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
#include "framework/Singleton.h"

#include <OgreFrameListener.h>


namespace EmberOgre {

class EmberEntity;

class IAnimated
{
public:
	virtual void updateAnimation(float timeSlice) = 0;
};

/**
 * This class will be responsible for making sure that entites moves
 * in a nice and fluid way. Eventually we'll also implement physics, perhaps it
 * will go into here.
 *
 * The manager also takes care of keeping tabs on all animations.
 */
class MotionManager : public Ogre::FrameListener, public Ember::Singleton<MotionManager> {
public:

	/**
	A struct containing information about the MotionManager.
	*/
	struct MotionManagerInfo
	{
		size_t AnimatedEntities;
		size_t MovingEntities;
	};

	/** Ctor
	 */
	MotionManager();
	/** Dtor
	 */
	virtual ~MotionManager();

	/**
	 * Adds a EmberEntity to the movement list.
	 * That means that until removeEntity is called for the specific entity
	 * new positions for the entity will be calculated for each frame.
	 */
	void addEntity(EmberEntity* entity);
	/**
	 * Removes a EmberEntity from the movement list.
	 * New positions for the entity will NOT be calculated for each frame.
	 */
	void removeEntity(EmberEntity* entity);

	/**
	 * Adds a EmberPhysicalEntity to the movement list.
	 * That means that until removeAnimatedEntity is called for the specific entity
	 * new positions (and animations) for the entity will be calculated for each frame.
	 */
	void addAnimatedEntity(const std::string& id, IAnimated* entity);
	/**
	 * Removes a EmberPhysicalEntity from the movement list.
	 * New positions (and animations) for the entity will NOT be calculated for each frame.
	 */
	void removeAnimatedEntity(const std::string& id);

	/**
	 * @see Ogre::FrameListener::frameStarted
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	/**
	 * @see Ogre::FrameListener::frameEnded
	 */
	bool frameEnded(const Ogre::FrameEvent& event);

	/**
	 *    Gets info about the MotionManager.
	 * @return Information about the motion manager
	 */
	const MotionManagerInfo& getInfo() const;

private:

	typedef std::map<std::string , IAnimated*> AnimatedStore;


	/** Information about this manager
	 */
	MotionManagerInfo mInfo;

	/**
	 * This contains all of the entities that will be moved each frame
	 */
	std::set<EmberEntity*> mMotionSet;

	/**
	 * This contains all of the entities that will be moved (and animated) each frame
	 */
	AnimatedStore mAnimatedEntities;


	/** This method will iterate over all registered moving entities and update
	 * their positions.
	 */
	void doMotionUpdate(Ogre::Real timeSlice);

	/** This method will iterate over all registered animationStates and update
	 * those that are enabled
	 */
	void doAnimationUpdate(Ogre::Real timeSlice);
};

inline const MotionManager::MotionManagerInfo& MotionManager::getInfo() const
{
	return mInfo;
}


}


#endif // MOTIONMANAGER_H
