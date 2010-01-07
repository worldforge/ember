/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "SceneNodeProvider.h"

#include <OgreSceneNode.h>
#include <OgreMovableObject.h>
#include <OgreSceneManager.h>

namespace EmberOgre
{

SceneNodeProvider::SceneNodeProvider(Ogre::SceneNode& parentNode, Ogre::MovableObject* object) :
	mParentNode(parentNode), mNode(0), mAttachedObject(object)
{
	mNode = parentNode.createChildSceneNode();
	if (mAttachedObject) {
		mAttachedObject->detachFromParent();
		mNode->attachObject(mAttachedObject);
	}
}

SceneNodeProvider::~SceneNodeProvider()
{
	//Only detach if it's attached to ourselves
	if (mAttachedObject) {
		if (mAttachedObject->getParentNode() == mNode) {
			mNode->detachObject(mAttachedObject);
		}
	}
	//If the visibility was changed for the node, we usually detach it from the parent node. Therefore we need to first check whether there's any parent set.
	if (mNode->getParent()) {
		mParentNode.removeAndDestroyChild(mNode->getName());
	}
	else {
		mNode->getCreator()->destroySceneNode(mNode);
	}
}

Ogre::Node& SceneNodeProvider::getNode() const
{
	return *mNode;
}

Ogre::Node* SceneNodeProvider::getParentNode() const
{
	return &mParentNode;
}

INodeProvider* SceneNodeProvider::createChildProvider(Ogre::MovableObject* attachedObject)
{
	return new SceneNodeProvider(*mNode, attachedObject);
}

void SceneNodeProvider::setVisible(bool visible)
{
	if (!visible) {
		if (mNode->getParent() == &mParentNode) {
			mParentNode.removeChild(mNode);
		}
	}
	else {
		if (!mNode->getParent()) {
			mParentNode.addChild(mNode);
		}
	}
}

void SceneNodeProvider::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "OgreBBox") {
		mNode->showBoundingBox(visualize);
	}
}

bool SceneNodeProvider::getVisualize(const std::string& visualization) const
{
	if (visualization == "OgreBBox") {
		return mNode->getShowBoundingBox();
	}
	return false;
}

void SceneNodeProvider::setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation)
{
	mNode->setPosition(position);
	mNode->setOrientation(orientation);
}

}
