//
// C++ Implementation: ParticleSystem
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>

#include "ParticleSystem.h"
#include "ParticleSystemBinding.h"
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {
namespace Model {

Ogre::ParticleSystem* ParticleSystem::getOgreParticleSystem() {
	return mOgreParticleSystem;
}

ParticleSystem::ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem, const Ogre::Vector3& direction) :
		mOgreParticleSystem(ogreParticleSystem), mDirection(direction) {
	assert(ogreParticleSystem);
	//If there's a direction, make sure that we adjust all emitters for each frame so that they are in world space.
	if (!mDirection.isNaN()) {
		ogreParticleSystem->setListener(this);
	}
}

ParticleSystem::~ParticleSystem() {
	//make sure all bindings are removed
	ParticleSystemBindingsPtrSet::const_iterator I = mBindings.begin();
	ParticleSystemBindingsPtrSet::const_iterator I_end = mBindings.end();
	for (; I != I_end; ++I) {
		delete *I;
	}

	//and then destroy the system to save resources
	mOgreParticleSystem->_getManager()->destroyParticleSystem(mOgreParticleSystem);

}


ParticleSystemBindingsPtrSet& ParticleSystem::getBindings() {
	return mBindings;
}

ParticleSystemBinding* ParticleSystem::addBinding(const std::string& emitterVal, const std::string& variableName) {
	ParticleSystemBinding* binding = new ParticleSystemBinding(this, emitterVal, variableName);
	mBindings.push_back(binding);
	return binding;
}

void ParticleSystem::setVisible(bool visibility) {
	if (mOgreParticleSystem) {
		mOgreParticleSystem->setVisible(visibility);
	}
}

const Ogre::Vector3& ParticleSystem::getDirection() const {
	return mDirection;
}

bool ParticleSystem::objectRendering(const Ogre::MovableObject* movable, const Ogre::Camera*) {
	//Adjust all emitters so that mDirection is applied in world space
	Ogre::Quaternion rotation = movable->getParentNode()->convertWorldToLocalOrientation(Ogre::Quaternion(Ogre::Degree(0), mDirection));
	for (unsigned short i = 0; i < mOgreParticleSystem->getNumEmitters(); ++i) {
		Ogre::ParticleEmitter* emitter = mOgreParticleSystem->getEmitter(i);
		emitter->setDirection(rotation * mDirection);
	}
	return true;
}


}
}
}
