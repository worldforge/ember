//
// C++ Implementation: EntityMoveAdjuster
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityMoveAdjuster.h"
#include "domain/EmberEntity.h"
#include "EntityMoveManager.h"

namespace Ember {
namespace OgreView {
namespace Authoring {
EntityMoveAdjustmentInstance::EntityMoveAdjustmentInstance(EntityMoveAdjuster* moveAdjuster, EmberEntity* entity, Eris::EventService& eventService) :
		mEntity(entity),
		mTimeout(eventService, boost::posix_time::milliseconds(1500), [&]() { this->timout_Expired(); }),
		mMoveAdjuster(moveAdjuster) {
}

void EntityMoveAdjustmentInstance::timout_Expired() {
	//	mEntity->synchronizeWithServer();
	mMoveAdjuster->removeInstance(this);
}

EntityMoveAdjuster::EntityMoveAdjuster(EntityMoveManager* manager, Eris::EventService& eventService) :
		mManager(manager), mEventService(eventService) {
	mManager->EventStartMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_StartMoving));
	mManager->EventFinishedMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_FinishedMoving));
	mManager->EventCancelledMoving.connect(sigc::mem_fun(*this, &EntityMoveAdjuster::EntityMoveManager_CancelledMoving));

}

void EntityMoveAdjuster::removeInstance(EntityMoveAdjustmentInstance* instance) {
	std::remove_if(mInstances.begin(), mInstances.end(), [instance](const std::unique_ptr<EntityMoveAdjustmentInstance>& entry) { return entry.get() == instance; });
}

void EntityMoveAdjuster::EntityMoveManager_FinishedMoving() {
	if (mActiveEntity) {
		mInstances.emplace_back(std::make_unique<EntityMoveAdjustmentInstance>(this, mActiveEntity, mEventService));
		mActiveEntity = nullptr;
	}
}

void EntityMoveAdjuster::EntityMoveManager_CancelledMoving() {
	mActiveEntity = nullptr;
}

void EntityMoveAdjuster::EntityMoveManager_StartMoving(EmberEntity& entity, EntityMover&) {
	mActiveEntity = &entity;
}

}
}
}
