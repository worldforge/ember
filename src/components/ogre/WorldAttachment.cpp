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

#include "WorldAttachment.h"

#include "domain/EmberEntity.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelAttachment.h"
#include "OgreInfo.h"
#include "EmberEntityActionCreator.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <components/ogre/mapping/EmberEntityMappingManager.h>

namespace Ember {
namespace OgreView {

WorldAttachment::WorldAttachment(EmberEntity& worldEntity, Ogre::SceneNode* worldNode, Scene& scene) :
        mWorldEntity(worldEntity),
        mWorldNode(worldNode),
        mScene(scene) {
    //set the position to always 0, 0, 0
    mWorldNode->setPosition(Ogre::Vector3(0, 0, 0));
}

WorldAttachment::~WorldAttachment() {
    mWorldNode->getCreator()->destroySceneNode(mWorldNode);
}

IGraphicalRepresentation* WorldAttachment::getGraphicalRepresentation() const {
    return nullptr;
}

EmberEntity& WorldAttachment::getAttachedEntity() const {
    return mWorldEntity;
}

EmberEntity* WorldAttachment::getParentEntity() const {
    return nullptr;
}

std::unique_ptr<IEntityAttachment> WorldAttachment::attachEntity(EmberEntity& entity) {

    //the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
    EmberEntityActionCreator creator(entity, mScene);
    auto mapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, creator, entity.getView());
    if (mapping) {
        mapping->initialize();
        std::shared_ptr<EntityMapping::EntityMapping> sharedMapping(std::move(mapping));
        //Retain the mapping while the signal exists.
        entity.BeingDeleted.connect([sharedMapping]() {});
    }


    std::unique_ptr<NodeAttachment> nodeAttachment;
    Ogre::SceneNode* node = mWorldNode->createChildSceneNode(OgreInfo::createUniqueResourceName(entity.getId()));
    if (Model::ModelRepresentation* modelRepresentation = Model::ModelRepresentation::getRepresentationForEntity(entity)) {
        nodeAttachment = std::make_unique<Model::ModelAttachment>(getAttachedEntity(), *modelRepresentation, new SceneNodeProvider(node, mWorldNode));
    } else {
        nodeAttachment = std::make_unique<NodeAttachment>(getAttachedEntity(), entity, new SceneNodeProvider(node, mWorldNode));
    }
    nodeAttachment->init();
    return nodeAttachment;
}

void WorldAttachment::updateScale() {
}

void WorldAttachment::updatePosition() {
}

void WorldAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) {
    //No offset
}

void WorldAttachment::setControlDelegate(IEntityControlDelegate* controllerDelegate) {
    //You can never control the world.
}

IEntityControlDelegate* WorldAttachment::getControlDelegate() const {
    return nullptr;
}

void WorldAttachment::setVisualize(const std::string& visualization, bool visualize) {
    if (visualization == "OgreBBox") {
        mWorldNode->showBoundingBox(visualize);
    }
}

bool WorldAttachment::getVisualize(const std::string& visualization) const {
    if (visualization == "OgreBBox") {
        return mWorldNode->getShowBoundingBox();
    }
    return false;
}

}
}

