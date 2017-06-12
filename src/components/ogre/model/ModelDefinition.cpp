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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <framework/MainLoopController.h>
#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"

namespace Ember {
namespace OgreView {

namespace Model {

ModelDefinition::ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader) :
		Resource(creator, name, handle, group, isManual, loader),
		mRenderingDistance(0.0f),
		mUseScaleOf(UseScaleOf::MODEL_ALL),
		mScale(1),
		mRotation(Ogre::Quaternion::IDENTITY),
		mContentOffset(Ogre::Vector3::ZERO),
		mShowContained(true),
		mTranslate(0, 0, 0),
		mIsValid(false),
		mRenderingDef(nullptr),
		mBackgroundLoader(nullptr),
		mAssetsLoaded(false),
		mActive(new bool) {
	*mActive.get() = true;
	if (createParamDictionary("ModelDefinition")) {
		// no custom params
	}
}

ModelDefinition::~ModelDefinition() {
	*mActive.get() = false;
	for (SubModelDefinitionsStore::iterator I = mSubModels.begin(); I != mSubModels.end(); ++I) {
		delete *I;
	}
	for (ActionDefinitionsStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		delete *I;
	}
	for (ViewDefinitionStore::iterator I = mViews.begin(); I != mViews.end(); ++I) {
		delete I->second;
	}
	for (BoneGroupDefinitionStore::iterator I = mBoneGroups.begin(); I != mBoneGroups.end(); ++I) {
		delete I->second;
	}
	delete mRenderingDef;
	// have to call this here rather than in Resource destructor
	// since calling virtual methods in base destructors causes crashes
	unload();
}

void ModelDefinition::loadImpl(void) {
}

void ModelDefinition::addModelInstance(Model* model) {
	mModelInstances.insert(model);
}

void ModelDefinition::removeModelInstance(Model* model) {
	mModelInstances.erase(model);
}

void ModelDefinition::unloadImpl(void) {
}

bool ModelDefinition::isValid(void) const {
	return mIsValid;
}

bool ModelDefinition::requestLoad(Model* model) {
	if (mAssetsLoaded) {
		model->createActualModel();
		return true;
	} else {
		mLoadingListeners.insert(model);
		if (!mBackgroundLoader) {
			mBackgroundLoader = new ModelBackgroundLoader(*this);
			return mBackgroundLoader->poll();
		}
		return false;
	}
}

void ModelDefinition::notifyAssetsLoaded() {
	mAssetsLoaded = true;
	MainLoopController::getSingleton().getEventService().runOnMainThread([this]() {
		if (!mLoadingListeners.empty()) {
			auto I = mLoadingListeners.begin();
			Model* model = *I;
			model->reload();
			mLoadingListeners.erase(I);
			if (!mLoadingListeners.empty()) {
				notifyAssetsLoaded();
			}
		}
	}, mActive);
}

void ModelDefinition::removeFromLoadingQueue(Model* model) {
	mLoadingListeners.erase(model);
}


ViewDefinition* ModelDefinition::createViewDefinition(const std::string& viewname) {
	ViewDefinitionStore::iterator view = mViews.find(viewname);
	if (view != mViews.end()) {
		return view->second;
	} else {
		ViewDefinition* def = new ViewDefinition();
		def->Name = viewname;
		def->Distance = 0;
		def->Rotation = Ogre::Quaternion::IDENTITY;
		mViews.insert(ViewDefinitionStore::value_type(viewname, def));
		return def;
	}
}

const ViewDefinitionStore& ModelDefinition::getViewDefinitions() const {
	return mViews;
}

void ModelDefinition::removeViewDefinition(const std::string& name) {
	mViews.erase(name);
}

BoneGroupDefinition* ModelDefinition::createBoneGroupDefinition(const std::string& name) {
	BoneGroupDefinitionStore::iterator group = mBoneGroups.find(name);
	if (group != mBoneGroups.end()) {
		return group->second;
	} else {
		BoneGroupDefinition* def = new BoneGroupDefinition();
		def->Name = name;
		mBoneGroups.insert(std::make_pair(name, def));
		return def;
	}
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

void ModelDefinition::setRotation(const Ogre::Quaternion rotation) {
	mRotation = rotation;
}

const RenderingDefinition* ModelDefinition::getRenderingDefinition() const {
	return mRenderingDef;
}

void ModelDefinition::reloadAllInstances() {
	for (auto& model : mModelInstances) {
		model->reload();
	}
}

SubModelDefinition* ModelDefinition::createSubModelDefinition(const std::string& meshname) {
	SubModelDefinition* def = new SubModelDefinition(meshname, *this);
	mSubModels.push_back(def);
	return def;
}

const std::vector<SubModelDefinition*>& ModelDefinition::getSubModelDefinitions() const {
	return mSubModels;
}

void ModelDefinition::removeSubModelDefinition(SubModelDefinition* def) {
	ModelDefinition::removeDefinition(def, mSubModels);
}

ActionDefinition* ModelDefinition::createActionDefinition(const std::string& actionname) {
	ActionDefinition* def = new ActionDefinition(actionname);
	mActions.push_back(def);
	return def;
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
	for (AttachPointDefinitionStore::iterator I = mAttachPoints.begin(); I != mAttachPoints.end(); ++I) {
		if (I->Name == definition.Name) {
			(*I) = definition;
			return;
		}
	}
	mAttachPoints.push_back(definition);
}

void ModelDefinition::removeActionDefinition(ActionDefinition* def) {
	ModelDefinition::removeDefinition(def, mActions);
}

template<typename T, typename T1>
void ModelDefinition::removeDefinition(T* def, T1& store) {
	typename T1::iterator I = std::find(store.begin(), store.end(), def);
	if (I != store.end()) {
		store.erase(I);
	}
}

SubModelDefinition::SubModelDefinition(const std::string& meshname, ModelDefinition& modelDef) :
		mMeshName(meshname), mModelDef(modelDef) {
}

SubModelDefinition::~SubModelDefinition() {
	for (std::vector<PartDefinition*>::iterator I = mParts.begin(); I != mParts.end(); ++I) {
		delete *I;
	}
}

const ModelDefinition& SubModelDefinition::getModelDefinition() const {
	return mModelDef;
}

const std::string& SubModelDefinition::getMeshName() const {
	return mMeshName;
}

PartDefinition* SubModelDefinition::createPartDefinition(const std::string& partname) {
	PartDefinition* def = new PartDefinition(partname, *this);
	mParts.push_back(def);
	return def;
}

const std::vector<PartDefinition*>& SubModelDefinition::getPartDefinitions() const {
	return mParts;
}

void SubModelDefinition::removePartDefinition(PartDefinition* def) {
	ModelDefinition::removeDefinition(def, mParts);
}

PartDefinition::PartDefinition(const std::string& name, SubModelDefinition& subModelDef) :
		mName(name), mShow(true), mSubModelDef(subModelDef) {
}

PartDefinition::~PartDefinition() {
	for (std::vector<SubEntityDefinition*>::iterator I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		delete *I;
	}
}

const SubModelDefinition& PartDefinition::getSubModelDefinition() const {
	return mSubModelDef;
}

void PartDefinition::setName(const std::string& name) {
	mName = name;
}

const std::string& PartDefinition::getName() const {
	return mName;
}

void PartDefinition::setGroup(const std::string& group) {
	mGroup = group;
}

const std::string& PartDefinition::getGroup() const {
	return mGroup;
}

void PartDefinition::setShow(bool show) {
	mShow = show;
}

bool PartDefinition::getShow() const {
	return mShow;
}

SubEntityDefinition* PartDefinition::createSubEntityDefinition(const std::string& subEntityName) {
	SubEntityDefinition* def = new SubEntityDefinition(subEntityName, *this);
	mSubEntities.push_back(def);
	return def;

}

SubEntityDefinition* PartDefinition::createSubEntityDefinition(unsigned int subEntityIndex) {
	SubEntityDefinition* def = new SubEntityDefinition(subEntityIndex, *this);
	mSubEntities.push_back(def);
	return def;
}

const std::vector<SubEntityDefinition*>& PartDefinition::getSubEntityDefinitions() const {
	return mSubEntities;
}

void PartDefinition::removeSubEntityDefinition(SubEntityDefinition* def) {
	ModelDefinition::removeDefinition(def, mSubEntities);
}

SubEntityDefinition::SubEntityDefinition(unsigned int subEntityIndex, PartDefinition& partdef) :
		mSubEntityIndex(subEntityIndex), mPartDef(partdef) {
}

SubEntityDefinition::SubEntityDefinition(const std::string& subEntityName, PartDefinition& partdef) :
		mSubEntityName(subEntityName), mSubEntityIndex(0), mPartDef(partdef) {
}

const PartDefinition& SubEntityDefinition::getPartDefinition() const {
	return mPartDef;
}

const std::string& SubEntityDefinition::getSubEntityName() const {
	return mSubEntityName;
}

unsigned int SubEntityDefinition::getSubEntityIndex() const {
	return mSubEntityIndex;
}

const std::string& SubEntityDefinition::getMaterialName() const {
	return mMaterialName;
}

void SubEntityDefinition::setMaterialName(const std::string& materialName) {
	mMaterialName = materialName;
}

AnimationDefinition::AnimationDefinition(int iterations) :
		mIterations(iterations) {
}

AnimationDefinition::~AnimationDefinition() {
	for (AnimationPartDefinitionsStore::iterator I = mAnimationParts.begin(); I != mAnimationParts.end(); ++I) {
		delete *I;
	}
}

AnimationPartDefinition* AnimationDefinition::createAnimationPartDefinition(const std::string& ogreAnimationName) {
	AnimationPartDefinition* def = new AnimationPartDefinition();
	def->Name = ogreAnimationName;
	mAnimationParts.push_back(def);
	return def;
}

const AnimationPartDefinitionsStore& AnimationDefinition::getAnimationPartDefinitions() const {
	return mAnimationParts;
}

void AnimationDefinition::removeAnimationPartDefinition(AnimationPartDefinition* def) {
	ModelDefinition::removeDefinition(def, mAnimationParts);
}

void AnimationDefinition::setIterations(int iterations) {
	mIterations = iterations;
}

ActionDefinition::ActionDefinition(const std::string& name) :
		mName(name), mAnimationSpeed(1.0) {
}

ActionDefinition::~ActionDefinition() {
	for (AnimationDefinitionsStore::iterator I = mAnimations.begin(); I != mAnimations.end(); ++I) {
		delete *I;
	}
	for (SoundDefinitionsStore::iterator I = mSounds.begin(); I != mSounds.end(); ++I) {
		delete *I;
	}
}

AnimationDefinition* ActionDefinition::createAnimationDefinition(int iterations) {
	AnimationDefinition* def = new AnimationDefinition(iterations);
	mAnimations.push_back(def);
	return def;
}

const AnimationDefinitionsStore& ActionDefinition::getAnimationDefinitions() const {
	return mAnimations;
}

AnimationDefinitionsStore& ActionDefinition::getAnimationDefinitions() {
	return mAnimations;
}

void ActionDefinition::removeAnimationDefinition(AnimationDefinition* def) {
	ModelDefinition::removeDefinition(def, mAnimations);
}

SoundDefinition* ActionDefinition::createSoundDefinition(const std::string& groupName, unsigned int play) {
	SoundDefinition* def = new SoundDefinition();
	def->groupName = groupName;
	def->playOrder = play;

	mSounds.push_back(def);
	return def;
}

const SoundDefinitionsStore& ActionDefinition::getSoundDefinitions() const {
	return mSounds;
}

SoundDefinitionsStore& ActionDefinition::getSoundDefinitions() {
	return mSounds;
}

void ActionDefinition::removeSoundDefinition(SoundDefinition* def) {
	ModelDefinition::removeDefinition(def, mSounds);
}

void ActionDefinition::createActivationDefinition(const ActivationDefinition::Type& type, const std::string& trigger) {
	ActivationDefinition def;
	def.type = type;
	def.trigger = trigger;

	mActivations.push_back(std::move(def));
}

const ActivationDefinitionStore& ActionDefinition::getActivationDefinitions() const {
	return mActivations;
}

ActivationDefinitionStore& ActionDefinition::getActivationDefinitions() {
	return mActivations;
}

const std::string& ActionDefinition::getName() const {
	return mName;
}

const std::string& RenderingDefinition::getScheme() const {
	return mScheme;
}

void RenderingDefinition::setScheme(const std::string& scheme) {
	mScheme = scheme;
}

const StringParamStore& RenderingDefinition::getParameters() const {
	return mParams;
}

}
}
}
