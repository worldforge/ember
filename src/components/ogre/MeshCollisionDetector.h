//
// C++ Interface: MeshCollisionDetector
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGREMESHCOLLISIONDETECTOR_H
#define EMBEROGREMESHCOLLISIONDETECTOR_H

#include "EmberEntityUserObject.h"
#include "ICollisionDetector.h"

namespace EmberOgre {

/**
	Uses a brute force approach to checking for intersection by iterating through all vertices to see which intersects the ray.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class MeshCollisionDetector : public ICollisionDetector
{
public:
    MeshCollisionDetector(Model::Model* model);

    virtual ~MeshCollisionDetector();

	virtual void testCollision(Ogre::Ray& ray, CollisionResult& result);
	virtual void refit();
	/**
	 *    Called when the entity changes, such as a subentity being hidden or shown. Implementations must reload the collision data.
	 */
	virtual void reload();

	virtual void setVisualize(bool visualize);
	virtual bool getVisualize() const;

protected:
	Model::Model* mModel;
	void getMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);
};

}

#endif
