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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ModelBoneProvider.h"
#include <OgreNode.h>
#include <OgreTagPoint.h>
#include <OgreSceneNode.h>

#include <utility>

namespace Ember {
namespace OgreView {
namespace Model {
ModelBoneProvider::ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, bool deleteMovableWhenDone) :
		mNode(parentSceneNode),
		mParentModel(parentModel),
		mParent(nullptr),
		mPosition(Ogre::Vector3::ZERO),
		mOrientation(Ogre::Quaternion::IDENTITY),
		mDeleteMovableWhenDone(deleteMovableWhenDone),
		mScale(Ogre::Vector3::UNIT_SCALE),
		mVisible(true),
		mOffsetTranslation(Ogre::Vector3::ZERO),
		mOffsetRotation(Ogre::Quaternion::IDENTITY) {
}

ModelBoneProvider::ModelBoneProvider(Ogre::Node* parentSceneNode, Model& parentModel, ModelBoneProvider* parent) :
		mNode(parentSceneNode),
		mParentModel(parentModel),
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
		auto I = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
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

std::unique_ptr<INodeProvider> ModelBoneProvider::createChildProvider(const std::string& name) {
	auto childProvider = std::make_unique<ModelBoneProvider>(mNode, mParentModel, this);
	childProvider->setAttachPointDefinition(mAttachPointDefinition);
	mChildren.push_back(childProvider.get());
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

	updatePositionAndOrientation();
}

void ModelBoneProvider::setOffsets(const Ogre::Vector3& translate, const Ogre::Quaternion& rotate) {
	if (!translate.isNaN()) {
		mOffsetTranslation = translate;
	} else {
		S_LOG_WARNING("Translation set for model bone provider for attach point '" << mAttachPointDefinition.BoneName << "' is invalid");
	}
	if (!rotate.isNaN()) {
		mOffsetRotation = rotate;
	} else {
		S_LOG_WARNING("Rotation set for model bone provider for attach point '" << mAttachPointDefinition.BoneName << "' is invalid");
	}
	updatePositionAndOrientation();
}

void ModelBoneProvider::setScale(const Ogre::Vector3& scale) {
	mScale = scale;
	for (auto tagpoint : mTagPoints) {
		tagpoint->setScale(scale);
	}
}

Ogre::Vector3 ModelBoneProvider::getScale() const {
	return mScale;
}

void ModelBoneProvider::updatePositionAndOrientation() {
	if (!mParent) {
		mOrientation = mAttachPointDefinition.Rotation * mOffsetRotation;
		mPosition = (mAttachPointDefinition.Rotation * mAttachPointDefinition.Translation) + (mOffsetRotation *  mOffsetTranslation);
	} else {
		mOrientation = mOffsetRotation;
		mPosition = mOffsetRotation * mOffsetTranslation;
	}
	for (auto tagpoint : mTagPoints) {
		tagpoint->setPosition(getDerivedPosition());
		tagpoint->setOrientation(getDerivedOrientation());
	}
	for (auto child : mChildren) {
		child->updatePositionAndOrientation();
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
	for (auto I = mTagPoints.begin(); I != mTagPoints.end();) {
		Ogre::TagPoint* tagPoint = *I;
		if (movable == tagPoint->getChildObject()) {
			//Erasing in a vector will invalidate the iterator.
			I = mTagPoints.erase(I);
		} else {
			I++;
		}
	}
	movable->setVisible(true);
	mParentModel.detachObject(movable);
}

void ModelBoneProvider::attachObject(Ogre::MovableObject* movable) {
	auto tagPoint = mParentModel.attachObject(mAttachPointDefinition.Name, movable);
	if (tagPoint) {
		tagPoint->setInheritScale(false);
		tagPoint->setInheritParentEntityScale(false);
		tagPoint->setScale(mScale);
		tagPoint->setPosition(getDerivedPosition());
		tagPoint->setOrientation(getDerivedOrientation());

		movable->setVisible(mVisible);
		mTagPoints.emplace_back(tagPoint);
	}
}

void ModelBoneProvider::setAttachPointDefinition(AttachPointDefinition attachPointDefinition) {
	mAttachPointDefinition = std::move(attachPointDefinition);
	updatePositionAndOrientation();
}


}
}
}
