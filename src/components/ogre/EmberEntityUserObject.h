//
// C++ Interface: EmberEntityUserObject
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
#ifndef EMBEROGREEMBERENTITYUSEROBJECT_H
#define EMBEROGREEMBERENTITYUSEROBJECT_H

#include "EmberOgrePrerequisites.h"
#include <OgreVector.h>
#include <memory>

namespace Ogre
{
class Entity;
}


namespace Ember
{
class EmberEntity;
namespace OgreView
{

namespace Model
{
class Model;
}

/**
 * @author Erik Ogenvik
 *
 * @brief An Ogre user object which connects to an EmberEntity.
 * Instances of this class can be attached to movable objects in the ogre system. They will allow for the Ember system to be accessed directly from Ogre, without having to do lookups.
 * This is generally mostly used for mouse picking and collision handling.
 */
class EmberEntityUserObject
{
public:
	typedef std::shared_ptr<EmberEntityUserObject> SharedPtr;

	/**
	 * Ctor.
	 * @param emberEntity A valid EmberEntity instance.
	 */
	explicit EmberEntityUserObject(EmberEntity& emberEntity);

	/**
	 * @brief Dtor.
	 */
	virtual ~EmberEntityUserObject();

	/**
	 * @brief Gets the EmberEntity contained.
	 * @return The entity to which this user object belongs.
	 */
	EmberEntity& getEmberEntity() const;

	/**
	 * @brief Tells the collision detector, is one such is available, to refit any collision shape in need of refitting.
	 * Call this before you test for collisions, since collision shapes often aren't refitted until needed (to increase performance).
	 */
	void refit();


protected:

	/**
	 * @brief The entity to which this user object belongs.
	 */
	EmberEntity& mEmberEntity;

};

}
}

#endif
