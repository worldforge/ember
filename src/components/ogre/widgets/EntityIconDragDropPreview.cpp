//
// C++ Implementation: EntityIconDragDropTarget
//
// Description:
//
//
// Author: Tiberiu Paunescu <tpa12@sfu.ca>, (C) 2010
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
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#include "EntityIconDragDropPreview.h"
#include "components/ogre/World.h"

#include "../Avatar.h"
#include "../Convert.h"

#include "QuickHelp.h"

#include "EntityIcon.h"
#include "../authoring/DetachedEntity.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/mapping/ModelActionCreator.h"

#include "domain/EmberEntity.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <framework/Singleton.h>

#include <utility>

using namespace Ember;
namespace Ember {
class EmberEntity;
namespace OgreView {


namespace Gui {

EntityIconDragDropPreview::EntityIconDragDropPreview(World& world) :
        mWorld(world),
        mIconEntity(nullptr) {
}

EntityIconDragDropPreview::~EntityIconDragDropPreview() = default;

void EntityIconDragDropPreview::createPreview(EntityIcon* icon) {
    if (!mModelPreviewWorker) {
        if (icon && icon->getEntity()) {
            mIconEntity = icon->getEntity();
            Gui::HelpMessage message("Entity Drag Preview", "Release the left mouse button to place the entity at the selected location. Press Escape to cancel.", "entity icon drag drop preview", "dragDropMessage");
            Gui::QuickHelp::getSingleton().updateText(message);
            mModelPreviewWorker = std::make_unique<ModelPreviewWorker>(mWorld, mIconEntity);
            mModelPreviewWorker->EventCleanupCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::cleanupCreation));
            mModelPreviewWorker->EventFinalizeCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::finalizeCreation));
        }
    }
}

void EntityIconDragDropPreview::cleanupCreation() {
    mIconEntity = nullptr;
    mModelPreviewWorker.reset();
}

void EntityIconDragDropPreview::finalizeCreation() {
    mDropOffset = mModelPreviewWorker->getPosition() - mWorld.getAvatar()->getClientSideAvatarPosition();
    mDropOrientation = mModelPreviewWorker->getOrientation();
    EventEntityFinalized.emit(mIconEntity);
    cleanupCreation();
}

WFMath::Vector<3> EntityIconDragDropPreview::getDropOffset() const {
    return mDropOffset;
}

WFMath::Quaternion EntityIconDragDropPreview::getDropOrientation() const {
    return mDropOrientation;
}

ModelPreviewWorker::ModelPreviewWorker(World& world, Eris::ViewEntity* entity) :
        mWorld(world),
        mEntity(nullptr),
        mEntityNode(nullptr),
        mModel(nullptr),
        mModelMount(nullptr),
        mMovement(nullptr) {
    mOrientation.identity();

    Eris::TypeInfo* erisType = entity->getType();

    mPos = WFMath::Point<3>::ZERO();

    mEntityMessage = entity->getInstanceProperties();

    // Temporary entity
    mEntity = new Authoring::DetachedEntity("-1", erisType, entity->getView()->getTypeService());
    mEntity->setFromMessage(mEntityMessage);

    // Creating scene node
    mEntityNode = mWorld.getSceneManager().getRootSceneNode()->createChildSceneNode();

    // Making model from temporary entity
    Mapping::ModelActionCreator actionCreator(*entity, [&](std::string modelName) {
        setModel(modelName);
    }, [&](std::string partName) {
        showModelPart(partName);
    });

    std::unique_ptr<EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, actionCreator, nullptr));
    if (modelMapping) {
        modelMapping->initialize();
    }

    // Registering move adapter to track mouse movements
    mMovement = std::make_unique<ModelPreviewWorkerMovement>(*this, mWorld.getMainCamera(), *mEntity, mEntityNode);
    //mMoveAdapter->addAdapter();
}

ModelPreviewWorker::~ModelPreviewWorker() {
    mEntityNode->detachAllObjects();
    mOrientation = Convert::toWF(mEntityNode->getOrientation());
    mWorld.getSceneManager().getRootSceneNode()->removeChild(mEntityNode);
    //	delete mEntityNode;

    // Deleting temporary entity
    mEntity->shutdown();
}

void ModelPreviewWorker::setModel(const std::string& modelName) {
    if (mModel) {
        if (mModel->getDefinition()->getOrigin() == modelName) {
            return;
        }
    }
    auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
    if (!modelDef) {
        modelDef = Model::ModelDefinitionManager::getSingleton().getByName("common/primitives/placeholder.modeldef");
    }
    mModel = std::make_unique<Model::Model>(mWorld.getSceneManager(), modelDef, modelName);
    mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelPreviewWorker::model_Reloaded));
    mModel->load();

    mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(mEntityNode, nullptr, false));
    mModelMount->reset();

    initFromModel();

    // Setting initial position and orientation
    if (mPos.isValid()) {
        mEntityNode->setPosition(Convert::toOgre(mPos));
    }
    mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

const WFMath::Point<3> ModelPreviewWorker::getPosition() const {
    return Convert::toWF<WFMath::Point<3>>(mEntityNode->getPosition());
}

const WFMath::Quaternion ModelPreviewWorker::getOrientation() const {
    return Convert::toWF(mEntityNode->getOrientation());
}

void ModelPreviewWorker::showModelPart(const std::string& partName) {
    if (mModel) {
        mModel->showPart(partName);
    }
}

void ModelPreviewWorker::hideModelPart(const std::string& partName) {
    if (mModel) {
        mModel->hidePart(partName);
    }
}

void ModelPreviewWorker::initFromModel() {
    scaleNode();
}

void ModelPreviewWorker::model_Reloaded() {
    initFromModel();
}

void ModelPreviewWorker::scaleNode() {
    if (mModelMount) {
        mModelMount->rescale(hasBBox() ? &getBBox() : nullptr);
    } else {
        S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
    }
}

bool ModelPreviewWorker::hasBBox() {
    return mEntity->hasBBox();
}

const WFMath::AxisBox<3>& ModelPreviewWorker::getBBox() {
    return mEntity->getBBox();
}

const Authoring::DetachedEntity* ModelPreviewWorker::getEntity() const {
    return mEntity;
}

ModelPreviewWorkerMovementBridge::ModelPreviewWorkerMovementBridge(ModelPreviewWorker& modelPreviewWorker, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
        Authoring::EntityMoverBase(entity, node, *node->getCreator()), mModelPreviewWorker(modelPreviewWorker) {
}

void ModelPreviewWorkerMovementBridge::finalizeMovement() {
    mModelPreviewWorker.EventFinalizeCreation.emit();
}

void ModelPreviewWorkerMovementBridge::cancelMovement() {
    mModelPreviewWorker.EventCleanupCreation.emit();
}

ModelPreviewWorkerMovement::ModelPreviewWorkerMovement(ModelPreviewWorker& mModelPreviewWorker, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
        mMoveAdapter(camera) {
    // When the point is moved, an instance of this will be created and the movement handled by it.
    // Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
    auto bridge = std::make_shared<ModelPreviewWorkerMovementBridge>(mModelPreviewWorker, entity, node);
    mMoveAdapter.attachToBridge(bridge);
    mMoveAdapter.update();
}

ModelPreviewWorkerMovement::~ModelPreviewWorkerMovement() {
    mMoveAdapter.detach();
}

}
}
}
