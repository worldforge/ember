#include <memory>

//
// C++ Implementation: EntityWorldPickListener
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EntityWorldPickListener.h"
#include "EmberEntityUserObject.h"
#include "Scene.h"

#include "domain/EmberEntity.h"

#include "MousePicker.h"
#include "Convert.h"
#include "EntityCollisionInfo.h"

#include <OgreSceneQuery.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include <Eris/View.h>
#include <Eris/Avatar.h>

namespace Ember
{
namespace OgreView
{

EntityWorldPickListenerVisualizer::EntityWorldPickListenerVisualizer(EntityWorldPickListener& pickListener, Ogre::SceneManager& sceneManager) :
		mEntity(nullptr),
		mDebugNode(nullptr)
{
	mDebugNode = sceneManager.getRootSceneNode()->createChildSceneNode();
	try {
		mEntity = sceneManager.createEntity("pickerDebugObject", "common/primitives/model/sphere.mesh");
		//start out with a normal material
		mEntity->setMaterialName("BasePointMarkerMaterial");
		mEntity->setRenderingDistance(300);
		mEntity->setQueryFlags(MousePicker::CM_NONPICKABLE);
		mDebugNode->attachObject(mEntity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when creating picking visualizer." << ex);
	}

	pickListener.EventPickedEntity.connect(sigc::mem_fun(*this, &EntityWorldPickListenerVisualizer::picker_EventPickedEntity));
}

EntityWorldPickListenerVisualizer::~EntityWorldPickListenerVisualizer()
{
	if (mEntity) {
		mEntity->_getManager()->destroyEntity(mEntity);
	}
	if (mDebugNode) {
		mDebugNode->getCreator()->destroySceneNode(mDebugNode);
	}
}

void EntityWorldPickListenerVisualizer::picker_EventPickedEntity(const std::vector<EntityPickResult>& result, const MousePickerArgs& mouseArgs)
{
	mDebugNode->setPosition(result.begin()->position);
}

EntityWorldPickListener::EntityWorldPickListener(Eris::View& view, Scene& scene) :
		VisualizePicking("visualize_picking", this, "Visualize mouse pickings."),
		mClosestPickingDistance(0),
		mFurthestPickingDistance(0),
		mContinuePickingThisContext(true),
		mVisualizer(nullptr),
		mView(view),
		mScene(scene)
{
}

void EntityWorldPickListener::initializePickingContext(bool& willParticipate, const MousePickerArgs& pickArgs)
{
	mResult.clear();
	mPersistedResult.clear();
	if (pickArgs.pickType == MPT_PRESS || pickArgs.pickType == MPT_HOVER || pickArgs.pickType == MPT_SELECT) {
		willParticipate = true;

		mClosestPickingDistance = 0;
		mFurthestPickingDistance = 0;
		mContinuePickingThisContext = true;
	}
}

void EntityWorldPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (!mResult.empty()) {
		EventPickedEntity(mResult, mousePickerArgs);

		if (mousePickerArgs.pickType != MPT_HOVER) {
			mPersistedResult.reserve(mResult.size());
			for (auto& resultEntry : mResult) {
				PersistentEntityPickResult persistedEntry;
				persistedEntry.entityRef = Eris::EntityRef(resultEntry.entity);
				persistedEntry.distance = resultEntry.distance;
				persistedEntry.isTransparent = resultEntry.isTransparent;
				persistedEntry.position = resultEntry.position;
				mPersistedResult.push_back(persistedEntry);
			}
		}
	}
}

void EntityWorldPickListener::processPickResult(bool& continuePicking, PickResult& result, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (!mContinuePickingThisContext) {
		return;
	}

	if (result.collisionInfo.type() == typeid(EntityCollisionInfo)) {
		auto& entityCollisionInfo = boost::any_cast<EntityCollisionInfo&>(result.collisionInfo);

		if (mFilter) {
			if (!mFilter(*entityCollisionInfo.entity)) {
				return;
			}
		}

		//handle composed entities

		std::list<EmberEntity*> entities;
		entities.push_front(entityCollisionInfo.entity);
		EmberEntity* entity = entityCollisionInfo.entity->getEmberLocation();
		while (entity) {
			if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION) {
				entities.push_front(entity);
			} else if (entity->getCompositionMode() == EmberEntity::CM_COMPOSITION_EXCLUSIVE) {
				entities.clear();
				entities.push_front(entity);
			}
			entity = entity->getEmberLocation();
		}

		for (auto& pickedEntity: entities) {
			EntityPickResult entityPickResult{};
			entityPickResult.entity = pickedEntity;
			entityPickResult.position = Convert::toOgre(result.point);
			entityPickResult.isTransparent = entityCollisionInfo.isTransparent;
			mResult.push_back(entityPickResult);
			if (!entityCollisionInfo.isTransparent) {
				mContinuePickingThisContext = false;
			}
		}
	}
}

void EntityWorldPickListener::processDelayedPick(const MousePickerArgs& mousePickerArgs)
{
	if (!mPersistedResult.empty()) {
		std::vector<EntityPickResult> resolvedResult;

		for (auto& persistedEntry : mPersistedResult) {
			if (persistedEntry.entityRef) {
				EntityPickResult entry;
				entry.entity = dynamic_cast<EmberEntity*>(persistedEntry.entityRef.get());
				entry.distance = persistedEntry.distance;
				entry.isTransparent = persistedEntry.isTransparent;
				entry.position = persistedEntry.position;
				resolvedResult.push_back(entry);
			}
		}

		if (!resolvedResult.empty()) {
			EventPickedEntity(resolvedResult, mousePickerArgs);
		}

	}
}

void EntityWorldPickListener::runCommand(const std::string &command, const std::string &args)
{
	if (VisualizePicking == command) {
		if (mVisualizer) {
			mVisualizer.reset();
		} else {
			mVisualizer = std::make_unique<EntityWorldPickListenerVisualizer>(*this, mScene.getSceneManager());
		}
	}
}

EmberEntity* EntityWorldPickListener::findTerrainEntity()
{
	auto entity = mView.getAvatar()->getEntity();

	while (entity != nullptr) {
		if (entity->hasAttr("terrain")) {
			return dynamic_cast<EmberEntity*>(entity);
		}
		entity = entity->getLocation();
	}
	return nullptr;
}

const std::vector<PersistentEntityPickResult>& EntityWorldPickListener::getPersistentResult() const {
	return mPersistedResult;
}

const std::vector<EntityPickResult>& EntityWorldPickListener::getResult() const {
	return mResult;
}


}
}
