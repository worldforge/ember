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
#include "EntityCreatorMovementBridge.h"
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

#include "components/ogre/authoring/DetachedEntity.h"

#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/mapping/ModelActionCreator.h"

#include <wfmath/atlasconv.h>
#include <wfmath/MersenneTwister.h>

#include <Eris/Avatar.h>
#include <Eris/Connection.h>
#include <Eris/TypeService.h>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

namespace Ember {
namespace OgreView {
namespace Gui {

EntityCreatorCreationInstance::EntityCreatorCreationInstance(World& world,
															 Eris::TypeService& typeService,
															 std::map<std::string, Atlas::Message::Element> entityMap,
															 bool randomizeOrientation) :
		mWorld(world),
		mTypeService(typeService),
		mEntityNode(nullptr),
		mEntityMessage(std::move(entityMap)),
		mPlantedOnGround(true),
		mAxisMarker(nullptr) {

	mInitialOrientation.identity();
	if (randomizeOrientation) {
		WFMath::MTRand rng;
		mInitialOrientation.rotation(1, rng.rand<float>() * 360.0f);
	}

}

EntityCreatorCreationInstance::~EntityCreatorCreationInstance() {
	if (mEntityNode) {
		mEntityNode->detachAllObjects();
	}

	if (mAxisMarker) {
		mWorld.getSceneManager().destroyMovableObject(mAxisMarker);
	}

	if (mEntityNode) {
		mWorld.getSceneManager().destroySceneNode(mEntityNode);
	}

	mConnection.disconnect();
}

void EntityCreatorCreationInstance::startCreation() {

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	// Making initial position (orientation is preserved)
	WFMath::Vector<3> offset(0, 0, -2);

	if (avatar.getPosition().isValid()) {
		mPos = avatar.getPosition();
	} else {
		mPos = WFMath::Point<3>::ZERO();
	}
	if (avatar.getOrientation().isValid()) {
		mPos += offset.rotate(avatar.getOrientation());
	}

	createEntity();
}

void EntityCreatorCreationInstance::createEntity() {
	// Creating entity data
	auto parentI = mEntityMessage.find("parent");
	if (parentI == mEntityMessage.end() || !parentI->second.isString()) {
		S_LOG_FAILURE("No parent found.");
		return;
	}

	Eris::TypeInfo* erisType = mTypeService.getTypeByName(parentI->second.String());
	if (!erisType) {
		S_LOG_FAILURE("Type " << parentI->second.String() << " not found.");
		return;
	}

	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	mEntityMessage["loc"] = avatar.getLocation()->getId();
	//mEntityMessage["parent"] = erisType->getName();

	// Temporary entity
	mEntity = std::make_unique<Authoring::DetachedEntity>("-1", erisType);
	Atlas::Objects::Entity::RootEntity rootEntity;
	mEntity->doInit(rootEntity);
	mEntity->setFromMessage(mEntityMessage);
	//Set the parent to the top level by default.
	mEntity->setLocationEntity(mWorld.getView().getTopLevel());

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
	Mapping::ModelActionCreator actionCreator(*mEntity, [&](const std::string& modelName) {
		setModel(modelName);
	}, [&](const std::string& partName) {
		showModelPart(partName);
	});

	std::unique_ptr<EntityMapping::EntityMapping> modelMapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*mEntity, actionCreator, mWorld.getView().getTypeService(), &mWorld.getView());
	if (modelMapping) {
		modelMapping->initialize();
	}

	// Registering move adapter to track mouse movements
	// 		mInputAdapter->addAdapter();
	mMovement = std::make_unique<EntityCreatorMovement>(*this, mWorld.getMainCamera(), *mEntity, mEntityNode);

}

void EntityCreatorCreationInstance::finalizeCreation() {

	// Final position

	auto pos = mMovement->getBridge()->getPosition();
	mEntityMessage["orientation"] = mMovement->getBridge()->getOrientation().toAtlas();
	mEntityMessage["pos"] = pos.toAtlas();
	if (mPlantedOnGround && mEntity->getLocation()) {
		mEntityMessage["mode"] = "planted";
		mEntityMessage["mode_data"] = Atlas::Message::MapType{{"mode", "planted"},
															  {"$eid", mEntity->getLocation()->getId()}};
	}


	// Making create operation message
	Atlas::Objects::Operation::Create c;
	c->setFrom(mWorld.getAvatar()->getId());
	//if the avatar is a "creator", i.e. and admin, we will set the TO property
	//this will bypass all of the server's filtering, allowing us to create any entity and have it have a working mind too
	if (mWorld.getAvatar()->isAdmin()) {
		c->setTo(mWorld.getAvatar()->getEmberEntity().getId());
	}

	c->setArgsAsList(Atlas::Message::ListType(1, mEntityMessage), &mWorld.getView().getAvatar().getConnection().getFactories());
	mWorld.getView().getAvatar().getConnection().send(c);

	std::stringstream ss;
	ss << mPos;
	S_LOG_INFO("Trying to create entity at position " << ss.str());
	S_LOG_VERBOSE("Sending entity data to server: " << AtlasHelper::serialize(c, "xml"));

}

void EntityCreatorCreationInstance::setModel(const std::string& modelName) {
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

	Ogre::SceneNode* node = mEntityNode->createChildSceneNode();
	mModelMount = std::make_unique<Model::ModelMount>(*mModel, std::make_unique<SceneNodeProvider>(node, mEntityNode));
	mModelMount->reset();

	initFromModel();

	// Setting initial position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mInitialOrientation));
}

void EntityCreatorCreationInstance::showModelPart(const std::string& partName) {
	if (mModel) {
		mModel->showPart(partName);
	}
}

void EntityCreatorCreationInstance::hideModelPart(const std::string& partName) {
	if (mModel) {
		mModel->hidePart(partName);
	}
}

Model::Model* EntityCreatorCreationInstance::getModel() {
	return mModel.get();
}

bool EntityCreatorCreationInstance::hasBBox() const {
	return mEntity->hasBBox();
}

const WFMath::AxisBox<3>& EntityCreatorCreationInstance::getBBox() const {
	return mEntity->getBBox();
}

void EntityCreatorCreationInstance::initFromModel() {
	scaleNode();
}

void EntityCreatorCreationInstance::model_Reloaded() {
	initFromModel();
}

void EntityCreatorCreationInstance::scaleNode() {
	if (mModelMount) {
		mModelMount->rescale(hasBBox() ? &getBBox() : nullptr);
	} else {
		S_LOG_WARNING("Tried to scale node without there being a valid model mount.");
	}
}

WFMath::Quaternion EntityCreatorCreationInstance::getOrientation() const {
	if (mEntityNode) {
		return Convert::toWF(mEntityNode->getOrientation());
	} else {
		return WFMath::Quaternion::IDENTITY();
	}
}

void EntityCreatorCreationInstance::setOrientation(const WFMath::Quaternion& orientation) {
	if (orientation.isValid()) {
		if (mEntityNode) {
			mEntityNode->setOrientation(Convert::toOgre(orientation));
		} else {
			mInitialOrientation = orientation;
		}
	}
}

const Authoring::DetachedEntity* EntityCreatorCreationInstance::getEntity() const {
	return mEntity.get();
}

EntityCreatorMovement* EntityCreatorCreationInstance::getMovement() {
	return mMovement.get();
}

void EntityCreatorCreationInstance::setPlantedOnGround(bool planted) {
	mPlantedOnGround = planted;
}


}
}
}
