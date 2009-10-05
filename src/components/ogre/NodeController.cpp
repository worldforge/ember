/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "components/ogre/EmberEntity.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/NodeAttachment.h"

#include <sigc++/signal.h>

namespace EmberOgre {

NodeController::NodeController(NodeAttachment& attachment)
: mAttachment(attachment)
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
	if (mAttachment.getAttachedEntity().isMoving())
	{
		motionManager.addMovable(this);
	}
	else
	{
		motionManager.removeMovable(this);
	}
}


void NodeController::updateMotion(float timeSlice)
{
	updatePosition();
}

void NodeController::updatePosition()
{
	mAttachment.setPosition(mAttachment.getAttachedEntity().getPredictedPos(), mAttachment.getAttachedEntity().getOrientation(), mAttachment.getAttachedEntity().getPredictedVelocity());
}

IEntityControlDelegate* NodeController::getControlDelegate() const
{
	return 0;
}



}
