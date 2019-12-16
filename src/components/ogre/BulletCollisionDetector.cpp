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

#include "BulletCollisionDetector.h"

#include "model/Model.h"
#include "model/SubModel.h"

#include <OgreNode.h>

namespace Ember {
namespace OgreView {

namespace {
//btVector3 toBullet(const Ogre::Vector3& p) {
//	return {p.x, p.y, p.z};
//}
//
//
//btQuaternion toBullet(const Ogre::Quaternion& aq) {
//	return {aq.x, aq.y, aq.z, aq.w};
//}

btVector3 toBullet(const WFMath::Point<3>& p) {
	return {static_cast<float>(p.x()), static_cast<float>(p.y()), static_cast<float>(p.z())};
}

btVector3 toBullet(const WFMath::Vector<3>& p) {
	return {static_cast<float>(p.x()), static_cast<float>(p.y()), static_cast<float>(p.z())};
}


btQuaternion toBullet(const WFMath::Quaternion& aq) {
	return {static_cast<float>(aq.vector().x()), static_cast<float>(aq.vector().y()), static_cast<float>(aq.vector().z()), static_cast<float>(aq.scalar())};
}
}

BulletCollisionDetector::BulletCollisionDetector(BulletWorld& bulletWorld)
		: mBulletWorld(bulletWorld),
		  mMask(COLLISION_MASK_OCCLUDING | COLLISION_MASK_PICKABLE) {
}

BulletCollisionDetector::~BulletCollisionDetector() {

	for (auto& collisionObject : mCollisionObjects) {
		mBulletWorld.getCollisionWorld().removeCollisionObject(collisionObject.get());
	}

}

void BulletCollisionDetector::updateTransforms(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation) {
	btTransform transform{};
	if (position.isValid()) {
		transform.setOrigin(toBullet(position));
	} else {
		transform.setOrigin(btVector3(0, 0, 0));
	}
	if (orientation.isValid()) {
		transform.setBasis(btMatrix3x3(toBullet(orientation)));
	} else {
		transform.setBasis(btMatrix3x3::getIdentity());
	}

	for (auto& collisionObject : mCollisionObjects) {
		collisionObject->setWorldTransform(transform);
		mBulletWorld.getCollisionWorld().updateSingleAabb(collisionObject.get());
	}
}

void BulletCollisionDetector::updateScale(const WFMath::Vector<3>& scale) {
	for (auto& collisionObject : mCollisionObjects) {
		collisionObject->getCollisionShape()->setLocalScaling(toBullet(scale));
		mBulletWorld.getCollisionWorld().updateSingleAabb(collisionObject.get());
	}
}

void BulletCollisionDetector::addCollisionShape(std::shared_ptr<btCollisionShape> shape) {
	if (shape) {
		std::unique_ptr<btCollisionObject> collisionObject(new btCollisionObject());
		collisionObject->setCollisionShape(shape.get());
		collisionObject->setUserPointer(this);
		mBulletWorld.getCollisionWorld().addCollisionObject(collisionObject.get(), mMask);
		mCollisionObjects.push_back(std::move(collisionObject));
		mCollisionShapes.push_back(std::move(shape));
	}

}

void BulletCollisionDetector::clear() {
	for (auto& collisionObject : mCollisionObjects) {
		mBulletWorld.getCollisionWorld().removeCollisionObject(collisionObject.get());
	}
	mCollisionObjects.clear();
	mCollisionShapes.clear();
}

void BulletCollisionDetector::setMask(short mask) {
	mMask = mask;
	for (auto& collisionObject : mCollisionObjects) {
		collisionObject->getBroadphaseHandle()->m_collisionFilterGroup = mask;
	}
}


}
}

