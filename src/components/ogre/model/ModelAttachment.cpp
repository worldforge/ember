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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "ModelAttachment.h"

#include "domain/IGraphicalRepresentation.h"
#include "domain/EmberEntity.h"
#include "components/ogre/HiddenAttachment.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/Convert.h"
#include "components/ogre/OgreInfo.h"
#include "framework/AttributeObserver.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelBoneProvider.h"
#include "components/ogre/model/ModelFitting.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/EmberEntityActionCreator.h"
#include "components/ogre/EmberEntityPartAction.h"
#include "components/ogre/EmberEntityHideModelAction.h"

#include <Eris/TypeInfo.h>
#include <Eris/View.h>

#include <sigc++/bind.h>
#include <components/ogre/EmberEntityModelAction.h>
#include <components/ogre/PresentAction.h>

#include <utility>

namespace Ember {
namespace OgreView {
namespace Model {


/**
 * Used when an entity is attached to a model through an attach point. We'll show the full model in most cases.
 */
struct ModelAttachedActionCreator : public EntityMapping::IActionCreator {
	EmberEntity& mEntity;
	Scene& mScene;
	AttachmentFunction mModelAttachmentFunction;

	ModelAttachedActionCreator(EmberEntity& entity, Scene& scene, AttachmentFunction modelAttachmentFunction) :
			mEntity(entity),
			mScene(scene),
			mModelAttachmentFunction(std::move(modelAttachmentFunction)) {

	}

	~ModelAttachedActionCreator() override = default;

	void createActions(EntityMapping::EntityMapping& modelMapping,
					   EntityMapping::Cases::CaseBase* aCase,
					   EntityMapping::Definitions::CaseDefinition& caseDefinition) override {
		for (auto& actionDef : caseDefinition.getActions()) {
			if (actionDef.Type == "display-part") {
				aCase->addAction(new EmberEntityPartAction(mEntity, actionDef.getValue()));
			} else if (actionDef.Type == "display-model") {
				aCase->addAction(new EmberEntityModelAction(mEntity, actionDef.getValue(), mScene, modelMapping, mModelAttachmentFunction));
			} else if (actionDef.Type == "hide-model") {
				aCase->addAction(new EmberEntityHideModelAction(mEntity));
			} else if (actionDef.Type == "present") {
				aCase->addAction(new PresentAction(mEntity, mScene, modelMapping, mModelAttachmentFunction));
			} else {
				S_LOG_WARNING("Could not recognize entity action '" << actionDef.Type << "'.");
			}
		}
	}

};

/**
 * Used when an entity is contained in a model, but not attached to an attach point. We'll only show an effect if it's available.
 */
struct ModelContainedActionCreator : public EntityMapping::IActionCreator {
	EmberEntity& mEntity;
	Scene& mScene;

	ModelContainedActionCreator(EmberEntity& entity, Scene& scene) :
			mEntity(entity),
			mScene(scene) {

	}

	~ModelContainedActionCreator() override = default;

	void createActions(EntityMapping::EntityMapping& modelMapping,
					   EntityMapping::Cases::CaseBase* aCase,
					   EntityMapping::Definitions::CaseDefinition& caseDefinition) override {
		for (auto& actionDef : caseDefinition.getActions()) {
			if (actionDef.Type == "display-effect") {
				//TODO: implement effect. One example would be an entity on fire.
			} else {
				S_LOG_WARNING("Could not recognize entity action '" << actionDef.Type << "'.");
			}
		}
	}

};


ModelAttachment::ModelAttachment(EmberEntity& parentEntity, std::unique_ptr<ModelRepresentation> modelRepresentation, std::unique_ptr<INodeProvider> nodeProvider, const std::string& pose) :
		NodeAttachment(parentEntity, modelRepresentation->getEntity(), *nodeProvider.get()),
		mModelRepresentation(std::move(modelRepresentation)),
		mPose(pose),
		mModelMount(std::make_unique<ModelMount>(mModelRepresentation->getModel(), std::move(nodeProvider), mPose)),
		mIgnoreEntityData(false) {
	if (!pose.empty()) {
		const PoseDefinitionStore& poses = mModelRepresentation->getModel().getDefinition()->getPoseDefinitions();
		auto I = poses.find(pose);
		if (I != poses.end()) {
			mIgnoreEntityData = I->second.IgnoreEntityData;
		}
	}

	mModelMount->reset();
	setupFittings();
	mModelRepresentation->getModel().Reloaded.connect(sigc::mem_fun(*this, &ModelAttachment::model_Reloaded));
	mModelMount->getModel().setVisible(mChildEntity.isVisible());
}

ModelAttachment::~ModelAttachment() = default;

IGraphicalRepresentation* ModelAttachment::getGraphicalRepresentation() const {
	return mModelRepresentation.get();
}

void ModelAttachment::attachEntity(EmberEntity& entity) {
//    //Check if we are a domain, and so handle that case.
//    if (entity.hasProperty("domain")) {
//        auto& domainProp = valueOfProperty("domain");
//        if (domainProp.isString()) {
//            if (domainProp.String() == "physical") {
//                setAttachment(std::make_unique<WorldAttachment>(*nearestPhysicalDomainEntity, mScene->getSceneManager().getRootSceneNode()->createChildSceneNode("entity_" + nearestPhysicalDomainEntity->getId())));
//
//            }
//        }
//    }


	std::string attachPoint;
	for (auto& fitting : mFittings) {
		if (fitting.second->getChildEntityId() == entity.getId()) {
			attachPoint = fitting.second->getMountPoint();

			fitting.second->attachChild(entity);
			entity.BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &ModelAttachment::fittedEntity_BeingDeleted), &entity));
			break;
		}
	}

	std::unique_ptr<EntityMapping::IActionCreator> creator;


	if (attachPoint.empty()) {
		//We're not attached, use the contained rules
		//the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
		creator = std::make_unique<ModelContainedActionCreator>(entity, mModelRepresentation->getScene());
	} else {
		//the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
		creator = std::make_unique<ModelAttachedActionCreator>(entity, mModelRepresentation->getScene(), [attachPoint, this, &entity](std::unique_ptr<ModelRepresentation> modelRepresentation) {
			if (modelRepresentation) {
				if (mModelRepresentation->getModel().isLoaded()) {
					try {
						const AttachPointDefinitionStore& attachpoints = mModelRepresentation->getModel().getDefinition()->getAttachPointsDefinitions();
						for (const auto& attachpoint : attachpoints) {
							if (attachpoint.Name == attachPoint) {
								auto nodeProvider = std::make_unique<ModelBoneProvider>(mModelMount->getNodeProvider()->getNode(), mModelRepresentation->getModel(), attachPoint);
								auto nodeAttachment = std::make_unique<ModelAttachment>(getAttachedEntity(), std::move(modelRepresentation), std::move(nodeProvider), attachpoint.Pose);
								entity.setAttachment(std::move(nodeAttachment));
							}
						}
					} catch (const std::exception& ex) {
						S_LOG_WARNING("Failed to attach to attach point '" << attachPoint << "' on model '" << mModelRepresentation->getModel().getDefinition()->getOrigin() << "'.");
					}
				}
			} else {
				entity.setAttachment({});
			}
		});

	}
	auto mapping = Ember::OgreView::Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, *creator, entity.getView()->getTypeService(), entity.getView());
	if (mapping) {
		mapping->initialize();
		std::shared_ptr<EntityMapping::EntityMapping> sharedMapping = std::move(mapping);
		//Retain the mapping while the signal exists.
		entity.BeingDeleted.connect([sharedMapping]() {});
	}

}

void ModelAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) {
	//if the model has an offset specified, use that, else just send to the base class
	const Ogre::Vector3& modelOffset = mModelRepresentation->getModel().getDefinition()->getContentOffset();
	if (modelOffset != Ogre::Vector3::ZERO) {
		offset = Convert::toWF<WFMath::Vector<3>>(modelOffset);
	} else {
		//If the attachment is on a fitting, don't do any adjustment
		for (auto& fitting : mFittings) {
			if (fitting.second->getChildEntityId() == attachment.getAttachedEntity().getId()) {
				return;
			}
		}
		NodeAttachment::getOffsetForContainedNode(attachment, localPosition, offset);
	}
}

void ModelAttachment::setVisible(bool visible) {
	NodeAttachment::setVisible(visible);
	//We set the visibility of the Model here too, even though one might think that it would suffice with the call to NodeAttachment (since that will tell the node provider to set the visibility).
	//However, the issue is that even though the Model has been detached from the scene graph, the light will still be taken into account unless they have their visibility turned off. Therefore the call to Model::setVisible.
	if (mModelMount) {
		mModelMount->getModel().setVisible(visible);
	}
}

void ModelAttachment::updateScale() {
	if (mModelMount) {
		if (getAttachedEntity().hasBBox()) {
			mModelMount->rescale(&getAttachedEntity().getBBox());
		} else {
			mModelMount->rescale(nullptr);
		}
	}
	mModelRepresentation->notifyTransformsChanged();
}

void ModelAttachment::entity_AttrChanged(const Atlas::Message::Element& attributeValue, const std::string& fittingName) {
	auto newFittingEntityId = Eris::Entity::extractEntityId(attributeValue);
	if (newFittingEntityId) {
		auto I = mFittings.find(fittingName);
		if (I != mFittings.end()) {
			EmberEntity* entity = I->second->getChild();
			mFittings.erase(I);
			//Check if we should detach the existing fitting
			if (entity && entity->getId() != *newFittingEntityId) {
				detachFitting(*entity);
			}
		}
		if (!newFittingEntityId->empty()) {
			createFitting(fittingName, *newFittingEntityId);
		}
	} else {
		//Detach any existing fitting.
		auto I = mFittings.find(fittingName);
		if (I != mFittings.end()) {
			EmberEntity* entity = I->second->getChild();
			mFittings.erase(I);
			if (entity) {
				detachFitting(*entity);
			}
		}
	}
}

void ModelAttachment::fittedEntity_BeingDeleted(EmberEntity* entity) {
	for (auto I = mFittings.begin(); I != mFittings.end(); ++I) {
		if (I->second->getChildEntityId() == entity->getId()) {
			mFittings.erase(I);
			break;
		}
	}
}

void ModelAttachment::setupFittings() {
	const AttachPointDefinitionStore& attachpoints = mModelRepresentation->getModel().getDefinition()->getAttachPointsDefinitions();
	for (const auto& attachpoint : attachpoints) {
		auto observer = new AttributeObserver(mChildEntity, attachpoint.Name, ".");
		observer->EventChanged.connect(sigc::bind(sigc::mem_fun(*this, &ModelAttachment::entity_AttrChanged), attachpoint.Name));
		observer->forceEvaluation();
		mFittingsObservers.emplace_back(observer);
	}
}

void ModelAttachment::detachFitting(EmberEntity& entity) {
	//If the detached entity still is a child of this entity, re-evaluate the attachment.
	if (entity.getLocation() == &mChildEntity) {
		entity.setAttachment(nullptr);
		attachEntity(entity);
//		if (attachment) {
//			attachment->updateScale();
//		}
	}
}

void ModelAttachment::createFitting(const std::string& fittingName, const std::string& entityId) {
	auto fitting = std::make_unique<ModelFitting>(mChildEntity, fittingName, entityId);
	mFittings.emplace(fittingName, std::move(fitting));
	for (size_t i = 0; i < mChildEntity.numContained(); ++i) {
		Eris::Entity* entity = mChildEntity.getContained(i);
		if (entity && entity->getId() == entityId) {
			auto emberEntity = dynamic_cast<EmberEntity*>(entity);
			emberEntity->setAttachment(nullptr);
			attachEntity(*emberEntity);
//			if (attachment) {
//				attachment->updateScale();
//			}
			break;
		}
	}
}

void ModelAttachment::model_Reloaded() {
	if (mModelMount) {
		mModelMount->reset();
	}
	updateScale();
	reattachEntities();
}

void ModelAttachment::reattachEntities() {
	for (auto& fitting : mFittings) {
		if (fitting.second->getChild() && mChildEntity.hasChild(fitting.second->getChildEntityId())) {
			EmberEntity* entity = fitting.second->getChild();
			entity->setAttachment(nullptr);
			attachEntity(*entity);
//			if (attachment) {
//				attachment->updateScale();
//			}
		}
	}
}

void ModelAttachment::setVisualize(const std::string& visualization, bool visualize) {
	if (visualization == "OgreBBox") {
		if (mModelMount && mModelMount->getNodeProvider()) {
			mModelMount->getNodeProvider()->setVisualize(visualization, visualize);
		}
	} else {
		NodeAttachment::setVisualize(visualization, visualize);
	}
}

bool ModelAttachment::getVisualize(const std::string& visualization) const {
	if (visualization == "OgreBBox") {
		if (mModelMount && mModelMount->getNodeProvider()) {
			return mModelMount->getNodeProvider()->getVisualize(visualization);
		} else {
			return false;
		}
	} else {
		return NodeAttachment::getVisualize(visualization);
	}
}

void ModelAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity) {
	if (!mIgnoreEntityData) {
		NodeAttachment::setPosition(position, orientation, velocity);

		mModelRepresentation->setLocalVelocity(WFMath::Vector<3>(velocity).rotate(orientation.inverse()));
	} else {
		NodeAttachment::setPosition(WFMath::Point<3>::ZERO(), WFMath::Quaternion::Identity(), WFMath::Vector<3>::ZERO());
	}

	mModelRepresentation->notifyTransformsChanged();
}

}
}
}
