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
#include "components/ogre/SceneNodeController.h"
#include "components/ogre/DelegatingSceneNodeController.h"

#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"


#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
namespace EmberOgre {

SceneNodeAttachment::SceneNodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, Ogre::SceneNode& parentNode)
: AttachmentBase::AttachmentBase(parentEntity, childEntity), mSceneNode(0), mAttachmentController(0)
{
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
	mSceneNode = parentNode.createChildSceneNode("entity_" + childEntity.getId());
}

SceneNodeAttachment::SceneNodeAttachment(SceneNodeAttachment& source, SceneNodeAttachment& newParentAttachment)
: AttachmentBase::AttachmentBase(newParentAttachment.getAttachedEntity(), source.getAttachedEntity()), mSceneNode(source.mSceneNode), mAttachmentController(0)
{
	setControlDelegate(mChildEntity.getAttachmentControlDelegate());
	source.mSceneNode = 0;
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

IEntityAttachment* SceneNodeAttachment::attachEntity(EmberEntity& entity)
{

	Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
	SceneNodeAttachment* currentSceneNodeAttachment = dynamic_cast<SceneNodeAttachment*>(entity.getAttachment());
	Model::ModelAttachment* currentModelAttachment = dynamic_cast<Model::ModelAttachment*>(entity.getAttachment());
	if (currentModelAttachment) {
		return new Model::ModelAttachment(*currentModelAttachment, *this);
	} else if (currentSceneNodeAttachment) {
		return new SceneNodeAttachment(*currentSceneNodeAttachment, *this);
	} else {
		if (modelRepresentation) {
			return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, *mSceneNode);
		} else {
			return new SceneNodeAttachment(getAttachedEntity(), entity, *mSceneNode);
		}
	}
}

void SceneNodeAttachment::setControlDelegate(IAttachmentControlDelegate* controllerDelegate)
{
	delete mAttachmentController;
	if (controllerDelegate) {
		mAttachmentController = new DelegatingSceneNodeController(*this, *controllerDelegate);
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



Ogre::SceneNode* SceneNodeAttachment::getSceneNode() const
{
	return mSceneNode;
}

void SceneNodeAttachment::updatePosition()
{
	if (mAttachmentController) {
		mAttachmentController->forceMovementUpdate();
	}

}


}
