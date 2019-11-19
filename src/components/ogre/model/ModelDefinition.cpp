//
// C++ Implementation: ModelDefinition
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
// Copyright (c) 2005 The Cataclysmos Team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framework/MainLoopController.h"
#include "ModelDefinition.h"

#include <utility>
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"


namespace Ember {
namespace OgreView {

namespace Model {

ModelDefinition::ModelDefinition() :
		mRenderingDistance(0.0f),
		mUseScaleOf(UseScaleOf::MODEL_ALL),
		mScale(1),
		mRotation(Ogre::Quaternion::IDENTITY),
		mContentOffset(Ogre::Vector3::ZERO),
		mShowContained(true),
		mTranslate(0, 0, 0),
		mIsValid(false),
		mAssetsLoaded(false) {

}

ModelDefinition::~ModelDefinition() = default;


void ModelDefinition::addModelInstance(Model* model) {
	mModelInstances.insert(model);
}

void ModelDefinition::removeModelInstance(Model* model) {
	mModelInstances.erase(model);
}

bool ModelDefinition::isValid() const {
	return mIsValid;
}

bool ModelDefinition::requestLoad(Model* model) {
	if (mAssetsLoaded) {
		bool result = model->loadAssets();
		if (!result) {
			mLoadingListeners.insert(model);
			MainLoopController::getSingleton().getEventService().runOnMainThread([this]() {
				reloadModels();
			}, mActive);
		}
		return result;
	}

	mLoadingListeners.insert(model);
	//If there's already a background loader, it's already in a "poll" loop, so we don't need to do anything.
	if (!mBackgroundLoader) {
		mBackgroundLoader = std::make_unique<ModelBackgroundLoader>(*this);
		return mBackgroundLoader->poll();
	}
	return false;
}

void ModelDefinition::notifyAssetsLoaded() {
	mAssetsLoaded = true;
	MainLoopController::getSingleton().getEventService().runOnMainThread([this]() {
		reloadModels();
	}, mActive);
}

void ModelDefinition::reloadModels() {
	if (!mLoadingListeners.empty()) {
		auto I = mLoadingListeners.begin();
		Model* model = *I;
		if (model->loadAssets()) {
			mLoadingListeners.erase(I);
		}
		if (!mLoadingListeners.empty()) {
			MainLoopController::getSingleton().getEventService().runOnMainThread([this]() {
				reloadModels();
			}, mActive);
		}
	}
}


void ModelDefinition::removeFromLoadingQueue(Model* model) {
	mLoadingListeners.erase(model);
}

void ModelDefinition::addViewDefinition(std::string name, ViewDefinition def) {
	mViews.emplace(std::move(name), std::move(def));
}

const ViewDefinitionStore& ModelDefinition::getViewDefinitions() const {
	return mViews;
}

void ModelDefinition::removeViewDefinition(const std::string& name) {
	mViews.erase(name);
}

void ModelDefinition::addBoneGroupDefinition(std::string name, BoneGroupDefinition def) {
	mBoneGroups.emplace(std::move(name), std::move(def));
}

void ModelDefinition::removeBoneGroupDefinition(const std::string& name) {
	mBoneGroups.erase(name);
}

const BoneGroupDefinitionStore& ModelDefinition::getBoneGroupDefinitions() const {
	return mBoneGroups;
}

const PoseDefinitionStore& ModelDefinition::getPoseDefinitions() const {
	return mPoseDefinitions;
}

void ModelDefinition::addPoseDefinition(const std::string& name, const PoseDefinition& definition) {
	mPoseDefinitions[name] = definition;
}

void ModelDefinition::removePoseDefinition(const std::string& name) {
	mPoseDefinitions.erase(name);
}

const Ogre::Vector3& ModelDefinition::getTranslate() const {
	return mTranslate;
}

void ModelDefinition::setTranslate(const Ogre::Vector3 translate) {
	mTranslate = translate;
}

bool ModelDefinition::getShowContained() const {
	return mShowContained;
}

void ModelDefinition::setShowContained(bool show) {
	mShowContained = show;
}

const Ogre::Quaternion& ModelDefinition::getRotation() const {
	return mRotation;
}

void ModelDefinition::setRotation(const Ogre::Quaternion& rotation) {
	mRotation = rotation;
}

const RenderingDefinition* ModelDefinition::getRenderingDefinition() const {
	return mRenderingDef.get();
}

void ModelDefinition::reloadAllInstances() {
	for (auto& model : mModelInstances) {
		model->reload();
	}
}

void ModelDefinition::addSubModelDefinition(SubModelDefinition def) {
	mSubModels.emplace_back(std::move(def));
}


const std::vector<SubModelDefinition>& ModelDefinition::getSubModelDefinitions() const {
	return mSubModels;
}

void ModelDefinition::removeSubModelDefinition(size_t index) {
	mSubModels.erase(mSubModels.begin() + index);
}

void ModelDefinition::addActionDefinition(ActionDefinition def) {
	mActions.emplace_back(std::move(def));
}


const ActionDefinitionsStore& ModelDefinition::getActionDefinitions() const {
	return mActions;
}

ActionDefinitionsStore& ModelDefinition::getActionDefinitions() {
	return mActions;
}


const AttachPointDefinitionStore& ModelDefinition::getAttachPointsDefinitions() const {
	return mAttachPoints;
}

void ModelDefinition::addAttachPointDefinition(const AttachPointDefinition& definition) {
	for (auto& mAttachPoint : mAttachPoints) {
		if (mAttachPoint.Name == definition.Name) {
			mAttachPoint = definition;
			return;
		}
	}
	mAttachPoints.push_back(definition);
}

void ModelDefinition::removeActionDefinition(size_t index) {
	mActions.erase(mActions.begin() + index);
}

template<typename T, typename T1>
void ModelDefinition::removeDefinition(T* def, T1& store) {
	auto I = std::find(store.begin(), store.end(), def);
	if (I != store.end()) {
		store.erase(I);
	}
}

void ModelDefinition::setOrigin(std::string origin) {
	mOrigin = std::move(origin);
}

const std::string& ModelDefinition::getOrigin() const {
	return mOrigin;
}

void ModelDefinition::moveFrom(ModelDefinition&& rhs) {
	//We need to preserve the instances.
	rhs.mModelInstances = std::move(mModelInstances);
	*this = std::move(rhs);
}

void SubModelDefinition::addPartDefinition(PartDefinition partDefinition) {
	parts.emplace_back(std::move(partDefinition));
}

const std::vector<PartDefinition>& SubModelDefinition::getPartDefinitions() const {
	return parts;
}

void SubModelDefinition::removePartDefinition(size_t index) {
	parts.erase(parts.begin() + index);
}

void PartDefinition::addSubEntityDefinition(SubEntityDefinition def) {
	subEntities.emplace_back(std::move(def));
}

const std::vector<SubEntityDefinition>& PartDefinition::getSubEntityDefinitions() const {
	return subEntities;
}

void PartDefinition::removeSubEntityDefinition(size_t index) {
	subEntities.erase(subEntities.begin() + index);
}

void AnimationDefinition::addAnimationPartDefinition(AnimationPartDefinition def) {
	animationParts.emplace_back(std::move(def));
}

const AnimationPartDefinitionsStore& AnimationDefinition::getAnimationPartDefinitions() const {
	return animationParts;
}

AnimationPartDefinitionsStore& AnimationDefinition::getAnimationPartDefinitions() {
	return animationParts;
}

void AnimationDefinition::removeAnimationPartDefinition(size_t index) {
	animationParts.erase(animationParts.begin() + index);
}


ActionDefinition::~ActionDefinition() = default;

void ActionDefinition::addAnimationDefinition(AnimationDefinition def) {
	animations.emplace_back(std::move(def));
}


const AnimationDefinitionsStore& ActionDefinition::getAnimationDefinitions() const {
	return animations;
}

AnimationDefinitionsStore& ActionDefinition::getAnimationDefinitions() {
	return animations;
}

void ActionDefinition::removeAnimationDefinition(size_t index) {
	animations.erase(animations.begin() + index);
}

void ActionDefinition::addSoundDefinition(SoundDefinition def) {
	sounds.emplace_back(std::move(def));
}

const SoundDefinitionsStore& ActionDefinition::getSoundDefinitions() const {
	return sounds;
}

SoundDefinitionsStore& ActionDefinition::getSoundDefinitions() {
	return sounds;
}

void ActionDefinition::removeSoundDefinition(size_t index) {
	sounds.erase(sounds.begin() + index);
}

const ActivationDefinitionStore& ActionDefinition::getActivationDefinitions() const {
	return activations;
}

ActivationDefinitionStore& ActionDefinition::getActivationDefinitions() {
	return activations;
}


}
}
}
