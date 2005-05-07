/*
    Copyright (C) 2004  Erik Hjortsberg

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

Model::Model(Ogre::SceneManager* sceneManager, std::string name, ModelDefinitionPtr modelDefPointer)
: MovableObject::MovableObject()
, mName(name)
, mSceneManager(sceneManager)
, mScale(0)
, mRotation(0)
, mVisible(true)
, mSkeletonInstance(0)
, mAnimationStateSet(0)
, mDefinition(modelDefPointer)
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
	//delete mAnimationStateSet;
}

Model* Model::Create(std::string type, std::string name)

{
	ModelDefinitionPtr def = ModelDefinitionManager::getSingleton().load(type, "modeldefinitions");
	if (!def->isValid()) {
		return NULL;
	}
	return def->createModel(name, EmberOgre::getSingleton().getSceneManager(), def);
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

//const std::vector<

void Model::startAnimation(std::string nameOfAnimation)
{
	if (mAnimationStateSet) {
		if (mRunningAnimations.find(nameOfAnimation) == mRunningAnimations.end()) 
		{
			//The animations is not started, start a new
			AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
			if (partmap_iter != mAnimationPartMap.end()) {
				std::cout << "Starting animation: " << nameOfAnimation << "\n";
				mRunningAnimations.insert(nameOfAnimation);
				std::multiset< AnimationPart* >* part = partmap_iter->second;
				std::multiset< AnimationPart* >::const_iterator I = part->begin();
				std::multiset< AnimationPart* >::const_iterator I_end = part->end();
				for (; I != I_end; ++I) {
					std::cout << "Starting subanimation: " << (*I)->name << "\n";
					Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
					if (J != mAnimationStateSet->end()) {
						//also set the weight of the animations part
						J->second.setWeight((*I)->weight);
						
						MotionManager::getSingleton().addAnimation(&J->second);
					} else {
						S_LOG_FAILURE("The subanimation " << (*I)->name << " does not exist.")
					}
				}
			}
		} else if (mPausedAnimations.find(nameOfAnimation) != mPausedAnimations.end()) {
			//The animation is started but is paused, unpause it
			AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
			if (partmap_iter != mAnimationPartMap.end()) {
				mPausedAnimations.erase(nameOfAnimation);
				std::multiset< AnimationPart* >* part = partmap_iter->second;
				std::multiset< AnimationPart* >::const_iterator I = part->begin();
				std::multiset< AnimationPart* >::const_iterator I_end = part->end();
				for (; I != I_end; ++I) {
					Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
					if (J != mAnimationStateSet->end()) {
						MotionManager::getSingleton().unpauseAnimation(&J->second);
					} else {
						S_LOG_FAILURE( "Error: the subanimation " << (*I)->name << " does not exist.")
					}
				}
			}
		}
	}
	
	//TODO check in pausedanimation to see if the anim is paused
	
}
void Model::pauseAnimation(std::string nameOfAnimation)
{
	if (mAnimationStateSet) {
		if (mRunningAnimations.find(nameOfAnimation) != mRunningAnimations.end()) 
		{
			AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
			if (partmap_iter != mAnimationPartMap.end()) {
				mPausedAnimations.insert(nameOfAnimation);
				std::multiset< AnimationPart* >* part = partmap_iter->second;
				std::multiset< AnimationPart* >::const_iterator I = part->begin();
				std::multiset< AnimationPart* >::const_iterator I_end = part->end();
				for (; I != I_end; ++I) {
					Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
					MotionManager::getSingleton().pauseAnimation(&J->second);
				}
			}
		}
	}	
}

void Model::stopAnimation(std::string nameOfAnimation)
{
	if (mAnimationStateSet) {
		if (mRunningAnimations.find(nameOfAnimation) != mRunningAnimations.end()) 
		{
			AnimationPartMap::iterator partmap_iter = mAnimationPartMap.find(nameOfAnimation);
			if (partmap_iter != mAnimationPartMap.end()) {
				mRunningAnimations.erase(nameOfAnimation);
				std::multiset< AnimationPart* >* part = partmap_iter->second;
				std::multiset< AnimationPart* >::const_iterator I = part->begin();
				std::multiset< AnimationPart* >::const_iterator I_end = part->end();
				for (; I != I_end; ++I) {
					Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
					MotionManager::getSingleton().removeAnimation(&J->second);
				}
			}
		}
	}	
	
}

void Model::resetAnimations()
{
	if (mAnimationStateSet) {
		std::set< std::string >::const_iterator iter_running = mRunningAnimations.begin();
		std::set< std::string >::const_iterator iter_running_end = mRunningAnimations.begin();
		
		for (; iter_running != iter_running_end; ++iter_running) {
			std::multiset< AnimationPart* >* part = mAnimationPartMap[*iter_running];
			std::multiset< AnimationPart* >::const_iterator I = part->begin();
			std::multiset< AnimationPart* >::const_iterator I_end = part->end();
			for (; I != I_end; ++I) {
				Ogre::AnimationStateSet::iterator J = mAnimationStateSet->find((*I)->name);
				MotionManager::getSingleton().removeAnimation(&J->second);
			}
		}	
	}	
	mRunningAnimations.clear();	
	mPausedAnimations.clear();	

}




Ogre::AnimationState* Model::getAnimationState(const Ogre::String& name)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAnimationState(name);
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAnimationState");		
	
}

Ogre::AnimationStateSet* Model::getAllAnimationStates()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getAllAnimationStates();
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getAllAnimationStates");		
	
}


Ogre::SkeletonInstance * Model::getSkeleton ()
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->getSkeleton();
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::getSkeleton");		
}

void Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		submodel->getEntity()->attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition);
	} else {
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::attachObjectToBone");		
	}	
}

Ogre::MovableObject * Model::detachObjectFromBone (const Ogre::String &movableName)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		return submodel->getEntity()->detachObjectFromBone(movableName);
	}
	OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::detachObjectFromBone");		
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
	 
//	 std::cout << "Boundingbox: " << full_aa_box->getMaximum().x << " : " << full_aa_box->getMaximum().y << " : " << full_aa_box->getMaximum().z;
	 
	 //full_aa_box->scale(mParentNode->_getDerivedScale());
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
	 
//	 std::cout << "Boundingbox: " << full_aa_box->getMaximum().x << " : " << full_aa_box->getMaximum().y << " : " << full_aa_box->getMaximum().z;
	 
	 //full_aa_box->scale(mParentNode->_getDerivedScale());
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

