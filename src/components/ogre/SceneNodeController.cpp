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

#include "SceneNodeController.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/SceneNodeAttachment.h"

#include <sigc++/signal.h>

namespace EmberOgre {

SceneNodeController::SceneNodeController(SceneNodeAttachment& attachment)
: mAttachment(attachment)
{
	mAttachment.getAttachedEntity().Moved.connect(sigc::mem_fun(*this, &SceneNodeController::entity_Moved));
}

SceneNodeController::~SceneNodeController()
{
}


void SceneNodeController::entity_Moved()
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

void SceneNodeController::updateMotion(float timeSlice)
{
	updatePosition();
}

void SceneNodeController::updatePosition()
{
	mAttachment.setPosition(mAttachment.getAttachedEntity().getPredictedPos(), mAttachment.getAttachedEntity().getOrientation());
}


}
