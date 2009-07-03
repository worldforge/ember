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

#include <OgreSceneNode.h>

namespace EmberOgre
{

EntityMover::EntityMover(EmberEntity& entity, EntityMoveManager& manager) :
	EntityMoverBase(entity, entity.getSceneNode()), mEntity(entity), mManager(manager)
{
}

void EntityMover::finalizeMovement()
{
	if (mEntity.getLocation())
	{
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

void EntityMover::newEntityPosition(const Ogre::Vector3& position)
{
	mEntity.adjustPosition(mEntity.getSceneNode()->getPosition());
}

}
