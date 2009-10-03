//
// C++ Class: EntityMoverBase
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

#include "EntityMoverBase.h"

#include "SnapToMovement.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/EmberOgre.h"

#include <Eris/Entity.h>
#include <OgreSceneNode.h>

namespace EmberOgre
{
namespace Authoring
{
EntityMoverBase::EntityMoverBase(Eris::Entity& entity, Ogre::Node* node) :
	SnapTo("+snaptomovement", this, "Activates the 'snap to' behavior when moving an entity.", true), mEntity(entity), mNode(node), mSnapping(0)
{
}

EntityMoverBase::~EntityMoverBase()
{
}

const WFMath::Quaternion& EntityMoverBase::getOrientation() const
{
	mOrientation = Convert::toWF(mNode->_getDerivedOrientation());
	return mOrientation;
}

const WFMath::Point<3>& EntityMoverBase::getPosition() const
{
	mPosition = Convert::toWF<WFMath::Point<3> >(mNode->_getDerivedPosition());
	return mPosition;
}

void EntityMoverBase::setPosition(const WFMath::Point<3>& position)
{
	WFMath::Point<3> finalPosition(position);
	if (position.isValid()) {
		WFMath::Vector<3> adjustment;
		EmberEntity* entity(0);
		if (mSnapping.get() && mSnapping->testSnapTo(position, getOrientation(), adjustment, entity)) {
			finalPosition = finalPosition.shift(adjustment);
		}

		///We need to offset into local space.
		Ogre::Vector3 posOffset = Ogre::Vector3::ZERO;
		if (mNode->getParent()) {
			posOffset = mNode->getParent()->_getDerivedPosition();
		}
		mNode->setPosition(Convert::toOgre(finalPosition) - posOffset);
		newEntityPosition(mNode->getPosition());
		Moved.emit();
	}
}
void EntityMoverBase::move(const WFMath::Vector<3>& directionVector)
{
	if (directionVector.isValid()) {
		mNode->translate(Convert::toOgre(directionVector));
		newEntityPosition(mNode->getPosition());
		Moved.emit();
	}
}
void EntityMoverBase::setRotation(int axis, WFMath::CoordType angle)
{
	///not implemented yet
}

void EntityMoverBase::yaw(WFMath::CoordType angle)
{
	mNode->yaw(Ogre::Degree(angle));
	Moved.emit();
}

void EntityMoverBase::setOrientation(const WFMath::Quaternion& rotation)
{
	if (rotation.isValid()) {
		///We need to offset into local space.
		Ogre::Quaternion rotOffset = Ogre::Quaternion::IDENTITY;
		if (mNode->getParent()) {
			rotOffset = mNode->getParent()->_getDerivedOrientation();
		}
		mNode->setOrientation(Convert::toOgre(rotation) - rotOffset);
		Moved.emit();
	}
}

void EntityMoverBase::newEntityPosition(const Ogre::Vector3& position)
{
}

void EntityMoverBase::runCommand(const std::string &command, const std::string &args)
{
	if (SnapTo == command) {
		setSnapToEnabled(true);
	} else if (SnapTo.getInverseCommand() == command) {
		setSnapToEnabled(false);
	}

}
void EntityMoverBase::setSnapToEnabled(bool snapTo)
{
	if (snapTo && EmberOgre::getSingleton().getSceneManager()) {
		if (!mSnapping.get()) {
			mSnapping.reset(new Authoring::SnapToMovement(mEntity, *mNode, 2.0f, *EmberOgre::getSingleton().getSceneManager(), true));
			setPosition(Convert::toWF<WFMath::Point<3> >(mNode->getPosition()));
		}
	} else {
		mSnapping.reset();
	}
}

}
}
