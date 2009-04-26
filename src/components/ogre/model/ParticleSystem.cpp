//
// C++ Implementation: ParticleSystem
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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

#include "ParticleSystem.h"
#include "ParticleSystemBinding.h"

namespace EmberOgre {
namespace Model {

Ogre::ParticleSystem* ParticleSystem::getOgreParticleSystem()
{
	return mOgreParticleSystem;
}

ParticleSystem::ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem) : 
		mOgreParticleSystem(ogreParticleSystem)
{
	assert(ogreParticleSystem);
}

ParticleSystem::~ParticleSystem()
{
	//make sure all bindings are removed
	ParticleSystemBindingsPtrSet::const_iterator I = mBindings.begin();
	ParticleSystemBindingsPtrSet::const_iterator I_end = mBindings.end();
	for (; I != I_end; ++I) {
		delete *I;
	}
	
	//and then destroy the system to save resources
	mOgreParticleSystem->_getManager()->destroyParticleSystem(mOgreParticleSystem);
	
}


ParticleSystemBindingsPtrSet& ParticleSystem::getBindings( )
{
	return mBindings;
}

ParticleSystemBinding * ParticleSystem::addBinding( const std::string & emitterVal, const std::string & variableName )
{
	ParticleSystemBinding* binding = new ParticleSystemBinding(this, emitterVal, variableName);
	mBindings.push_back(binding);
	return binding;
}

void ParticleSystem::setVisible(bool visibility)
{
	if (mOgreParticleSystem) {
		mOgreParticleSystem->setVisible(visibility);
	}
}


}
}
