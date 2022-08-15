/*
 Copyright (C) 2009 Erik Ogenvik

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

#include "AuthoringHandler.h"
#include "AuthoringVisualization.h"
#include "AuthoringVisualizationMover.h"
#include "components/ogre/World.h"
#include "components/ogre/Scene.h"
#include "domain/EmberEntity.h"
#include "framework/LoggingInstance.h"
#include <Eris/View.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <sigc++/bind.h>

namespace Ember::OgreView::Authoring {

AuthoringMoveInstance::AuthoringMoveInstance(EmberEntity& entity, AuthoringVisualization& visualization, EntityMover& mover, AuthoringHandler& moveHandler) :
		EntityObserverBase(entity, false),
		mMover(std::make_unique<AuthoringVisualizationMover>(visualization, mover)),
		mMoveHandler(moveHandler),
		mVisualization(visualization) {
}

AuthoringMoveInstance::~AuthoringMoveInstance() = default;

void AuthoringMoveInstance::cleanup() {
	mMoveHandler.stopMovement();
}

AuthoringVisualization& AuthoringMoveInstance::getVisualization() {
	return mVisualization;
}

AuthoringHandler::AuthoringHandler(World& world) :
		mMoveInstance(nullptr), mWorld(world) {
	world.getView().InitialSightEntity.connect(sigc::mem_fun(*this, &AuthoringHandler::view_EntityInitialSight));
	createVisualizationsForExistingEntities(world.getView());
}

AuthoringHandler::~AuthoringHandler() = default;

void AuthoringHandler::view_EntityInitialSight(Eris::Entity* entity) {
	createVisualizationForEntity(dynamic_cast<EmberEntity*> (entity));
}

void AuthoringHandler::createVisualizationForEntity(EmberEntity* entity) {

	auto I = mVisualizations.find(entity);
	if (I == mVisualizations.end()) {
		entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &AuthoringHandler::view_EntityDeleted), entity));
		entity->LocationChanged.connect(sigc::bind(sigc::mem_fun(*this, &AuthoringHandler::view_EntityLocationChanged), entity));

		Ogre::SceneNode* parentNode;
		if (entity->getLocation()) {
			auto parentVisIterator = mVisualizations.find(entity->getEmberLocation());
			if (parentVisIterator != mVisualizations.end()) {
				parentNode = parentVisIterator->second->getSceneNode();
			} else {
				S_LOG_WARNING("Could not find parent visualization for entity. " << *entity);
				parentNode = mWorld.getScene().getSceneManager().getRootSceneNode();
			}
		} else {
			parentNode = mWorld.getScene().getSceneManager().getRootSceneNode();
		}

		Ogre::SceneNode* sceneNode = parentNode->createChildSceneNode();
		mVisualizations.emplace(entity, std::make_unique<AuthoringVisualization>(*entity, sceneNode));
	} else {
		S_LOG_WARNING("Got create signal for entity which already has an authoring visualization. This should not happen. " << *entity);
	}

}

void AuthoringHandler::view_EntityDeleted(Eris::Entity* entity) {
	auto I = mVisualizations.find(dynamic_cast<EmberEntity*> (entity));
	if (I != mVisualizations.end()) {
		//see if there's an ongoing movement for the deleted entity, and if we therefore should stop that
		if (mMoveInstance) {
			if (I->second.get() == &mMoveInstance->getVisualization()) {
				stopMovement();
			}
		}
		mVisualizations.erase(I);
	} else {
		S_LOG_WARNING("Got delete signal for entity which doesn't has an authoring visualization. This should not happen. " << *entity);
	}
}

void AuthoringHandler::view_EntityLocationChanged(Eris::Entity* oldLocation, EmberEntity* entity) {
	auto I = mVisualizations.find(entity);
	//The entity could have already been deleted (we get a LocationChanged signal after EntityDeleted)
	if (I != mVisualizations.end()) {
		if (I->second->getSceneNode()->getParent()) {
			I->second->getSceneNode()->getParent()->removeChild(I->second->getSceneNode());
		}
		auto newLocation = entity->getEmberLocation();
		if (newLocation) {
			auto parentI = mVisualizations.find(newLocation);

			Ogre::SceneNode* parentNode;
			if (parentI != mVisualizations.end()) {
				parentNode = parentI->second->getSceneNode();
			} else {
				parentNode = mWorld.getScene().getSceneManager().getRootSceneNode();
			}
			parentNode->addChild(I->second->getSceneNode());
			I->second->updatePositionAndOrientation();
		}
	}
}

void AuthoringHandler::createVisualizationsForExistingEntities(Eris::View& view) {
	if (view.getTopLevel()) {
		dynamic_cast<EmberEntity*> (view.getTopLevel())->accept(*this);
	}
}

void AuthoringHandler::visit(EmberEntity& entity) {
	createVisualizationForEntity(&entity);
}

void AuthoringHandler::startMovement(EmberEntity& entity, EntityMover& mover) {
	mMoveInstance.reset();
	auto I = mVisualizations.find(&entity);
	if (I != mVisualizations.end()) {
		mMoveInstance = std::make_unique<AuthoringMoveInstance>(entity, *(I->second), mover, *this);
	}
}

void AuthoringHandler::stopMovement() {
	mMoveInstance.reset();
}

}
