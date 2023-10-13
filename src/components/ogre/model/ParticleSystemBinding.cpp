//
// C++ Implementation: ParticleSystemBinding
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "ParticleSystemBinding.h"
#include "ParticleSystem.h"
#include <OgreParticleEmitter.h>
#include <OgreParticleSystem.h>

#include <utility>

namespace Ember {
namespace OgreView {
namespace Model {

void ParticleSystemBinding::scaleValue(Ogre::Real scaler) const {
	updateSettings(*mParticleSystem->getOgreParticleSystem(), mEmitterVal, std::max(0.f, mOriginalValue * scaler));
}

void ParticleSystemBinding::updateSettings(Ogre::ParticleSystem& particleSystem, ModelDefinition::ParticleSystemSetting setting, float value) {
	//TODO: add more emitter values to bind
	if (setting == ModelDefinition::ParticleSystemSetting::EMISSION_RATE) {
		for (auto i = 0; i < particleSystem.getNumEmitters(); ++i) {
			particleSystem.getEmitter(i)->setEmissionRate(value);
		}
	} else if (setting == ModelDefinition::ParticleSystemSetting::TIME_TO_LIVE) {
		for (auto i = 0; i < particleSystem.getNumEmitters(); ++i) {
			particleSystem.getEmitter(i)->setTimeToLive(value);
		}
	}

}

}
}
}
