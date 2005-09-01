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


#include "components/ogre/EmberOgre.h"
#include "ModelDefinitionManager.h"
#include "ModelDefinition.h"


#include <OgreTagPoint.h>

namespace EmberOgre {


void AnimationSet::addAnimationPart(AnimationPart part)
{
	mAnimations.push_back(part);
}

void AnimationSet::addTime(Ogre::Real timeSlice)
{
	AnimationPartSet::iterator I = mAnimations.begin();
	for (; I != mAnimations.end(); ++I) {
		I->state->addTime(timeSlice);
	}
}

void AnimationSet::setEnabled(bool state)
{
	AnimationPartSet::iterator I = mAnimations.begin();
	for (; I != mAnimations.end(); ++I) {
		I->state->setEnabled(state);
	}
}

ParticleSystemBinding::ParticleSystemBinding(ParticleSystem* parentSystem, const std::string& emitterVal, const std::string& variableName) :
mParticleSystem(parentSystem)
, mEmitterVal(emitterVal)
, mVariableName(variableName)
{
	//TODO: add more emitter values to bind
	if (mEmitterVal == "emission_rate") {
		Ogre::ParticleEmitter* emitter = mParticleSystem->getOgreParticleSystem()->getEmitter(0);
		if (emitter) {
			mOriginalValue = emitter->getEmissionRate();
		}
	}

}

void ParticleSystemBinding::scaleValue(Ogre::Real scaler)
{
	
	//TODO: add more emitter values to bind
	if (mEmitterVal == "emission_rate") {
		Ogre::ParticleEmitter* emitter = mParticleSystem->getOgreParticleSystem()->getEmitter(0);
		if (emitter) {
			emitter->setEmissionRate(mOriginalValue * scaler);
		}
	}
}














Ogre::String Model::msMovableType = "Model";

Model::Model(const std::string& name)
: MovableObject::MovableObject()
, mName(name)
, mScale(0)
, mRotation(0)
, mSkeletonInstance(0)
, mAnimationStateSet(0)
{
 mVisible = true;
}
Model::~Model()
{
}

void Model::reset()
{
//	resetAnimations();
	resetSubmodels();

}


bool Model::create(const std::string& modelType)
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
	{
		bool success =  createFromDefn();
		if (!success) {
			reset();
		}
		return success;
	}
}



bool Model::createFromDefn()
{
	// create instance of model from definition
	Ogre::SceneManager* sceneManager = ModelDefinitionManager::instance().getSceneManager();
	assert(sceneManager);
	mScale=_masterModel->mScale ;
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
						ModelDefinition::SubEntityDefinition def = *I_subEntities;
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
		catch (Ogre::Exception& e) 
		{
			//std::cerr << "Error when loading the submodel " << entityName << ".\n";
			std::string desc = e.getFullDescription();
			S_LOG_FAILURE( "Submodel load error for " + entityName + ". \nOgre error: " + e.getFullDescription());
			return false;
		}
	}

	std::vector<ModelDefinition::ActionDefinition>::const_iterator I_actions = _masterModel->mActions.begin();
	std::vector<ModelDefinition::ActionDefinition>::const_iterator I_actions_end = _masterModel->mActions.end();
	for (;I_actions != I_actions_end; ++I_actions) {
		//std::multiset< Model::AnimationPart* >* animationPartSet = new std::multiset< Model::AnimationPart* >();
		Action action;
		action.setName(I_actions->Name);
		
		if (mSubmodels.size()) {
			std::list<ModelDefinition::AnimationDefinition>::const_iterator I_anims = (*I_actions).Animations.begin();
			std::list<ModelDefinition::AnimationDefinition>::const_iterator I_anims_end = (*I_actions).Animations.end();
			for (;I_anims != I_anims_end; ++I_anims) {
				if (getSkeleton() && getAllAnimationStates() && getAllAnimationStates()->find(I_anims->Name) != getAllAnimationStates()->end()) {
					AnimationPart animPart;
					try {
						Ogre::AnimationState* state = getAnimationState(I_anims->Name);
						animPart.state = state;
						animPart.weight = I_anims->Weight;
						action.getAnimations()->addAnimationPart(animPart);
					} catch (Ogre::Exception& ex) {
						S_LOG_FAILURE("Error when loading animation: " +I_anims->Name + ".\n" + ex.getFullDescription() );
					}
				}
			}
		}
		
		//TODO: add sounds too
		
		mActions[I_actions->Name] = action;
	}
	
	std::vector<ModelDefinition::ParticleSystemDefinition>::const_iterator I_particlesys = _masterModel->mParticleSystems.begin();
	std::vector<ModelDefinition::ParticleSystemDefinition>::const_iterator I_particlesys_end = _masterModel->mParticleSystems.end();
	for (;I_particlesys != I_particlesys_end; ++I_particlesys) {
		//first try to create the ogre particle system
		std::string name(mName + "/particle" + I_particlesys->Script);
		Ogre::ParticleSystem* ogreParticleSystem;
		try {		
			 ogreParticleSystem = Ogre::ParticleSystemManager::getSingleton().createSystem(name, I_particlesys->Script);
		} catch (Ogre::Exception& ex) {
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

	


	
	std::vector<std::string>::const_iterator I = showPartVector.begin();
	std::vector<std::string>::const_iterator I_end = showPartVector.end();
	for (;I != I_end; I++) {
		showPart(*I);	
	}
	return true;
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
	if (mSubmodels.size()) {
		SubModel* existingSubmodel = *(mSubmodels.begin());
		if (existingSubmodel->getEntity()->getSkeleton()) {
			submodel->getEntity()->shareSkeletonInstanceWith(existingSubmodel->getEntity());
		}
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

void Model::showPart(const std::string& partName)
{
	mSubModelPartMap[partName]->show();	
}

void Model::hidePart(const std::string& partName)
{
	mSubModelPartMap[partName]->hide();	
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

const Ogre::Real Model::getRotation() const
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
 		SubModel* submodel = *I;
		Ogre::SceneManager* sceneManager = ModelDefinitionManager::instance().getSceneManager();
		sceneManager->removeEntity(submodel->getEntity());
	}
	mSubmodels.clear();
}


void Model::attachObjectToAttachPoint(const Ogre::String &attachPointName, Ogre::MovableObject *pMovable, const Ogre::Vector3 &scale, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition)
{
	std::string boneName;
	for (ModelDefinition::AttachPointDefinitionStore::iterator I = _masterModel->mAttachPoints.begin(); I < _masterModel->mAttachPoints.end();  ++I) {
		if (I->Name == attachPointName) {
			boneName = I->BoneName;
			break;
		}
	}
	if (boneName != "") {
		attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition, scale);
	}
}

bool Model::hasAttachPoint(const std::string& attachPoint) const
{
	for (ModelDefinition::AttachPointDefinitionStore::iterator I = _masterModel->mAttachPoints.begin(); I < _masterModel->mAttachPoints.end();  ++I) {
		if (I->Name == attachPoint) {
			return true;
		}
	}
	return false;
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
	attachObjectToBone(boneName, pMovable, offsetOrientation, offsetPosition, Ogre::Vector3::UNIT_SCALE);
}

void Model::attachObjectToBone (const Ogre::String &boneName, Ogre::MovableObject *pMovable, const Ogre::Quaternion &offsetOrientation, const Ogre::Vector3 &offsetPosition, const Ogre::Vector3 &scale)
{
	if (mSubmodels.size()) {
		SubModel* submodel = *(mSubmodels.begin());
		Ogre::Entity* entity = submodel->getEntity();
		
		Ogre::SkeletonInstance* skeletonInstance = getSkeleton();

        if (mChildObjectList.find(pMovable->getName()) != mChildObjectList.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
                "An object with the name " + pMovable->getName() + " already attached",
                "Entity::attachObjectToBone");
        }
        if(pMovable->isAttached())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Object already attached to a sceneNode or a Bone", 
                "Entity::attachObjectToBone");
        }
        if (!entity->hasSkeleton())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "This entity's mesh has no skeleton to attach object to.", 
                "Entity::attachObjectToBone");
        }
        Ogre::Bone* bone = skeletonInstance->getBone(boneName);
        if (!bone)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Cannot locate bone named " + boneName, 
                "Entity::attachObjectToBone");
        }

        Ogre::TagPoint *tp = skeletonInstance->createTagPointOnBone(
            bone, offsetOrientation, offsetPosition);
        tp->setParentEntity(entity);
        tp->setChildObject(pMovable);
        if (mParentNode) {
        	//since we're using inherit scale on the tagpoint, divide by the parent's scale now, so it evens out later on when the TagPoint is scaled in TagPoint::_updateFromParent(
        	Ogre::Vector3 parentScale = mParentNode->_getDerivedScale();
			tp->setScale(scale / parentScale);
		} else {
			//no parent node, this is not good...
			tp->setScale(scale);	
		}
		//tp->setInheritScale(false);

        attachObjectImpl(pMovable, tp);

		tp->_updateFromParent();
		
        // Trigger update of bounding box if necessary
        if (mParentNode)
            mParentNode->needUpdate();
	} else {
		Ogre::Exception(Ogre::Exception::ERR_ITEM_NOT_FOUND, "There's no entities loaded!", "Model::attachObjectToBone");		
	}	
}

//-----------------------------------------------------------------------
void Model::attachObjectImpl(Ogre::MovableObject *pObject, Ogre::TagPoint *pAttachingPoint)
{
	assert(mChildObjectList.find(pObject->getName()) == mChildObjectList.end());
	mChildObjectList[pObject->getName()] = pObject;
	pObject->_notifyAttached(pAttachingPoint, true);
}


Ogre::MovableObject * Model::detachObjectFromBone (const Ogre::String &movableName)
{
	Ogre::Entity::ChildObjectList::iterator i = mChildObjectList.find(movableName);

	if (i == mChildObjectList.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "No child object entry found named " + movableName, 
			"Entity::detachObjectFromBone");
	}
	MovableObject *obj = i->second;
	detachObjectImpl(obj);
	mChildObjectList.erase(i);

	// Trigger update of bounding box if necessary
	if (mParentNode)
		mParentNode->needUpdate();

	return obj;
	
}


//-----------------------------------------------------------------------
void Model::detachAllObjectsFromBone(void)
{
	detachAllObjectsImpl();

	// Trigger update of bounding box if necessary
	if (mParentNode)
		mParentNode->needUpdate();
}
//-----------------------------------------------------------------------
void Model::detachObjectImpl(MovableObject* pObject)
{
	Ogre::TagPoint* tp = static_cast<Ogre::TagPoint*>(pObject->getParentNode());

	// free the TagPoint so we can reuse it later
	getSkeleton()->freeTagPoint(tp);

	pObject->_notifyAttached((Ogre::TagPoint*)0);
}
//-----------------------------------------------------------------------
void Model::detachAllObjectsImpl(void)
{
	Ogre::Entity::ChildObjectList::const_iterator i, iend;
	iend = mChildObjectList.end();
	for (i = mChildObjectList.begin(); i != iend; ++i)
	{
		detachObjectImpl(i->second);
	}
	mChildObjectList.clear();
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
		
		// Notify any child objects
		Ogre::Entity::ChildObjectList::iterator child_itr = mChildObjectList.begin();
		Ogre::Entity::ChildObjectList::iterator child_itr_end = mChildObjectList.end();
		for( ; child_itr != child_itr_end; child_itr++)
		{
			(*child_itr).second->_notifyCurrentCamera(cam);
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
		
		Ogre::Entity::ChildObjectList::iterator child_itr = mChildObjectList.begin();
		Ogre::Entity::ChildObjectList::iterator child_itr_end = mChildObjectList.end();
		for( ; child_itr != child_itr_end; child_itr++)
		{
			if ((*child_itr).second->isVisible())
				(*child_itr).second->_updateRenderQueue(queue);
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

Ogre::ParticleSystem* ParticleSystem::getOgreParticleSystem()
{
	return mOgreParticleSystem;
}

ParticleSystem::ParticleSystem(Ogre::ParticleSystem* ogreParticleSystem) : 
		mOgreParticleSystem(ogreParticleSystem)
{
	assert(ogreParticleSystem);
}

ParticleSystemBindingsPtrSet& ParticleSystem::getBindings( )
{
	return mBindings;
}

ParticleSystemBinding * ParticleSystem::addBinding( const std::string & emitterVal, const std::string & variableName )
{
	ParticleSystemBinding* binding = new ParticleSystemBinding(this, emitterVal, variableName);
	mBindings.push_back(binding);
	return binding;
}

}
