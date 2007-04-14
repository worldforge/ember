/*
    Copyright (C) 2004  Erik Hjortsberg
	Copyright (c) 2005 The Cataclysmos Team

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


#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"
#include "AnimationSet.h"
#include "Action.h"
#include "ParticleSystem.h"
#include "ParticleSystemBinding.h"


#include "components/ogre/EmberOgre.h"
#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"


#include <OgreTagPoint.h>

namespace EmberOgre {
namespace Model {


const Ogre::String Model::sMovableType = "Model";
unsigned long Model::msAutoGenId = 0;

// Model::Model()
// : mScale(0)
// , mRotation(Ogre::Quaternion::IDENTITY)
// , mSkeletonInstance(0)
// // , mAnimationStateSet(0)
// , mSkeletonOwnerEntity(0)
// {
// 	std::stringstream ss;
// 	ss << "__AutogenModel_" << msAutoGenId++;
// 	mName = ss.str();
// 	mVisible = true;
// }

Model::Model(const std::string& name)
: mScale(0)
, mRotation(Ogre::Quaternion::IDENTITY)
, mSkeletonInstance(0)
// , mAnimationStateSet(0)
, mSkeletonOwnerEntity(0)
, Ogre::MovableObject(name)
{
 mVisible = true;
}
Model::~Model()
{
	resetSubmodels();
	resetParticles();	
	if (!_masterModel.isNull()) {
		_masterModel->removeModelInstance(this);
	}
}

void Model::reset()
{
	Resetting.emit();
//	resetAnimations();
	resetSubmodels();
	resetParticles();	
	mScale = 0;
	mRotation = Ogre::Quaternion::IDENTITY;
	mSkeletonInstance = 0;
// , mAnimationStateSet(0)
	mSkeletonOwnerEntity = 0;

}

void Model::reload()
{
//	resetAnimations();
/*	resetSubmodels();
	resetParticles();	*/
	reset();
	createFromDefn();
	//if we are attached, we have to nofify the new entities, else they won't appear in the scene
	if (mParentNode != 0) {
		Ogre::Node* theParent = mParentNode;
		_notifyAttached(0, false);
		_notifyAttached(theParent, mParentIsTagPoint);
	}
	Reloaded.emit();
}

bool Model::create(const std::string& modelType)
{
	if (!_masterModel.isNull() && _masterModel->isValid()) {
		S_LOG_WARNING("Trying to call create('" + modelType +  "') on a Model instance that already have been created as a '" + _masterModel->getName() + "'.");
		return false;
	}

	static const Ogre::String groupName("ModelDefinitions");
	//Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	try {
		_masterModel= ModelDefinitionManager::instance().load(modelType, groupName);
	} catch (const Ogre::Exception& ex) {
		S_LOG_FAILURE("Could not load model of type " << modelType << " from group " << groupName << ".\nMessage: " << ex.getFullDescription());
		return false;
	}
	if (false && !_masterModel->isValid()) {
		S_LOG_FAILURE("Model of type " << modelType << " from group " << groupName << " is not valid.");
		return false;
	}
	else
	{
		_masterModel->addModelInstance(this);
		return true;
/*		bool success =  createFromDefn();
		if (!success) {
			reset();
		}
		return success;*/
	}
}

void Model::_notifyManager(Ogre::SceneManager* man)
{
	Ogre::MovableObject::_notifyManager(man);
	bool success =  createFromDefn();
	if (!success) {
		reset();
	}
}



bool Model::createFromDefn()
{
	// create instance of model from definition
	Ogre::SceneManager* sceneManager = _getManager();
	assert(sceneManager);
	mScale=_masterModel->mScale ;
	mRotation = _masterModel->mRotation;
	setRenderingDistance(_masterModel->getRenderingDistance());
	
	std::vector<std::string> showPartVector;

/*	const SubModelDefinitionsStore&
	std::vector<ModelDefinition::SubModelDefinition>::iterator I_subModels = _masterModel->mSubModels.begin();
	std::vector<ModelDefinition::SubModelDefinition>::iterator I_subModels_end = _masterModel->mSubModels.end();*/
	S_LOG_VERBOSE("Number of submodels: " << _masterModel->getSubModelDefinitions().size()); 
	
	for (SubModelDefinitionsStore::const_iterator I_subModels = _masterModel->getSubModelDefinitions().begin(); I_subModels != _masterModel->getSubModelDefinitions().end(); ++I_subModels) 
	{
		std::string entityName = mName + "/" + (*I_subModels)->getMeshName();
		try {
// 			bool firstLoad = false;
// 			Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName((*I_subModels).Mesh);
// 			if (!mesh.isNull()) {
// 				if (!mesh->isLoaded()) {
// 					firstLoad = true;
// 				}
// 			} 
			Ogre::Entity* entity = sceneManager->createEntity(entityName, (*I_subModels)->getMeshName());
			if (_masterModel->getRenderingDistance()) {
				entity->setRenderingDistance(_masterModel->getRenderingDistance());
			}

// 			//for convenience, if it's a new mesh, check if there's a skeleton file in the same directory
// 			//if so, use that
// //			if (!entity->getMesh()->isLoaded()) {
// /*				std::string fileName;
// 				std::string path;
// 				Ogre::StringUtil::splitFilename((*I_subModels).Mesh, fileName, path);*/
// 			if (firstLoad) {
// 				std::string meshname = (*I_subModels).Mesh.substr(0, (*I_subModels).Mesh.size() - 5);
// 				
// /*				std::vector<Ogre::String> strings = Ogre::StringUtil::split((*I_subModels).Mesh, ".mesh");
// 				if (strings.size() > 0) {
// 					meshname = strings[0];*/
// 				entity->getMesh()->setSkeletonName(meshname + ".skeleton");
// 			}
// // 				}
// 				
// //			}

			SubModel* submodel = new SubModel(entity);
			//Model::SubModelPartMapping* submodelPartMapping = new Model::SubModelPartMapping();
				
			for (PartDefinitionsStore::const_iterator I_parts = (*I_subModels)->getPartDefinitions().begin(); I_parts != (*I_subModels)->getPartDefinitions().end(); ++I_parts) {
				SubModelPart* part = submodel->createSubModelPart((*I_parts)->getName());
				std::string groupName("");
				
				if ((*I_parts)->getSubEntityDefinitions().size() > 0)
				{
					for (SubEntityDefinitionsStore::const_iterator I_subEntities = (*I_parts)->getSubEntityDefinitions().begin(); I_subEntities != (*I_parts)->getSubEntityDefinitions().end(); ++I_subEntities) {
						Ogre::SubEntity* subEntity;
						//try with a submodelname first
						if ((*I_subEntities)->getSubEntityName() != "") {
							subEntity = entity->getSubEntity((*I_subEntities)->getSubEntityName());
						} else {
							//no name specified, use the index instead
							subEntity = entity->getSubEntity((*I_subEntities)->getSubEntityIndex());
						}
						part->addSubEntity(subEntity, *I_subEntities);
						
						if ((*I_subEntities)->getMaterialName() != "") {
							subEntity->setMaterialName((*I_subEntities)->getMaterialName());
						}
					}		
				} else {
					//if no subentities are defined, add all subentities
					unsigned int numSubEntities = entity->getNumSubEntities();
					for (unsigned int i = 0;i < numSubEntities; ++i) {
						part->addSubEntity(entity->getSubEntity(i), 0);
					}
				}
				if ((*I_parts)->getGroup() != "") {
					mGroupsToPartMap[(*I_parts)->getGroup()].push_back((*I_parts)->getName());
					//mPartToGroupMap[(*I_parts)->getName()] = (*I_parts)->getGroup();
				}
				
				if ((*I_parts)->getShow()) {
					showPartVector.push_back((*I_parts)->getName());
				}
				
				ModelPart& modelPart = mModelParts[(*I_parts)->getName()];
				modelPart.addSubModelPart(part);
				modelPart.setGroupName((*I_parts)->getGroup());
			}
			addSubmodel(submodel);
		
		} 
		catch (const Ogre::Exception& e) 
		{
			S_LOG_FAILURE( "Submodel load error for " + entityName + ". \nOgre error: " + e.getFullDescription());
			return false;
		}
	}

	createActions();
	
	createParticles();

	


	
	std::vector<std::string>::const_iterator I = showPartVector.begin();
	std::vector<std::string>::const_iterator I_end = showPartVector.end();
	for (;I != I_end; I++) {
		showPart(*I);	
	}
	return true;
}

void Model::createActions()
{
	ActionDefinitionsStore::const_iterator I_actions = _masterModel->getActionDefinitions().begin();
	ActionDefinitionsStore::const_iterator I_actions_end = _masterModel->getActionDefinitions().end();
	for (;I_actions != I_actions_end; ++I_actions) {
		//std::multiset< Model::AnimationPart* >* animationPartSet = new std::multiset< Model::AnimationPart* >();
		Action action;
		action.setName((*I_actions)->getName());
		action.getAnimations().setSpeed((*I_actions)->getAnimationSpeed());
		
		if (getSkeleton() && getAllAnimationStates()) {
			if (mSubmodels.size()) {
				AnimationDefinitionsStore::const_iterator I_anims = (*I_actions)->getAnimationDefinitions().begin();
				AnimationDefinitionsStore::const_iterator I_anims_end = (*I_actions)->getAnimationDefinitions().end();
				for (;I_anims != I_anims_end; ++I_anims) {
					Animation animation((*I_anims)->getIterations());
					AnimationPartDefinitionsStore::const_iterator I_animParts = (*I_anims)->getAnimationPartDefinitions().begin();
					AnimationPartDefinitionsStore::const_iterator I_animParts_end = (*I_anims)->getAnimationPartDefinitions().end();
					for (;I_animParts != I_animParts_end; ++I_animParts) {
						if (getAllAnimationStates()->hasAnimationState((*I_animParts)->Name)) {
							AnimationPart animPart;
							try {
								Ogre::AnimationState* state = getAnimationState((*I_animParts)->Name);
								animPart.state = state;
								animPart.weight = (*I_animParts)->Weight;
								animation.addAnimationPart(animPart);
							} catch (const Ogre::Exception& ex) {
								S_LOG_FAILURE("Error when loading animation: " << (*I_animParts)->Name << ".\n" + ex.getFullDescription() );
							}
						}
					}
					action.getAnimations().addAnimation(animation);
				}
			}
		}
		
		//TODO: add sounds too
		
		mActions[(*I_actions)->getName()] = action;
	}
}

void Model::createParticles()
{
	std::vector<ModelDefinition::ParticleSystemDefinition>::const_iterator I_particlesys = _masterModel->mParticleSystems.begin();
	std::vector<ModelDefinition::ParticleSystemDefinition>::const_iterator I_particlesys_end = _masterModel->mParticleSystems.end();
	for (;I_particlesys != I_particlesys_end; ++I_particlesys) {
		//first try to create the ogre particle system
		std::string name(mName + "/particle" + I_particlesys->Script);
		Ogre::ParticleSystem* ogreParticleSystem;
		try {		
			 ogreParticleSystem = _getManager()->createParticleSystem(name, I_particlesys->Script);
		} catch (const Ogre::Exception& ex) {
			S_LOG_FAILURE("Could not create particle system: " + name);
			std::cerr << ex.getFullDescription() + "\n";
			continue;
		}
		if (ogreParticleSystem) {
			//ogreParticleSystem->setDefaultDimensions(1, 1);
			ParticleSystem* particleSystem = new ParticleSystem(ogreParticleSystem);
			for (ModelDefinition::BindingSet::const_iterator I = I_particlesys->Bindings.begin(); I != I_particlesys->Bindings.end(); ++I) {
				ParticleSystemBinding* binding = particleSystem->addBinding(I->EmitterVar, I->AtlasAttribute);
				mAllParticleSystemBindings.push_back(binding);
			}
			mParticleSystems.push_back(particleSystem);
		}
		
	}
}


bool Model::hasParticles() const
{
	return mParticleSystems.size() > 0;
}

const ParticleSystemBindingsPtrSet& Model::getAllParticleSystemBindings() const
{
	return mAllParticleSystemBindings;
}

ParticleSystemSet& Model::getParticleSystems()
{
	return mParticleSystems;
}


bool Model::addSubmodel(SubModel* submodel)
{
	///if the submodel has a skeleton, check if it should be shared with existing models
	if (submodel->getEntity()->getSkeleton()) {
		if (mSkeletonOwnerEntity != 0) {
			submodel->getEntity()->shareSkeletonInstanceWith(mSkeletonOwnerEntity);
		} else {
			mSkeletonOwnerEntity = submodel->getEntity();
// 			mAnimationStateSet = submodel->getEntity()->getAllAnimationStates();
		}
	}
	mSubmodels.insert(submodel);
	return true;	
}

bool Model::removeSubmodel(SubModel* submodel)
{
	mSubmodels.erase(submodel);
	return true;
}

// SubModel* Model::getSubModel(const std::string& name)
// {
// 	SubModelSet::const_iterator I = mSubmodels.begin();
// 	SubModelSet::const_iterator I_end = mSubmodels.end();
// 	for (; I != I_end; ++I) {
// 		if ((*I)->getName() == name) {
// 			return *I;
// 		}
// 	}
// 	S_LOG_FAILURE("Could not find submodel with name "<< name << " in model " << getName());
// 	return 0:
// 
// }

SubModel* Model::getSubModel(size_t index)
{
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	size_t i = 0;
	for (; I != I_end; ++I) {
		if (i == index) {
			return *I;
		}
	}
	S_LOG_FAILURE("Could not find submodel with index "<< index << " in model " << getName());
	return 0;

}

ModelPart::ModelPart() : mShown(false), mVisible(false)
{
}


void ModelPart::show()
{
	mVisible = true;
	for (SubModelPartStore::iterator I = mSubModelParts.begin(); I != mSubModelParts.end(); ++I) {
		(*I)->show();
	}
}

void ModelPart::hide()
{
	for (SubModelPartStore::iterator I = mSubModelParts.begin(); I != mSubModelParts.end(); ++I) {
		(*I)->hide();
	}
}

bool ModelPart::getVisible() const
{
	return mVisible;
}

void ModelPart::setVisible(bool visible)
{
	mVisible = visible;
}


const std::string& ModelPart::getGroupName() const
{
	return mGroupName;
}

void ModelPart::setGroupName(const std::string& groupName)
{
	mGroupName = groupName;
}

void ModelPart::addSubModelPart(SubModelPart* part)
{
	mSubModelParts.push_back(part);
}



void Model::showPart(const std::string& partName, bool hideOtherParts)
{
	ModelPartStore::iterator I = mModelParts.find(partName);
	if (I != mModelParts.end()) {
		ModelPart& modelPart = I->second;
		if (hideOtherParts) {
			const std::string& groupName = modelPart.getGroupName();
			///make sure that all other parts in the same group are hidden
			PartGroupStore::iterator partBucketI = mGroupsToPartMap.find(groupName);
			if (partBucketI != mGroupsToPartMap.end()) {
				for (std::vector< std::string >::iterator I = partBucketI->second.begin(); I != partBucketI->second.end(); ++I) {
					if (*I != partName) {
						hidePart(*I, true);
					}
				}
			}
		}
		
		modelPart.show();
	}
}

void Model::hidePart(const std::string& partName, bool dontChangeVisibility)
{
	ModelPartStore::iterator I = mModelParts.find(partName);
	if (I != mModelParts.end()) {
		ModelPart& modelPart = I->second;
		modelPart.hide();
		if (!dontChangeVisibility) {
			modelPart.setVisible(false);
			const std::string& groupName = modelPart.getGroupName();
			///if some part that was hidden before now should be visible 
			PartGroupStore::iterator partBucketI = mGroupsToPartMap.find(groupName);
			if (partBucketI != mGroupsToPartMap.end()) {
				for (std::vector< std::string >::iterator J = partBucketI->second.begin(); J != partBucketI->second.end(); ++J) {
					if (*J != partName) {
						ModelPartStore::iterator I_modelPart = mModelParts.find(partName);
						if (I_modelPart != mModelParts.end()) {
							if (I_modelPart->second.getVisible()) {
								I_modelPart->second.show();
								break;
							}
						}
					}
				}
			}
			
		}
	}	


}

void Model::setVisible(bool visible) 
{
	mVisible = visible;
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->setVisible(visible);		
	}
	
}



const Ogre::Real Model::getScale() const
{
	return mScale;
}

const Ogre::Quaternion& Model::getRotation() const
{
	return mRotation;
}

const ModelDefinition::UseScaleOf Model::getUseScaleOf() const
{
	return _masterModel->getUseScaleOf();
}


Action* Model::getAction(const std::string& name)
{
	ActionStore::iterator I = mActions.find(name);
	if (I == mActions.end()) {
		return 0;
	}
	return &(I->second);
}

// void Model::startAnimation(const std::string& nameOfAnimation)
// {
// 	enableAnimation(nameOfAnimation,true);
// }
// 
// void Model::stopAnimation(const std::string& nameOfAnimation)
// {
// 	enableAnimation(nameOfAnimation,false);
// }	



// void Model::enableAnimation(const std::string& nameOfAnimation,bool enable)
// {
// 	if (mAnimationStateSet)
// 	{
// 		AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
// 		if (partmap_iter != mAnimationPartMap.end()) 
// 		{
// 			std::multiset< AnimationPart* >* part = partmap_iter->second;
// 			std::multiset< AnimationPart* >::const_iterator I = part->begin();
// 			std::multiset< AnimationPart* >::const_iterator I_end = part->end();
// 			for (; I != I_end; ++I) 
// 			{ // go through each anim part and update its related anim state 
// 				Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
// 				if (J != mAnimationStateSet->end()) {
// 					if (enable) {
// 						//also set the weight of the animations part
// 						J->second.setWeight((*I)->weight);
// 						MotionManager::getSingleton().addAnimation(&J->second);
// 					} else {
// 						MotionManager::getSingleton().removeAnimation(&J->second);
// 					}
// 				} else {
// 					S_LOG_FAILURE("The subanimation " << (*I)->name << " does not exist.");
// 				}
// 			}
// 		}
// 	}
// }
// 
// 
// void Model::resetAnimations()
// {
// 	if (mAnimationStateSet)
// 	{
// 		AnimationPartMap::iterator it;
// 		for ( it= mAnimationPartMap.begin();it != mAnimationPartMap.end();it++) 
// 		{
// 			std::multiset< AnimationPart* >* part = it->second;
// 			std::multiset< AnimationPart* >::const_iterator I = part->begin();
// 			std::multiset< AnimationPart* >::const_iterator I_end = part->end();
// 			for (; I != I_end; ++I) 
// 			{ // go through each anim part and update its related anim state 
// 				Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
// 				if (J != mAnimationStateSet->end()) {
// 					MotionManager::getSingleton().removeAnimation(&J->second);
// 				} else {
// 					S_LOG_FAILURE("The subanimation " << (*I)->name << " does not exist.");
// 				}
// 					
// 				
// 			}
// 		}
// 	}
// }

void Model::resetSubmodels()
{
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		delete *I;
/* 		SubModel* submodel = *I;
		Ogre::SceneManager* sceneManager = ModelDefinitionManager::instance().getSceneManager();
		sceneManager->removeEntity(submodel->getEntity());*/
	}
	mSubmodels.clear();
	mModelParts.clear();
}

void Model::resetParticles()
{
	ParticleSystemSet::const_iterator I = mParticleSystems.begin();
	ParticleSystemSet::const_iterator I_end = mParticleSystems.end();
	for (; I != I_end; ++I) {
 		ParticleSystem* system = *I;
 		delete system;
	}
	mParticleSystems.clear();
	mAllParticleSystemBindings.clear();
}

Ogre::TagPoint* Model::attachObjectToAttachPoint(const Ogre::String &attachPointName, Ogre::MovableObject *pMovable, const Ogre::Vector3 &scale, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	for (AttachPointDefinitionStore::iterator I = _masterModel->mAttachPoints.begin(); I < _masterModel->mAttachPoints.end();  ++I) {
		if (I->Name == attachPointName) {
			const std::string& boneName = I->BoneName;
			return attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition, scale);
		}
	}
	return 0;
}

bool Model::hasAttachPoint(const std::string& attachPoint) const
{
	for (AttachPointDefinitionStore::iterator I = _masterModel->mAttachPoints.begin(); I < _masterModel->mAttachPoints.end();  ++I) {
		if (I->Name == attachPoint) {
			return true;
		}
	}
	return false;
}



Ogre::AnimationState* Model::getAnimationState(const Ogre::String& name)
{
	if (mSubmodels.size()) {
		if (mSkeletonOwnerEntity) {
			return mSkeletonOwnerEntity->getAnimationState(name);
		} else {
			return 0;
		}
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAnimationState");		
	
}

Ogre::AnimationStateSet* Model::getAllAnimationStates()
{
	if (mSubmodels.size()) {
		if (mSkeletonOwnerEntity) {
			return mSkeletonOwnerEntity->getAllAnimationStates();
		} else {
			return 0;
		}
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAllAnimationStates");		
	
}


Ogre::SkeletonInstance * Model::getSkeleton ()
{
	if (mSubmodels.size()) {
		if (mSkeletonOwnerEntity) {
			return mSkeletonOwnerEntity->getSkeleton();
		} else {
			return 0;
		}
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getSkeleton");		
}

Ogre::TagPoint* Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	return attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition, Ogre::Vector3::UNIT_SCALE);
}

Ogre::TagPoint* Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition, const Ogre::Vector3 &scale)
{
	if (mSubmodels.size()) {
		Ogre::Entity* entity = mSkeletonOwnerEntity;
		
		Ogre::TagPoint* tagPoint = entity->attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition);
	
		if (mParentNode) {
			//since we're using inherit scale on the tagpoint, divide by the parent's scale now, so it evens out later on when the TagPoint is scaled in TagPoint::_updateFromParent(
			Ogre::Vector3 parentScale = mParentNode->_getDerivedScale();
			tagPoint->setScale(scale / parentScale);
		} else {
			//no parent node, this is not good...
			tagPoint->setScale(scale);	
		}
		return tagPoint;


	} else {
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::attachObjectToBone");		
	}	
}



Ogre::MovableObject * Model::detachObjectFromBone (const Ogre::String &movableName)
{

	if (mSubmodels.size() && mSkeletonOwnerEntity) {
		return mSkeletonOwnerEntity->detachObjectFromBone(movableName);

	} else {
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::detachObjectFromBone");	
	}	
	
}


//-----------------------------------------------------------------------
void Model::detachAllObjectsFromBone(void)
{
	if (mSubmodels.size() && mSkeletonOwnerEntity) {
		return mSkeletonOwnerEntity->detachAllObjectsFromBone();

	} else {
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::detachAllObjectsFromBone");		
	}	
}





/** Overridden - see MovableObject.
*/
void Model::_notifyCurrentCamera(Ogre::Camera* cam)
{
	MovableObject::_notifyCurrentCamera(cam);
	if (isVisible()) {
		SubModelSet::const_iterator I = mSubmodels.begin();
		SubModelSet::const_iterator I_end = mSubmodels.end();
		if (mVisible) {
			for (; I != I_end; ++I) {
				(*I)->getEntity()->_notifyCurrentCamera(cam);		
			}
			
			// Notify any child objects
			Ogre::Entity::ChildObjectList::iterator child_itr = mChildObjectList.begin();
			Ogre::Entity::ChildObjectList::iterator child_itr_end = mChildObjectList.end();
			for( ; child_itr != child_itr_end; child_itr++)
			{
				child_itr->second->_notifyCurrentCamera(cam);
			}
		}
	}	
	
}

void Model::setUserObject (Ogre::UserDefinedObject *obj)
{
	Ogre::MovableObject::setUserObject(obj);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->setUserObject(obj);
	}
}


/// Overridden - see MovableObject.
void Model::setRenderQueueGroup(Ogre::RenderQueueGroupID queueID)
{
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->setRenderQueueGroup(queueID);
	}
		
}

/** Overridden - see MovableObject.
*/
const Ogre::AxisAlignedBox& Model::getBoundingBox(void) const
{
	mFull_aa_box.setNull();
	 
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();
	for( ; child_itr != child_itr_end; child_itr++)
	{
		mFull_aa_box.merge((*child_itr)->getEntity()->getBoundingBox());
	}

	return mFull_aa_box;
}

/** Overridden - see MovableObject.
*/
const Ogre::AxisAlignedBox& Model::getWorldBoundingBox(bool derive) const
{
	mWorldFull_aa_box.setNull();
	 
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();

	for( ; child_itr != child_itr_end; child_itr++)
	{
 		mWorldFull_aa_box.merge((*child_itr)->getEntity()->getWorldBoundingBox(derive));
	}
	 
	return mWorldFull_aa_box;
}

Ogre::Real Model::getBoundingRadius() const
{
	Ogre::Real rad(0);
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();
	for( ; child_itr != child_itr_end; child_itr++)
	{
        rad = std::max<Ogre::Real>(rad, (*child_itr)->getEntity()->getBoundingRadius());
	}
    return rad;

}




/** Overridden - see MovableObject.
*/
void Model::_updateRenderQueue(Ogre::RenderQueue* queue)
{
	///check with both the model visibility setting and with the general model setting to see whether the model should be shown
	if (isVisible()) {
		SubModelSet::const_iterator I = mSubmodels.begin();
		SubModelSet::const_iterator I_end = mSubmodels.end();
		for (; I != I_end; ++I) {
			(*I)->getEntity()->_updateRenderQueue(queue);
		}
		
		if (getSkeleton() != 0) {
			//updateAnimation();
			Ogre::Entity::ChildObjectList::iterator child_itr = mChildObjectList.begin();
			Ogre::Entity::ChildObjectList::iterator child_itr_end = mChildObjectList.end();
			for( ; child_itr != child_itr_end; child_itr++)
			{
				///make sure to do _update here, else attached entities won't be updated if no animation is playing
				child_itr->second->getParentNode()->_update(true, true);
				if (child_itr->second->isVisible())
					child_itr->second->_updateRenderQueue(queue);
			}
		}
		
	}	
	
	
}


/** Overridden from MovableObject */
// const Ogre::String& Model::getName(void) const
// {
// 	return mName;	
// }

/** Overridden from MovableObject */
const Ogre::String& Model::getMovableType(void) const
{
	return sMovableType;
}

void Model::setRenderingDistance (Ogre::Real dist)
{
	MovableObject::setRenderingDistance(dist);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->setRenderingDistance(dist);
	}
}

void Model::setQueryFlags(unsigned long flags) 
{
	MovableObject::setQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->setQueryFlags(flags);
	}
}

void Model::addQueryFlags(unsigned long flags)
{
	MovableObject::addQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->addQueryFlags(flags);
	}
	
}
		

void Model::removeQueryFlags(unsigned long flags)
{
	//for now, only return. This is because this is often called at shutdown, when the entities already have been destroyed
	//and we don't want segfaults
//	return;
	MovableObject::removeQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->removeQueryFlags(flags);
	}
	
}


/** Overridden from MovableObject */
void Model::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
{
	MovableObject::_notifyAttached(parent, isTagPoint);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		(*I)->getEntity()->_notifyAttached(parent, isTagPoint);
	}
}

bool Model::isVisible (void) const
{
	///check with both the model visibility setting and with the general model setting to see whether the model should be shown
	return Ogre::MovableObject::isVisible() && ModelDefinitionManager::getSingleton().getShowModels();
}

	
Model* Model::createModel(Ogre::SceneManager* sceneManager, const std::string& modelType, const std::string& name)
{
	
	Ogre::String modelName(name);
	if (name == "") {
		std::stringstream ss;
		ss << "__AutogenModel_" << Model::msAutoGenId++;
		modelName = ss.str();
	}
	
	// delegate to factory implementation
	Ogre::NameValuePairList params;
	params["modeldefinition"] = modelType;
	return static_cast<Model*>(sceneManager->createMovableObject(modelName, ModelFactory::FACTORY_TYPE_NAME, &params));

}
	
	
	
Ogre::String ModelFactory::FACTORY_TYPE_NAME = "Model";
//-----------------------------------------------------------------------
const Ogre::String& ModelFactory::getType(void) const
{
	return FACTORY_TYPE_NAME;
}
//-----------------------------------------------------------------------
Ogre::MovableObject* ModelFactory::createInstanceImpl( const Ogre::String& name,
	const Ogre::NameValuePairList* params)
{

	// must have mesh parameter
	if (params != 0)
	{
		Ogre::NameValuePairList::const_iterator ni = params->find("modeldefinition");
		if (ni != params->end())
		{
			Model* model = new Model(name);
			model->create(ni->second);
			return model;
		}

	}
	OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
		"'modeldefinition' parameter required when constructing a Model.",
		"ModelFactory::createInstance");

}
//-----------------------------------------------------------------------
void ModelFactory::destroyInstance( Ogre::MovableObject* obj)
{
	delete obj;
}



}
}
