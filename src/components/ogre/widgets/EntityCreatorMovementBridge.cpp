//
// C++ Implementation: EntityCreatorMovementBridge
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "EntityCreatorMovementBridge.h"
#include "EntityCreator.h"
#include "components/ogre/manipulation/DetachedEntity.h"
#include <OgreSceneNode.h>
#include "../MathConverter.h"

namespace EmberOgre {

namespace Gui {

EntityCreatorMovementBridge::EntityCreatorMovementBridge(EntityCreator& creator, DetachedEntity& entity, Ogre::SceneNode* node)
: mEntity(entity), mCreator(creator), mNode(node)
{
}


EntityCreatorMovementBridge::~EntityCreatorMovementBridge()
{
}


const WFMath::Quaternion& EntityCreatorMovementBridge::getOrientation() const
{
	mOrientation = Ogre2Atlas(mNode->_getDerivedOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityCreatorMovementBridge::getPosition() const
{
	mPosition = Ogre2Atlas(mNode->_getDerivedPosition());
	return mPosition;
}
void EntityCreatorMovementBridge::setPosition(const WFMath::Point<3>& position)
{
	if (position.isValid()) {
		///We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (mNode->getParent()) {
			posOffset = mNode->getParent()->_getDerivedPosition();
		}
		mNode->setPosition(Atlas2Ogre(position) - posOffset);
		///adjust it so that it moves according to the ground for example
// 		mEntity.adjustPosition(mEntity.getSceneNode()->getPosition());
	}
}
void EntityCreatorMovementBridge::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		mNode->translate(Atlas2Ogre(directionVector));
		///adjust it so that it moves according to the ground for example
// 		mEntity.adjustPosition(mEntity.getSceneNode()->getPosition());
	}
}
void EntityCreatorMovementBridge::setRotation (int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void EntityCreatorMovementBridge::yaw(WFMath::CoordType angle)
{
	mNode->yaw(Ogre::Degree(angle));
}

void EntityCreatorMovementBridge::setOrientation(const WFMath::Quaternion& rotation)
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

void EntityCreatorMovementBridge::finalizeMovement()
{
	mCreator.finalizeCreation();
}
void EntityCreatorMovementBridge::cancelMovement()
{
	mCreator.stopCreation();
}

}

}
