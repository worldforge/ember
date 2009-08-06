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

#include "SimpleWaterCollisionDetector.h"
#include "SimpleWater.h"
#include <OgreMath.h>
#include <OgrePlane.h>
#include <OgreRay.h>
#include <OgreVector3.h>

namespace EmberOgre
{

namespace Environment
{

SimpleWaterCollisionDetector::SimpleWaterCollisionDetector(SimpleWater& simpleWater)
: mSimpleWater(simpleWater)
{

}

SimpleWaterCollisionDetector::~SimpleWaterCollisionDetector()
{
}

void SimpleWaterCollisionDetector::reload()
{
}

void SimpleWaterCollisionDetector::refit()
{
}

void SimpleWaterCollisionDetector::setVisualize(bool visualize)
{
}
bool SimpleWaterCollisionDetector::getVisualize() const
{
	return false;
}

void SimpleWaterCollisionDetector::testCollision(Ogre::Ray& ray, CollisionResult& result)
{

	Ogre::Plane waterPlane(Ogre::Vector3::UNIT_Y, mSimpleWater.getLevel());
	std::pair<bool, Ogre::Real> intersectionResult(Ogre::Math::intersects(ray, waterPlane));
	if (intersectionResult.first) {
		// raycast success
		result.collided = true;
		result.position = ray.getPoint(intersectionResult.second);
		result.distance = intersectionResult.second;
	} else {
		// raycast failed
		result.collided = false;
	}
}
}
}
