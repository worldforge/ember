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

#include "ModelBoneProvider.h"
#include <OgreNode.h>
#include <OgreTagPoint.h>
#include <OgreSceneNode.h>

namespace Ember {
namespace OgreView {
namespace Model {
ModelBoneProvider::ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, const std::string& attachPointName, bool deleteMovableWhenDone) :
		mNode(parentSceneNode),
		mParentModel(parentModel),
		mAttachPointName(attachPointName),
		mParent(nullptr),
		mPosition(Ogre::Vector3::ZERO),
		mOrientation(Ogre::Quaternion::IDENTITY),
		mDeleteMovableWhenDone(deleteMovableWhenDone),
		mScale(Ogre::Vector3::UNIT_SCALE),
		mVisible(true),
		mOffsetTranslation(Ogre::Vector3::ZERO),
		mOffsetRotation(Ogre::Quaternion::IDENTITY) {
	auto& attachPointDefinitions = parentModel.getDefinition()->getAttachPointsDefinitions();
	auto I = std::find_if(std::begin(attachPointDefinitions), std::end(attachPointDefinitions), [&](AttachPointDefinition def) { return def.Name == attachPointName; });
	if (I != attachPointDefinitions.end()) {
		mAttachPointDefinition = *I;
		mPosition = mAttachPointDefinition.Translation;
		mOrientation = mAttachPointDefinition.Rotation;
	}
}

ModelBoneProvider::ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, const std::string& attachPointName, ModelBoneProvider* parent) :
		mNode(parentSceneNode),
		mParentModel(parentModel),
		mAttachPointName(attachPointName),
		mParent(parent),
		mPosition(Ogre::Vector3::ZERO),
		mOrientation(Ogre::Quaternion::IDENTITY),
		mDeleteMovableWhenDone(false),
		mScale(Ogre::Vector3::UNIT_SCALE),
		mVisible(true),
		mOffsetTranslation(Ogre::Vector3::ZERO),
		mOffsetRotation(Ogre::Quaternion::IDENTITY) {
}

ModelBoneProvider::~ModelBoneProvider() {
	assert(mTagPoints.empty());
	if (mParent) {
		ModelBoneProviderStore::iterator I = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
		if (I != mParent->mChildren.end()) {
			mParent->mChildren.erase(I);
		}
	}
}

Ogre::Node* ModelBoneProvider::getNode() const {
	return mNode;
}

Ogre::Node* ModelBoneProvider::getParentNode() const {
	return nullptr;
}

INodeProvider* ModelBoneProvider::createChildProvider(const std::string& name) {
	ModelBoneProvider* childProvider = new ModelBoneProvider(mNode, mParentModel, mAttachPointName, this);
	mChildren.push_back(childProvider);
	return childProvider;
}

void ModelBoneProvider::setVisible(bool visible) {
	mVisible = visible;
	for (auto& tagPoint : mTagPoints) {
		tagPoint->getChildObject()->setVisible(visible);
	}
}

void ModelBoneProvider::setVisualize(const std::string&, bool) {
}

bool ModelBoneProvider::getVisualize(const std::string&) const {
	return false;
}

void ModelBoneProvider::setPositionAndOrientation(const Ogre::Vector3& position, const Ogre::Quaternion& orientation) {
	//Only apply the attach point definition rotation and translation to the first provider in a chained list of providers.
	//This is because all providers will in reality use the same Ogre::TagPoint, and therefore the orientation should only be applied once.
	if (!mParent) {
		mOrientation = orientation * mAttachPointDefinition.Rotation * mOffsetRotation;
		mPosition = position + mAttachPointDefinition.Translation + mOffsetTranslation;
	} else {
		mOrientation = orientation * mOffsetRotation;
		mPosition = position + mOffsetTranslation;
	}
	updatePositionAndOrientation();
}

void ModelBoneProvider::setOffsets(const Ogre::Vector3& translate, const Ogre::Quaternion& rotate) {
	mOffsetTranslation = translate;
	mOffsetRotation = rotate;
	updatePositionAndOrientation();
}


void ModelBoneProvider::setScale(const Ogre::Vector3& scale) {
	mScale = scale;
	for (auto tagpoint : mTagPoints) {
		tagpoint->setScale(scale);
	}
}


void ModelBoneProvider::updatePositionAndOrientation() {
	for (auto tagpoint : mTagPoints) {
		tagpoint->setPosition(getDerivedPosition());
		tagpoint->setOrientation(getDerivedOrientation());
	}
	for (ModelBoneProviderStore::const_iterator I = mChildren.begin(); I != mChildren.end(); ++I) {
		(*I)->updatePositionAndOrientation();
	}
}

Ogre::Vector3 ModelBoneProvider::getDerivedPosition() const {
	if (mParent) {
		return mParent->getDerivedPosition() + mPosition;
	}
	return mPosition;
}

Ogre::Quaternion ModelBoneProvider::getDerivedOrientation() const {
	if (mParent) {
		return mParent->getDerivedOrientation() * mOrientation;
	}
	return mOrientation;
}


void ModelBoneProvider::detachObject(Ogre::MovableObject* movable) {
	for (auto I = mTagPoints.begin(); I != mTagPoints.end(); I++) {
		Ogre::TagPoint* tagPoint = *I;
		if (movable == tagPoint->getChildObject()) {
			mTagPoints.erase(I);
		}
	}
	movable->setVisible(true);
	mParentModel.detachObject(movable);
}

void ModelBoneProvider::attachObject(Ogre::MovableObject* movable) {
	auto tagPoint = mParentModel.attachObject(mAttachPointName, movable);
	if (tagPoint) {
		tagPoint->setInheritScale(false);
		tagPoint->setInheritParentEntityScale(false);
		tagPoint->setScale(mScale);
		tagPoint->setPosition(getDerivedPosition());
		tagPoint->setOrientation(getDerivedOrientation());

		movable->setVisible(mVisible);
		mTagPoints.insert(tagPoint);
	}
}

}
}
}
