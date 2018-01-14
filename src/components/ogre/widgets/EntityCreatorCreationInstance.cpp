/*
 Copyright (C) 2011 Erik Ogenvik <erik@ogenvik.org>

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreatorCreationInstance.h"

#include "EntityCreatorMovement.h"
#include "EntityCreatorActionCreator.h"
#include "AtlasHelper.h"

#include "components/ogre/Avatar.h"
#include "domain/EmberEntity.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/World.h"
#include "components/ogre/Convert.h"
#include "components/ogre/OgreInfo.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/ogre/model/ModelMount.h"

#include "components/ogre/authoring/EntityRecipe.h"
#include "components/ogre/authoring/DetachedEntity.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"

#include <wfmath/atlasconv.h>
#include <wfmath/MersenneTwister.h>

#include <Eris/Avatar.h>
#include <Eris/Connection.h>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

EntityCreatorCreationInstance::EntityCreatorCreationInstance(World& world, Eris::TypeService& typeService, Authoring::EntityRecipe& recipe, bool randomizeOrientation, sigc::slot<void>& adapterValueChangedSlot) :
		mWorld(world), mTypeService(typeService), mRecipe(recipe), mEntity(0), mEntityNode(0), mModelMount(0), mModel(0), mMovement(0), mAxisMarker(0)
{
	mConnection = mRecipe.EventValueChanged.connect(adapterValueChangedSlot);

	mInitialOrientation.identity();
	if (randomizeOrientation) {
		WFMath::MTRand rng;
		mInitialOrientation.rotation(1, rng.rand() * 360.0f);
	}

}

EntityCreatorCreationInstance::~EntityCreatorCreationInstance()
{
	delete mMovement;
	// 	mInputAdapter->removeAdapter();

	delete mModelMount;

	mEntityNode->detachAllObjects();


	delete mModel;

	mWorld.getSceneManager().destroyMovableObject(mAxisMarker);

	// Deleting temporary entity
	mEntity->shutdown();
	delete mEntity;

	mWorld.getSceneManager().destroySceneNode(mEntityNode);

	mConnection.disconnect();
}

void EntityCreatorCreationInstance::startCreation()
{

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	// Making initial position (orientation is preserved)
	WFMath::Vector<3> offset(0, 0, -2);

	mPos = (avatar.getPosition().isValid() ? avatar.getPosition() : WFMath::Point<3>::ZERO()) + (avatar.getOrientation().isValid() ? offset.rotate(avatar.getOrientation()) : WFMath::Vector<3>::ZERO());

	createEntity();
}

void EntityCreatorCreationInstance::createEntity()
{
	// Creating entity data
	mEntityMessage = mRecipe.createEntity(mTypeService);
	Eris::TypeInfo* erisType = mTypeService.getTypeByName(mRecipe.getEntityType());
	if (!erisType) {
		S_LOG_FAILURE("Type " << mRecipe.getEntityType() << " not found in recipe " << mRecipe.getName());
		return;
	}

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	mEntityMessage["loc"] = avatar.getLocation()->getId();
	mEntityMessage["parent"] = erisType->getName();

	// Temporary entity
	mEntity = new Authoring::DetachedEntity("-1", erisType, &mTypeService);
	Atlas::Objects::Entity::RootEntity rootEntity;
	mEntity->doInit(rootEntity);
	mEntity->setFromMessage(mEntityMessage);

	// Creating scene node
	mEntityNode = mWorld.getSceneManager().getRootSceneNode()->createChildSceneNode();

	if (!mAxisMarker) {
		try {
			mAxisMarker = mEntityNode->getCreator()->createEntity(OgreInfo::createUniqueResourceName("EntityCreator_axisMarker"), "common/primitives/model/axes.mesh");
			mEntityNode->attachObject(mAxisMarker);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when loading axes mesh." << ex);
		}
	}

	// Making model from temporary entity
	EntityCreatorActionCreator actionCreator(*this);
	std::unique_ptr<EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, actionCreator, 0));
	if (modelMapping) {
		modelMapping->initialize();
	}

	// Registering move adapter to track mouse movements
	// 		mInputAdapter->addAdapter();
	mMovement = new EntityCreatorMovement(*this, mWorld.getMainCamera(), *mEntity, mEntityNode);

}

void EntityCreatorCreationInstance::finalizeCreation()
{
	// Final position
	mEntityMessage["pos"] = Convert::toWF<WFMath::Point<3>>(mEntityNode->getPosition()).toAtlas();
	mEntityMessage["orientation"] = Convert::toWF(mEntityNode->getOrientation()).toAtlas();

	// Making create operation message
	Atlas::Objects::Operation::Create c;
	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();
	c->setFrom(avatar.getId());
	//if the avatar is a "creator", i.e. and admin, we will set the TO property
	//this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (avatar.getType()->isA(mTypeService.getTypeByName("creator"))) {
		c->setTo(avatar.getId());
	}

	c->setArgsAsList(Atlas::Message::ListType(1, mEntityMessage));
	mWorld.getView().getAvatar()->getConnection()->send(c);

	std::stringstream ss;
	ss << mPos;
	S_LOG_INFO("Trying to create entity at position " << ss.str());
	S_LOG_VERBOSE("Sending entity data to server: " << AtlasHelper::serialize(c, "xml"));

}

void EntityCreatorCreationInstance::setModel(const std::string& modelName)
{
	if (mModel) {
		if (mModel->getDefinition()->getName() == modelName) {
			return;
		} else {
			//Reset the model mount to start with.
			delete mModelMount;
			mModelMount = 0;
			delete mModel;
		}
	}
	auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	if (!modelDef) {
		modelDef = Model::ModelDefinitionManager::getSingleton().getByName("placeholder");
	}
	mModel = new Model::Model(mWorld.getSceneManager(), modelDef, modelName);
	mModel->Reloaded.connect(sigc::mem_fun(*this, &EntityCreatorCreationInstance::model_Reloaded));
	mModel->load();

//	//if the model definition isn't valid, use a placeholder
//	if (!mModel->getDefinition()->isValid()) {
//		S_LOG_FAILURE("Could not find " << modelName << ", using placeholder.");
//		//add a placeholder model
//		Model::ModelDefinitionPtr modelDef = mModel->getDefinition();
//		modelDef->createSubModelDefinition("common/primitives/model/box.mesh")->createPartDefinition("main")->setShow(true);
//		modelDef->setValid(true);
//		modelDef->reloadAllInstances();
//	}

	Ogre::SceneNode* node = mEntityNode->createChildSceneNode(OgreInfo::createUniqueResourceName(mRecipe.getName()));
	mModelMount = new Model::ModelMount(*mModel, new SceneNodeProvider(node, mEntityNode));
	mModelMount->reset();

	initFromModel();

	// Setting inital position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mInitialOrientation));
}

void EntityCreatorCreationInstance::showModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->showPart(partName);
	}
}

void EntityCreatorCreationInstance::hideModelPart(const std::string& partName)
{
	if (mModel) {
		mModel->hidePart(partName);
	}
}

Model::Model* EntityCreatorCreationInstance::getModel()
{
	return mModel;
}

bool EntityCreatorCreationInstance::hasBBox() const
{
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3> & EntityCreatorCreationInstance::getBBox() const
{
	return mEntity->getBBox();
}

void EntityCreatorCreationInstance::initFromModel()
{
	scaleNode();
}

void EntityCreatorCreationInstance::model_Reloaded()
{
	initFromModel();
}

void EntityCreatorCreationInstance::scaleNode()
{
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : nullptr);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

WFMath::Quaternion EntityCreatorCreationInstance::getOrientation() const
{
	return Convert::toWF(mEntityNode->getOrientation());
}

void EntityCreatorCreationInstance::setOrientation(const WFMath::Quaternion& orientation)
{
	if (orientation.isValid()) {
		if (mEntityNode) {
			mEntityNode->setOrientation(Convert::toOgre(orientation));
		} else {
			mInitialOrientation = orientation;
		}
	}
}

}
}
}
