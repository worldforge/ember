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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelBoneProvider.h"
#include "components/ogre/model/ModelFitting.h"

#include <Eris/TypeInfo.h>

#include <OgreManualObject.h>

#include <sigc++/bind.h>

namespace Ember {
namespace OgreView {
namespace Model {

ModelAttachment::ModelAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider, const std::string& pose) :
		NodeAttachment(parentEntity, modelRepresentation.getEntity(), nodeProvider), mModelRepresentation(modelRepresentation), mModelMount(0), mIgnoreEntityData(false), mPose(pose) {
	if (pose != "") {
		const PoseDefinitionStore& poses = mModelRepresentation.getModel().getDefinition()->getPoseDefinitions();
		PoseDefinitionStore::const_iterator I = poses.find(pose);
		if (I != poses.end()) {
			mIgnoreEntityData = I->second.IgnoreEntityData;
		}
	}
}

ModelAttachment::~ModelAttachment() {
	mNodeProvider = nullptr;
	//When the modelmount is deleted the scale node will also be destroyed.
	//Note that there's no need to destroy the light nodes since they are attached to the scale node, which is deleted (along with its children) when the model mount is destroyed.
	delete mModelMount;

	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		delete I->second;
	}

	for (AttributeObserverStore::iterator I = mFittingsObservers.begin(); I != mFittingsObservers.end(); ++I) {
		delete *I;
	}

}

void ModelAttachment::init() {
	NodeAttachment::init();

	mModelMount = new ModelMount(mModelRepresentation.getModel(), mNodeProvider, mPose);
	mModelMount->reset();
	setupFittings();
	mModelRepresentation.getModel().Reloaded.connect(sigc::mem_fun(*this, &ModelAttachment::model_Reloaded));
	mModelMount->getModel().setVisible(mChildEntity.isVisible());
}

IGraphicalRepresentation* ModelAttachment::getGraphicalRepresentation() const {
	return &mModelRepresentation;
}

IEntityAttachment* ModelAttachment::attachEntity(EmberEntity& entity) {
	std::string attachPoint;
	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		if (I->second->getChildEntityId() == entity.getId()) {
			attachPoint = I->second->getMountPoint();
			I->second->attachChild(entity);
			entity.BeingDeleted.connect(sigc::bind(sigc::mem_fun(*this, &ModelAttachment::fittedEntity_BeingDeleted), &entity));
			break;
		}
	}
	//Don't show a graphical representation if the model is set not to show any contained entities AND we're not set to attach ourselves to an attach point.
	if (attachPoint == "" && !mModelRepresentation.getModel().getDefinition()->getShowContained()) {
		return new HiddenAttachment(getAttachedEntity(), entity);
	} else {
		ModelRepresentation* modelRepresentation = ModelRepresentationManager::getSingleton().getRepresentationForEntity(entity);
		//		NodeAttachment* currentNodeAttachment = dynamic_cast<NodeAttachment*> (entity.getAttachment());
		//		ModelAttachment* currentModelAttachment = dynamic_cast<ModelAttachment*> (entity.getAttachment());
		//		if (attachPoint != "") {
		//			return new ModelAttachment(getAttachedEntity(), *modelRepresentation, );
		//		}
		//		else {

		//		if (attachPoint == "" && currentModelAttachment) {
		//			return new ModelAttachment(*currentModelAttachment, *this);
		//		}
		//		else if (attachPoint == "" && currentNodeAttachment) {
		//			return new NodeAttachment(*currentNodeAttachment, *this);
		//		}
		//		else {
		INodeProvider* nodeProvider = nullptr;
		std::string pose;
		if (modelRepresentation && attachPoint != "") {
			if (mModelRepresentation.getModel().isLoaded()) {
				try {
					const AttachPointDefinitionStore& attachpoints = mModelRepresentation.getModel().getDefinition()->getAttachPointsDefinitions();
					for (AttachPointDefinitionStore::const_iterator I = attachpoints.begin(); I != attachpoints.end(); ++I) {
						if (I->Name == attachPoint) {
							pose = I->Pose;
							break;
						}
					}

					nodeProvider = new ModelBoneProvider(mNodeProvider->getNode(), mModelRepresentation.getModel(), attachPoint);
				} catch (const std::exception& ex) {
					S_LOG_WARNING("Failed to attach to attach point '" << attachPoint << "' on model '" << mModelRepresentation.getModel().getDefinition()->getName() << "'.");
					return new HiddenAttachment(entity, getAttachedEntity());
				}
			} else {
				//If the model isn't loaded yet we can't attach yet. Instead we'll return a null attachment and wait until the model is reloaded, at which point reattachEntities() is called.
				return nullptr;
			}
		} else {
			nodeProvider = mNodeProvider->createChildProvider(OgreInfo::createUniqueResourceName(entity.getId()));
		}
		NodeAttachment* nodeAttachment;
		if (modelRepresentation) {
			nodeAttachment = new ModelAttachment(getAttachedEntity(), *modelRepresentation, nodeProvider, pose);
		} else {
			nodeAttachment = new NodeAttachment(getAttachedEntity(), entity, nodeProvider);
		}
		nodeAttachment->init();
		return nodeAttachment;
	}
}

void ModelAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) {
	//if the model has an offset specified, use that, else just send to the base class
	const Ogre::Vector3& modelOffset = mModelRepresentation.getModel().getDefinition()->getContentOffset();
	if (modelOffset != Ogre::Vector3::ZERO) {
		offset = Convert::toWF<WFMath::Vector<3>>(modelOffset);
	} else {
		//If the attachment is on a fitting, don't do any adjustment
		for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
			if (I->second->getChildEntityId() == attachment.getAttachedEntity().getId()) {
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
			mModelMount->rescale(0);
		}
	}
}

void ModelAttachment::entity_AttrChanged(const Atlas::Message::Element& attributeValue, const std::string& fittingName) {
	std::string newFittingEntityId;
	if (Eris::Entity::extractEntityId(attributeValue, newFittingEntityId)) {
		ModelFittingStore::iterator I = mFittings.find(fittingName);
		if (I != mFittings.end()) {
			EmberEntity* entity = I->second->getChild();
			mFittings.erase(I);
			//Check if we should detach the existing fitting
			if (entity && entity->getId() != newFittingEntityId) {
				detachFitting(*entity);
			}
		}
		if (newFittingEntityId != "") {
			createFitting(fittingName, newFittingEntityId);
		}
	}
}

void ModelAttachment::fittedEntity_BeingDeleted(EmberEntity* entity) {
	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		if (I->second->getChildEntityId() == entity->getId()) {
			mFittings.erase(I);
			break;
		}
	}
}

void ModelAttachment::setupFittings() {
	const AttachPointDefinitionStore& attachpoints = mModelRepresentation.getModel().getDefinition()->getAttachPointsDefinitions();
	for (AttachPointDefinitionStore::const_iterator I = attachpoints.begin(); I != attachpoints.end(); ++I) {
		AttributeObserver* observer = new AttributeObserver(mChildEntity, I->Name, ".");
		observer->EventChanged.connect(sigc::bind(sigc::mem_fun(*this, &ModelAttachment::entity_AttrChanged), I->Name));
		mFittingsObservers.push_back(observer);
		observer->forceEvaluation();
	}
}

void ModelAttachment::detachFitting(EmberEntity& entity) {
	//If the detached entity still is a child of this entity, re-evaluate the attachment.
	if (entity.getLocation() == &mChildEntity) {
		IEntityAttachment* attachment = attachEntity(entity);
		entity.setAttachment(attachment);
		if (attachment) {
			attachment->updateScale();
		}
	}
}

void ModelAttachment::createFitting(const std::string& fittingName, const std::string& entityId) {
	ModelFitting* fitting = new ModelFitting(mChildEntity, fittingName, entityId);
	mFittings.insert(ModelFittingStore::value_type(fittingName, fitting));
	for (unsigned int i = 0; i < mChildEntity.numContained(); ++i) {
		Eris::Entity* entity = mChildEntity.getContained(i);
		if (entity && entity->getId() == entityId) {
			EmberEntity* emberEntity = static_cast<EmberEntity*>(entity);
			IEntityAttachment* attachment = attachEntity(*emberEntity);
			emberEntity->setAttachment(attachment);
			if (attachment) {
				attachment->updateScale();
			}
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
	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		if (I->second->getChild() && mChildEntity.hasChild(I->second->getChildEntityId())) {
			EmberEntity* entity = I->second->getChild();
			IEntityAttachment* attachment = attachEntity(*entity);
			entity->setAttachment(attachment);
			if (attachment) {
				attachment->updateScale();
			}
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

		mModelRepresentation.setLocalVelocity(WFMath::Vector<3>(velocity).rotate(orientation.inverse()));
	} else {
		NodeAttachment::setPosition(WFMath::Point<3>::ZERO(), WFMath::Quaternion::Identity(), WFMath::Vector<3>::ZERO());
	}
}

}
}
}
