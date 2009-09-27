/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "AuthoringHandler.h"
#include "AuthoringVisualization.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/EmberEntity.h"
#include "framework/LoggingInstance.h"
#include <Eris/View.h>
#include <Eris/Entity.h>
#include <OgreSceneNode.h>
#include <sigc++/bind.h>

namespace EmberOgre
{

namespace Authoring
{

AuthoringHandler::AuthoringHandler(Eris::View& view)
{
	view.EntitySeen.connect(sigc::mem_fun(*this, &AuthoringHandler::view_EntitySeen));
	createVisualizationsForExistingEntities(view);
}

AuthoringHandler::~AuthoringHandler()
{
	for (VisualizationStore::iterator I = mVisualizations.begin(); I != mVisualizations.end(); ++I) {
		delete I->second;
	}
}

void AuthoringHandler::view_EntitySeen(Eris::Entity* entity)
{
	createVisualizationForEntity(static_cast<EmberEntity*> (entity));
}

void AuthoringHandler::createVisualizationForEntity(EmberEntity* entity)
{
	entity->BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &AuthoringHandler::view_EntityDeleted), entity));
	VisualizationStore::iterator I = mVisualizations.find(entity);
	if (I == mVisualizations.end()) {
		AuthoringVisualization* parentVis(0);
		Ogre::SceneNode* parentNode(0);
		if (entity->getLocation()) {
			VisualizationStore::iterator parentVisIterator = mVisualizations.find(static_cast<EmberEntity*> (entity->getLocation()));
			if (parentVisIterator != mVisualizations.end()) {
				parentVis = parentVisIterator->second;
				parentNode = parentVis->getSceneNode();
			} else {
				S_LOG_WARNING("Could not find parent visualization for entity.");
				parentNode = EmberOgre::getSingleton().getRootSceneNode();
			}
		} else {
			parentNode = EmberOgre::getSingleton().getRootSceneNode();
		}

		Ogre::SceneNode* sceneNode = parentNode->createChildSceneNode();
		AuthoringVisualization* visualization = new AuthoringVisualization(*entity, sceneNode);
		mVisualizations.insert(VisualizationStore::value_type(entity, visualization));
	} else {
		S_LOG_WARNING("Got create signal for entity which already has an authoring visualization. This should not happen.");
	}
}

void AuthoringHandler::view_EntityDeleted(Eris::Entity* entity)
{
	VisualizationStore::iterator I = mVisualizations.find(static_cast<EmberEntity*> (entity));
	if (I != mVisualizations.end()) {
		mVisualizations.erase(I);
		delete I->second;
	} else {
		S_LOG_WARNING("Got delete signal for entity which doesn't has an authoring visualization. This should not happen.");
	}

}

void AuthoringHandler::createVisualizationsForExistingEntities(Eris::View& view)
{
	if (view.getTopLevel()) {
		static_cast<EmberEntity*> (view.getTopLevel())->accept(*this);
	}
}

void AuthoringHandler::visit(EmberEntity& entity)
{
	createVisualizationForEntity(&entity);
}

}

}
