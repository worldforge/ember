//
// C++ Implementation: EntityMover
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "EntityMover.h"
#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "../EmberEntity.h"
#include "../MathConverter.h"


namespace EmberOgre {

EntityMover::EntityMover(EmberEntity* entity) : mEntity(entity)
{
}

const WFMath::Quaternion& EntityMover::getOrientation() const
{
	mOrientation = Ogre2Atlas(mEntity->getSceneNode()->getOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityMover::getPosition() const
{
	mPosition = Ogre2Atlas(mEntity->getSceneNode()->getPosition());
	return mPosition;
}
void EntityMover::setPosition(const WFMath::Point<3>& position)
{
	if (position.isValid()) {
		mEntity->getSceneNode()->setPosition(Atlas2Ogre(position));
		///adjust it so that it moves according to the ground for example
		mEntity->adjustPosition(mEntity->getSceneNode()->getPosition());
	}
}
void EntityMover::move(const WFMath::Vector<3> directionVector)
{
	if (directionVector.isValid()) {
		mEntity->getSceneNode()->translate(Atlas2Ogre(directionVector));
		///adjust it so that it moves according to the ground for example
		mEntity->adjustPosition(mEntity->getSceneNode()->getPosition());
	}
}
void EntityMover::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void EntityMover::yaw(WFMath::CoordType angle)
{
	mEntity->getSceneNode()->yaw(Ogre::Degree(angle));
}

void EntityMover::setOrientation(const WFMath::Quaternion& rotation)
{
	if (rotation.isValid()) {
		mEntity->getSceneNode()->setOrientation(Atlas2Ogre(rotation));
	}
}

void EntityMover::finalizeMovement()
{
	if (mEntity->getLocation()) {
		///send to server
		Ember::EmberServices::getSingleton().getServerService()->place(mEntity, mEntity->getLocation(), Ogre2Atlas(mEntity->getSceneNode()->getPosition()), Ogre2Atlas(mEntity->getSceneNode()->getOrientation()));
	}
	
}
void EntityMover::cancelMovement()
{
	mEntity->synchronizeWithServer();
}


}
