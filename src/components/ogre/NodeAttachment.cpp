/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "NodeAttachment.h"

#include "domain/IEntityControlDelegate.h"
#include "components/ogre/INodeProvider.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/NodeController.h"
#include "components/ogre/DelegatingNodeController.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelAttachment.h"
#include "components/ogre/model/Model.h"
#include "OgreInfo.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {

NodeAttachment::NodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, std::unique_ptr<INodeProvider> nodeProvider) :
		AttachmentBase(&parentEntity, childEntity),
		mNodeProvider(std::move(nodeProvider)),
		mAttachmentController(nullptr) {
	setupListeners();
}

NodeAttachment::~NodeAttachment() = default;

void NodeAttachment::init() {
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
}

void NodeAttachment::setupListeners() {
	mChildEntity.VisibilityChanged.connect(sigc::mem_fun(this, &NodeAttachment::entity_VisibilityChanged));
	mChildEntity.EventPositioningModeChanged.connect(sigc::mem_fun(this, &NodeAttachment::entity_PositioningModeChanged));
}

void NodeAttachment::entity_VisibilityChanged(bool visible) {
	setVisible(visible);
}

void NodeAttachment::entity_PositioningModeChanged(EmberEntity::PositioningMode newMode) {
//	if (newMode == EmberEntity::PositioningMode::PROJECTILE) {
//		mScene.registerEntityWithTechnique(mEntity, "projectile");
//	} else if (mChildEntity.getPositioningMode() == EmberEntity::PositioningMode::PROJECTILE) {
//		mScene.deregisterEntityWithTechnique(mEntity, "projectile");
//	}
}

void NodeAttachment::setVisible(bool visible) {
	mNodeProvider->setVisible(visible);
}

std::unique_ptr<IEntityAttachment> NodeAttachment::attachEntity(EmberEntity& entity) {
    //No special kind of attachment for things attached to a node.
    return nullptr;
}

void NodeAttachment::setControlDelegate(IEntityControlDelegate* controllerDelegate) {
	if (controllerDelegate) {
		mAttachmentController = std::make_unique<DelegatingNodeController>(*this, *controllerDelegate);
	} else {
		mAttachmentController = std::make_unique<NodeController>(*this);
	}
}

IEntityControlDelegate* NodeAttachment::getControlDelegate() const {
	if (mAttachmentController) {
		return mAttachmentController->getControlDelegate();
	}
	return nullptr;
}

void NodeAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity) {
	assert(position.isValid());
	assert(orientation.isValid());
	assert(velocity.isValid());
	WFMath::Vector<3> adjustedOffset = WFMath::Vector<3>::ZERO();
	//If it's fixed it shouldn't be adjusted
	if (getAttachedEntity().getPositioningMode() != EmberEntity::PositioningMode::FIXED) {
		if (mParentEntity->getAttachment()) {
			mParentEntity->getAttachment()->getOffsetForContainedNode(*this, position, adjustedOffset);
		}
	}
	mNodeProvider->setPositionAndOrientation(Convert::toOgre(position + adjustedOffset), Convert::toOgre(orientation));
}

Ogre::Node* NodeAttachment::getNode() const {
	return mNodeProvider->getNode();
}

void NodeAttachment::updatePosition() {
	if (mAttachmentController) {
		mAttachmentController->forceMovementUpdate();
	}
}

void NodeAttachment::setVisualize(const std::string& visualization, bool visualize) {
	mNodeProvider->setVisualize(visualization, visualize);
	AttachmentBase::setVisualize(visualization, visualize);
}

bool NodeAttachment::getVisualize(const std::string& visualization) const {
	bool providerResult = mNodeProvider->getVisualize(visualization);
	return AttachmentBase::getVisualize(visualization) || providerResult;
}

}
}
