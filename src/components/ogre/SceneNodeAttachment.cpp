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

#include "SceneNodeAttachment.h"

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/IAttachmentControlDelegate.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/SceneNodeController.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"


#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
namespace EmberOgre {

SceneNodeAttachment::SceneNodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, Ogre::SceneNode& parentNode)
: mParentEntity(parentEntity), mChildEntity(childEntity), mSceneNode(0), mAttachmentController(new SceneNodeController(*this))
{
	mSceneNode = parentNode.createChildSceneNode("entity_" + childEntity.getId());
}

SceneNodeAttachment::SceneNodeAttachment(const SceneNodeAttachment& source)
: mParentEntity(source.mParentEntity), mChildEntity(source.mChildEntity), mSceneNode(source.mSceneNode), mAttachmentController(0)
{
}


SceneNodeAttachment::~SceneNodeAttachment()
{
	if (mSceneNode) {
		Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*> (mSceneNode->getParent());
		if (parent)
		{
			parent->removeAndDestroyChild(mSceneNode->getName());
		}
		else
		{
			mSceneNode->getCreator()->destroySceneNode(mSceneNode->getName());
		}
	}
}

IGraphicalRepresentation* SceneNodeAttachment::getGraphicalRepresentation() const
{
	return 0;
}

EmberEntity& SceneNodeAttachment::getAttachedEntity() const
{
	return mChildEntity;
}

EmberEntity* SceneNodeAttachment::getParentEntity() const
{
	return &mParentEntity;
}

IEntityAttachment* SceneNodeAttachment::attachEntity(EmberEntity& entity)
{
	if (Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity)) {
		return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, *mSceneNode);
	} else {
		return new SceneNodeAttachment(getAttachedEntity(), entity, *mSceneNode);
	}
}

void SceneNodeAttachment::updateScale()
{
}

void SceneNodeAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset)
{
	if (mParentEntity.getAttachment()) {
		WFMath::Vector<3> localPositionShift(mChildEntity.getPredictedPos());
		WFMath::Point<3> adjustedLocalPosition = localPosition + localPositionShift;
		mParentEntity.getAttachment()->getOffsetForContainedNode(attachment, adjustedLocalPosition, offset);
	}
}

void SceneNodeAttachment::setControllerDelegate(IAttachmentControlDelegate* controllerDelegate)
{
	delete mAttachmentController;
	if (controllerDelegate) {
		mAttachmentController = new SceneNodeController(*this);
	} else {
		mAttachmentController = new SceneNodeController(*this);
	}
}

void SceneNodeAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation)
{
	if (position.isValid()) {
		WFMath::Vector<3> adjustedOffset = WFMath::Vector<3>::ZERO();
		if (mParentEntity.getAttachment()) {
			mParentEntity.getAttachment()->getOffsetForContainedNode(*this, position, adjustedOffset);
		}
		mSceneNode->setPosition(Convert::toOgre(position + adjustedOffset));
		mSceneNode->setOrientation(Convert::toOgre(orientation));
	}
}




SceneNodeAttachment* SceneNodeAttachment::transferToNewParent(SceneNodeAttachment& newParentAttachment)
{
	SceneNodeAttachment* newAttachment = new SceneNodeAttachment(*this);
	mSceneNode = 0;
	return newAttachment;
}


}
