/*
 Copyright (C) 2009 erik

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

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/IAttachmentControlDelegate.h"
#include "components/ogre/INodeProvider.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/NodeController.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/DelegatingNodeController.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"
#include "components/ogre/model/Model.h"

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
namespace EmberOgre
{

NodeAttachment::NodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, INodeProvider* nodeProvider) :
	AttachmentBase::AttachmentBase(parentEntity, childEntity), mNode(0), mNodeProvider(nodeProvider), mAttachmentController(0)
{
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
	mNode = &mNodeProvider->getNode();
	//	mNode = parentNode.createChildSceneNode("entity_" + childEntity.getId());
	setupListeners();
}

NodeAttachment::NodeAttachment(NodeAttachment& source, NodeAttachment& newParentAttachment) :
	AttachmentBase::AttachmentBase(newParentAttachment.getAttachedEntity(), source.getAttachedEntity()), mNode(source.mNode), mAttachmentController(0)
{
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
	source.mNode = 0;
	setupListeners();
}

NodeAttachment::~NodeAttachment()
{
	delete mNodeProvider;
	delete mAttachmentController;
}

void NodeAttachment::setupListeners()
{
	mChildEntity.VisibilityChanged.connect(sigc::mem_fun(this, &NodeAttachment::entity_VisibilityChanged));
}

void NodeAttachment::entity_VisibilityChanged(bool visible)
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
	if (modelRepresentation) {
		return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, mNodeProvider->createChildProvider(&modelRepresentation->getModel()));
	}
	else {
		return new NodeAttachment(getAttachedEntity(), entity, mNodeProvider->createChildProvider(&modelRepresentation->getModel()));
	}
	//	}
}

void NodeAttachment::setControlDelegate(IAttachmentControlDelegate* controllerDelegate)
{
	delete mAttachmentController;
	if (controllerDelegate) {
		mAttachmentController = new DelegatingNodeController(*this, *controllerDelegate);
	}
	else {
		mAttachmentController = new NodeController(*this);
	}
}

IAttachmentControlDelegate* NodeAttachment::getControlDelegate() const
{
	if (mAttachmentController) {
		return mAttachmentController->getControlDelegate();
	}
	return 0;
}

void NodeAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation)
{
	if (position.isValid()) {
		WFMath::Vector<3> adjustedOffset = WFMath::Vector<3>::ZERO();
		if (mParentEntity.getAttachment()) {
			mParentEntity.getAttachment()->getOffsetForContainedNode(*this, position, adjustedOffset);
		}
		mNode->setPosition(Convert::toOgre(position + adjustedOffset));
		mNode->setOrientation(Convert::toOgre(orientation));
	}
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

}
