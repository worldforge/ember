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
															 std::vector<Atlas::Message::MapType> entityMaps,
															 bool randomizeOrientation) :
		mWorld(world),
		mTypeService(typeService),
		mEntityNode(nullptr),
		mAxisMarker(nullptr) {

	for (auto& entityMap : entityMaps) {
		mEntityPreviews.emplace_back(EntityPreview{std::move(entityMap)});
	}

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

	mPos = {};

	createEntity();
}

void EntityCreatorCreationInstance::createEntity() {
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


	EmberEntity& avatar = mWorld.getAvatar()->getEmberEntity();

	for (auto& entry : mEntityPreviews) {

		auto& entityMap = entry.entityMap;
		// Creating entity data
		auto parentI = entityMap.find("parent");
		if (parentI == entityMap.end() || !parentI->second.isString()) {
			S_LOG_FAILURE("No parent found.");
			return;
		}

		Eris::TypeInfo* erisType = mTypeService.getTypeByName(parentI->second.String());
		if (!erisType) {
			S_LOG_FAILURE("Type " << parentI->second.String() << " not found.");
			return;
		}


		//Default to the same loc as the avatar, which can be overwritten later on.
		entityMap["loc"] = avatar.getLocation()->getId();

		// Temporary entity
		entry.mEntity = std::make_unique<Authoring::DetachedEntity>("-1", erisType);
		Atlas::Objects::Entity::RootEntity rootEntity;
		entry.mEntity->doInit(rootEntity);
		entry.mEntity->setFromMessage(entityMap);
		//Set the parent to the top level by default.
		entry.mEntity->setLocationEntity(mWorld.getView().getTopLevel());

		// Making model from temporary entity
		Mapping::ModelActionCreator actionCreator(*entry.mEntity, [this, &entry](const std::string& modelName) {
			setModel(entry, modelName);
		}, [&](const std::string& partName) {
			showModelPart(entry, partName);
		});

		std::unique_ptr<EntityMapping::EntityMapping> modelMapping = Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*entry.mEntity, actionCreator, mWorld.getView().getTypeService(), &mWorld.getView());
		if (modelMapping) {
			modelMapping->initialize();
		}
	}


	//If only one entity use it for snapping.
	//TODO: handle snapping if many entities.
	mMovement = std::make_unique<EntityCreatorMovement>(*this, mWorld.getMainCamera(), mEntityPreviews.size() == 1 ? mEntityPreviews.front().mEntity.get() : nullptr, mEntityNode);
	mMovement->getBridge()->Moved.connect([&]() {
		EventMoved(mMovement->getBridge()->mCollidedEntity.get(), mMovement->getBridge()->getPosition());
	});
}

void EntityCreatorCreationInstance::setModel(EntityPreview& entry, const std::string& modelName) {


	if (entry.mModel) {
		if (entry.mModel->getDefinition()->getOrigin() == modelName) {
			return;
		}
	}
	auto modelDef = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
	if (!modelDef) {
		modelDef = Model::ModelDefinitionManager::getSingleton().getByName("common/primitives/placeholder.modeldef");
	}
	entry.mModel = std::make_unique<Model::Model>(mWorld.getSceneManager(), modelDef, modelName);
	entry.mModel->Reloaded.connect([this, &entry]() {
		if (entry.mModelMount) {
			entry.mModelMount->rescale(entry.mEntity->hasBBox() ? &entry.mEntity->getBBox() : nullptr);
		}
	});
	entry.mModel->load();

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
	entry.mModelMount = std::make_unique<Model::ModelMount>(*entry.mModel, std::make_unique<SceneNodeProvider>(node, mEntityNode));
	entry.mModelMount->reset();
	entry.mModelMount->rescale(entry.mEntity->hasBBox() ? &entry.mEntity->getBBox() : nullptr);

	// Setting initial position and orientation
	if (mPos.isValid()) {
		mEntityNode->setPosition(Convert::toOgre(mPos));
	}
	mEntityNode->setOrientation(Convert::toOgre(mInitialOrientation));
}

void EntityCreatorCreationInstance::showModelPart(EntityPreview& entry, const std::string& partName) {
	if (entry.mModel) {
		entry.mModel->showPart(partName);
	}
}

void EntityCreatorCreationInstance::hideModelPart(EntityPreview& entry, const std::string& partName) {
	if (entry.mModel) {
		entry.mModel->hidePart(partName);
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

EntityCreatorMovement* EntityCreatorCreationInstance::getMovement() {
	return mMovement.get();
}

}
}
}
