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

#include "WorldAttachment.h"

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"
#include "components/ogre/OgreAttachment.h"
#include "WorldEmberEntity.h"

#include <OgreSceneNode.h>
#include <OgreVector3.h>

namespace EmberOgre {

WorldAttachment::WorldAttachment(WorldEmberEntity& worldEntity, Ogre::SceneNode& worldNode)
: mWorldEntity(worldEntity), mWorldNode(worldNode)
{
	///set the position to always 0, 0, 0
	mWorldNode.setPosition(Ogre::Vector3(0, 0, 0));

}

WorldAttachment::~WorldAttachment()
{
}

IGraphicalRepresentation* WorldAttachment::getGraphicalRepresentation() const
{
	return 0;
}

EmberEntity& WorldAttachment::getAttachedEntity() const
{
	return mWorldEntity;
}

EmberEntity* WorldAttachment::getParentEntity() const
{
	return 0;
}

IEntityAttachment* WorldAttachment::attachEntity(EmberEntity& entity)
{
	if (Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity)) {
		return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, mWorldNode);
	} else {
		return new OgreAttachment(getAttachedEntity(), entity, mWorldNode);
	}
}

void WorldAttachment::updateScale()
{

}


}

