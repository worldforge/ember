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

#ifndef SIMPLEWATERCOLLISIONDETECTOR_H_
#define SIMPLEWATERCOLLISIONDETECTOR_H_

#include "components/ogre/ICollisionDetector.h"
namespace EmberOgre
{

namespace Environment
{

class SimpleWater;
class SimpleWaterCollisionDetector: public ICollisionDetector
{
public:
	SimpleWaterCollisionDetector(SimpleWater& simpleWater);
	virtual ~SimpleWaterCollisionDetector();

	/**
	 * @brief Test whether the provided ray hits the entity.
	 * @param ray The ray to test.
	 * @param result The result of the collision. If the ray hits, the collision detector must update this object.
	 */
	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result);

	/**
	 * @brief Refits the collision mesh against the entity. This is called to ensure that the collision mesh fits animated entities.
	 */
	virtual void refit();

	/**
	 * @brief Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	virtual void reload();

	/**
	 * @brief Sets whether the collision data should be visualized for debugging purposes.
	 * @param visualize
	 */
	virtual void setVisualize(bool visualize);

	/**
	 * @brief Gets whether the collision data should be visualized for debugging purposes.
	 * @return
	 */
	virtual bool getVisualize() const;

private:
	SimpleWater& mSimpleWater;
};
}
}

#endif /* SIMPLEWATERCOLLISIONDETECTOR_H_ */
