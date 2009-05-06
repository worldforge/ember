/*
 * EntityMoverBase.cpp
 *
 *  Created on: 2009-maj-06
 *      Author: erik
 */

#include "EntityMoverBase.h"

EntityMoverBase::EntityMoverBase(EmberEntity& entity, Ogre::SceneNode* node)
: mEntity(entity), mNode(node)
{

}

EntityMoverBase::~EntityMoverBase()
{
}

const WFMath::Quaternion& EntityMoverBase::getOrientation() const
{
	mOrientation = Ogre2Atlas(mNode->_getDerivedOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityMoverBase::getPosition() const
{
	mPosition = Ogre2Atlas(mNode->_getDerivedPosition());
	return mPosition;
}

void EntityMoverBase::setPosition(const WFMath::Point<3>& position)
{
	WFMath::Point<3> finalPosition(position);
	if (position.isValid()) {
		WFMath::Vector<3> adjustment;
		EmberEntity* entity(0);
		if (mSnapping->testSnapTo(position, getOrientation(), adjustment, entity)) {
			finalPosition = finalPosition.shift(adjustment);
		}

		///We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (mNode->getParent()) {
			posOffset = mNode->getParent()->_getDerivedPosition();
		}
		mNode->setPosition(Atlas2Ogre(finalPosition) - posOffset);
		///adjust it so that it moves according to the ground for example
		mEntity.adjustPosition(mNode->getPosition());
	}
}
void EntityMoverBase::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		mNode->translate(Atlas2Ogre(directionVector));
		///adjust it so that it moves according to the ground for example
		mEntity.adjustPosition(mNode->getPosition());
	}
}
void EntityMoverBase::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void EntityMoverBase::yaw(WFMath::CoordType angle)
{
	mNode->yaw(Ogre::Degree(angle));
}

void EntityMoverBase::setOrientation(const WFMath::Quaternion& rotation)
{
	if (rotation.isValid()) {
		///We need to offset into local space.
		Ogre::Quaternion rotOffset = Ogre::Quaternion::IDENTITY;
		if (mNode->getParent()) {
			rotOffset = mNode->getParent()->_getDerivedOrientation();
		}
		mNode->setOrientation(Atlas2Ogre(rotation) - rotOffset);
	}
}
