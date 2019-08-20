/*
    Copyright (C) 2004  Erik Ogenvik

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

#include "MotionManager.h"

#include "IMovable.h"
#include "IAnimated.h"


template<> Ember::OgreView::MotionManager* Ember::Singleton<Ember::OgreView::MotionManager>::ms_Singleton = nullptr;
namespace Ember {
namespace OgreView {


MotionManager::MotionManager()
{
	mInfo.MovingEntities = mMotionSet.size();
	mInfo.AnimatedEntities = mAnimatedEntities.size();
}


MotionManager::~MotionManager()= default;


void MotionManager::doMotionUpdate(Ogre::Real timeSlice)
{
	for (auto I : mMotionSet) {
		I->updateMotion(timeSlice);
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
}
