//
// C++ Implementation: EntityMover
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityMover.h"
#include "EntityMoveManager.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "../EmberEntity.h"
#include "../MathConverter.h"

#include "SnapToMovement.h"


namespace EmberOgre {

EntityMover::EntityMover(EmberEntity& entity, EntityMoveManager& manager) : mEntity(entity), mManager(manager), mSnapping(new Manipulation::SnapToMovement(entity, 5.0f))
{
}

const WFMath::Quaternion& EntityMover::getOrientation() const
{
	mOrientation = Ogre2Atlas(mEntity.getSceneNode()->_getDerivedOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityMover::getPosition() const
{
	mPosition = Ogre2Atlas(mEntity.getSceneNode()->_getDerivedPosition());
	return mPosition;
}
void EntityMover::setPosition(const WFMath::Point<3>& position)
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
		if (mEntity.getSceneNode()->getParent()) {
			posOffset = mEntity.getSceneNode()->getParent()->_getDerivedPosition();
		}
		mEntity.getSceneNode()->setPosition(Atlas2Ogre(finalPosition) - posOffset);
		///adjust it so that it moves according to the ground for example
		mEntity.adjustPosition(mEntity.getSceneNode()->getPosition());
	}
}
void EntityMover::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		mEntity.getSceneNode()->translate(Atlas2Ogre(directionVector));
		///adjust it so that it moves according to the ground for example
		mEntity.adjustPosition(mEntity.getSceneNode()->getPosition());
	}
}
void EntityMover::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void EntityMover::yaw(WFMath::CoordType angle)
{
	mEntity.getSceneNode()->yaw(Ogre::Degree(angle));
}

void EntityMover::setOrientation(const WFMath::Quaternion& rotation)
{
	if (rotation.isValid()) {
		///We need to offset into local space.
		Ogre::Quaternion rotOffset = Ogre::Quaternion::IDENTITY;
		if (mEntity.getSceneNode()->getParent()) {
			rotOffset = mEntity.getSceneNode()->getParent()->_getDerivedOrientation();
		}
		mEntity.getSceneNode()->setOrientation(Atlas2Ogre(rotation) - rotOffset);
	}
}

void EntityMover::finalizeMovement()
{
	if (mEntity.getLocation()) {
		///send to server
		Ember::EmberServices::getSingleton().getServerService()->place(&mEntity, mEntity.getLocation(), Ogre2Atlas(mEntity.getSceneNode()->getPosition()), Ogre2Atlas(mEntity.getSceneNode()->getOrientation()));
	}
	mManager.EventFinishedMoving.emit();
	
}
void EntityMover::cancelMovement()
{
	mEntity.synchronizeWithServer();
	mManager.EventCancelledMoving.emit();
}


}
