//
// C++ Interface: MeshCollisionDetector
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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
#ifndef EMBEROGREMESHCOLLISIONDETECTOR_H
#define EMBEROGREMESHCOLLISIONDETECTOR_H

#include <OgreSharedPtr.h>
#include "EmberEntityUserObject.h"
#include "ICollisionDetector.h"

namespace Ember {
namespace OgreView {

/**
	Uses a brute force approach to checking for intersection by iterating through all vertices to see which intersects the ray.
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class MeshCollisionDetector : public ICollisionDetector
{
public:
	explicit MeshCollisionDetector(Model::Model* model);

	~MeshCollisionDetector() override = default;

	void testCollision(Ogre::Ray& ray, CollisionResult& result) override;
	void refit() override;
	/**
	 *    Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	void reload() override;

	void setVisualize(bool visualize) override;

	bool getVisualize() const override;

protected:
	Model::Model* mModel;
	void getMeshInformation(const Ogre::MeshPtr& mesh,
                                size_t &vertex_count,
                                std::vector<Ogre::Vector3> &vertices,
                                size_t &index_count,
                                std::vector<unsigned long> &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);

	/**
	 * We keep preallocated vectors of vertices and indices to avoid allocating new memory each time.
	 */
	std::vector<Ogre::Vector3> mVertices;
	/**
	 * We keep preallocated vectors of vertices and indices to avoid allocating new memory each time.
	 */
	std::vector<unsigned long> mIndices;
};

}

}

#endif
