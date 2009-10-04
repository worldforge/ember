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
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelAttachment.h"
#include "components/ogre/terrain/TerrainGenerator.h"

#include <OgreSceneNode.h>
#include <OgreVector3.h>

#include <wfmath/vector.h>
#include <wfmath/point.h>

namespace EmberOgre
{

WorldAttachment::WorldAttachment(WorldEmberEntity& worldEntity, Ogre::SceneNode& worldNode, Terrain::TerrainGenerator& terrainGenerator) :
	mWorldEntity(worldEntity), mWorldNode(worldNode), mTerrainGenerator(terrainGenerator)
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
	if (Model::ModelRepresentation * modelRepresentation = Model::ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity)) {
		return new Model::ModelAttachment(getAttachedEntity(), *modelRepresentation, new SceneNodeProvider(mWorldNode));
	}
	else {
		return new NodeAttachment(getAttachedEntity(), entity, new SceneNodeProvider(mWorldNode));
	}
}

void WorldAttachment::updateScale()
{
}

void WorldAttachment::updatePosition()
{
}

void WorldAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset)
{
	float height = 0;
	if (mTerrainGenerator.getHeight(WFMath::Point<2>(localPosition.x(), localPosition.y()), height)) {
		offset.z() = height - localPosition.z();
	}
}

void WorldAttachment::setControlDelegate(IEntityControlDelegate* controllerDelegate)
{
	//You can never control the world.
}

IEntityControlDelegate* WorldAttachment::getControlDelegate() const
{
	return 0;
}

void WorldAttachment::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "OgreBBox") {
		mWorldNode.showBoundingBox(visualize);
	}
}

bool WorldAttachment::getVisualize(const std::string& visualization) const
{
	if (visualization == "OgreBBox") {
		return mWorldNode.getShowBoundingBox();
	}
	return false;
}

}

