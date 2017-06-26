/*
 Copyright (C) 2004  Erik Ogenvik
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Model.h"
#include "SubModel.h"
#include "SubModelPart.h"
#include "ParticleSystemBinding.h"

#include "ModelDefinitionManager.h"


#include "framework/TimeFrame.h"
#include "framework/TimedLog.h"

#include <OgreTagPoint.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSubEntity.h>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>

namespace Ember {
namespace OgreView {
namespace Model {

Model::Model(Ogre::SceneManager& manager, const Ogre::SharedPtr<ModelDefinition>& definition, const std::string& name) :
		mManager(manager),
		mDefinition(definition),
		mParentNodeProvider(nullptr),
		mName(name),
		mSkeletonOwnerEntity(nullptr),
		mSkeletonInstance(nullptr),
		mScale(0.0f),
		mRotation(Ogre::Quaternion::IDENTITY),
		mAnimationStateSet(nullptr),
		mAttachPoints(nullptr),
		mVisible(true),
		mRenderingDistance(0),
		mQueryFlags(0),
		mLoaded(false) {
}

Model::~Model() {

	for (auto& movable : mMovableObjects) {
		mManager.destroyMovableObject(movable);
	}
	mDefinition->removeFromLoadingQueue(this);

//	S_LOG_VERBOSE("Deleted "<< getName());
}

void Model::reset() {
//	S_LOG_VERBOSE("Resetting "<< getName());
	Resetting.emit();
	//	resetAnimations();
	resetSubmodels();
	resetParticles();
	resetLights();
	mScale = 0;
	mRotation = Ogre::Quaternion::IDENTITY;
	mSkeletonInstance = nullptr;
	// , mAnimationStateSet(0)
	mSkeletonOwnerEntity = nullptr;
	mAttachPoints = std::unique_ptr<std::vector<AttachPointWrapper>>(nullptr);

}

const Ogre::SharedPtr<ModelDefinition>& Model::getDefinition() const {
	return mDefinition;
}

bool Model::load() {
	return mDefinition->requestLoad(this);
}

bool Model::reload() {
	if (mAssetCreationContext.mCurrentlyLoadingSubModelIndex == 0) {
		reset();
	}
	bool result = createModelAssets();
	//if we are attached, we have to notify the new entities, else they won't appear in the scene
	//_notifyAttached(mParentNode, mParentIsTagPoint);

	if (result) {
		Reloaded.emit();
	}
	return result;
}

//void Model::loadingComplete(Ogre::Resource*) {
//	//This is called when the mesh is reloaded and it has a skeleton; we need to reset the actions since they now refer to invalid animation states
//
//	for (auto& subModel : mSubmodels) {
//		//We need to call _initialise in order for the animation states to get recreated;
//		//else this will happen lazily the next time the entity is rendered.
//		subModel->getEntity()->_initialise(true);
//	}
//
//	for (auto& actionDef : mDefinition->getActionDefinitions()) {
//		auto actionI = mActions.find(actionDef->getName());
//		if (actionI != mActions.end()) {
//			Action& action = actionI->second;
//			//Important these calls happen in this order, else we'll risk segfaults
//			action.getAnimations().getAnimations().clear();
//			action.getAnimations().reset();
//
//			for (auto& animationDef : actionDef->getAnimationDefinitions()) {
//				Animation animation(animationDef->getIterations(), getSkeleton()->getNumBones());
//				for (auto& animationPartDef : animationDef->getAnimationPartDefinitions()) {
//					if (getAllAnimationStates()->hasAnimationState(animationPartDef->Name)) {
//						AnimationPart animPart;
//						try {
//							Ogre::AnimationState* state = getAnimationState(animationPartDef->Name);
//							animPart.state = state;
//							for (auto& boneGroupDef : animationPartDef->BoneGroupRefs) {
//								auto I_boneGroup = mDefinition->getBoneGroupDefinitions().find(boneGroupDef.Name);
//								if (I_boneGroup != mDefinition->getBoneGroupDefinitions().end()) {
//									BoneGroupRef boneGroupRef;
//									boneGroupRef.boneGroupDefinition = I_boneGroup->second;
//									boneGroupRef.weight = boneGroupDef.Weight;
//									animPart.boneGroupRefs.push_back(boneGroupRef);
//								}
//							}
//							animation.addAnimationPart(animPart);
//						} catch (const std::exception& ex) {
//							S_LOG_FAILURE("Error when loading animation: " << animationPartDef->Name << "." << ex);
//						}
//					}
//				}
//				action.getAnimations().addAnimation(animation);
//			}
//		}
//	}
//}

bool Model::createModelAssets() {
	TimedLog timedLog("Model::createActualModel " + mDefinition->getName());

	if (mAssetCreationContext.mCurrentlyLoadingSubModelIndex < mDefinition->getSubModelDefinitions().size()) {
		auto& submodelDef = mDefinition->getSubModelDefinitions()[mAssetCreationContext.mCurrentlyLoadingSubModelIndex];
		try {
			Ogre::Entity* entity;
			if (!mName.empty()) {
				std::string entityName = mName + "/" + submodelDef->getMeshName();
				entity = mManager.createEntity(entityName, submodelDef->getMeshName());
			} else {
				entity = mManager.createEntity(submodelDef->getMeshName());
			}
			timedLog.report("Created entity " + submodelDef->getMeshName());
			if (entity->getMesh().isNull()) {
				S_LOG_FAILURE("Could not load mesh " << submodelDef->getMeshName() << " which belongs to model " << mDefinition->getName() << ".");
			}

			SubModel* submodel = new SubModel(*entity);
			//Model::SubModelPartMapping* submodelPartMapping = new Model::SubModelPartMapping();

			if (!submodelDef->getPartDefinitions().empty()) {
				for (auto& partDef : submodelDef->getPartDefinitions()) {
					SubModelPart& part = submodel->createSubModelPart(partDef->getName());
					//std::string groupName("");

					if (partDef->getSubEntityDefinitions().size() > 0) {
						for (auto& subEntityDef : partDef->getSubEntityDefinitions()) {
							try {
								Ogre::SubEntity* subEntity(0);
								//try with a submodelname first
								if (subEntityDef->getSubEntityName() != "") {
									subEntity = entity->getSubEntity(subEntityDef->getSubEntityName());
								} else {
									//no name specified, use the index instead
									if (entity->getNumSubEntities() > subEntityDef->getSubEntityIndex()) {
										subEntity = entity->getSubEntity(subEntityDef->getSubEntityIndex());
									} else {
										S_LOG_WARNING("Model definition " << mDefinition->getName() << " has a reference to entity with index " << subEntityDef->getSubEntityIndex() << " which is out of bounds.");
									}
								}
								if (subEntity) {
									part.addSubEntity(subEntity, subEntityDef);

									if (subEntityDef->getMaterialName() != "") {
										subEntity->setMaterialName(subEntityDef->getMaterialName());
									}
								} else {
									S_LOG_WARNING("Could not add subentity.");
								}
							} catch (const std::exception& ex) {
								S_LOG_WARNING("Error when getting sub entities for model '" << mDefinition->getName() << "'." << ex);
							}
						}
					} else {
						//if no subentities are defined, add all subentities
						unsigned int numSubEntities = entity->getNumSubEntities();
						for (unsigned int i = 0; i < numSubEntities; ++i) {
							part.addSubEntity(entity->getSubEntity(i), 0);
						}
					}
					if (partDef->getGroup() != "") {
						mAssetCreationContext.mGroupsToPartMap[partDef->getGroup()].push_back(partDef->getName());
						//mPartToGroupMap[partDef->getName()] = partDef->getGroup();
					}

					if (partDef->getShow()) {
						mAssetCreationContext.showPartVector.push_back(partDef->getName());
					}

					ModelPart& modelPart = mAssetCreationContext.mModelParts[partDef->getName()];
					modelPart.addSubModelPart(&part);
					modelPart.setGroupName(partDef->getGroup());
				}
			} else {
				//if no parts are defined, add a default "main" part and add all subentities to it. This ought to be a good default behaviour
				SubModelPart& part = submodel->createSubModelPart("main");
				for (unsigned int i = 0; i < entity->getNumSubEntities(); ++i) {
					Ogre::SubEntity* subentity = entity->getSubEntity(i);
					part.addSubEntity(subentity, nullptr);
				}
				mAssetCreationContext.showPartVector.push_back(part.getName());
				ModelPart& modelPart = mAssetCreationContext.mModelParts[part.getName()];
				modelPart.addSubModelPart(&part);
			}
			mAssetCreationContext.mSubmodels.insert(submodel);
			timedLog.report("Created submodel.");
		} catch (const std::exception& e) {
			S_LOG_FAILURE("Submodel load error for mesh '" << submodelDef->getMeshName() << "'." << e);
		}
		mAssetCreationContext.mCurrentlyLoadingSubModelIndex++;
		return false;
	}


	setRenderingDistance(mDefinition->getRenderingDistance());

	for (auto submodel : mAssetCreationContext.mSubmodels) {
		addSubmodel(submodel);
	}
	mModelParts = mAssetCreationContext.mModelParts;
	mGroupsToPartMap = mAssetCreationContext.mGroupsToPartMap;

	createActions();
	timedLog.report("Created actions.");

	createParticles();
	timedLog.report("Created particles.");

	createLights();
	timedLog.report("Created lights.");

	mScale = mDefinition->mScale;
	mRotation = mDefinition->mRotation;
	for (auto& part : mAssetCreationContext.showPartVector) {
		showPart(part);
	}
	mLoaded = true;
	mAssetCreationContext = AssetCreationContext();
	return true;
}

void Model::createActions() {

//	//If the mesh has a skeleton we'll add a listener to the mesh, so that we can reload the animation states when
//	//the mesh or skeleton gets reloaded.
//	if (getSkeleton()) {
//		mSkeletonOwnerEntity->getMesh()->addListener(this);
//	}

	for (auto& actionDef : mDefinition->getActionDefinitions()) {
		Action action;
		action.mActivations = actionDef->getActivationDefinitions();
		action.setName(actionDef->getName());
		action.getAnimations().setSpeed(actionDef->getAnimationSpeed());

		if (getSkeleton() && getAllAnimationStates()) {

			if (!mDefinition->getBoneGroupDefinitions().empty()) {
				//If there are bone groups, we need to use a cumulative blend mode. Note that this will affect all animations in the model.
				getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
			}
			if (!mSubmodels.empty()) {
				for (auto& animationDef : actionDef->getAnimationDefinitions()) {
					Animation animation(animationDef->getIterations(), getSkeleton()->getNumBones());
					for (auto& animationPartDef : animationDef->getAnimationPartDefinitions()) {
						if (getAllAnimationStates()->hasAnimationState(animationPartDef->Name)) {
							AnimationPart animPart;
							try {
								Ogre::AnimationState* state = getAnimationState(animationPartDef->Name);
								animPart.state = state;
								for (auto& boneGroupDef : animationPartDef->BoneGroupRefs) {
									auto I_boneGroup = mDefinition->getBoneGroupDefinitions().find(boneGroupDef.Name);
									if (I_boneGroup != mDefinition->getBoneGroupDefinitions().end()) {
										BoneGroupRef boneGroupRef;
										boneGroupRef.boneGroupDefinition = I_boneGroup->second;
										boneGroupRef.weight = boneGroupDef.Weight;
										animPart.boneGroupRefs.push_back(boneGroupRef);
									}
								}
								animation.addAnimationPart(animPart);
							} catch (const std::exception& ex) {
								S_LOG_FAILURE("Error when loading animation: " << animationPartDef->Name << "." << ex);
							}
						}
					}
					action.getAnimations().addAnimation(animation);
				}
			}
		}

		//TODO: add sounds too

		mActions[actionDef->getName()] = action;
	}
}

void Model::createParticles() {
	for (auto& particleSystemDef : mDefinition->mParticleSystems) {
		//first try to create the ogre particle system
		Ogre::ParticleSystem* ogreParticleSystem;

		try {
			if (!mName.empty()) {
				std::string name(mName + "/particle" + particleSystemDef.Script);
				ogreParticleSystem = mManager.createParticleSystem(name, particleSystemDef.Script);
			} else {
				ogreParticleSystem = mManager.createParticleSystem(particleSystemDef.Script);
			}
			if (ogreParticleSystem) {
				//Try to trigger a load of any image resources used by affectors.
				//The reason we want to do this now is that otherwise it will happen during rendering. An exception will then be thrown
				//which will bubble all the way up to the main loop, thus aborting all frames.
				ogreParticleSystem->_update(0);

				//ogreParticleSystem->setDefaultDimensions(1, 1);
				ParticleSystem* particleSystem = new ParticleSystem(ogreParticleSystem, particleSystemDef.Direction);
				for (auto& bindingDef : particleSystemDef.Bindings) {
					ParticleSystemBinding* binding = particleSystem->addBinding(bindingDef.EmitterVar, bindingDef.AtlasAttribute);
					mAllParticleSystemBindings.push_back(binding);
				}
				mParticleSystems.push_back(particleSystem);


				//Check if the material used is transparent. If so, assign it a later render queue.
				//This is done to make transparent particle systems play better with the foliage and the water.
				//The foliage would be rendered at an earlier render queue (RENDER_QUEUE_6 normally) and the water at RENDER_QUEUE_8.
				//This of course means that there's still an issue when the camera is below the water
				//(as the water, being rendered first, will prevent the particles from being rendered). That will need to be solved.
				Ogre::MaterialPtr materialPtr = Ogre::MaterialManager::getSingleton().getByName(ogreParticleSystem->getMaterialName());
				if (!materialPtr.isNull()) {
					if (materialPtr->isTransparent()) {
						ogreParticleSystem->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
					}
				}

				addMovable(ogreParticleSystem);
			}
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Could not create particle system: " << particleSystemDef.Script << "." << ex);
		}
	}
}

void Model::createLights() {
	int j = 0;
	for (auto& lightDef : mDefinition->mLights) {
		//first try to create the ogre lights
		LightInfo lightInfo;
		Ogre::Light* ogreLight;
		try {
			if (!mName.empty()) {
				std::stringstream name;
				name << mName << "/light" << (j++);
				ogreLight = mManager.createLight(name.str());
			} else {
				ogreLight = mManager.createLight();
			}
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Could not create light." << ex);
			continue;
		}
		if (ogreLight) {
			ogreLight->setType(Ogre::Light::LT_POINT);
			ogreLight->setDiffuseColour(lightDef.diffuseColour);
			ogreLight->setSpecularColour(lightDef.specularColour);
			ogreLight->setAttenuation(lightDef.range, lightDef.constant, lightDef.linear, lightDef.quadratic);
			ogreLight->setPosition(lightDef.position);
			//ogreLight->setDiffuseColour(Ogre::ColourValue(0.5f,0.0f,0.0f));
			//ogreLight->setSpecularColour(Ogre::ColourValue(0.5f,0.0f,0.0f));
			//ogreLight->setAttenuation(100,1,0,0);
			//ogreLight->setSpotlightRange(Ogre::Degree(60), Ogre::Degree(70));
			//ogreLight->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y);

			lightInfo.light = ogreLight;
			lightInfo.position = lightDef.position;
			mLights.push_back(lightInfo);
			addMovable(ogreLight);
		}

	}
}

bool Model::hasParticles() const {
	return !mParticleSystems.empty();
}

const ParticleSystemBindingsPtrSet& Model::getAllParticleSystemBindings() const {
	return mAllParticleSystemBindings;
}

ParticleSystemSet& Model::getParticleSystems() {
	return mParticleSystems;
}

LightSet& Model::getLights() {
	return mLights;
}

bool Model::addSubmodel(SubModel* submodel) {
	//if the submodel has a skeleton, check if it should be shared with existing models
	if (submodel->getEntity()->getSkeleton()) {
		if (mSkeletonOwnerEntity != nullptr) {
			submodel->getEntity()->shareSkeletonInstanceWith(mSkeletonOwnerEntity);
		} else {
			mSkeletonOwnerEntity = submodel->getEntity();
			// 			mAnimationStateSet = submodel->getEntity()->getAllAnimationStates();
		}
	}
	auto result = mSubmodels.insert(submodel);
	if (result.second) {
		addMovable(submodel->getEntity());
	}
	return true;
}

bool Model::removeSubmodel(SubModel* submodel) {
	auto numberRemoved = mSubmodels.erase(submodel);
	if (numberRemoved > 0) {
		removeMovable(submodel->getEntity());
	}
	return true;
}

SubModel* Model::getSubModel(size_t index) {
	size_t i = 0;
	auto result = std::find_if(mSubmodels.begin(), mSubmodels.end(),
							   [&i, &index](SubModel*) -> bool { return i++ == index; }
	);
	if (result != mSubmodels.end()) {
		return *result;
	}
	S_LOG_FAILURE("Could not find submodel with index " << index << " in model " << mName);
	return 0;

}

void Model::showPart(const std::string& partName, bool hideOtherParts) {
	auto I = mModelParts.find(partName);
	if (I != mModelParts.end()) {
		ModelPart& modelPart = I->second;
		if (hideOtherParts) {
			const std::string& groupName = modelPart.getGroupName();
			//make sure that all other parts in the same group are hidden
			PartGroupStore::iterator partBucketI = mGroupsToPartMap.find(groupName);
			if (partBucketI != mGroupsToPartMap.end()) {
				for (auto& part : partBucketI->second) {
					if (part != partName) {
						hidePart(part, true);
					}
				}
			}
		}

		modelPart.show();
	}
}

void Model::hidePart(const std::string& partName, bool dontChangeVisibility) {
	ModelPartStore::iterator I = mModelParts.find(partName);
	if (I != mModelParts.end()) {
		ModelPart& modelPart = I->second;
		modelPart.hide();
		if (!dontChangeVisibility) {
			modelPart.setVisible(false);
			const std::string& groupName = modelPart.getGroupName();
			//if some part that was hidden before now should be visible
			PartGroupStore::iterator partBucketI = mGroupsToPartMap.find(groupName);
			if (partBucketI != mGroupsToPartMap.end()) {
				for (auto& part : partBucketI->second) {
					if (part != partName) {
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

void Model::setVisible(bool visible) {
	mVisible = visible;
	for (auto& movable : mMovableObjects) {
		movable->setVisible(visible);
	}
}

void Model::setDisplaySkeleton(bool display) {
	for (auto& submodel : mSubmodels) {
		submodel->getEntity()->setDisplaySkeleton(display);
	}
}

bool Model::getDisplaySkeleton(void) const {
	const auto I = mSubmodels.begin();
	if (I != mSubmodels.end()) {
		return (*I)->getEntity()->getDisplaySkeleton();
	}
	return false;
}

Ogre::Real Model::getScale() const {
	return mScale;
}

const Ogre::Quaternion& Model::getRotation() const {
	return mRotation;
}

ModelDefinition::UseScaleOf Model::getUseScaleOf() const {
	return mDefinition->getUseScaleOf();
}

Action* Model::getAction(const std::string& name) {
	const auto I = mActions.find(name);
	if (I == mActions.end()) {
		return nullptr;
	}
	return &(I->second);
}

Action* Model::getAction(const ActivationDefinition::Type type, const std::string& trigger) {
	for (auto& entry : mActions) {
		Action& action = entry.second;
		for (auto& activationDefinition : action.mActivations) {
			if (type == activationDefinition.type && trigger == activationDefinition.trigger) {
				//FIXME: Should keep actions as pointers
				return &action;
			}
		}
	}
	return nullptr;
}

void Model::addMovable(Ogre::MovableObject* movable) {
	if (mParentNodeProvider) {
		mParentNodeProvider->attachObject(movable);
	}
	if (mUserObject) {
		movable->getUserObjectBindings().setUserAny(Ogre::Any(mUserObject));
	}

	movable->setQueryFlags(mQueryFlags);
	if (mRenderingDistance > 0) {
		movable->setRenderingDistance(mRenderingDistance);
	}

	mMovableObjects.push_back(movable);

}

void Model::removeMovable(Ogre::MovableObject* movable) {
	if (mParentNodeProvider) {
		mParentNodeProvider->detachObject(movable);
	}
	if (mUserObject) {
		movable->getUserObjectBindings().clear();
	}

	mMovableObjects.erase(std::find(std::begin(mMovableObjects), std::end(mMovableObjects), movable));
}

void Model::resetSubmodels() {
	if (!mSubmodels.empty()) {
		for (auto& submodel : mSubmodels) {
			removeSubmodel(submodel);
			delete submodel;
		}
		mSubmodels.clear();
	}
	mModelParts.clear();
}

void Model::resetParticles() {
	if (!mParticleSystems.empty()) {
		for (auto& system : mParticleSystems) {
			removeMovable(system->getOgreParticleSystem());
			delete system;
		}
		mParticleSystems.clear();
	}
	mAllParticleSystemBindings.clear();
}

void Model::resetLights() {
	if (!mLights.empty()) {
		for (auto& lightInfo : mLights) {
			Ogre::Light* light = lightInfo.light;
			if (light) {
				removeMovable(light);
				mManager.destroyLight(light);
			}
		}
		mLights.clear();
	}
}

void Model::attachToNode(INodeProvider* nodeProvider) {
	for (auto& movable : mMovableObjects) {

		if (mParentNodeProvider && mParentNodeProvider != nodeProvider) {
			mParentNodeProvider->detachObject(movable);
		}
		if (nodeProvider) {
			nodeProvider->attachObject(movable);
		}
	}
	mParentNodeProvider = nodeProvider;

}

Ogre::TagPoint* Model::attachObject(const std::string& attachPoint, Ogre::MovableObject* movable) {
	if (mSkeletonOwnerEntity) {
		for (auto& attachPointDef : mDefinition->getAttachPointsDefinitions()) {
			if (attachPointDef.Name == attachPoint) {
				const std::string& boneName = attachPointDef.BoneName;
				//use the rotation in the attach point def
				Ogre::TagPoint* tagPoint = mSkeletonOwnerEntity->attachObjectToBone(boneName, movable);
				if (!mAttachPoints.get()) {
					mAttachPoints = std::make_unique<std::vector<AttachPointWrapper>>();
				}

				AttachPointWrapper wrapper;
				wrapper.TagPoint = tagPoint;
				wrapper.Movable = movable;
				wrapper.Definition = attachPointDef;
				mAttachPoints.get()->push_back(wrapper);
				return tagPoint;
			}
		}
	}
	return nullptr;
}

bool Model::hasAttachPoint(const std::string& attachPoint) const {
	return std::find_if(mDefinition->mAttachPoints.begin(), mDefinition->mAttachPoints.end(), [&attachPoint](const AttachPointDefinition& def) -> bool { return def.Name == attachPoint; }) != mDefinition->mAttachPoints.end();
}

void Model::detachObject(Ogre::MovableObject* movable) {
	if (mSkeletonOwnerEntity) {
		mSkeletonOwnerEntity->detachObjectFromBone(movable);
	}
	if (mAttachPoints) {
		std::vector<AttachPointWrapper>& attachPoints = *mAttachPoints.get();
		for (auto I = attachPoints.begin(); I != attachPoints.end(); ++I) {
			if (I->Movable == movable) {
				attachPoints.erase(I);
				break;
			}
		}
	}
}

Ogre::AnimationState* Model::getAnimationState(const Ogre::String& name) {
	if (!mSubmodels.empty() && mSkeletonOwnerEntity) {
		return mSkeletonOwnerEntity->getAnimationState(name);
	} else {
		return nullptr;
	}
}

Ogre::AnimationStateSet* Model::getAllAnimationStates() {
	if (!mSubmodels.empty() && mSkeletonOwnerEntity) {
		return mSkeletonOwnerEntity->getAllAnimationStates();
	} else {
		return nullptr;
	}
}

Ogre::SkeletonInstance* Model::getSkeleton() const {
	if (!mSubmodels.empty() && mSkeletonOwnerEntity) {
		return mSkeletonOwnerEntity->getSkeleton();
	} else {
		return nullptr;
	}
}


void Model::setRenderingDistance(Ogre::Real dist) {
	mRenderingDistance = dist;
	if (dist > 0) {
		for (auto& movable : mMovableObjects) {
			movable->setRenderingDistance(dist);
		}
	}
}

void Model::setQueryFlags(unsigned int flags) {
	mQueryFlags = flags;
	for (auto& movable : mMovableObjects) {
		movable->setQueryFlags(flags);
	}
}

/** Overridden from MovableObject */
//void Model::_notifyAttached(Ogre::Node* parent, bool isTagPoint) {


//	auto I = mParticleSystems.begin();
//	while (I != mParticleSystems.end()) {
//
//		(*I)->getOgreParticleSystem()->_notifyAttached(parent, isTagPoint);
//		try {
//			//Try to trigger a load of any image resources used by affectors.
//			//The reason we want to do this now is that otherwise it will happen during rendering. An exception will then be thrown
//			//which will bubble all the way up to the main loop, thus aborting all frames.
//			(*I)->getOgreParticleSystem()->_update(0);
//
//			//Check if the material used is transparent. If so, assign it a later render queue.
//			//This is done to make transparent particle systems play better with the foliage and the water.
//			//The foliage would be rendered at an earlier render queue (RENDER_QUEUE_6 normally) and the water at RENDER_QUEUE_8.
//			//This of course means that there's still an issue when the camera is below the water
//			//(as the water, being rendered first, will prevent the particles from being rendered). That will need to be solved.
//			std::pair<Ogre::ResourcePtr, bool> result = Ogre::MaterialManager::getSingleton().createOrRetrieve((*I)->getOgreParticleSystem()->getMaterialName(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//			Ogre::MaterialPtr materialPtr = result.first.staticCast<Ogre::Material>();
//			if (!materialPtr.isNull()) {
//				if (materialPtr->isTransparent()) {
//					(*I)->getOgreParticleSystem()->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
//				}
//			}
//
//			++I;
//		} catch (const Ogre::Exception& ex) {
//			//An exception occurred when forcing an update of the particle system. Remove it.
//			S_LOG_FAILURE("Error when loading particle system " << (*I)->getOgreParticleSystem()->getName() << ". Removing it.");
//			mMovableObjects.erase(std::find(std::begin(mMovableObjects), std::end(mMovableObjects), (*I)->getOgreParticleSystem()));
//			delete *I;
//			I = mParticleSystems.erase(I);
//		}
//	}


//}

const std::unique_ptr<std::vector<Model::AttachPointWrapper>>& Model::getAttachedPoints() const {
	return mAttachPoints;
}

void Model::setUserObject(std::shared_ptr<EmberEntityUserObject> userObject) {
	for (auto& movable : mMovableObjects) {
		movable->getUserObjectBindings().setUserAny(Ogre::Any(userObject));
	}

	mUserObject = userObject;
}

Ogre::SceneManager& Model::getManager() {
	return mManager;
}

float Model::getCombinedBoundingRadius() const {
	float radius = 0;
	for (auto& movable : mMovableObjects) {
		if (movable->isVisible()) {
			radius = std::max(movable->getBoundingRadius(), radius);
		}
	}
	return radius;
}

Ogre::AxisAlignedBox Model::getCombinedBoundingBox() const {
	Ogre::AxisAlignedBox aabb;
	for (auto& movable : mMovableObjects) {
		if (movable->isVisible()) {
			aabb.merge(movable->getBoundingBox());
		}
	}
	return aabb;
}

const INodeProvider* Model::getNodeProvider() const {
	return mParentNodeProvider;
}

bool Model::isLoaded() const {
	return mLoaded;
}


}
}
}
