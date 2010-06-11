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
		mModelPreviewBase(0), mIconEntity(0), mActiveIcon(false)
{
}

EntityIconDragDropPreview::~EntityIconDragDropPreview()
{
	//everything should be cleaned up, but we'll check anyway
	cleanupCreation();
}

void EntityIconDragDropPreview::createPreview(EntityIcon* icon)
{
	if (!mActiveIcon)
	{
		if (icon && icon->getEntity())
		{
			mIconEntity = icon->getEntity();
			mModelPreviewBase = new ModelPreviewBase(mIconEntity);
			mModelPreviewBase->EventCleanupCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::cleanupCreation));
			mModelPreviewBase->EventFinalizeCreation.connect(sigc::mem_fun(*this, &EntityIconDragDropPreview::finalizeCreation));
			mActiveIcon = true;
		}
	}
}

void EntityIconDragDropPreview::cleanupCreation()
{
	if (mActiveIcon)
	{
		mIconEntity = 0;
		delete mModelPreviewBase;
		mActiveIcon = false;
	}
}

void EntityIconDragDropPreview::finalizeCreation()
{
	mDropOffset = mModelPreviewBase->getPosition() - EmberOgre::getSingleton().getWorld()->getAvatar()->getClientSideAvatarPosition();
	EventEntityFinalized.emit(mIconEntity);
	cleanupCreation();
}

WFMath::Vector<3> EntityIconDragDropPreview::getDropOffset()
{
	return mDropOffset;
}

ModelPreviewBase::ModelPreviewBase(Eris::Entity* entity) : mModel(0), mEntity(0), mEntityNode(0), mModelMount(0), mMovement(0)
{
	mOrientation.identity();

	Eris::TypeInfo* erisType = entity->getType();

	EmberEntity& avatar = EmberOgre::getSingleton().getWorld()->getAvatar()->getEmberEntity();

	WFMath::Vector<3> offset(2, 0, 0);
	mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

	mEntityMessage = entity->getAttributes();
	mEntityMessage["loc"] = avatar.getLocation()->getId();
	mEntityMessage["name"] = erisType->getName();
	mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());


	// Temporary entity
	mEntity = new Authoring::DetachedEntity("-1", erisType, EmberOgre::getSingleton().getWorld()->getView().getAvatar()->getConnection()->getTypeService());
	mEntity->setFromMessage(mEntityMessage);

	// Creating scene node
	mEntityNode = EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->createChildSceneNode();

	// Making model from temporary entity
	ModelPreviewBaseActionCreator actionCreator(*this);
	std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, &actionCreator, 0));
	if (modelMapping.get()) {
		modelMapping->initialize();
	}

	// Registering move adapter to track mouse movements
	mMovement = new ModelPreviewBaseMovement(*this, EmberOgre::getSingleton().getWorld()->getMainCamera(), *mEntity, mEntityNode);
	//mMoveAdapter->addAdapter();
}

ModelPreviewBase::~ModelPreviewBase()
{
	delete mMovement;
	mMovement = 0;

	delete mModelMount;
	mModelMount = 0;

	mEntityNode->detachAllObjects();
	mOrientation = Convert::toWF(mEntityNode->getOrientation());
	EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->removeChild(mEntityNode);
	//	delete mEntityNode;
	mEntityNode = 0;

	EmberOgre::getSingleton().getWorld()->getSceneManager().destroyMovableObject(mModel);
	mModel = 0;

	// Deleting temporary entity
	mEntity->shutdown();
	delete mEntity;
}

void ModelPreviewBase::setModel(const std::string& modelName)
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
	mModel->Reloaded.connect(sigc::mem_fun(*this, &ModelPreviewBase::model_Reloaded));

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

WFMath::Point<3> ModelPreviewBase::getPosition()
{
	return Convert::toWF<WFMath::Point<3> >(mEntityNode->getPosition());
}

void ModelPreviewBase::showModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->showPart(partName);
	}
}

void ModelPreviewBase::hideModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->hidePart(partName);
	}
}

void ModelPreviewBase::initFromModel()
{
	scaleNode();
}

void ModelPreviewBase::model_Reloaded()
{
	initFromModel();
}

void ModelPreviewBase::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : 0);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

bool ModelPreviewBase::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & ModelPreviewBase::getBBox()
{
	return mEntity->getBBox();
}

ModelPreviewBasePartAction::ModelPreviewBasePartAction(ModelPreviewBase& modelPreviewBase, std::string partName)
		: mModelPreviewBase(modelPreviewBase), mPartName(partName)
{
}

ModelPreviewBasePartAction::~ModelPreviewBasePartAction()
{
}

void ModelPreviewBasePartAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing creator part " << mPartName);
	mModelPreviewBase.showModelPart(mPartName);
}

void ModelPreviewBasePartAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding creator part " << mPartName);
	mModelPreviewBase.hideModelPart(mPartName);
}


ModelPreviewBaseModelAction::ModelPreviewBaseModelAction(ModelPreviewBase& modelPreviewBase, std::string modelName)
		: mModelPreviewBase(modelPreviewBase), mModelName(modelName)
{
}

ModelPreviewBaseModelAction::~ModelPreviewBaseModelAction()
{
}

void ModelPreviewBaseModelAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Showing creator model " << mModelName);
	mModelPreviewBase.setModel(mModelName);
}

void ModelPreviewBaseModelAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
	S_LOG_VERBOSE("Hiding creator model " << mModelName);
	mModelPreviewBase.setModel("");
}

ModelPreviewBaseHideModelAction::ModelPreviewBaseHideModelAction(ModelPreviewBase& modelPreviewBase)
		: mModelPreviewBase(modelPreviewBase)
{
}

ModelPreviewBaseHideModelAction::~ModelPreviewBaseHideModelAction()
{
}

void ModelPreviewBaseHideModelAction::activate(Ember::EntityMapping::ChangeContext& context)
{
	mModelPreviewBase.setModel("");
}

void ModelPreviewBaseHideModelAction::deactivate(Ember::EntityMapping::ChangeContext& context)
{
}


ModelPreviewBaseActionCreator::ModelPreviewBaseActionCreator(ModelPreviewBase& modelPreviewBase)
		: mModelPreviewBase(modelPreviewBase)
{
}

ModelPreviewBaseActionCreator::~ModelPreviewBaseActionCreator()
{
}

void ModelPreviewBaseActionCreator::createActions(EntityMapping::EntityMapping& modelMapping, EntityMapping::Cases::CaseBase* aCase, EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	EntityMapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (EntityMapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-part") {
			ModelPreviewBasePartAction* action = new ModelPreviewBasePartAction(mModelPreviewBase, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "display-model") {
			ModelPreviewBaseModelAction* action = new ModelPreviewBaseModelAction(mModelPreviewBase, J->getValue());
			aCase->addAction(action);
		} else if (J->getType() == "hide-model") {
			ModelPreviewBaseHideModelAction* action = new ModelPreviewBaseHideModelAction(mModelPreviewBase);
			aCase->addAction(action);
		}
	}
}

ModelPreviewBaseMovementBridge::ModelPreviewBaseMovementBridge(ModelPreviewBase& modelPreviewBase, Authoring::DetachedEntity& entity, Ogre::SceneNode* node) :
	::EmberOgre::Authoring::EntityMoverBase(entity, node, *node->getCreator()), mModelPreviewBase(modelPreviewBase)
{
}

ModelPreviewBaseMovementBridge::~ModelPreviewBaseMovementBridge()
{
}

void ModelPreviewBaseMovementBridge::finalizeMovement()
{
	mModelPreviewBase.EventFinalizeCreation.emit();
}
void ModelPreviewBaseMovementBridge::cancelMovement()
{
	mModelPreviewBase.EventCleanupCreation.emit();
}

ModelPreviewBaseMovement::ModelPreviewBaseMovement(ModelPreviewBase& mModelPreviewBase, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node)
:mMoveAdapter(camera)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't keep a reference
	ModelPreviewBaseMovementBridge* bridge = new ModelPreviewBaseMovementBridge(mModelPreviewBase, entity, node);
	mMoveAdapter.attachToBridge(bridge);
	mMoveAdapter.update();
}

ModelPreviewBaseMovement::~ModelPreviewBaseMovement()
{
}

}
}
