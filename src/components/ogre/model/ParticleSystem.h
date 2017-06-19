//
// C++ Interface: ParticleSystem
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
#ifndef EMBEROGREPARTICLESYSTEM_H
#define EMBEROGREPARTICLESYSTEM_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include <OgreVector3.h>
#include <vector>
#include <OgreMovableObject.h>

namespace Ember {
namespace OgreView {
namespace Model {

class ParticleSystemBinding;

typedef std::vector<ParticleSystemBinding*> ParticleSystemBindingsPtrSet;

/**
@author Erik Ogenvik
*/
class ParticleSystem : public Ogre::MovableObject::Listener
{
public:
	ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem, const Ogre::Vector3& direction);
	virtual ~ParticleSystem();
	Ogre::ParticleSystem* getOgreParticleSystem();
	
	/**
	 * @brief Adds a new binding.
	 * @param emitterVal the name of the particle emitter
	 * @param variableName the name of the atlas attribute
	 * @return a pointer to the newly created binding
	 */
	ParticleSystemBinding* addBinding(const std::string& emitterVal, const std::string& variableName);
	ParticleSystemBindingsPtrSet& getBindings();
	
	/**
	 * @brief Sets the visibility of the particle system.
	 * @param visibility Whether the particle system should be shown.
	 */
	void setVisible(bool visibility);

	/**
	 * @brief The direction of the particle system, in world coords.
	 *
	 * If the vector is invalid no direction is specified, and the particle system should use the local coords of its parent scene node.
	 */
	const Ogre::Vector3& getDirection() const;

	bool objectRendering(const Ogre::MovableObject*, const Ogre::Camera*) override;


protected:
	Ogre::ParticleSystem* mOgreParticleSystem;
	ParticleSystemBindingsPtrSet mBindings;

	/**
	 * @brief The direction of the particle system, in world coords.
	 *
	 * If the vector is invalid no direction is specified, and the particle system should use the local coords of its parent scene node.
	 */
	Ogre::Vector3 mDirection;

};

}
}

}

#endif
