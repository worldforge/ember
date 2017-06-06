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
#include "components/ogre/OgreInfo.h"
#include <OgreNode.h>
#include <OgreTagPoint.h>

namespace Ember {
namespace OgreView {
namespace Model {
ModelBoneProvider::ModelBoneProvider(Model& parentModel, const std::string& attachPointName, bool deleteMovableWhenDone) :
        mParentModel(parentModel), mAttachPointName(attachPointName), mParent(nullptr), mPosition(Ogre::Vector3::ZERO), mOrientation(Ogre::Quaternion::IDENTITY), mAttachPointWrapper(0), mAttachPointDefinition(0), mNode(0), mDeleteMovableWhenDone(deleteMovableWhenDone) {
//    init();
}

ModelBoneProvider::ModelBoneProvider(Model& parentModel, const std::string& attachPointName, ModelBoneProvider* parent) :
        mParentModel(parentModel), mAttachPointName(attachPointName), mParent(parent), mPosition(Ogre::Vector3::ZERO), mOrientation(Ogre::Quaternion::IDENTITY), mAttachPointWrapper(0), mAttachPointDefinition(0), mNode(0), mDeleteMovableWhenDone(false) {
//    //Check if the attached object is the same for this new instance as for its parent. If so, it's a "scale node".
//    //If so, we should just transfer the ownership to the new instance.
//    if (parent->mAttachedModel == mAttachedModel) {
//        parent->mAttachedModel = 0;
//        mDeleteMovableWhenDone = parent->mDeleteMovableWhenDone;
//        parent->mDeleteMovableWhenDone = false;
//        mAttachPointWrapper = parent->mAttachPointWrapper;
//        mAttachPointDefinition = new AttachPointDefinition(*parent->mAttachPointDefinition);
//        parent->mAttachPointWrapper = 0;
//        mNode = parent->mNode;
//    } else {
//        init();
//    }
}

ModelBoneProvider::~ModelBoneProvider() {
    if (mParent) {
        ModelBoneProviderStore::iterator I = std::find(mParent->mChildren.begin(), mParent->mChildren.end(), this);
        if (I != mParent->mChildren.end()) {
            mParent->mChildren.erase(I);
        }
    }
    delete mAttachPointWrapper;
    delete mAttachPointDefinition;
}

Ogre::Node& ModelBoneProvider::getNode() const {
    if (mNode) {
        return *mNode;
    }
    throw Ember::Exception("Trying to get node for non existing bone attachment. This should never happen and represents an error in the code.");
}

Ogre::Node* ModelBoneProvider::getParentNode() const {
    return nullptr;
}

INodeProvider* ModelBoneProvider::createChildProvider(const std::string& name) {
    ModelBoneProvider* childProvider = new ModelBoneProvider(mParentModel, mAttachPointName, this);
    mChildren.push_back(childProvider);
    return childProvider;
}

void ModelBoneProvider::setVisible(bool visible) {
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
    if (mAttachPointDefinition && !mParent) {
        mOrientation = orientation * mAttachPointDefinition->Rotation;
        mPosition = position + mAttachPointDefinition->Translation;
    } else {
        mOrientation = orientation;
        mPosition = position;
    }
    updatePositionAndOrientation();
}

void ModelBoneProvider::updatePositionAndOrientation() {
    if (mAttachPointWrapper && mAttachPointWrapper->TagPoint) {
        mAttachPointWrapper->TagPoint->setPosition(getDerivedPosition());
        mAttachPointWrapper->TagPoint->setOrientation(getDerivedOrientation());
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

Model::AttachPointWrapper* ModelBoneProvider::getAttachPointWrapper() {
    return mAttachPointWrapper;
}

void ModelBoneProvider::detachObject(Ogre::MovableObject* movable) {
    for (auto I = mTagPoints.begin(); I != mTagPoints.end(); I++) {
        Ogre::TagPoint* tagPoint = *I;
        if (movable == tagPoint->getChildObject()) {
            mTagPoints.erase(I);
        }
    }
    mParentModel.detachObject(movable);
}

void ModelBoneProvider::attachObject(Ogre::MovableObject* movable) {
    Ogre::TagPoint* tagPoint = mParentModel.attachObject(mAttachPointName, movable);
    if (tagPoint) {
        mTagPoints.insert(tagPoint);
    }
}

}
}
}
