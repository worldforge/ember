/*
 Copyright (C) 2009 Erik Ogenvik

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

#include "NodeController.h"

#include "domain/EmberEntity.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/NodeAttachment.h"

namespace Ember
{
namespace OgreView
{

NodeController::NodeController(NodeAttachment& attachment) :
	mAttachment(attachment)
{
	mAttachment.getAttachedEntity().Moved.connect(sigc::mem_fun(*this, &NodeController::entity_Moved));
	updatePosition();
}

NodeController::~NodeController()
{
	MotionManager::getSingleton().removeMovable(this);
}

void NodeController::entity_Moved()
{
	movementUpdate();
}

void NodeController::forceMovementUpdate()
{
	movementUpdate();
}
void NodeController::movementUpdate()
{
	updatePosition();
	MotionManager& motionManager = MotionManager::getSingleton();
	if (mAttachment.getAttachedEntity().isMoving()) {
		motionManager.addMovable(this);
	} else {
		motionManager.removeMovable(this);
	}
}

void NodeController::updateMotion(float timeSlice)
{
	updatePosition();
}

void NodeController::updatePosition()
{
	const WFMath::Point<3>& pos = mAttachment.getAttachedEntity().getPredictedPos();
	const WFMath::Quaternion& orientation = mAttachment.getAttachedEntity().getPredictedOrientation() * WFMath::Quaternion(2, WFMath::numeric_constants<float>::pi() / 2);
	const WFMath::Vector<3>& velocity = mAttachment.getAttachedEntity().getPredictedVelocity();
	mAttachment.setPosition(pos.isValid() ? pos : WFMath::Point<3>::ZERO(), orientation.isValid() ? orientation : WFMath::Quaternion::IDENTITY(), velocity.isValid() ? velocity : WFMath::Vector<3>::ZERO());
}

IEntityControlDelegate* NodeController::getControlDelegate() const
{
	return nullptr;
}

}
}
