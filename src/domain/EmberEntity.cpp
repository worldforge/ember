/*
 Copyright (C) 2004  Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EmberEntity.h"

#include "IGraphicalRepresentation.h"
#include "IEntityAttachment.h"
#include "IEntityVisitor.h"
#include "EntityTalk.h"
#include "IHeightProvider.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/MultiLineListFormatter.h>

namespace Ember
{


/**
 * @brief Handles dispatching of attribute changes to global listeners.
 *
 * This is useful for any listeners that want to observe changes to specific attribute, independent of on which entity it's placed.
 */
class GlobalAttributeDispatcher {
private:
	std::unordered_multimap<std::string, std::function<void(EmberEntity&, const Atlas::Message::Element&)>*> mCallbacks;
public:

	~GlobalAttributeDispatcher() {
		//Make sure that all callbacks are de-registered.
		assert(mCallbacks.empty());
	}

	void registerListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener) {
		mCallbacks.insert(std::make_pair(attributeName, &listener));
	}

	void deregisterListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener) {
		for (auto range = mCallbacks.equal_range(attributeName); range.first != range.second; ++range.first) {
			if (range.first->second == &listener) {
				mCallbacks.erase(range.first);
				break;
			}
		}
	}

	void dispatchAttributeChange(EmberEntity& entity, const std::string& attributeName, const Atlas::Message::Element& value) {
		for (auto range = mCallbacks.equal_range(attributeName); range.first != range.second; ++range.first) {
			(*range.first->second)(entity, value);
		}
	}
};

GlobalAttributeDispatcher sGlobalDispatcher;

EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw) :
		Eris::ViewEntity(id, ty, vw),
		mIsInitialized(false),
		mPositioningMode(PositioningMode::DEFAULT),
		mCompositionMode(CM_DISABLED),
		mGraphicalRepresentation(nullptr),
		mAttachment(nullptr),
		mAttachmentControlDelegate(nullptr),
		mHeightProvider(nullptr)
{
}

EmberEntity::~EmberEntity()
{
	delete mAttachment;
	delete mGraphicalRepresentation;
}

void EmberEntity::registerGlobalAttributeListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener)
{
	sGlobalDispatcher.registerListener(attributeName, listener);
}

void EmberEntity::deregisterGlobalAttributeListener(const std::string& attributeName, std::function<void(EmberEntity&, const Atlas::Message::Element&)>& listener)
{
	sGlobalDispatcher.deregisterListener(attributeName, listener);
}


void EmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	Eris::Entity::init(ge, fromCreateOp);

	// Setup Sounds
	//	setSounds();

	if (getPredictedPos().isValid()) {
		std::stringstream ss;
		ss << "Entity " << getId() << " (" << getName() << ", " << getType()->getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
		S_LOG_VERBOSE(ss.str());
	}

	mIsInitialized = true;

	//If the entity had no bounding box, the onBboxChanged will never have been called, and we want to do that now instead.
	if (!hasBBox()) {
		onBboxChanged();
	}
}

void EmberEntity::adjustPosition()
{
	if (mAttachment) {
		mAttachment->updatePosition();
	}
}

void EmberEntity::setHeightProvider(IHeightProvider* heightProvider)
{
	mHeightProvider = heightProvider;
}

float EmberEntity::getHeight(const WFMath::Point<2>& localPosition) const
{

	if (mHeightProvider) {
		float height = 0;
		if (mHeightProvider->getHeight(WFMath::Point<2>(localPosition.x(), localPosition.y()), height)) {
			return height;
		}
	}

	//A normal EmberEntity shouldn't know anything about the terrain, so we can't handle the area here.
	//Instead we just pass it on to the parent until we get to someone who knows how to handle this (preferably the terrain).
	if (getEmberLocation()) {

		WFMath::Point<2> adjustedLocalPosition(getPredictedPos().x(), getPredictedPos().z());

		WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(getOrientation());
		auto theta = std::atan2(xVec.z(), xVec.x()); // rotation about Y
		WFMath::RotMatrix<2> rm;
		WFMath::Vector<2> adjustment(localPosition.x(), localPosition.y());
		adjustment.rotate(rm.rotation(theta));
		adjustedLocalPosition += adjustment;

		return getEmberLocation()->getHeight(adjustedLocalPosition) - getPredictedPos().y();
	}

	WFMath::Point<3> predictedPos = getPredictedPos();
	if (predictedPos.isValid()) {
		return predictedPos.y();
	} else {
		return 0.0f;
	}
}

void EmberEntity::onTalk(const Atlas::Objects::Operation::RootOperation& talkArgs)
{
	EntityTalk entityTalk(talkArgs);

	//some talk operations come with a predefined set of suitable responses, so we'll store those so that they can later on be queried by the GUI for example
	mSuggestedResponses = entityTalk.getSuggestedResponses();

	S_LOG_VERBOSE("Entity " << getName() << " ("<< getType()->getName() << ") says: \"" << entityTalk.getMessage() << "\"");

	EventTalk.emit(entityTalk);

	// Make a sound in OpenAL -- mafm: call doesn't exist
	//	EmberServices::getSingleton().getSoundService().playTalk(msg,
	//		getPosition(),getOrientation());

	// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}

void EmberEntity::onSoundAction(const Atlas::Objects::Operation::RootOperation & op)
{
	//We'll just catch the call and write something to both the log and the console, and then pass it on.

	std::string message = getName() + " emits a " + op->getParent() + ".";
	ConsoleBackend::getSingletonPtr()->pushMessage(message, "info");
	S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") sound action: " << op->getParent());

	Eris::Entity::onSoundAction(op);
}

void EmberEntity::setAttachmentControlDelegate(IEntityControlDelegate* delegate)
{
	mAttachmentControlDelegate = delegate;
	if (mAttachment) {
		mAttachment->setControlDelegate(delegate);
	}
}

IEntityControlDelegate* EmberEntity::getAttachmentControlDelegate() const
{
	return mAttachmentControlDelegate;
}

void EmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
	updateAttachment();

	Eris::Entity::onLocationChanged(oldLocation);

}

void EmberEntity::updateAttachment()
{
	//Get the new location. We use getEmberLocation() since we always know that all entities are of type EmberEntity.
	EmberEntity* newLocationEntity = getEmberLocation();

	if (newLocationEntity && newLocationEntity->getAttachment()) {
		try {
			delete mAttachment;
			mAttachment = nullptr;
			IEntityAttachment* newAttachment = newLocationEntity->getAttachment()->attachEntity(*this);
			if (newAttachment) {
				setAttachment(newAttachment);
				newAttachment->updateScale();
			}
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Problem when creating new attachment for entity." << ex);
		}
		//If we're the top level entity the attachment has been set from the outside and shouldn't be changed.
		//FIXME This is a little hackish; how can we improve it to not require special cases?
	} else if (m_view->getTopLevel() == this) {
		return;
	} else {
		try {
			setAttachment(nullptr);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Problem when setting attachment for entity." << ex);
		}
	}
}


void EmberEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	std::string message = getName() + " performs a " + act->getParent() + ".";
	ConsoleBackend::getSingletonPtr()->pushMessage(message, "info");
	S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") action: " << act->getParent());

	Entity::onAction(act);
}

const std::vector<std::string>& EmberEntity::getSuggestedResponses() const
{
	return mSuggestedResponses;
}

bool EmberEntity::hasSuggestedResponses() const
{
	return !mSuggestedResponses.empty();
}

void EmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	if (str == "mode") {
		parsePositioningModeChange(v);
	} else if (str == "bbox") {
		Entity::onAttrChanged(str, v);
		onBboxChanged();
		return;
	}

	//Dispatch attribute changes to any global listeners.
	sGlobalDispatcher.dispatchAttributeChange(*this, str, v);

	Entity::onAttrChanged(str, v);

}

void EmberEntity::parsePositioningModeChange(const Atlas::Message::Element& v)
{
	const std::string& mode = v.asString();
	PositioningMode newMode;
	if (mode.empty()) {
		newMode = PositioningMode::DEFAULT;
	} else if (mode == "floating") {
		newMode = PositioningMode::FLOATING;
	} else if (mode == "fixed") {
		newMode = PositioningMode::FIXED;
	} else if (mode == "submerged") {
		newMode = PositioningMode::SUBMERGED;
	} else {
		newMode = PositioningMode::DEFAULT;
	}

	onPositioningModeChanged(newMode);
}

void EmberEntity::onPositioningModeChanged(PositioningMode newMode)
{
	if (newMode != mPositioningMode) {
		adjustPosition();
	}
	EventPositioningModeChanged.emit(newMode);
	mPositioningMode = newMode;
}

void EmberEntity::setVisualize(const std::string& visualization, bool visualize)
{
	if (mAttachment) {
		mAttachment->setVisualize(visualization, visualize);
	}
}

bool EmberEntity::getVisualize(const std::string& visualization) const
{
	if (mAttachment) {
		return mAttachment->getVisualize(visualization);
	}
	return false;
}

void EmberEntity::onBboxChanged()
{
	if (mAttachment) {
		mAttachment->updateScale();
	}
}

EmberEntity* EmberEntity::getEmberLocation() const
{
	return dynamic_cast<EmberEntity*>(getLocation());
}

EmberEntity* EmberEntity::getEmberContained(unsigned int index) const
{
	return dynamic_cast<EmberEntity*>(getContained(index));
}

std::vector<std::string> EmberEntity::getActions()
{
	//get the actions from Eris and return them a simple vector of strings
	std::vector<std::string> actions;

	if (hasAttr("actions")) {
		const Atlas::Message::Element& operations = valueOfAttr("actions");
		if (operations.isList()) {
			const Atlas::Message::ListType& list = operations.asList();
			actions.reserve(list.size());
			auto J = list.begin();
			for (; J != list.end(); ++J) {
				if (J->isString()) {
					actions.push_back(J->asString());
				}
			}
		}
	}

	return actions;
}

std::vector<std::string> EmberEntity::getDefaultUseOperators()
{
	//get the use operations from Eris and return them a simple vector of strings
	std::vector<std::string> operators;

	Eris::TypeInfoArray types = getUseOperations();

	for (auto& type : types) {
		operators.push_back(type->getName());
	}

	return operators;
}

void EmberEntity::dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const
{
	logOutstream << "Dumping attributes for entity " << getId() << "(" << getName() << ")" << std::endl;

	Atlas::Message::QueuedDecoder decoder;

	Atlas::Codecs::XML codec(outstream, outstream, decoder);
	Atlas::MultiLineListFormatter formatter(outstream, codec);
	Atlas::Message::Encoder encoder(formatter);
	formatter.streamBegin();
	encoder.streamMessageElement(getAttributes());

	formatter.streamEnd();
}

IGraphicalRepresentation* EmberEntity::getGraphicalRepresentation() const
{
	return mGraphicalRepresentation;
}

void EmberEntity::setGraphicalRepresentation(IGraphicalRepresentation* graphicalRepresentation)
{
	if (graphicalRepresentation != mGraphicalRepresentation) {
		//If we're the top level entity the attachment has been set from the outside and shouldn't be changed.
		//FIXME This is a little hackish; how can we improve it to not require special cases?
		if (m_view->getTopLevel() != this) {
			//We must delete the attachment before we delete the graphical representation.
			setAttachment(nullptr);
		}
		delete mGraphicalRepresentation;
		mGraphicalRepresentation = graphicalRepresentation;
		updateAttachment();
		EventChangedGraphicalRepresentation();
	}
}

void EmberEntity::setAttachment(IEntityAttachment* attachment)
{
	IEntityAttachment* oldAttachment = mAttachment;
	mAttachment = attachment;
	if (attachment != oldAttachment) {
		reattachChildren();
		delete oldAttachment;
	}

}

void EmberEntity::reattachChildren()
{
	for (unsigned int i = 0; i < numContained(); ++i) {
		EmberEntity* entity = getEmberContained(i);
		if (entity) {
			entity->updateAttachment();
			entity->reattachChildren();
		}
	}
}

IEntityAttachment* EmberEntity::getAttachment() const
{
	return mAttachment;
}

EmberEntity* EmberEntity::getAttachedEntity(const std::string& namedPoint)
{
	//HACK: this is just a temporary solution
	if (hasAttr(namedPoint)) {
		const Atlas::Message::Element& idElement = valueOfAttr(namedPoint);
		std::string id;
		if (Eris::Entity::extractEntityId(idElement, id)) {
			for (unsigned int i = 0; i < numContained(); ++i) {
				EmberEntity* entity = getEmberContained(i);
				if (entity && entity->getId() == id) {
					return entity;
				}
			}
		}
	}

	return nullptr;
}

void EmberEntity::accept(IEntityVisitor& visitor)
{
	visitor.visit(*this);
	for (unsigned int i = 0; i < numContained(); ++i) {
		EmberEntity* entity = getEmberContained(i);
		if (entity) {
			entity->accept(visitor);
		}
	}
}

void EmberEntity::accept(std::function<bool(EmberEntity&)>& visitor)
{
	if (visitor(*this)) {
		for (unsigned int i = 0; i < numContained(); ++i) {
			EmberEntity* entity = getEmberContained(i);
			if (entity) {
				entity->accept(visitor);
			}
		}
	}
}

void EmberEntity::accept(std::function<bool(const EmberEntity&)>& visitor) const
{
	if (visitor(*this)) {
		for (unsigned int i = 0; i < numContained(); ++i) {
			EmberEntity* entity = getEmberContained(i);
			if (entity) {
				entity->accept(visitor);
			}
		}
	}
}

EmberEntity::CompositionMode EmberEntity::getCompositionMode() const
{
	return mCompositionMode;
}

void EmberEntity::setCompositionMode(EmberEntity::CompositionMode mode)
{
	mCompositionMode = mode;
}

}
