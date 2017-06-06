/*
 Copyright (C) 2009 Erik Ogenvik <erik@ogenvik.org>

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

namespace Ember {
namespace OgreView {

SceneNodeProvider::SceneNodeProvider(Ogre::SceneNode* node, Ogre::SceneNode* parentNode, bool transferNodeOwnership) :
		mNode(node), mParentNode(parentNode), mOwnsNode(transferNodeOwnership) {
}

SceneNodeProvider::~SceneNodeProvider() {
	mNode->removeAllChildren();
	if (mOwnsNode) {
		mNode->getCreator()->destroySceneNode(mNode);
	}
}

Ogre::Node& SceneNodeProvider::getNode() const {
	return *mNode;
}

Ogre::Node* SceneNodeProvider::getParentNode() const {
	return mNode->getParent();
}

INodeProvider* SceneNodeProvider::createChildProvider(const std::string& name) {
	Ogre::SceneNode* node;
	if (name != "") {
		node = mNode->createChildSceneNode(name);
	} else {
		node = mNode->createChildSceneNode();
	}
	return new SceneNodeProvider(node, mNode, mOwnsNode);
}

void SceneNodeProvider::setVisible(bool visible) {
	if (mParentNode) {
		if (!visible) {
			if (mNode->getParent()) {
				mNode->getParent()->removeChild(mNode);
			}
		} else {
			if (!mNode->getParent()) {
				mParentNode->addChild(mNode);
			}
		}
	} else {
		mNode->setVisible(visible);
	}
}

void SceneNodeProvider::setVisualize(const std::string& visualization, bool visualize) {
	if (visualization == "OgreBBox") {
		mNode->showBoundingBox(visualize);
	}
}

bool SceneNodeProvider::getVisualize(const std::string& visualization) const {
	if (visualization == "OgreBBox") {
		return mNode->getShowBoundingBox();
	}
	return false;
}

void SceneNodeProvider::setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation) {
	mNode->setPosition(position);
	mNode->setOrientation(orientation);
}

void SceneNodeProvider::detachObject(Ogre::MovableObject* movable) {
	mNode->detachObject(movable);
}

void SceneNodeProvider::attachObject(Ogre::MovableObject* movable) {
	mNode->attachObject(movable);
}

}
}
