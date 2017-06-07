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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "NodeAttachment.h"

#include "domain/IGraphicalRepresentation.h"
#include "domain/IEntityControlDelegate.h"
#include "components/ogre/INodeProvider.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/NodeController.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/DelegatingNodeController.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"
#include "components/ogre/model/Model.h"
#include "OgreInfo.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
namespace Ember
{
namespace OgreView
{

NodeAttachment::NodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, INodeProvider* nodeProvider) :
	AttachmentBase(parentEntity, childEntity), mNode(nullptr), mNodeProvider(nodeProvider), mAttachmentController(nullptr)
{
	mNode = &mNodeProvider->getNode();
	setupListeners();
}

NodeAttachment::~NodeAttachment()
{
	delete mNodeProvider;
	delete mAttachmentController;
}

void NodeAttachment::init()
{
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
}

void NodeAttachment::setupListeners()
{
	mChildEntity.VisibilityChanged.connect(sigc::mem_fun(this, &NodeAttachment::entity_VisibilityChanged));
}

void NodeAttachment::entity_VisibilityChanged(bool visible)
{
	setVisible(visible);
}

void NodeAttachment::setVisible(bool visible)
{
	mNodeProvider->setVisible(visible);
}

IEntityAttachment* NodeAttachment::attachEntity(EmberEntity& entity)
{

	Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
	//	NodeAttachment* currentNodeAttachment = dynamic_cast<NodeAttachment*> (entity.getAttachment());
	//	Model::ModelAttachment* currentModelAttachment = dynamic_cast<Model::ModelAttachment*> (entity.getAttachment());
	//	if (currentModelAttachment) {
	//		return new Model::ModelAttachment(*currentModelAttachment, *this);
	//	}
	//	else if (currentNodeAttachment) {
	//		return new NodeAttachment(*currentNodeAttachment, *this);
	//	}
	//	else {

	//If there's a model representation available, use a "ModelAttachment" instance to attach to it, otherwise just use a regular NodeAttachment.
	NodeAttachment* nodeAttachment(0);
	INodeProvider* nodeProvider = mNodeProvider->createChildProvider(OgreInfo::createUniqueResourceName(entity.getId()));
	if (modelRepresentation) {
		nodeAttachment = new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, nodeProvider);
	} else {
		nodeAttachment = new NodeAttachment(getAttachedEntity(), entity, nodeProvider);
	}
	nodeAttachment->init();
	return nodeAttachment;
	//	}
}

void NodeAttachment::setControlDelegate(IEntityControlDelegate* controllerDelegate)
{
	delete mAttachmentController;
	if (controllerDelegate) {
		mAttachmentController = new DelegatingNodeController(*this, *controllerDelegate);
	} else {
		mAttachmentController = new NodeController(*this);
	}
}

IEntityControlDelegate* NodeAttachment::getControlDelegate() const
{
	if (mAttachmentController) {
		return mAttachmentController->getControlDelegate();
	}
	return 0;
}

void NodeAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity)
{
	assert(position.isValid());
	assert(orientation.isValid());
	assert(velocity.isValid());
	WFMath::Vector<3> adjustedOffset = WFMath::Vector<3>::ZERO();
	//If it's fixed it shouldn't be adjusted
	if (getAttachedEntity().getPositioningMode() != EmberEntity::PM_FIXED) {
		if (getAttachedEntity().getPositioningMode() == EmberEntity::PM_FLOATING) {
			//If the entity is floating, the z position should be 0.
			adjustedOffset.z() = -position.z();
		} else if (mParentEntity.getAttachment()) {
			//If it's swimming, the position should be either floating (z==0) or above ground.
			if (getAttachedEntity().getPositioningMode() == EmberEntity::PM_SWIMMING) {
				if (position.z() > 0) {
					adjustedOffset.z() = -position.z();
				} else {
					mParentEntity.getAttachment()->getOffsetForContainedNode(*this, position, adjustedOffset);
				}
			} else {
				mParentEntity.getAttachment()->getOffsetForContainedNode(*this, position, adjustedOffset);
			}
		}
	}
	mNodeProvider->setPositionAndOrientation(Convert::toOgre(position + adjustedOffset), Convert::toOgre(orientation));
}
Ogre::Node* NodeAttachment::getNode() const
{
	return mNode;
}

void NodeAttachment::updatePosition()
{
	if (mAttachmentController) {
		mAttachmentController->forceMovementUpdate();
	}
}

void NodeAttachment::setVisualize(const std::string& visualization, bool visualize)
{
	mNodeProvider->setVisualize(visualization, visualize);
	AttachmentBase::setVisualize(visualization, visualize);
}

bool NodeAttachment::getVisualize(const std::string& visualization) const
{
	bool providerResult = mNodeProvider->getVisualize(visualization);
	return AttachmentBase::getVisualize(visualization) || providerResult;
}

}
}
