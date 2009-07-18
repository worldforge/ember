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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "MotionManager.h"

#include "IMovable.h"
#include "IAnimated.h"


template<> EmberOgre::MotionManager* Ember::Singleton<EmberOgre::MotionManager>::ms_Singleton = 0;
namespace EmberOgre {


MotionManager::MotionManager()
{
	mInfo.MovingEntities = mMotionSet.size();
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}


MotionManager::~MotionManager()
{}


void MotionManager::doMotionUpdate(Ogre::Real timeSlice)
{
	for (MovableStore::const_iterator I = mMotionSet.begin(); I != mMotionSet.end(); ++I) {
		(*I)->updateMotion(timeSlice);
	}
}

void MotionManager::doAnimationUpdate(Ogre::Real timeSlice)
{
	for (AnimatedStore::const_iterator I = mAnimatedEntities.begin(); I != mAnimatedEntities.end(); ++I) {
		I->second->updateAnimation(timeSlice);
	}
}

bool MotionManager::frameStarted(const Ogre::FrameEvent& event)
{
	doMotionUpdate(event.timeSinceLastFrame);
	doAnimationUpdate(event.timeSinceLastFrame);
	return true;
}

bool MotionManager::frameEnded(const Ogre::FrameEvent& event)
{
	return true;
}

void MotionManager::addMovable(IMovable* movable)
{
	mMotionSet.insert(movable);
	mInfo.MovingEntities = mMotionSet.size();
	movable->updateMotion(0);
}

void MotionManager::removeMovable(IMovable* movable)
{
	mMotionSet.erase(movable);
	mInfo.MovingEntities = mMotionSet.size();
}

void MotionManager::addAnimated(const std::string& id, IAnimated* animated)
{
	mAnimatedEntities[id] = animated;
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}

void MotionManager::removeAnimated(const std::string& id)
{
	mAnimatedEntities.erase(id);
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}

}
