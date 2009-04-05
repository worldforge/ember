//
// C++ Interface: ParticleSystem
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
#ifndef EMBEROGREPARTICLESYSTEM_H
#define EMBEROGREPARTICLESYSTEM_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <vector>

namespace EmberOgre {
namespace Model {

class ParticleSystemBinding;

typedef std::vector<ParticleSystemBinding*> ParticleSystemBindingsPtrSet;

/**
@author Erik Hjortsberg
*/
class ParticleSystem
{
public:
	ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem);
	virtual ~ParticleSystem();
	Ogre::ParticleSystem* getOgreParticleSystem();
	
	/**
	 *        Adds a new binding.
	 * @param emitterVal the name of the particle emitter
	 * @param variableName the name of the atlas attribute
	 * @return a pointer to the newly created binding
	 */
	ParticleSystemBinding* addBinding(const std::string& emitterVal, const std::string& variableName);
	ParticleSystemBindingsPtrSet& getBindings();
protected:
	Ogre::ParticleSystem* mOgreParticleSystem;
	ParticleSystemBindingsPtrSet mBindings;

};

}
}

#endif
