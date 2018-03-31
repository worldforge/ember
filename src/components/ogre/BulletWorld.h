/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_BULLETWORLD_H
#define EMBER_BULLETWORLD_H

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <OgreResource.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>
#include <unordered_map>
#include <memory>

namespace Ember {
namespace OgreView {

/**
 * Handles the Bullet collision world, as well as keeping a cache of mesh shapes.
 *
 * We use Bullet for collision checks, and mainly for picking in the world and camera movement.
 */
class BulletWorld {

public:
	BulletWorld();

	std::shared_ptr<btScaledBvhTriangleMeshShape> createMeshShape(const Ogre::MeshPtr& meshPtr);

	btCollisionWorld& getCollisionWorld() const;

private:

	std::shared_ptr<btCollisionWorld> mCollisionWorld;

	/**
	 * A cache of mesh shapes. This allows us to reuse a mesh shape multiple times.
	 */
	std::unordered_map<Ogre::ResourceHandle, std::shared_ptr<btBvhTriangleMeshShape>> mTriangleMeshShapes;

	std::shared_ptr<btBvhTriangleMeshShape> buildTriangleMeshShape(const Ogre::MeshPtr& sharedPtr);

	static void getMeshInformation(const Ogre::MeshPtr& mesh,
								   std::vector<float> &vertices,
								   std::vector<unsigned int> &indices);
};

}
}

#endif //EMBER_BULLETWORLD_H
