//
// C++ Implementation: EntityCreator
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreator.h"

#include "EntityCreatorMovement.h"
#include "EntityCreatorActionCreator.h"
#include "components/ogre/World.h"
#include "components/ogre/authoring/DetachedEntity.h"

#include "../GUIManager.h"
#include "components/ogre/widgets/adapters/atlas/AdapterFactory.h"

#include "../authoring/DetachedEntity.h"
#include "../Convert.h"

#include "../Avatar.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/SceneNodeProvider.h"

#include "QuickHelp.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/model/ModelMount.h"
#include "components/entitymapping/EntityMappingManager.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/IActionCreator.h"

#include <Atlas/Message/Element.h>
#include <wfmath/atlasconv.h>
#include <wfmath/MersenneTwister.h>
#include <Eris/TypeInfo.h>
#include <Eris/Connection.h>
#include <Eris/Avatar.h>

#include <CEGUIWindow.h>
#include <OgreRoot.h>
#include <wfmath/axisbox.h>

using namespace Ember;
namespace EmberOgre
{

namespace Gui
{

EntityCreator::EntityCreator(World& world) :
	mWorld(world), mTypeService(*mWorld.getView().getAvatar()->getConnection()->getTypeService()), mCreateMode(false), mRecipe(0), mModelMount(0), mModel(0), mRandomizeOrientation(true), mMovement(0), mAxisMarker(0)
{
	mTypeService.BoundType.connect(sigc::mem_fun(*this, &EntityCreator::typeService_BoundType));

	mMoveAdapter = new EntityCreatorMoveAdapter(*this);

	mOrientation.identity();
}

EntityCreator::~EntityCreator()
{
	delete mMoveAdapter;
	delete mModelMount;
}

void EntityCreator::setRecipe(Authoring::EntityRecipe& recipe)
{
	mRecipe = &recipe;
	checkTypeInfoBound();
}

void EntityCreator::toggleCreateMode()
{
	if (!mCreateMode) {
		startCreation();
	} else {
		stopCreation();
	}
}

void EntityCreator::setRandomizeOrientation(bool randomize)
{
	mRandomizeOrientation = randomize;
}

void EntityCreator::startCreation()
{
	loadAllTypes();
	// No recipe selected, nothing to do
	if (!mRecipe) {
		return;
	}

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	// Making initial position (orientation is preserved)
	WFMath::Vector<3> offset(2, 0, 0);

	mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

	mRecipeConnection = mRecipe->EventValueChanged.connect(sigc::mem_fun(*this, &EntityCreator::adapterValueChanged));

	mWidget->getMainWindow()->setAlpha(0.6);

	Gui::HelpMessage message("EntityCreator", "Click the left mouse button to place the entity. Press Escape to exit from CREATE mode.", "entity creator placement", "entityCreatorMessage");
	Gui::QuickHelp::getSingleton().updateText(message);

	createEntity();
}

void EntityCreator::loadAllTypes()
{
	Eris::TypeInfo* typeInfo = mTypeService.getTypeByName("game_entity");
	if (typeInfo) {
		if (typeInfo->hasUnresolvedChildren()) {
			typeInfo->resolveChildren();
		}
	}
}

void EntityCreator::stopCreation()
{
	mRecipeConnection.disconnect();

	cleanupCreation();

	mWidget->getMainWindow()->setAlpha(1.0);
}

void EntityCreator::createEntity()
{
	// Creating entity data
	mEntityMessage = mRecipe->createEntity(mTypeService);
	Eris::TypeInfo* erisType = mTypeService.getTypeByName(mRecipe->getEntityType());
	if (!erisType) {
		S_LOG_FAILURE("Type " << mRecipe->getEntityType() << " not found in recipe " << mRecipe->getName());
		return;
	}

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	/*
	 if (mName->getText().length() > 0) {
	 msg["name"] = mName->getText().c_str();
	 } else {
	 msg["name"] = typeinfo->getName();
	 }
	 */
	mEntityMessage["loc"] = avatar.getLocation()->getId();
	mEntityMessage["name"] = erisType->getName();
	mEntityMessage["parents"] = Atlas::Message::ListType(1, erisType->getName());

	// Temporary entity
	mEntity = new Authoring::DetachedEntity("-1", erisType, &mTypeService);
	mEntity->setFromMessage(mEntityMessage);

	// Creating scene node
	mEntityNode = mWorld.getSceneManager().getRootSceneNode()->createChildSceneNode();

	if (!mAxisMarker) {
		try {
			mAxisMarker = mEntityNode->getCreator()->createEntity("EntityCreator_axisMarker", "axes.mesh");
			mEntityNode->attachObject(mAxisMarker);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when loading axes mesh."<< ex);
		}
	}

	// Making model from temporary entity
	EntityCreatorActionCreator actionCreator(*this);
	std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, &actionCreator, 0));
	if (modelMapping.get()) {
		modelMapping->initialize();
	}

	// Registering move adapter to track mouse movements
	// 		mInputAdapter->addAdapter();
	mMovement = new EntityCreatorMovement(*this, mWorld.getMainCamera(), *mEntity, mEntityNode);
	mMoveAdapter->addAdapter();

	mCreateMode = true;
}

void EntityCreator::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			///Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			mModel->_getManager()->destroyMovableObject(mModel);
		}
	}
	mModel = Model::Model::createModel(mWorld.getSceneManager(), modelName);
	mModel->Reloaded.connect(sigc::mem_fun(*this, &EntityCreator::model_Reloaded));

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
	if (mRandomizeOrientation) {
		WFMath::MTRand rng;
		mOrientation.rotation(2, rng.rand() * 360.0f);
	}
	mEntityNode->setOrientation(Convert::toOgre(mOrientation));
}

void EntityCreator::showModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->showPart(partName);
	}
}

void EntityCreator::hideModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->hidePart(partName);
	}
}

Model::Model* EntityCreator::getModel()
{
	return mModel;
}

bool EntityCreator::hasBBox()
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & EntityCreator::getBBox()
{
	return mEntity->getBBox();
}

void EntityCreator::initFromModel()
{
	scaleNode();
}

void EntityCreator::model_Reloaded()
{
	initFromModel();
}

void EntityCreator::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : 0);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

void EntityCreator::adapterValueChanged()
{
	// Recreating the entity.
	cleanupCreation();
	createEntity();
}

void EntityCreator::finalizeCreation()
{
	// Final position
	mEntityMessage["pos"] = Convert::toWF<WFMath::Point<3> >(mEntityNode->getPosition()).toAtlas();
	mEntityMessage["orientation"] = Convert::toWF(mEntityNode->getOrientation()).toAtlas();

	// Making create operation message
	Atlas::Objects::Operation::Create c;
	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();
	c->setFrom(avatar.getId());
	///if the avatar is a "creator", i.e. and admin, we will set the TO property
	///this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (avatar.getType()->isA(mTypeService.getTypeByName("creator"))) {
		c->setTo(avatar.getId());
	}

	c->setArgsAsList(Atlas::Message::ListType(1, mEntityMessage));
	mWorld.getView().getAvatar()->getConnection()->send(c);

	std::stringstream ss;
	ss << mPos;
	S_LOG_INFO("Trying to create entity at position " << ss.str() );

	// Recreating the entity to allow change in random attributes.
	cleanupCreation();
	createEntity();
}

void EntityCreator::cleanupCreation()
{
	delete mMovement;
	mMovement = 0;
	// 	mInputAdapter->removeAdapter();
	mMoveAdapter->removeAdapter();

	delete mModelMount;
	mModelMount = 0;

	mEntityNode->detachAllObjects();
	mOrientation = Convert::toWF(mEntityNode->getOrientation());
	mWorld.getSceneManager().getRootSceneNode()->removeChild(mEntityNode);
	//	delete mEntityNode;
	mEntityNode = 0;

	mWorld.getSceneManager().destroyMovableObject(mModel);
	mModel = 0;

	mCreateMode = false;

	// Deleting temporary entity
	mEntity->shutdown();
	delete mEntity;
}

void EntityCreator::checkTypeInfoBound()
{
	if (mRecipe) {
		const std::string& typeName = mRecipe->getEntityType();
		///Calling getTypeByName will also send a request for type info to the server if no type info exists yet
		Eris::TypeInfo* typeInfo = mTypeService.getTypeByName(typeName);
		if (typeInfo) {
			if (typeInfo->isBound()) {
				EventTypeInfoLoaded.emit();
			}
		}
	}
}

void EntityCreator::typeService_BoundType(Eris::TypeInfo* typeInfo)
{
	if (mRecipe) {
		if (typeInfo->getName() == mRecipe->getEntityType()) {
			EventTypeInfoLoaded.emit();
		}
	}
}

EntityCreatorMoveAdapter::EntityCreatorMoveAdapter(EntityCreator& entityCreator) :
	mEntityCreator(entityCreator)
{
}

EntityCreatorMoveAdapter::~EntityCreatorMoveAdapter()
{
}

void EntityCreatorMoveAdapter::addAdapter()
{
	/// Register this as a frame listener
	Ogre::Root::getSingleton().addFrameListener(this);
}

void EntityCreatorMoveAdapter::removeAdapter()
{
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool EntityCreatorMoveAdapter::frameStarted(const Ogre::FrameEvent& event)
{
	return true;
}

}
}
