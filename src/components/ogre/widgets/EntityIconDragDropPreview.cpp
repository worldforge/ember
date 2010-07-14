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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//#ifndef EMBEROGRE_GUIENTITYICONDRAGDROPPREVIEW_H
//
#include "EntityIconDragDropPreview.h"
#include "components/ogre/World.h"

#include "main/Application.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "../Convert.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "QuickHelp.h"

#include "EntityIcon.h"
#include <Eris/TypeInfo.h>
#include <Eris/Avatar.h>
#include "components/ogre/EmberEntity.h"
#include "../authoring/DetachedEntity.h"
#include <Eris/Connection.h>
#include <OgreSceneManager.h>
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include "components/entitymapping/Definitions/CaseDefinition.h"
#include "components/entitymapping/Cases/CaseBase.h"

using namespace Ember;
namespace EmberOgre
{

class EmberEntity;

namespace Gui
{



EntityIconDragDropPreview::EntityIconDragDropPreview() :
		mModelPreviewWorker(0), mIconEntity(0)
{
}

EntityIconDragDropPreview::~EntityIconDragDropPreview()
{
	//everything should be cleaned up, but we'll check anyway
	cleanupCreation();
}

void EntityIconDragDropPreview::createPreview(EntityIcon* icon)
{
	if (!mModelPreviewWorker)
	{
		if (icon && icon->getEntity())
		{
			Gui::QuickHelp::getSingleton().updateText("Drag your item to the world to see a preview of it. You can drop the item by releasing the left mouse button");
			mIconEntity = icon->getEntity();
			mModelPreviewWorker = new ModelPreviewWorker(mIconEntity);
			mModelPreviewWorker->EventCleanupCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::cleanupCreation));
			mModelPreviewWorker->EventFinalizeCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::finalizeCreation));
		}
	}
}

void EntityIconDragDropPreview::cleanupCreation()
{
	if (mModelPreviewWorker)
	{
		mIconEntity = 0;
		delete mModelPreviewWorker;
		mModelPreviewWorker = 0;
	}
}

void EntityIconDragDropPreview::finalizeCreation()
{
	mDropOffset = mModelPreviewWorker->getPosition() - EmberOgre::getSingleton().getWorld()->getAvatar()->getClientSideAvatarPosition();
	EventEntityFinalized.emit(mIconEntity);
	cleanupCreation();
}

WFMath::Vector<3> EntityIconDragDropPreview::getDropOffset()
{
	return mDropOffset;
}

ModelPreviewWorker::ModelPreviewWorker(Eris::Entity* entity) : mModel(0), mEntity(0), mEntityNode(0), mModelMount(0), mMovement(0)
{
	mOrientation.identity();

	Eris::TypeInfo* erisType = entity->getType();

	EmberEntity& avatar = EmberOgre::getSingleton().getWorld()->getAvatar()->getEmberEntity();

	WFMath::Vector<3> offset(2, 0, 0);
	mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

	mEntityMessage = entity->getInstanceAttributes();
	//mEntityMessage["loc"] = avatar.getLocation()->getId();
	//mEntityMessage["name"] = erisType->getName();
	//mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());


	// Temporary entity
	mEntity = new Authoring::DetachedEntity("-1", erisType, EmberOgre::getSingleton().getWorld()->getView().getAvatar()->getConnection()->getTypeService());
	mEntity->setFromMessage(mEntityMessage);

	// Creating scene node
	mEntityNode = EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->createChildSceneNode();

	// Making model from temporary entity
	ModelPreviewWorkerActionCreator actionCreator(*this);
	std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, &actionCreator, 0));
	if (modelMapping.get()) {
		modelMapping->initialize();
	}

	// Registering move adapter to track mouse movements
	mMovement = new ModelPreviewWorkerMovement(*this, EmberOgre::getSingleton().getWorld()->getMainCamera(), *mEntity, mEntityNode);
	//mMoveAdapter->addAdapter();
}

ModelPreviewWorker::~ModelPreviewWorker()
{
	delete mMovement;

	delete mModelMount;

	mEntityNode->detachAllObjects();
	mOrientation = Convert::toWF(mEntityNode->getOrientation());
	EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->removeChild(mEntityNode);
	//	delete mEntityNode;

	EmberOgre::getSingleton().getWorld()->getSceneManager().destroyMovableObject(mModel);

	// Deleting temporary entity
	mEntity->shutdown();
	delete mEntity;
}

void ModelPreviewWorker::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			EmberOgre::getSingleton().getWorld()->getSceneManager().destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(EmberOgre::getSingleton().getWorld()->getSceneManager(), modelName);
	mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelPreviewWorker::model_Reloaded));

	///if the model definition isn't valid, use a placeholder
	if (!mModel->getDefinition()->isValid()) {
		S_LOG_FAILURE( "Could not find " << modelName << ", using placeholder.");
		///add a placeholder model
		Model::ModelDefnPtr modelDef = mModel->getDefinition();
		modelDef->createSubModelDefinition("3d_objects/primitives/models/box.mesh")->createPartDefinition("main")->setShow(true);
		modelDef->setValid(true);
		modelDef->reloadAllInstances();
	}

	mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(*mEntityNode, mModel));

	initFromModel();

	// Setting inital position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

const WFMath::Point<3> ModelPreviewWorker::getPosition()
{
	return Convert::toWF<WFMath::Point<3> >(mEntityNode->getPosition());
}

void ModelPreviewWorker::showModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->showPart(partName);
	}
}

void ModelPreviewWorker::hideModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->hidePart(partName);
	}
}

void ModelPreviewWorker::initFromModel()
{
	scaleNode();
}

void ModelPreviewWorker::model_Reloaded()
{
	initFromModel();
}

void ModelPreviewWorker::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : 0);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

bool ModelPreviewWorker::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & ModelPreviewWorker::getBBox()
{
	return mEntity->getBBox();
}

ModelPreviewWorkerPartAction::ModelPreviewWorkerPartAction(ModelPreviewWorker& modelPreviewWorker, std::string partName)
		: mModelPreviewWorker(modelPreviewWorker), mPartName(partName)
{
}

ModelPreviewWorkerPartAction::~ModelPreviewWorkerPartAction()
{
}

void ModelPreviewWorkerPartAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing creator part " << mPartName);
	mModelPreviewWorker.showModelPart(mPartName);
}

void ModelPreviewWorkerPartAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding creator part " << mPartName);
	mModelPreviewWorker.hideModelPart(mPartName);
}


ModelPreviewWorkerModelAction::ModelPreviewWorkerModelAction(ModelPreviewWorker& modelPreviewWorker, std::string modelName)
		: mModelPreviewWorker(modelPreviewWorker), mModelName(modelName)
{
}

ModelPreviewWorkerModelAction::~ModelPreviewWorkerModelAction()
{
}

void ModelPreviewWorkerModelAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing creator model " << mModelName);
	mModelPreviewWorker.setModel(mModelName);
}

void ModelPreviewWorkerModelAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding creator model " << mModelName);
	mModelPreviewWorker.setModel("");
}

ModelPreviewWorkerHideModelAction::ModelPreviewWorkerHideModelAction(ModelPreviewWorker& modelPreviewWorker)
		: mModelPreviewWorker(modelPreviewWorker)
{
}

ModelPreviewWorkerHideModelAction::~ModelPreviewWorkerHideModelAction()
{
}

void ModelPreviewWorkerHideModelAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	mModelPreviewWorker.setModel("");
}

void ModelPreviewWorkerHideModelAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
}


ModelPreviewWorkerActionCreator::ModelPreviewWorkerActionCreator(ModelPreviewWorker& modelPreviewWorker)
		: mModelPreviewWorker(modelPreviewWorker)
{
}

ModelPreviewWorkerActionCreator::~ModelPreviewWorkerActionCreator()
{
}

void ModelPreviewWorkerActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	EntityMapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (EntityMapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			ModelPreviewWorkerPartAction* action = new ModelPreviewWorkerPartAction(mModelPreviewWorker, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			ModelPreviewWorkerModelAction* action = new ModelPreviewWorkerModelAction(mModelPreviewWorker, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			ModelPreviewWorkerHideModelAction* action = new ModelPreviewWorkerHideModelAction(mModelPreviewWorker);
			aCase->addAction(action);
		}
	}
}

ModelPreviewWorkerMovementBridge::ModelPreviewWorkerMovementBridge(ModelPreviewWorker& modelPreviewWorker, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
	::EmberOgre::Authoring::EntityMoverBase(entity, node, *node->getCreator()), mModelPreviewWorker(modelPreviewWorker)
{
}

ModelPreviewWorkerMovementBridge::~ModelPreviewWorkerMovementBridge()
{
}

void ModelPreviewWorkerMovementBridge::finalizeMovement()
{
	mModelPreviewWorker.EventFinalizeCreation.emit();
}
void ModelPreviewWorkerMovementBridge::cancelMovement()
{
	mModelPreviewWorker.EventCleanupCreation.emit();
}

ModelPreviewWorkerMovement::ModelPreviewWorkerMovement(ModelPreviewWorker& mModelPreviewWorker, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node)
:mMoveAdapter(camera)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
	ModelPreviewWorkerMovementBridge* bridge = new ModelPreviewWorkerMovementBridge(mModelPreviewWorker, entity, node);
	mMoveAdapter.attachToBridge(bridge);
	mMoveAdapter.update();
}

ModelPreviewWorkerMovement::~ModelPreviewWorkerMovement()
{
	mMoveAdapter.detach();
}

}
}
