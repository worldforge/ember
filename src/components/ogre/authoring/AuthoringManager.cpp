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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <OgreSceneManager.h>
#include <Eris/Avatar.h>
#include <Eris/View.h>
#include <Eris/Connection.h>
#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

namespace Authoring
{
AuthoringManager::AuthoringManager(World& world) :
	DisplayAuthoringVisualizations("displayauthoringvisualizations", this, "Displays authoring markers for all entities."), HideAuthoringVisualizations("hideauthoringvisualizations", this, "Hides authoring markers for all entities."), mWorld(world), mHandler(nullptr), mEntityConsoleEditor(nullptr)
{
	//Delay checking the visualization config value until we've entered the world and can see if we're an admin or not.
	world.EventGotAvatar.connect(sigc::mem_fun(*this, &AuthoringManager::worldGotAvatar));
}

AuthoringManager::~AuthoringManager()
{
	for (SimpleEntityVisualizationStore::iterator I = mSimpleVisualizations.begin(); I != mSimpleVisualizations.end(); ++I) {
		SimpleEntityVisualization* vis = I->second.first;
		sigc::connection& conn = I->second.second;
		delete vis;
		conn.disconnect();
	}
	delete mHandler;
	delete mEntityConsoleEditor;
}

void AuthoringManager::worldGotAvatar() {
	delete mEntityConsoleEditor;
	mEntityConsoleEditor = nullptr;
	if (mWorld.getAvatar()->isAdmin()) {
		registerConfigListener("authoring", "visualizations", sigc::mem_fun(*this, &AuthoringManager::config_AuthoringVisualizations));
		mEntityConsoleEditor = new EntityConsoleEditor();
	}
}

void AuthoringManager::displayAuthoringVisualization()
{
	//Only show authoring visualizations for admins. Mainly to not confuse users.
	//This isn't optimal, since if this method is called before the avatar has been received, we'll go ahead anyway. Should be fixed...
	if (mWorld.getAvatar() == nullptr || mWorld.getAvatar()->isAdmin()) {
		if (!mHandler) {
			mHandler = new AuthoringHandler(mWorld);
		}
	}
}

void AuthoringManager::hideAuthoringVisualization()
{
	delete mHandler;
	mHandler = 0;
}

void AuthoringManager::displaySimpleEntityVisualization(EmberEntity& entity)
{
	if (!mSimpleVisualizations.count(&entity)) {
		Ogre::SceneNode* node = mWorld.getScene().getSceneManager().getRootSceneNode()->createChildSceneNode();
		SimpleEntityVisualization* vis(0);
		try {
			vis = new SimpleEntityVisualization(entity, node);
		} catch (const std::exception& ex) {
			//just delete the node and return
			node->getCreator()->destroySceneNode(node);
			return;
		}
		sigc::connection conn = entity.BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &AuthoringManager::simpleEntityVisualizationBeingDeleted), &entity));
		mSimpleVisualizations.insert(SimpleEntityVisualizationStore::value_type(&entity, std::make_pair(vis, conn)));
	}

}

void AuthoringManager::hideSimpleEntityVisualization(EmberEntity& entity)
{
	SimpleEntityVisualizationStore::iterator I = mSimpleVisualizations.find(&entity);
	if (I != mSimpleVisualizations.end()) {
		SimpleEntityVisualization* vis = I->second.first;
		sigc::connection& conn = I->second.second;
		delete vis;
		conn.disconnect();
		mSimpleVisualizations.erase(I);
	}
}

bool AuthoringManager::hasSimpleEntityVisualization(const EmberEntity& entity) const
{
	return mSimpleVisualizations.count(&entity) != 0;
}

void AuthoringManager::simpleEntityVisualizationBeingDeleted(EmberEntity* entity) {
	hideSimpleEntityVisualization(*entity);
}

void AuthoringManager::runCommand(const std::string &command, const std::string &)
{
	if (DisplayAuthoringVisualizations == command) {
		EmberServices::getSingleton().getConfigService().setValue("authoring", "visualizations", true);
	} else if (HideAuthoringVisualizations == command) {
		EmberServices::getSingleton().getConfigService().setValue("authoring", "visualizations", false);
	}
}

void AuthoringManager::config_AuthoringVisualizations(const std::string& section, const std::string&, varconf::Variable& variable)
{
	if (variable.is_bool()) {
		if (static_cast<bool> (variable)) {
			displayAuthoringVisualization();
		} else {
			hideAuthoringVisualization();
		}
	}
}

void AuthoringManager::startMovement(EmberEntity& entity, EntityMover& mover)
{
	if (mHandler) {
		mHandler->startMovement(entity, mover);
	}
}

void AuthoringManager::stopMovement()
{
	if (mHandler) {
		mHandler ->stopMovement();
	}
}


}
}
}
