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

#ifndef EMBER_BULLETCOLLISIONDETECTOR_H
#define EMBER_BULLETCOLLISIONDETECTOR_H

#include <domain/EmberEntity.h>
#include <boost/any.hpp>
#include "BulletWorld.h"

namespace Ember {
namespace OgreView {
class INodeProvider;
namespace Model {
class Model;
}


/**
 * A collision detection instance. This will take care of registering and de-registering collision shapes and objects
 * with the BulletWorld.
 */
class BulletCollisionDetector {
public:

	explicit BulletCollisionDetector(BulletWorld& bulletWorld);

	~BulletCollisionDetector();

	void addCollisionShape(std::shared_ptr<btCollisionShape> shape);

	void updateTransforms(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation);

	void updateScale(const WFMath::Vector<3>& scale);

	void clear();

	/**
	 * Arbitrary collision info to be used by the pick listeners.
	 */
	boost::any collisionInfo;

private:

	BulletWorld& mBulletWorld;

	std::vector<std::unique_ptr<btCollisionObject>> mCollisionObjects;
	std::vector<std::shared_ptr<btCollisionShape>> mCollisionShapes;


};
}
}


#endif //EMBER_BULLETCOLLISIONDETECTOR_H
