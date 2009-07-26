/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "EmberOgrePrerequisites.h"

#ifndef ICOLLISIONDETECTOR_H_
#define ICOLLISIONDETECTOR_H_

namespace EmberOgre
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Represents the result of a collision query.
 */
struct CollisionResult
{
	/**
	 * @brief If the query resulted in a collision, this is set to true.
	 * Note that the other values in the struct will be meaningless if this is set to false, since no collision occurred then.
	 */
	bool collided;

	/**
	 * @brief The position of the collision, in world space.
	 */
	Ogre::Vector3 position;

	/**
	 * @brief The distance to the collision, in world units.
	 */
	float distance;
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 *
 * @brief Interface for collision detectors, responsible for determining collision information for the entity that they are attached to.
 */
class ICollisionDetector
{
public:
	virtual ~ICollisionDetector() {}

	/**
	 * @brief Test whether the provided ray hits the entity.
	 * @param ray The ray to test.
	 * @param result The result of the collision. If the ray hits, the collision detector must update this object.
	 */
	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result) = 0;
	/**
	 * @brief Refits the collision mesh against the entity. This is called to ensure that the collision mesh fits animated entities.
	 */
	virtual void refit() = 0;

	/**
	 * @brief Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	virtual void reload() = 0;

	/**
	 * @brief Sets whether the collision data should be visualized for debugging purposes.
	 * @param visualize
	 */
	virtual void setVisualize(bool visualize) = 0;
	/**
	 * @brief Gets whether the collision data should be visualized for debugging purposes.
	 * @return
	 */
	virtual bool getVisualize() const = 0;

};
}

#endif /* ICOLLISIONDETECTOR_H_ */
