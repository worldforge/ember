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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "ModelDefinition.h"
#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"

namespace EmberOgre {

namespace Model {

ModelDefinition::ModelDefinition(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
    const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader) 
    : Resource(creator, name, handle, group, isManual, loader)
    , mRenderingDistance(0.0f)
    , mUseScaleOf(MODEL_ALL)
    , mScale(0)
    , mRotation(Ogre::Quaternion::IDENTITY)
    , mContentOffset(Ogre::Vector3::ZERO)
    , mShowContained(true)
    , mTranslate(0,0,0)
    , mIsValid(false)
    , mRenderingDef(0)
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
	for (ViewDefinitionStore::iterator I = mViews.begin(); I != mViews.end(); ++I) {
		delete I->second;
	}
	delete mRenderingDef;
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

ViewDefinition* ModelDefinition::createViewDefinition(const std::string& viewname)
{
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

const ViewDefinitionStore& ModelDefinition::getViewDefinitions()
{
	return mViews;
}

void ModelDefinition::removeViewDefinition(const std::string name)
{
	mViews.erase(name);
// 	for (ViewDefinitionStore::iterator I = mViews.begin(); I != mViews.end(); ++I) {
// 		if (I->second == def) {
// 		
// 		}
// 	}
	//ViewDefinitionStore::iterator I = 
// 	ModelDefinition::removeDefinition(def, mViews);
/*	SubModelDefinitionsStore::iterator I;
	for (I = mSubModels.begin(); I != mSubModels.end(); ++I) {
		if (*I == def)
			break;
	}
	if (I != mSubModels.end()) {
		mSubModels.erase(I);
	}*/
}



const Ogre::Vector3& ModelDefinition::getTranslate() const
{
	return mTranslate;
}
	
void ModelDefinition::setTranslate(const Ogre::Vector3 translate)
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

const RenderingDefinition* ModelDefinition::getRenderingDefinition() const
{
	return mRenderingDef;
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

const AttachPointDefinitionStore& ModelDefinition::getAttachPointsDefinitions()
{
	return mAttachPoints;
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
	typename T1::iterator I = std::find(store.begin(), store.end(), def);
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

void PartDefinition::setGroup(const std::string& group)
{
	mGroup = group;
}

const std::string& PartDefinition::getGroup() const
{
	return mGroup;
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


AnimationDefinition::AnimationDefinition(int iterations) : mIterations(iterations)
{
}

AnimationDefinition::~AnimationDefinition()
{
	for (AnimationPartDefinitionsStore::iterator I = mAnimationParts.begin(); I != mAnimationParts.end(); ++I) {
		delete *I;
	}
}


AnimationPartDefinition* AnimationDefinition::createAnimationPartDefinition(const std::string& ogreAnimationName, Ogre::Real weight)
{
	AnimationPartDefinition* def = new AnimationPartDefinition();
	def->Name = ogreAnimationName;
	def->Weight = weight;
	mAnimationParts.push_back(def);
	return def;
}

const AnimationPartDefinitionsStore& AnimationDefinition::getAnimationPartDefinitions()
{
	return mAnimationParts;
}

void AnimationDefinition::removeAnimationPartDefinition(AnimationPartDefinition* def)
{
	ModelDefinition::removeDefinition(def, mAnimationParts);
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

AnimationDefinition* ActionDefinition::createAnimationDefinition(int iterations)
{
	AnimationDefinition* def = new AnimationDefinition(iterations);
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


SoundDefinition* ActionDefinition::createSoundDefinition(const std::string& groupName, unsigned int play)
{
	SoundDefinition* def = new SoundDefinition();
	def->groupName = groupName;
	def->playOrder = play;

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

const std::string& RenderingDefinition::getScheme() const
{
	return mScheme;
}
void RenderingDefinition::setScheme(const std::string& scheme)
{
	mScheme = scheme;
}
const StringParamStore& RenderingDefinition::getParameters() const
{
	return mParams;
}



ModelDefnPtr::ModelDefnPtr(const Ogre::ResourcePtr& r) : Ogre::SharedPtr<ModelDefinition>()
{
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		pRep = static_cast<ModelDefinition*>(r.getPointer());
		pUseCount = r.useCountPointer();
		if (pUseCount)
		{
			++(*pUseCount);
		}
	}
}
//-----------------------------------------------------------------------
ModelDefnPtr& ModelDefnPtr::operator=(const Ogre::ResourcePtr& r)
{
	if (pRep == static_cast<ModelDefinition*>(r.getPointer()))
		return *this;
	release();
	// lock & copy other mutex pointer
	OGRE_MUTEX_CONDITIONAL(r.OGRE_AUTO_MUTEX_NAME)
	{
		OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
		OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
		pRep = static_cast<ModelDefinition*>(r.getPointer());
		pUseCount = r.useCountPointer();
		if (pUseCount)
		{
			++(*pUseCount);
		}
	}
	else
	{
		// RHS must be a null pointer
		assert(r.isNull() && "RHS must be null if it has no mutex!");
		setNull();
	}
	return *this;
}
    //-----------------------------------------------------------------------
//     void ModelDefnPtr::destroy(void)
//     {
//         // We're only overriding so that we can destroy after full definition of Mesh
//         Ogre::SharedPtr<ModelDefinition>::destroy();
//     }


}
}
