//
// C++ Implementation: EntityMover
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
#include "EntityMover.h"
#include "EntityMoveManager.h"
#include "domain/EmberEntity.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/Convert.h"
#include "components/ogre/World.h"
#include "components/ogre/Scene.h"
#include "components/ogre/terrain/TerrainManager.h"
#include "components/ogre/model/ModelRepresentation.h"

#include "services/server/ServerService.h"
#include <OgreSceneNode.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>

namespace Ember {
namespace OgreView {
namespace Authoring {
EntityMover::EntityMover(NodeAttachment& nodeAttachment, EntityMoveManager& manager) :
		EntityMoverBase(&nodeAttachment.getAttachedEntity(), nodeAttachment.getNode(), manager.getWorld().getScene().getSceneManager()),
		mNodeAttachment(nodeAttachment),
		mManager(manager),
		mPreviousControlDelegate(nodeAttachment.getControlDelegate()),
		mControlDelegate(std::make_unique<EntityMoverControlDelegate>(*this)) {
	nodeAttachment.setControlDelegate(mControlDelegate.get());


	auto modelRepresentation = dynamic_cast<Model::ModelRepresentation*>(nodeAttachment.getAttachedEntity().getGraphicalRepresentation());
	if (modelRepresentation) {
		//Disable occlusion while moving the entity
		modelRepresentation->getCollisionDetector().setMask(COLLISION_MASK_PICKABLE);
	}


	//Check if the entity already is offset from the terrain, and retain that offset.
	auto position = nodeAttachment.getAttachedEntity().getPosition();
	float height;
	manager.getWorld().getTerrainManager().getHeight({position.x(), position.z()}, height);

	if (std::abs(height - position.y()) > 0.01) {
		setOffset({static_cast<float>(position.y()) - height});
	}
}

EntityMover::~EntityMover() {
	auto modelRepresentation = dynamic_cast<Model::ModelRepresentation*>(mNodeAttachment.getAttachedEntity().getGraphicalRepresentation());
	if (modelRepresentation) {
		modelRepresentation->getCollisionDetector().setMask(COLLISION_MASK_PICKABLE | COLLISION_MASK_OCCLUDING);
	}
}

const IEntityControlDelegate& EntityMover::getControlDelegate() const {
	return *mControlDelegate;
}

void EntityMover::finalizeMovement() {
	if (mEntity->getLocation()) {
		mManager.getWorld().getView().getAvatar().place(mEntity.get(), mEntity->getLocation(), getPosition(), getOrientation());
	}

	//Let the manager perform a check after some seconds to adjust the position of the entity (if movement failed etc.).
	mManager.delayedUpdatePositionForEntity(mEntity->getId());

	cleanup();
	mManager.EventFinishedMoving.emit();

}

void EntityMover::cancelMovement() {
	cleanup();
	mNodeAttachment.updatePosition();
	mManager.EventCancelledMoving.emit();
}

void EntityMover::cleanup() {
	mNodeAttachment.setControlDelegate(mPreviousControlDelegate);
}

void EntityMover::newEntityPosition(const Ogre::Vector3&) {
	mNodeAttachment.updatePosition();
}

EntityMoverControlDelegate::EntityMoverControlDelegate(EntityMover& entityMover) :
		mEntityMover(entityMover) {

}

const WFMath::Point<3>& EntityMoverControlDelegate::getPosition() const {
	return mEntityMover.getPosition();
}

const WFMath::Quaternion& EntityMoverControlDelegate::getOrientation() const {
	return mEntityMover.getOrientation();
}

const WFMath::Vector<3>& EntityMoverControlDelegate::getVelocity() const {
	return WFMath::Vector<3>::ZERO();
}

}

}
}
