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

#include "OgreAttachment.h"

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/MotionManager.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"


#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
namespace EmberOgre {

OgreAttachment::OgreAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, Ogre::SceneNode& parentNode)
: mParentEntity(parentEntity), mChildEntity(childEntity), mSceneNode(0)
{
	mSceneNode = parentNode.createChildSceneNode("entity_" + childEntity.getId());
	mChildEntity.Moved.connect(sigc::mem_fun(*this, &OgreAttachment::entity_Moved));
	setupConnections();
}

OgreAttachment::OgreAttachment(const OgreAttachment& source)
: mParentEntity(source.mParentEntity), mChildEntity(source.mChildEntity), mSceneNode(source.mSceneNode)
{
	mChildEntity.Moved.connect(sigc::mem_fun(*this, &OgreAttachment::entity_Moved));
	setupConnections();
}


OgreAttachment::~OgreAttachment()
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

void OgreAttachment::setupConnections()
{
	mChildEntity.Moved.connect(sigc::mem_fun(*this, &OgreAttachment::entity_Moved));

}

IGraphicalRepresentation* OgreAttachment::getGraphicalRepresentation() const
{
	return 0;
}

EmberEntity& OgreAttachment::getAttachedEntity() const
{
	return mChildEntity;
}

EmberEntity* OgreAttachment::getParentEntity() const
{
	return &mParentEntity;
}

IEntityAttachment* OgreAttachment::attachEntity(EmberEntity& entity)
{
	if (Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity)) {
		return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, *mSceneNode);
	} else {
		return new OgreAttachment(getAttachedEntity(), entity, *mSceneNode);
	}
}

void OgreAttachment::updateScale()
{
}

void OgreAttachment::entity_Moved()
{
	updatePosition();
	MotionManager& motionManager = MotionManager::getSingleton();
	if (mChildEntity.isMoving())
	{
		motionManager.addMovable(this);
	}
	else
	{
		motionManager.removeMovable(this);
	}
}

void OgreAttachment::updatePosition()
{
	if (mChildEntity.getPredictedPos().isValid()) {
		mSceneNode->setPosition(Convert::toOgre(mChildEntity.getPredictedPos()));
		mSceneNode->setOrientation(Convert::toOgre(mChildEntity.getOrientation()));
	}
}

void OgreAttachment::updateMotion(float timeSlice)
{
	updatePosition();
}


OgreAttachment* OgreAttachment::transferToNewParent(OgreAttachment& newParentAttachment)
{
	OgreAttachment* newAttachment = new OgreAttachment(*this);
	mSceneNode = 0;
	return newAttachment;
}


}
