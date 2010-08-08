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
class IAnimated;
class IMovable;

/**
 * @brief Responsible for making sure that movement and animation within the graphical system is managed and synchronized.
 *
 * The main task of the manager is to keep track of all movables and animatables, i.e. implementations of IMovable and IAnimated, and make sure that these are asked to update their movement or animation when needed (usually each frame).
 */
class MotionManager : public Ogre::FrameListener, public Ember::Singleton<MotionManager> {
public:

	/**
	 * @brief Struct containing information about the MotionManager.
	 */
	struct MotionManagerInfo
	{
		size_t AnimatedEntities;
		size_t MovingEntities;
	};

	/**
	 * @brief Ctor
	 */
	MotionManager();

	/**
	 * @brief Dtor
	 */
	virtual ~MotionManager();

	/**
	 * @brief Adds a movable to the movement list.
	 * @param movable The movable instance to add to the movable list.
	 * That means that until removeMovable is called for the specific movable it will receive calls to updateMotion each frame.
	 * It's safe to add the same movable multiple times.
	 */
	void addMovable(IMovable* movable);

	/**
	 * @brief Removes a movable from the movement list.
	 * @param movable The movable instance to add to the movable list.
	 * It's safe to remove the same movable multiple times.
	 */
	void removeMovable(IMovable* movable);

	/**
	 * @brief Adds an animatable to the animation list.
	 * That means that until removeAnimated is called for the specific animatable it will receive calls to updateAnimation each frame.
	 * @param id The id of the animatable.
	 * @param animatable The animatable instance.
	 */
	void addAnimated(const std::string& id, IAnimated* animatable);

	/**
	 * @brief Removes an animatable from the animation list.
	 * @param The id of the animatable to remove.
	 * It's safe to remove an animateble which is not in the current list.
	 */
	void removeAnimated(const std::string& id);

	/**
	 * @see Ogre::FrameListener::frameStarted
	 */
	bool frameStarted(const Ogre::FrameEvent& event);
	/**
	 * @see Ogre::FrameListener::frameEnded
	 */
	bool frameEnded(const Ogre::FrameEvent& event);

	/**
	 * @brief Gets info about the MotionManager.
	 * @return Information about the motion manager
	 */
	const MotionManagerInfo& getInfo() const;

private:

	/**
	 * @brief A store of animatables, identified by a string.
	 */
	typedef std::map<std::string , IAnimated*> AnimatedStore;

	/**
	 * @brief A store of movables.
	 */
	typedef std::set<IMovable*> MovableStore;


	/**
	 * @brief Information about this manager.
	 */
	MotionManagerInfo mInfo;

	/**
	 * @brief Contains all of the entities that will be moved each frame.
	 */
	MovableStore mMotionSet;

	/**
	 * @brief Contains all of the entities that will be animated each frame.
	 */
	AnimatedStore mAnimatedEntities;


	/**
	 * @brief Will iterate over all registered movables and ask them to update their positions.
	 */
	void doMotionUpdate(Ogre::Real timeSlice);

	/**
	 * @brief Will iterate over all registered animatables and update those that are enabled.
	 */
	void doAnimationUpdate(Ogre::Real timeSlice);
};

inline const MotionManager::MotionManagerInfo& MotionManager::getInfo() const
{
	return mInfo;
}


}


#endif // MOTIONMANAGER_H
