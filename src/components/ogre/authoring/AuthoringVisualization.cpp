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
#include "AuthoringVisualizationCollisionDetector.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/Convert.h"
#include "components/ogre/OgreInfo.h"
#include "components/ogre/EmberEntityUserObject.h"
#include "components/ogre/MousePicker.h"


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
	removeGraphicalRepresentation();
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
	mSceneNode->setPosition(Convert::toOgre(mEntity.getPredictedPos()));
	mSceneNode->setOrientation(Convert::toOgre(mEntity.getOrientation()));
}

void AuthoringVisualization::createGraphicalRepresentation()
{
	try {
		mGraphicalRepresentation = mSceneNode->getCreator()->createEntity(OgreInfo::createUniqueResourceName("authoring_visualization_" + mEntity.getId()), "axes.mesh");
		if (mGraphicalRepresentation) {
			try {
				mSceneNode->attachObject(mGraphicalRepresentation);
				mGraphicalRepresentation->setRenderingDistance(300);
				mGraphicalRepresentation->setQueryFlags(MousePicker::CM_UNDEFINED);
				mGraphicalRepresentation->setUserObject(new EmberEntityUserObject(mEntity, new AuthoringVisualizationCollisionDetector(*mGraphicalRepresentation)));
			} catch (const std::exception& ex) {
				S_LOG_WARNING("Error when attaching axes mesh."<< ex);
				mSceneNode->getCreator()->destroyMovableObject(mGraphicalRepresentation);
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when loading axes mesh."<< ex);
	}
}

void AuthoringVisualization::removeGraphicalRepresentation()
{
	mSceneNode->detachAllObjects();
	if (mGraphicalRepresentation) {
		delete mGraphicalRepresentation->getUserObject();
		mSceneNode->getCreator()->destroyEntity(mGraphicalRepresentation);
		mGraphicalRepresentation = 0;
	}
	mSceneNode->getCreator()->destroySceneNode(mSceneNode);
	mSceneNode = 0;
}

}
}
