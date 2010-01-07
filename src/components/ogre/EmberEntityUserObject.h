//
// C++ Interface: EmberEntityUserObject
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
#ifndef EMBEROGREEMBERENTITYUSEROBJECT_H
#define EMBEROGREEMBERENTITYUSEROBJECT_H

#include "EmberOgrePrerequisites.h"
#include <OgreVector3.h>

namespace Ogre
{
class Entity;
}
;

namespace EmberOgre
{

namespace Model
{
class Model;
}
class EmberEntity;
class ICollisionDetector;

/**
 * @author Erik Hjortsberg
 *
 * @brief An Ogre user object which connects to an EmberEntity.
 * Instances of this class can be attached to movable objects in the ogre system. They will allow for the Ember system to be accessed directly from Ogre, without having to do lookups.
 * This is generally mostly used for mouse picking and collision handling.
 */
class EmberEntityUserObject
{
public:

	/**
	 * Ctor.
	 * @param emberEntity A valid EmberEntity instance.
	 * @param collisionObject A valid vector of collision objects.
	 */
	EmberEntityUserObject(EmberEntity& emberEntity, ICollisionDetector* collisionDetector);

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

	/**
	 * @brief Gets the collision detector in use.
	 * @return The collision detector, if any.
	 */
	ICollisionDetector* getCollisionDetector() const;

protected:

	/**
	 * @brief The entity to which this user object belongs.
	 */
	EmberEntity& mEmberEntity;

	/**
	 * @brief The collision detector, responsible for checking collisions.
	 */
	ICollisionDetector* mCollisionDetector;

};

inline ICollisionDetector* EmberEntityUserObject::getCollisionDetector() const
{
	return mCollisionDetector;
}

}
;

#endif
