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
		mNode(node), mParentNode(parentNode), mOwnsNode(transferNodeOwnership), mOffsetNode(nullptr) {
}

SceneNodeProvider::~SceneNodeProvider() {
	Ogre::SceneNode* node = mOffsetNode ? mOffsetNode : mNode;
	node->removeAllChildren();
	if (mOwnsNode) {
		mNode->getCreator()->destroySceneNode(mNode);
	}
}

Ogre::Node* SceneNodeProvider::getNode() const {
	return mNode;
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
	mNode->setInheritScale(false);
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

void SceneNodeProvider::setOffsets(const Ogre::Vector3& translate, const Ogre::Quaternion& rotate) {
	if (translate.isNaN() || rotate.isNaN() || (translate == Ogre::Vector3::UNIT_SCALE && rotate == Ogre::Quaternion::IDENTITY)) {
		if (mOffsetNode) {
			while (mOffsetNode->numAttachedObjects()) {
				auto movable = mOffsetNode->detachObject((unsigned short)0);
				mNode->attachObject(movable);
			}
		}
	} else {
		if (mOffsetNode) {
			mOffsetNode->setPosition(translate);
			mOffsetNode->setOrientation(rotate);
		} else {
			mOffsetNode = mNode->createChildSceneNode(translate, rotate);
			mOffsetNode->setInheritScale(true);
			while (mNode->numAttachedObjects()) {
				auto movable = mNode->detachObject((unsigned short)0);
				mOffsetNode->attachObject(movable);
			}
		}
	}
}


void SceneNodeProvider::setScale(const Ogre::Vector3& scale) {
	mNode->setScale(scale);
}


void SceneNodeProvider::detachObject(Ogre::MovableObject* movable) {
	Ogre::SceneNode* node = mOffsetNode ? mOffsetNode : mNode;
	node->detachObject(movable);
}

void SceneNodeProvider::attachObject(Ogre::MovableObject* movable) {
	Ogre::SceneNode* node = mOffsetNode ? mOffsetNode : mNode;
	node->attachObject(movable);
}

}
}
