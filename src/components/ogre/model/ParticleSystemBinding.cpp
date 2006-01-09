//
// C++ Implementation: ParticleSystemBinding
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "ParticleSystemBinding.h"
#include "ParticleSystem.h"

namespace EmberOgre {
namespace Model {

ParticleSystemBinding::ParticleSystemBinding(ParticleSystem* parentSystem, const std::string& emitterVal, const std::string& variableName) :
mParticleSystem(parentSystem)
, mEmitterVal(emitterVal)
, mVariableName(variableName)
{
	//TODO: add more emitter values to bind
	if (mEmitterVal == "emission_rate") {
		Ogre::ParticleEmitter* emitter = mParticleSystem->getOgreParticleSystem()->getEmitter(0);
		if (emitter) {
			mOriginalValue = emitter->getEmissionRate();
		}
	}

}

void ParticleSystemBinding::scaleValue(Ogre::Real scaler)
{
	
	//TODO: add more emitter values to bind
	if (mEmitterVal == "emission_rate") {
		Ogre::ParticleEmitter* emitter = mParticleSystem->getOgreParticleSystem()->getEmitter(0);
		if (emitter) {
			emitter->setEmissionRate(mOriginalValue * scaler);
		}
	}
}

}
}
