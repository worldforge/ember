/*
 Copyright (C) 2009 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef AUTHORINGVISUALIZATIONCOLLISIONDETECTOR_H_
#define AUTHORINGVISUALIZATIONCOLLISIONDETECTOR_H_

#include "components/ogre/ICollisionDetector.h"
#include "components/ogre/OgreIncludes.h"

namespace Ember
{
namespace OgreView
{
namespace Authoring
{

class AuthoringVisualizationCollisionDetector: public ICollisionDetector
{
public:
	explicit AuthoringVisualizationCollisionDetector(Ogre::Entity& entity);

	~AuthoringVisualizationCollisionDetector() override;

	/**
	 * @brief Test whether the provided ray hits the entity.
	 * @param ray The ray to test.
	 * @param result The result of the collision. If the ray hits, the collision detector must update this object.
	 */
	void testCollision(Ogre::Ray& ray, CollisionResult& result) override;

	/**
	 * @brief Refits the collision mesh against the entity. This is called to ensure that the collision mesh fits animated entities.
	 */
	void refit() override;

	/**
	 * @brief Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	void reload() override;

	/**
	 * @brief Sets whether the collision data should be visualized for debugging purposes.
	 * @param visualize
	 */
	void setVisualize(bool visualize) override;

	/**
	 * @brief Gets whether the collision data should be visualized for debugging purposes.
	 * @return
	 */
	bool getVisualize() const override;

private:
	Ogre::Entity& mEntity;
};
}
}
}
#endif /* AUTHORINGVISUALIZATIONCOLLISIONDETECTOR_H_ */
