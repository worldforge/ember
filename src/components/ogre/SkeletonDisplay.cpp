/*
 Copyright (C) 2018 Erik Ogenvik

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

#include "SkeletonDisplay.h"
#include <OgreSkeletonInstance.h>
#include <OgreManualObject.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreBone.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>


namespace Ember {
namespace OgreView {

SkeletonDisplay::SkeletonDisplay(Ogre::Entity& entity)
		: mEntity(entity),
		  mManualObject(entity._getManager()->createManualObject()) {
	mManualObject->setDynamic(true);
	//We want to render the lines on top of everything, so that they aren't hidden by anything
	mManualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1);

	mEntity.getParentSceneNode()->attachObject(mManualObject);
	Ogre::Root::getSingleton().addFrameListener(this);
}

SkeletonDisplay::~SkeletonDisplay() {
	mManualObject->_getManager()->destroyManualObject(mManualObject);
	Ogre::Root::getSingleton().removeFrameListener(this);
}

bool SkeletonDisplay::frameStarted(const Ogre::FrameEvent& evt) {
	update();
	return true;
}

void SkeletonDisplay::update() {
	mManualObject->clear();

	auto skeleton = mEntity.getSkeleton();
	if (skeleton) {
		mManualObject->begin("/common/base/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_LIST);
		for (auto& rootBone : skeleton->getRootBones()) {
			for (auto& childBone : rootBone->getChildren()) {
				drawChild(rootBone, childBone);
			}
		}
		mManualObject->end();
	}
}

void SkeletonDisplay::drawChild(Ogre::Node* const& parent, Ogre::Node* const& child) {
	mManualObject->position(parent->_getDerivedPosition());
	mManualObject->position(child->_getDerivedPosition());

	for (auto& childBone : child->getChildren()) {
		drawChild(child, childBone);
	}
}


}
}