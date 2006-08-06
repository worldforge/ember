//
// C++ Implementation: ModelDefinition
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2004
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


#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"

namespace EmberOgre {

namespace Model {

ModelDefinition::ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader) 
    : Resource(creator, name, handle, group, isManual, loader), mScale(0), mUseScaleOf(MODEL_ALL), mRotation(Ogre::Quaternion::IDENTITY), mIsValid(false), mTranslate(0,0,0), mShowContained(true), mContentOffset(Ogre::Vector3::ZERO), mRenderingDistance(0.0f)
{
    if (createParamDictionary("ModelDefinition"))
    {
        // no custom params
    }
}

ModelDefinition::~ModelDefinition()
{
	for (SubModelDefinitionsStore::iterator I = mSubModels.begin(); I != mSubModels.end(); ++I) {
		delete *I;
	}
	for (ActionDefinitionsStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
		delete *I;
	}
//	delete(mContentOffset);
    // have to call this here reather than in Resource destructor
    // since calling virtual methods in base destructors causes crash
    unload(); 
}

void ModelDefinition::loadImpl(void)
{
//     XMLModelDefinitionSerializer serializer;
// 
// 	Ogre::String filename = mName + ".modeldef.xml";
// 
// 	S_LOG_INFO( "ModelDefinition: Loading " + filename );
// 
// 	Ogre::DataStreamPtr stream = 
// 		Ogre::ResourceGroupManager::getSingleton().openResource(filename, mGroup);
// 
//     serializer.importModelDefinition(stream, this);
// 
// 	mIsValid = true;
// 	mIsLoaded = true;

}


void ModelDefinition::addModelInstance(Model* model)
{
	mModelInstances[model->getName()] = model;
}

void ModelDefinition::removeModelInstance(Model* model)
{
	mModelInstances.erase(model->getName());
}

void ModelDefinition::unloadImpl(void)
{
}

bool ModelDefinition::isValid(void)
{
	return mIsValid;
}
	
const Ogre::Vector3& ModelDefinition::getTranslate() const
{
	return mTranslate;
}
	
void  ModelDefinition::setTranslate(const Ogre::Vector3 translate)
{
	mTranslate = translate;
}
	
	
bool ModelDefinition::getShowContained() const
{
	return mShowContained;
}

void ModelDefinition::getShowContained(bool show)
{
	mShowContained = show;
}

	

const Ogre::Quaternion& ModelDefinition::getRotation() const
{
	return mRotation;
}

void ModelDefinition::setRotation(const Ogre::Quaternion rotation)
{
	mRotation = rotation;
}

void ModelDefinition::reloadAllInstances()
{
	for (ModelInstanceStore::iterator I = mModelInstances.begin(); I != mModelInstances.end(); ++I) {
		I->second->reload();
	}
//	std::for_each(mModelInstances.begin(), mModelInstances.end(), std::mem_fun(&EmberOgre::Model::Model::reload));
}

	
SubModelDefinition* ModelDefinition::createSubModelDefinition(const std::string& meshname)
{
	SubModelDefinition* def = new SubModelDefinition(meshname, *this);
	mSubModels.push_back(def);
	return def;
}

const std::vector<SubModelDefinition*>& ModelDefinition::getSubModelDefinitions()
{
	return mSubModels;
}

void ModelDefinition::removeSubModelDefinition(SubModelDefinition* def)
{
	ModelDefinition::removeDefinition(def, mSubModels);
/*	SubModelDefinitionsStore::iterator I;
	for (I = mSubModels.begin(); I != mSubModels.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != mSubModels.end()) {
		mSubModels.erase(I);
	}*/
}

ActionDefinition* ModelDefinition::createActionDefinition(const std::string& actionname)
{
	ActionDefinition* def = new ActionDefinition(actionname);
	mActions.push_back(def);
	return def;
}

const ActionDefinitionsStore& ModelDefinition::getActionDefinitions()
{
	return mActions;
}

void ModelDefinition::removeActionDefinition(ActionDefinition* def)
{
	ModelDefinition::removeDefinition(def, mActions);
/*	SubModelDefinitionsStore::iterator I;
	for (I = mSubModels.begin(); I != mSubModels.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != mSubModels.end()) {
		mSubModels.erase(I);
	}*/
}


template <typename T, typename T1>
void ModelDefinition::removeDefinition(T* def, T1& store) 
{
	typename T1::iterator I;
	for (I = store.begin(); I != store.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != store.end()) {
		store.erase(I);
	}
}








SubModelDefinition::SubModelDefinition(const std::string& meshname, ModelDefinition& modelDef)
: mMeshName(meshname), mModelDef(modelDef)
{
}

SubModelDefinition::~SubModelDefinition()
{
	for (std::vector<PartDefinition*>::iterator I = mParts.begin(); I != mParts.end(); ++I) {
		delete *I;
	}
}

ModelDefinition& SubModelDefinition::getModelDefinition()
{
	return mModelDef;
}

const std::string& SubModelDefinition::getMeshName() const
{
	return mMeshName;
}


PartDefinition* SubModelDefinition::createPartDefinition(const std::string& partname)
{
	PartDefinition* def = new PartDefinition(partname, *this);
	mParts.push_back(def);
	return def;
	
}
const std::vector<PartDefinition*>& SubModelDefinition::getPartDefinitions()
{
	return mParts;
}

void SubModelDefinition::removePartDefinition(PartDefinition* def)
{
	ModelDefinition::removeDefinition(def, mParts);
/*	std::vector<PartDefinition*>::iterator I;
	for (I = mParts.begin(); I != mParts.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != mParts.end()) {
		mParts.erase(I);
	}*/
}




PartDefinition::PartDefinition(const std::string& name, SubModelDefinition& subModelDef) : mName(name), mSubModelDef(subModelDef)
{
}

PartDefinition::~PartDefinition()
{
	for (std::vector<SubEntityDefinition*>::iterator I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		delete *I;
	}
}

SubModelDefinition& PartDefinition::getSubModelDefinition()
{
	return mSubModelDef;
}

void PartDefinition::setName(const std::string& name)
{
	mName = name;
}

const std::string& PartDefinition::getName() const
{
	return mName;
}

void PartDefinition::setShow(bool show)
{
	mShow = show;
}
bool PartDefinition::getShow() const
{
	return mShow;
}

SubEntityDefinition* PartDefinition::createSubEntityDefinition(const std::string& subEntityName)
{
	SubEntityDefinition* def = new SubEntityDefinition(subEntityName, *this);
	mSubEntities.push_back(def);
	return def;

}

SubEntityDefinition* PartDefinition::createSubEntityDefinition(unsigned int subEntityIndex)
{
	SubEntityDefinition* def = new SubEntityDefinition(subEntityIndex, *this);
	mSubEntities.push_back(def);
	return def;
}

const std::vector<SubEntityDefinition*>& PartDefinition::getSubEntityDefinitions()
{
	return mSubEntities;
}
void PartDefinition::removeSubEntityDefinition(SubEntityDefinition* def)
{
	ModelDefinition::removeDefinition(def, mSubEntities);
/*	std::vector<SubEntityDefinition*>::iterator I;
	for (I = mSubEntities.begin(); I != mSubEntities.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != mSubEntities.end()) {
		mSubEntities.erase(I);
	}*/
}


SubEntityDefinition::SubEntityDefinition(unsigned int subEntityIndex, PartDefinition& partdef) : mSubEntityIndex(subEntityIndex), mPartDef(partdef)
{
}

SubEntityDefinition::SubEntityDefinition(const std::string& subEntityName, PartDefinition& partdef) : mSubEntityName(subEntityName), mPartDef(partdef)
{
/*	SubEntityDefinition* def = new SubEntityDefinition(subEntityName);
	mSubEntities.push_back(def);
	return def;*/
}

PartDefinition& SubEntityDefinition::getPartDefinition()
{
	return mPartDef;
}
const std::string& SubEntityDefinition::getSubEntityName() const
{
	return mSubEntityName;
}

unsigned int SubEntityDefinition::getSubEntityIndex() const
{
	return mSubEntityIndex;
}
//void setSubEntityName(const std::string&);

const std::string& SubEntityDefinition::getMaterialName() const
{
	return mMaterialName;
}

void SubEntityDefinition::setMaterialName(const std::string& materialName)
{
	mMaterialName = materialName;
}



ActionDefinition::ActionDefinition(const std::string& name) : mName(name), mAnimationSpeed(1.0)
{
}

ActionDefinition::~ActionDefinition()
{
	for (AnimationDefinitionsStore::iterator I = mAnimations.begin(); I != mAnimations.end(); ++I) {
		delete *I;
	}
	for (SoundDefinitionsStore::iterator I = mSounds.begin(); I != mSounds.end(); ++I) {
		delete *I;
	}

}

AnimationDefinition* ActionDefinition::createAnimationDefinition(const std::string& name, Ogre::Real weight)
{
	AnimationDefinition* def = new AnimationDefinition();
	def->Name = name;
	def->Weight = weight;
	mAnimations.push_back(def);
	return def;
}

const AnimationDefinitionsStore& ActionDefinition::getAnimationDefinitions()
{
	return mAnimations;
}

void ActionDefinition::removeAnimationDefinition(AnimationDefinition* def)
{
	ModelDefinition::removeDefinition(def, mAnimations);
}


SoundDefinition* ActionDefinition::createSoundDefinition(const std::string& name, bool repeat, Ogre::Real volume)
{
	SoundDefinition* def = new SoundDefinition();
	def->Name = name;
	def->Repeat = repeat;
	def->Volume = volume;
	mSounds.push_back(def);
	return def;
}

const SoundDefinitionsStore& ActionDefinition::getSoundDefinitions()
{
	return mSounds;
}

void ActionDefinition::removeSoundDefinition(SoundDefinition* def)
{
	ModelDefinition::removeDefinition(def, mSounds);
}

const std::string& ActionDefinition::getName() const
{
	return mName;
}


};
};
