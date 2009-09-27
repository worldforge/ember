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

#include "AuthoringVisualizationCollisionDetector.h"
#include <OgreMath.h>
#include <OgreEntity.h>
#include <OgreRay.h>
#include <OgreVector3.h>

namespace EmberOgre
{
namespace Authoring
{

AuthoringVisualizationCollisionDetector::AuthoringVisualizationCollisionDetector(Ogre::Entity& entity) :
	mEntity(entity)
{
}

AuthoringVisualizationCollisionDetector::~AuthoringVisualizationCollisionDetector()
{
}

void AuthoringVisualizationCollisionDetector::reload()
{
}

void AuthoringVisualizationCollisionDetector::refit()
{
}

void AuthoringVisualizationCollisionDetector::setVisualize(bool visualize)
{
}
bool AuthoringVisualizationCollisionDetector::getVisualize() const
{
	return false;
}

void AuthoringVisualizationCollisionDetector::testCollision(Ogre::Ray& ray, CollisionResult& result)
{
	std::pair<bool, Ogre::Real> intersectionResult(Ogre::Math::intersects(ray, mEntity.getWorldBoundingBox()));
	if (intersectionResult.first) {
		// raycast success
		result.collided = true;
		result.position = ray.getPoint(intersectionResult.second);
		result.distance = intersectionResult.second;
		result.isTransparent = true; //The authoring selector should always be transparent so that we can select things behind it
	} else {
		// raycast failed
		result.collided = false;
	}
}
}
}
