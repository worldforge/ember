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

#include "AuthoringManager.h"
#include "AuthoringHandler.h"
#include "SimpleEntityVisualization.h"
#include "EntityConsoleEditor.h"
#include "components/ogre/World.h"
#include "components/ogre/Scene.h"
#include "domain/EmberEntity.h"
#include "components/ogre/Avatar.h"
#include "services/config/ConfigService.h"
#include "services/server/ServerService.h"
#include "GeometryVisualization.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>
#include <Eris/Connection.h>

namespace Ember {
namespace OgreView {

namespace Authoring {
AuthoringManager::AuthoringManager(World& world) :
		DisplayAuthoringVisualizations("displayauthoringvisualizations", this, "Displays authoring markers for all entities."),
		HideAuthoringVisualizations("hideauthoringvisualizations", this, "Hides authoring markers for all entities."),
		mWorld(world) {
	//Delay checking the visualization config value until we've entered the world and can see if we're an admin or not.
	world.EventGotAvatar.connect(sigc::mem_fun(*this, &AuthoringManager::worldGotAvatar));
}

AuthoringManager::~AuthoringManager() {
	for (auto& simpleVisualization : mSimpleVisualizations) {
		sigc::connection& conn = simpleVisualization.second.second;
		conn.disconnect();
	}
}

void AuthoringManager::worldGotAvatar() {
	mEntityConsoleEditor.reset();
	//We'll only look at the config if we're an admin.
	// This means that visualizations won't be turned on automatically for non-admin characters,
	// but can still be enabled by issuing the "displayauthoringvisualizations"
	// console command. This is to not confuse users.
	if (mWorld.getAvatar()->isAdmin()) {
		registerConfigListener("authoring", "visualizations", sigc::mem_fun(*this, &AuthoringManager::config_AuthoringVisualizations));
		mEntityConsoleEditor = std::make_unique<EntityConsoleEditor>(mWorld.getAvatar());
	}
}

void AuthoringManager::displayAuthoringVisualization() {
	if (!mHandler) {
		mHandler = std::make_unique<AuthoringHandler>(mWorld);
	}
}

void AuthoringManager::hideAuthoringVisualization() {
	mHandler.reset();
}


void AuthoringManager::displayGeometryVisualization(EmberEntity& entity) {
	if (!mGeometryVisualizations.count(&entity)) {
		Ogre::SceneNode* node = mWorld.getScene().getSceneManager().getRootSceneNode()->createChildSceneNode();
		std::unique_ptr<GeometryVisualization> vis;
		try {
			vis = std::make_unique<GeometryVisualization>(entity, node);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Error when displaying geometry." << ex);
			//just delete the node and return
			node->getCreator()->destroySceneNode(node);
			return;
		}
		sigc::connection conn = entity.BeingDeleted.connect([&]() { hideGeometryVisualization(entity); });
		mGeometryVisualizations.emplace(&entity, std::make_pair(std::move(vis), conn));
	}
}

void AuthoringManager::hideGeometryVisualization(EmberEntity& entity) {
	auto I = mGeometryVisualizations.find(&entity);
	if (I != mGeometryVisualizations.end()) {
		sigc::connection& conn = I->second.second;
		conn.disconnect();
		mGeometryVisualizations.erase(I);
	}
}


bool AuthoringManager::hasGeometryVisualization(const EmberEntity& entity) const {
	return mGeometryVisualizations.count(&entity) != 0;
}


void AuthoringManager::displaySimpleEntityVisualization(EmberEntity& entity) {
	if (!mSimpleVisualizations.count(&entity)) {
		Ogre::SceneNode* node = mWorld.getScene().getSceneManager().getRootSceneNode()->createChildSceneNode();
		std::unique_ptr<SimpleEntityVisualization> vis;
		try {
			vis = std::make_unique<SimpleEntityVisualization>(entity, node);
		} catch (const std::exception&) {
			//just delete the node and return
			node->getCreator()->destroySceneNode(node);
			return;
		}
		sigc::connection conn = entity.BeingDeleted.connect([&]() { hideSimpleEntityVisualization(entity); });
		mSimpleVisualizations.emplace(&entity, std::make_pair(std::move(vis), conn));
	}
}

void AuthoringManager::hideSimpleEntityVisualization(EmberEntity& entity) {
	auto I = mSimpleVisualizations.find(&entity);
	if (I != mSimpleVisualizations.end()) {
		sigc::connection& conn = I->second.second;
		conn.disconnect();
		mSimpleVisualizations.erase(I);
	}
}

bool AuthoringManager::hasSimpleEntityVisualization(const EmberEntity& entity) const {
	return mSimpleVisualizations.count(&entity) != 0;
}

void AuthoringManager::runCommand(const std::string& command, const std::string&) {
	if (DisplayAuthoringVisualizations == command) {
		displayAuthoringVisualization();
	} else if (HideAuthoringVisualizations == command) {
		hideAuthoringVisualization();
	}
}

void AuthoringManager::config_AuthoringVisualizations(const std::string& section, const std::string&, varconf::Variable& variable) {
	if (variable.is_bool()) {
		if (static_cast<bool> (variable)) {
			displayAuthoringVisualization();
		} else {
			hideAuthoringVisualization();
		}
	}
}

void AuthoringManager::startMovement(EmberEntity& entity, EntityMover& mover) {
	if (mHandler) {
		mHandler->startMovement(entity, mover);
	}
}

void AuthoringManager::stopMovement() {
	if (mHandler) {
		mHandler->stopMovement();
	}
}


}
}
}
