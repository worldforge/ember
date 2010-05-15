/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "components/ogre/IGraphicalRepresentation.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/HiddenAttachment.h"
#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/Convert.h"
#include "framework/AttributeObserver.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelMount.h"
#include "components/ogre/model/ModelRepresentation.h"
#include "components/ogre/model/ModelRepresentationManager.h"
#include "components/ogre/model/ModelBoneProvider.h"
#include "components/ogre/model/ModelFitting.h"

#include <sigc++/bind.h>
#include <sigc++/slot.h>
#include <sigc++/signal.h>
#include <sigc++/connection.h>

#include <Atlas/Message/Element.h>

#include <OgreVector3.h>

namespace EmberOgre
{
namespace Model
{

ModelAttachment::ModelAttachment(EmberEntity& parentEntity, ModelRepresentation& modelRepresentation, INodeProvider* nodeProvider) :
	NodeAttachment::NodeAttachment(parentEntity, modelRepresentation.getEntity(), nodeProvider), mModelRepresentation(modelRepresentation), mModelMount(0)
{
	mModelMount = new ModelMount(mModelRepresentation.getModel(), nodeProvider->createChildProvider(&mModelRepresentation.getModel()));
	mModelMount->reset();
	setupFittings();
	mModelRepresentation.getModel().Reloaded.connect(sigc::mem_fun(*this, &ModelAttachment::model_Reloaded));
}

ModelAttachment::ModelAttachment(ModelAttachment& source, NodeAttachment& newParentAttachment) :
	NodeAttachment::NodeAttachment(source, newParentAttachment), mModelRepresentation(source.mModelRepresentation), mModelMount(source.mModelMount)
{
	source.mModelMount = 0;
	updateScale();
	mModelRepresentation.getModel().Reloaded.connect(sigc::mem_fun(*this, &ModelAttachment::model_Reloaded));
}

ModelAttachment::~ModelAttachment()
{
	///When the modelmount is deleted the scale node will also be destroyed.
	///Note that there's no need to destroy the light nodes since they are attached to the scale node, which is deleted (along with its children) when the model mount is destroyed.
	delete mModelMount;

	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		delete I->second;
	}

	for (AttributeObserverStore::iterator I = mFittingsObservers.begin(); I != mFittingsObservers.end(); ++I) {
		delete *I;
	}

}

IGraphicalRepresentation* ModelAttachment::getGraphicalRepresentation() const
{
	return &mModelRepresentation;
}

IEntityAttachment* ModelAttachment::attachEntity(EmberEntity& entity)
{
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
		INodeProvider* nodeProvider(0);
		if (attachPoint != "") {
			if (mModelRepresentation.getModel().isLoaded()) {
				nodeProvider = new ModelBoneProvider(mModelRepresentation.getModel(), attachPoint, modelRepresentation ? &modelRepresentation->getModel() : 0);
			} else {
				//If the model isn't loaded yet we can't attach yet. Instead we'll return a null attachment and wait until the model is reloaded, at which point reattachEntities() is called.
				return 0;
			}
		} else {
			nodeProvider = mNodeProvider->createChildProvider(modelRepresentation ? &modelRepresentation->getModel() : 0);
		}
		if (modelRepresentation) {
			return new ModelAttachment(getAttachedEntity(), *modelRepresentation, nodeProvider);
		} else {
			return new NodeAttachment(getAttachedEntity(), entity, nodeProvider);
		}
		//		}
		//		}
	}
}

void ModelAttachment::getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset)
{
	///if the model has an offset specified, use that, else just send to the base class
	const Ogre::Vector3& modelOffset(mModelRepresentation.getModel().getDefinition()->getContentOffset());
	if (modelOffset != Ogre::Vector3::ZERO) {
		offset = Convert::toWF<WFMath::Vector<3> >(modelOffset);
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

void ModelAttachment::setVisible(bool visible)
{
	NodeAttachment::setVisible(visible);
	//We set the visibility of the Model here too, even though one might think that it would suffice with the call to NodeAttachment (since that will tell the node provider to set the visibility).
	//However, the issue is that even though the Model has been detached from the scene graph, the light will still be taken into account unless they have their visibility turned off. Therefore the call to Model::setVisible.
	if (mModelMount) {
		mModelMount->getModel().setVisible(visible);
	}
}

void ModelAttachment::updateScale()
{
	if (mModelMount) {
		if (getAttachedEntity().hasBBox()) {
			mModelMount->rescale(&getAttachedEntity().getBBox());
		} else {
			mModelMount->rescale(0);
		}
	}
}

void ModelAttachment::entity_AttrChanged(const Atlas::Message::Element& attributeValue, const std::string& fittingName)
{
	if (attributeValue.isString()) {
		const std::string& newFittingEntityId = attributeValue.asString();

		ModelFittingStore::iterator I = mFittings.find(fittingName);
		if (I != mFittings.end()) {
			mFittings.erase(I);
			//Check if we should detach the existing fitting
			if (I->second->getChild() && I->second->getChild()->getId() != newFittingEntityId) {
				detachFitting(*I->second->getChild());
			}
		}
		if (newFittingEntityId != "") {
			createFitting(fittingName, newFittingEntityId);
		}
	}
}

void ModelAttachment::fittedEntity_BeingDeleted(EmberEntity* entity)
{
	for (ModelFittingStore::iterator I = mFittings.begin(); I != mFittings.end(); ++I) {
		if (I->second->getChildEntityId() == entity->getId()) {
			mFittings.erase(I);
			break;
		}
	}
}

void ModelAttachment::setupFittings()
{
	const AttachPointDefinitionStore& attachpoints = mModelRepresentation.getModel().getDefinition()->getAttachPointsDefinitions();
	for (AttachPointDefinitionStore::const_iterator I = attachpoints.begin(); I != attachpoints.end(); ++I) {
		Ember::AttributeObserver* observer = new Ember::AttributeObserver(mChildEntity, I->Name, ".");
		observer->EventChanged.connect(sigc::bind(sigc::mem_fun(*this, &ModelAttachment::entity_AttrChanged), I->Name));
		mFittingsObservers.push_back(observer);
		observer->forceEvaluation();
	}
}

void ModelAttachment::detachFitting(EmberEntity& entity)
{
	//If the detached entity still is a child of this entity, re-evaluate the attachment.
	if (entity.getLocation() == &mChildEntity) {
		IEntityAttachment* attachment = attachEntity(entity);
		entity.setAttachment(attachment);
		if (attachment) {
			attachment->updateScale();
		}
	}
}

void ModelAttachment::createFitting(const std::string& fittingName, const std::string& entityId)
{
	ModelFitting* fitting = new ModelFitting(mChildEntity, fittingName, entityId);
	mFittings.insert(ModelFittingStore::value_type(fittingName, fitting));
	for (unsigned int i = 0; i < mChildEntity.numContained(); ++i) {
		Eris::Entity* entity = mChildEntity.getContained(i);
		if (entity && entity->getId() == entityId) {
			EmberEntity* emberEntity = static_cast<EmberEntity*> (entity);
			IEntityAttachment* attachment = attachEntity(*emberEntity);
			emberEntity->setAttachment(attachment);
			if (attachment) {
				attachment->updateScale();
			}
		}
	}
}

void ModelAttachment::model_Reloaded()
{
	updateScale();
	reattachEntities();
}

void ModelAttachment::reattachEntities()
{
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

void ModelAttachment::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "OgreBBox") {
		if (mModelMount && mModelMount->getNodeProvider()) {
			mModelMount->getNodeProvider()->setVisualize(visualization, visualize);
		}
	} else {
		NodeAttachment::setVisualize(visualization, visualize);
	}
}

bool ModelAttachment::getVisualize(const std::string& visualization) const
{
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

void ModelAttachment::setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation, const WFMath::Vector<3>& velocity)
{
	NodeAttachment::setPosition(position, orientation, velocity);

	mModelRepresentation.setLocalVelocity(WFMath::Vector<3>(velocity).rotate(orientation.inverse()));
}

}
}
