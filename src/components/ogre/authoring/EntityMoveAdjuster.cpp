//
// C++ Implementation: EntityMoveAdjuster
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

#include "EntityMoveAdjuster.h"
#include "../EmberEntity.h"
#include "EntityMoveManager.h"

namespace EmberOgre
{
namespace Authoring
{
EntityMoveAdjustmentInstance::EntityMoveAdjustmentInstance(EntityMoveAdjuster* moveAdjuster, EmberEntity* entity) :
	mEntity(entity), mTimeout(1500), mMoveAdjuster(moveAdjuster)
{
	mTimeout.Expired.connect(sigc::mem_fun(this, &EntityMoveAdjustmentInstance::timout_Expired));
}

void EntityMoveAdjustmentInstance::timout_Expired()
{
	//	mEntity->synchronizeWithServer();
	mMoveAdjuster->removeInstance(this);
}

EntityMoveAdjuster::EntityMoveAdjuster(EntityMoveManager* manager) :
	mManager(manager)
{
	mManager->EventStartMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_StartMoving));
	mManager->EventFinishedMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_FinishedMoving));
	mManager->EventCancelledMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_CancelledMoving));

}

void EntityMoveAdjuster::removeInstance(EntityMoveAdjustmentInstance* instance)
{
	MoveAdjustmentInstanceStore::iterator I = std::find(mInstances.begin(), mInstances.end(), instance);
	delete *I;
	mInstances.erase(I);
}

void EntityMoveAdjuster::EntityMoveManager_FinishedMoving()
{
	if (mActiveEntity) {
		EntityMoveAdjustmentInstance* instance = new EntityMoveAdjustmentInstance(this, mActiveEntity);
		mInstances.push_back(instance);
		mActiveEntity = 0;
	}
}

void EntityMoveAdjuster::EntityMoveManager_CancelledMoving()
{
	mActiveEntity = 0;
}

void EntityMoveAdjuster::EntityMoveManager_StartMoving(EmberEntity& entity, EntityMoverBase& mover)
{
	mActiveEntity = &entity;
}

}
}
