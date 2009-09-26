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

#include "AuthoringVisualization.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/OgreInfo.h"

#include "framework/LoggingInstance.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

namespace EmberOgre
{

namespace Authoring
{
AuthoringVisualization::AuthoringVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode) :
	mEntity(entity), mSceneNode(sceneNode), mGraphicalRepresentation(0)
{
	createGraphicalRepresentation();
	mEntity.Moved.connect(sigc::mem_fun(*this, &AuthoringVisualization::entity_Moved));
	updatePositionAndOrientation();
}

AuthoringVisualization::~AuthoringVisualization()
{
}

Ogre::SceneNode* AuthoringVisualization::getSceneNode() const
{
	return mSceneNode;
}

void AuthoringVisualization::entity_Moved()
{
	updatePositionAndOrientation();
}

void AuthoringVisualization::updatePositionAndOrientation()
{
	mSceneNode->setPosition(Convert::toOgre(mEntity.getViewPosition()));
	mSceneNode->setOrientation(Convert::toOgre(mEntity.getViewOrientation()));
}

void AuthoringVisualization::createGraphicalRepresentation()
{
	try {
		mGraphicalRepresentation = mSceneNode->getCreator()->createEntity(OgreInfo::createUniqueResourceName("authoring_visualization_" + mEntity.getId()), "axes.mesh");
		if (mGraphicalRepresentation) {
			try {
				mSceneNode->attachObject(mGraphicalRepresentation);
			} catch (const std::exception& ex) {
				S_LOG_WARNING("Error when attaching axes mesh."<< ex);
				mSceneNode->getCreator()->destroyMovableObject(mGraphicalRepresentation);
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when loading axes mesh."<< ex);
	}
}

}
}
