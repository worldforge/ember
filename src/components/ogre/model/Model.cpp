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
#include "components/ogre/MotionManager.h"


#include "components/ogre/EmberOgre.h"
#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"

#include "components/ogre/EmberSceneManager/include/EmberTerrainSceneManager.h"

namespace EmberOgre {

Ogre::String Model::msMovableType = "Model";

Model::Model(std::string name)
: MovableObject::MovableObject()
, mName(name)
, mSceneManager(0)
, mScale(0)
, mRotation(0)
, mVisible(true)
, mSkeletonInstance(0)
, mAnimationStateSet(0)
{}
Model::~Model()
{
	resetAnimations();
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
 		SubModel* submodel = *I;
		mSceneManager->removeEntity(submodel->getEntity());
	}
	delete mAnimationStateSet;
}

bool Model::create(std::string modelType)
{
	const Ogre::String groupName = "ModelDefinitions";
	//Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
	try {
		_masterModel= ModelDefinitionManager::instance().load(modelType, groupName);
	} catch (Ogre::Exception& ex) {
		return false;
	}
	if (!_masterModel->isValid()) {
		return false;
	}
	else
		return createFromDefn();
}


bool Model::createFromDefn()
{
	// create instance of model from definition
	Ogre::SceneManager* sceneManager = ModelDefinitionManager::instance().getSceneManager();
	assert(sceneManager);
	mScale=_masterModel->mScale ;
	mUseScaleOf=_masterModel->mUseScaleOf;
	mRotation = _masterModel->mRotation;

	std::vector<std::string> showPartVector;

	std::vector<ModelDefinition::SubModelDefinition>::iterator I_subModels = _masterModel->mSubModels.begin();
	std::vector<ModelDefinition::SubModelDefinition>::iterator I_subModels_end = _masterModel->mSubModels.end();
	S_LOG_INFO("Number of submodels: " << _masterModel->mSubModels.size()); 
	
	for (; I_subModels != I_subModels_end; ++I_subModels) 
	{
		std::string entityName = mName + "/" + (*I_subModels).Mesh;
		try {
			Ogre::Entity* entity = sceneManager->createEntity(entityName, (*I_subModels).Mesh);

			SubModel* submodel = new SubModel(entity);

			Model::SubModelPartMapping* submodelPartMapping = new Model::SubModelPartMapping();
				
			std::list<ModelDefinition::PartDefinition>::iterator I_parts = (*I_subModels).Parts.begin();
			std::list<ModelDefinition::PartDefinition>::iterator I_parts_end = (*I_subModels).Parts.end();
			for (; I_parts != I_parts_end; ++I_parts) {
				Model::StringSet parts;
				if ((*I_parts).SubEntities.size() > 0)
				{
					std::list<ModelDefinition::SubEntityDefinition>::iterator I_subEntities = (*I_parts).SubEntities.begin();
					std::list<ModelDefinition::SubEntityDefinition>::iterator I_subEntities_end = (*I_parts).SubEntities.end();
					for (; I_subEntities != I_subEntities_end; ++I_subEntities) {
						parts.insert((*I_subEntities).SubEntity);
						if ((*I_subEntities).Material != "") {
							entity->getSubEntity((*I_subEntities).SubEntity)->setMaterialName((*I_subEntities).Material);
						}
					}		
				} 
				Model::SubModelPartMapping::value_type part((*I_parts).Name, parts);
				submodelPartMapping->insert(part);
				if ((*I_parts).Show) {
					showPartVector.push_back((*I_parts).Name);
				}
			}
			submodel->createSubModelParts(submodelPartMapping);
			addSubmodel(submodel);
		} 
		catch (Ogre::Exception e) 
		{
			//std::cerr << "Error when loading the submodel " << entityName << ".\n";
			S_LOG_FAILURE( "Submodel load error for " + entityName );
			return false;
		}
	}

	std::vector<ModelDefinition::ActionDefinition>::const_iterator I_actions = _masterModel->mActions.begin();
	std::vector<ModelDefinition::ActionDefinition>::const_iterator I_actions_end = _masterModel->mActions.end();
	for (;I_actions != I_actions_end; ++I_actions) {
		std::multiset< Model::AnimationPart* >* animationPartSet = new std::multiset< Model::AnimationPart* >();
		
		
		std::list<ModelDefinition::AnimationDefinition>::const_iterator I_anims = (*I_actions).Animations.begin();
		std::list<ModelDefinition::AnimationDefinition>::const_iterator I_anims_end = (*I_actions).Animations.end();
		for (;I_anims != I_anims_end; ++I_anims) {
			Model::AnimationPart* animPart = new Model::AnimationPart();
			animPart->name = (*I_anims).Name;
			animPart->weight = (*I_anims).Weight;
			animationPartSet->insert(animPart);
		}
		mAnimationPartMap.insert(Model::AnimationPartMap::value_type((*I_actions).Name, animationPartSet));
	}
	
	
	std::vector<std::string>::const_iterator I = showPartVector.begin();
	std::vector<std::string>::const_iterator I_end = showPartVector.end();
	for (;I != I_end; I++) {
		showPart(*I);	
	}
	return true;
}


bool Model::addSubmodel(SubModel* submodel)
{
	if (mSubmodels.size()) {
		SubModel* existingSubmodel = *(mSubmodels.begin());
		submodel->getEntity()->shareSkeletonInstanceWith(existingSubmodel->getEntity());
	} else {
		mAnimationStateSet = submodel->getEntity()->getAllAnimationStates();
	}
	mSubmodels.insert(submodel);
	SubModelPartMap* submodelMap = submodel->getSubModelPartMap();
	for (SubModelPartMap::const_iterator I = submodelMap->begin(); I != submodelMap->end(); ++I) {
		mSubModelPartMap.insert(*I);
	}
	return true;	
}

bool Model::removeSubmodel(SubModel* submodel)
{
	mSubmodels.erase(submodel);
	return true;
}

void Model::showPart(std::string partName)
{
	mSubModelPartMap[partName]->show();	
}

void Model::hidePart(std::string partName)
{
	mSubModelPartMap[partName]->hide();	
}

void Model::setVisible(bool visible) 
{
	mVisible = visible;
}



const Ogre::Real Model::getScale() const
{
	return mScale;
}

const Ogre::Real Model::getRotation() const
{
	return mRotation;
}

const unsigned short Model::getUseScaleOf() const
{
	return mUseScaleOf;
}

void Model::startAnimation(std::string nameOfAnimation)
{
	enableAnimation(nameOfAnimation,true);
}

void Model::stopAnimation(std::string nameOfAnimation)
{
	enableAnimation(nameOfAnimation,false);
}	

void Model::enableAnimation(std::string nameOfAnimation,bool enable)
{
	if (mAnimationStateSet)
	{
		AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
		if (partmap_iter != mAnimationPartMap.end()) 
		{
			std::multiset< AnimationPart* >* part = partmap_iter->second;
			std::multiset< AnimationPart* >::const_iterator I = part->begin();
			std::multiset< AnimationPart* >::const_iterator I_end = part->end();
			for (; I != I_end; ++I) 
			{ // go through each anim part and update its related anim state 
				Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
				if (J != mAnimationStateSet->end()) {
					if (enable) {
						//also set the weight of the animations part
						J->second.setWeight((*I)->weight);
						MotionManager::getSingleton().addAnimation(&J->second);
					} else {
						MotionManager::getSingleton().removeAnimation(&J->second);
					}
				} else {
					S_LOG_FAILURE("The subanimation " << (*I)->name << " does not exist.");
				}
			}
		}
	}
}


void Model::resetAnimations()
{
	if (mAnimationStateSet)
	{
		AnimationPartMap::iterator it;
		for ( it= mAnimationPartMap.begin();it != mAnimationPartMap.end();it++) 
		{
			std::multiset< AnimationPart* >* part = it->second;
			std::multiset< AnimationPart* >::const_iterator I = part->begin();
			std::multiset< AnimationPart* >::const_iterator I_end = part->end();
			for (; I != I_end; ++I) 
			{ // go through each anim part and update its related anim state 
				Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
				if (J != mAnimationStateSet->end()) {
					MotionManager::getSingleton().removeAnimation(&J->second);
				} else {
					S_LOG_FAILURE("The subanimation " << (*I)->name << " does not exist.");
				}
					
				
			}
		}
	}
}



Ogre::AnimationState* Model::getAnimationState(const Ogre::String& name)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAnimationState(name);
	}
	Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAnimationState");		
	
}

Ogre::AnimationStateSet* Model::getAllAnimationStates()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAllAnimationStates();
	}
	Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAllAnimationStates");		
	
}


Ogre::SkeletonInstance * Model::getSkeleton ()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getSkeleton();
	}
	Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getSkeleton");		
}

void Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		submodel->getEntity()->attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition);
	} else {
		Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::attachObjectToBone");		
	}	
}

Ogre::MovableObject * Model::detachObjectFromBone (const Ogre::String &movableName)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->detachObjectFromBone(movableName);
	}
	Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::detachObjectFromBone");		
}

/** Overridden - see MovableObject.
*/
void Model::_notifyCurrentCamera(Ogre::Camera* cam)
{
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	if (mVisible) {
	 	for (; I != I_end; ++I) {
			SubModel* submodel = *I;
			submodel->getEntity()->_notifyCurrentCamera(cam);		
		}
	}
	
}

void Model::setUserObject (Ogre::UserDefinedObject *obj)
{
	Ogre::MovableObject::setUserObject(obj);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
 		SubModel* submodel = *I;
		Ogre::Entity* entity = submodel->getEntity();
		entity->setUserObject(obj);
	}
}


/// Overridden - see MovableObject.
void Model::setRenderQueueGroup(Ogre::RenderQueueGroupID queueID)
{
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->setRenderQueueGroup(queueID);
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
		SubModel* submodel = *child_itr;
             
		mFull_aa_box.merge(submodel->getEntity()->getBoundingBox());
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
		SubModel* submodel = *child_itr;

 		mWorldFull_aa_box.merge(submodel->getEntity()->getWorldBoundingBox(derive));
	}
	 
	return mWorldFull_aa_box;
}

Ogre::Real Model::getBoundingRadius() const
{
	Ogre::Real rad = 0;
	SubModelSet::const_iterator child_itr = mSubmodels.begin();
	SubModelSet::const_iterator child_itr_end = mSubmodels.end();
	for( ; child_itr != child_itr_end; child_itr++)
	{
		SubModel* submodel = *child_itr;
		Ogre::Real newRad = submodel->getEntity()->getBoundingRadius();
        rad = std::max(rad, newRad);
	}
    return rad;

}




/** Overridden - see MovableObject.
*/
void Model::_updateRenderQueue(Ogre::RenderQueue* queue)
{
	if (mVisible) {
		SubModelSet::const_iterator I = mSubmodels.begin();
		SubModelSet::const_iterator I_end = mSubmodels.end();
		for (; I != I_end; ++I) {
			SubModel* submodel = *I;
			submodel->getEntity()->_updateRenderQueue(queue);
		}
	}	
}


/** Overridden from MovableObject */
const Ogre::String& Model::getName(void) const
{
	return mName;	
}

/** Overridden from MovableObject */
const Ogre::String& Model::getMovableType(void) const
{
	return msMovableType;
}

void Model::setQueryFlags(unsigned long flags) 
{
	MovableObject::setQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->setQueryFlags(flags);
	}
}

void Model::addQueryFlags(unsigned long flags)
{
	MovableObject::addQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->addQueryFlags(flags);
	}
	
}
		

void Model::removeQueryFlags(unsigned long flags)
{
	MovableObject::removeQueryFlags(flags);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->removeQueryFlags(flags);
	}
	
}


/** Overridden from MovableObject */
void Model::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
{
	MovableObject::_notifyAttached(parent, isTagPoint);
	SubModelSet::const_iterator I = mSubmodels.begin();
	SubModelSet::const_iterator I_end = mSubmodels.end();
	for (; I != I_end; ++I) {
		SubModel* submodel = *I;
		submodel->getEntity()->_notifyAttached(parent, isTagPoint);
	}
}

}

