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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "EmberEntity.h"

#include "EmberEntityFactory.h"
#include "GUIManager.h"
#include "World.h"
#include "terrain/TerrainArea.h"
#include "terrain/TerrainMod.h"
#include "terrain/TerrainParser.h"
#include "terrain/TerrainDefPoint.h"
#include "Convert.h"
#include "EmberEntityActionCreator.h"
#include "IGraphicalRepresentation.h"
#include "IEntityAttachment.h"
#include "IEntityVisitor.h"
#include "EmberOgre.h"
#include "mapping/EmberEntityMappingManager.h"
#include "framework/ConsoleBackend.h"
#include "framework/MultiLineListFormatter.h"
#include "services/EmberServices.h"
#include "authoring/AuthoringManager.h"


#include <OgreWireBoundingBox.h>
#include <OgreMaterialManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>

using namespace Ogre;


namespace EmberOgre
{

const std::string EmberEntity::MODE_FLOATING("floating");
const std::string EmberEntity::MODE_FIXED("fixed");
const std::string EmberEntity::MODE_PROJECTILE("projectile");

EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Scene& scene) :
	Eris::ViewEntity(id, ty, vw), mIsInitialized(false), mTerrainArea(0), mTerrainMod(0), mPositioningMode(PM_DEFAULT), mGraphicalRepresentation(0), mEntityMapping(0), mAttachment(0), mAttachmentControlDelegate(0)
{
	/// we need a model mapping
	createEntityMapping(scene);

	assert( mEntityMapping);
}

EmberEntity::~EmberEntity()
{
	delete mAttachment;
	delete mGraphicalRepresentation;
	delete mEntityMapping;
}

void EmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	Eris::Entity::init(ge, fromCreateOp);

	///calling this will result in a call to setModel(...)
	mEntityMapping->initialize();

	// Setup Sounds
	//	setSounds();



	// set the Ogre node position and orientation based on Atlas data
	std::stringstream ss;
	if (getPredictedPos().isValid()) {
		ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
	}
	S_LOG_VERBOSE(ss.str());

	mIsInitialized = true;

	///Delay the checking and creation of area and terrainmod, since if we do it in onAttrChanged before the entity is properly initialized we'll get problems with multiple reparsings, as well as problems with placing the areas or terrainmod before the entity has been moved to it's proper place.
	///Another way of doing this would be by attaching listeners, but that require more memory. It might be in the end that that's a better approach, depending on how much memory is needed, contrasted with the extra cycles spent here for every entity.
	if (hasAttr("area")) {
		createDependentObject("area");
	}

	if (hasAttr("terrainmod")) {
		createDependentObject("terrainmod");
	}

	//If the entity had no bounding box, the onBboxChanged will never have been called, and we want to do that now instead.
	if (!hasBBox()) {
		onBboxChanged();
	}
	EventInitComplete.emit();
}

bool EmberEntity::createDependentObject(const std::string& attributeName)
{
	///if the area attribute has changed, and we _don't_ have any mTerrainArea instance, try to create one such.
	///if we do have an mTerrainArea instance, all updates will be taken care of by that instead and we can ignore this
	if (attributeName == "area" && !mTerrainArea.get()) {
		mTerrainArea = std::auto_ptr<Terrain::TerrainArea>(new Terrain::TerrainArea(*this));
		if (mTerrainArea->init()) {
			addArea(*mTerrainArea.get());
			return true;
		} else {
			///if we couldn't properly initialize, delete the instance now, and then hopefully the next time the "area" attribute is changed we'll be able to properly create an area
			mTerrainArea.reset();
		}
	}

	///if the area attribute has changed, and we _don't_ have any mTerrainMod instance, try to create one such.
	///if we do have an mTerrainMod instance, all updates will be taken care of by that instead and we can ignore this
	if (attributeName == "terrainmod" && !mTerrainMod.get()) {
		mTerrainMod = std::auto_ptr<Terrain::TerrainMod>(new Terrain::TerrainMod(this));
		if (mTerrainMod->init()) {
			addTerrainMod(mTerrainMod.get());
			return true;
		} else {
			///if we couldn't properly initialize, delete the instance now, and then hopefully the next time the "area" attribute is changed we'll be able to properly create a mod
			mTerrainMod.reset();
		}
	}
	return false;
}

void EmberEntity::createEntityMapping(Scene& scene)
{
	delete mEntityMapping;
	///the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
	EmberEntityActionCreator creator(*this, scene);
	mEntityMapping = ::EmberOgre::Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*this, &creator, getView());
}

Ember::EntityMapping::EntityMapping* EmberEntity::getMapping() const
{
	return mEntityMapping;
}

void EmberEntity::adjustPosition()
{
	if (mAttachment) {
		mAttachment->updatePosition();
	}
}

float EmberEntity::getHeight(const WFMath::Point<2>& localPosition) const
{
	///A normal EmberEntity shouldn't know anything about the terrain, so we can't handle the area here.
	///Instead we just pass it on to the parent until we get to someone who knows how to handle this (preferably the terrain).
	if (getEmberLocation()) {

		WFMath::Point<2> adjustedLocalPosition(getPredictedPos().x(), getPredictedPos().y());

		WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(getOrientation());
		double theta = atan2(xVec.y(), xVec.x()); // rotation about Z
		WFMath::RotMatrix<2> rm;
		WFMath::Vector<2> adjustment(localPosition.x(), localPosition.y());
		adjustment.rotate(rm.rotation(theta));
		adjustedLocalPosition += adjustment;

		return getEmberLocation()->getHeight(adjustedLocalPosition) - getPredictedPos().z();
	}

	return getPredictedPos().z();
}

void EmberEntity::onTalk(const Atlas::Objects::Operation::RootOperation& talkArgs)
{
	const std::vector<Atlas::Objects::Root>& args = talkArgs->getArgs();
	if (args.empty()) {
		Eris::Entity::onTalk(talkArgs);
		return;
	}

	const Atlas::Objects::Root& talk = args.front();

	if (!talk->hasAttr("say")) {
		Eris::Entity::onTalk(talkArgs);
		return;
	}

	///some talk operations come with a predefined set of suitable responses, so we'll store those so that they can later on be queried by the GUI for example
	mSuggestedResponses.clear();
	if (talk->hasAttr("responses")) {
		if (talk->getAttr("responses").isList()) {
			const Atlas::Message::ListType & responseList = talk->getAttr("responses").asList();
			Atlas::Message::ListType::const_iterator I = responseList.begin();
			for (; I != responseList.end(); ++I) {
				mSuggestedResponses.push_back(I->asString());
			}

		}
	}

	const std::string& msg = talk->getAttr("say").asString();

	std::string message = "<";
	message.append(getName());
	message.append(",");
	const std::string& type = getType()->getName(); // Eris type as a string
	message.append(type);
	message.append("> ");
	message.append(msg);
	S_LOG_VERBOSE("Entity says: [" << message << "]\n");

	/// Make the message appear in the chat box
	GUIManager::getSingleton().AppendIGChatLine.emit(msg, this);

	/// Make a sound in OpenAL -- mafm: call doesn't exist
	//	Ember::EmberServices::getSingleton().getSoundService()->playTalk(msg,
	//		getPosition(),getOrientation());

	/// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}

void EmberEntity::onSoundAction(const Atlas::Objects::Operation::RootOperation & op)
{
	///We'll just catch the call and write something to both the log and the console, and then pass it on.
	const std::list<std::string> &p = op->getParents();
	std::list<std::string>::const_iterator I = p.begin();

	if (I != p.end()) {

		const std::string& name = *I;
		std::string message = getName() + " emits a " + name + ".";

		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);

		S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") sound action: " << name);
	}

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
	Eris::Entity::onLocationChanged(oldLocation);

	///Get the new location. We use getEmberLocation() since we always know that all entities are of type EmberEntity.
	EmberEntity* newLocationEntity = getEmberLocation();

	if (newLocationEntity && newLocationEntity->getAttachment()) {
		try {
			IEntityAttachment* newAttachment = newLocationEntity->getAttachment()->attachEntity(*this);
			setAttachment(newAttachment);
			if (newAttachment) {
				newAttachment->updateScale();
			}
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Problem when creating new attachment for entity." << ex);
		}
	} else {
		try {
			setAttachment(0);
		} catch (const std::exception& ex) {
			S_LOG_WARNING("Problem when setting attachment for entity." << ex);
		}
	}
}

void EmberEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();

	if (I != p.end()) {

		const std::string& name = *I;
		std::string message = getName() + " performs a " + name + ".";

		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);

		S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") action: " << name);
	}
	Entity::onAction(act);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	Atlas::Message::Element attr;
	if (act->copyAttr("description", attr) && attr.isString()) {
		std::string message = getName() + " " + attr.asString() + ".";

		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);

		S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") imaginary: " << attr.String());
	}

	Entity::onImaginary(act);

}

const std::vector<std::string>& EmberEntity::getSuggestedResponses() const
{
	return mSuggestedResponses;
}

bool EmberEntity::hasSuggestedResponses() const
{
	return mSuggestedResponses.size() > 0;
}

void EmberEntity::addArea(Terrain::TerrainArea& area)
{
	///A normal EmberEntity shouldn't know anything about the terrain, so we can't handle the area here.
	///Instead we just pass it on to the parent until we get to someone who knows how to handle this (preferrably the terrain).
	if (getEmberLocation()) {
		getEmberLocation()->addArea(area);
	}
}

void EmberEntity::addTerrainMod(Terrain::TerrainMod* mod)
{
	///Same as addArea: pass it on to the parent until it gets to someone who knows how to handle this
	if (getEmberLocation()) {
		getEmberLocation()->addTerrainMod(mod);
	}
}

void EmberEntity::updateTerrain(const std::vector<Terrain::TerrainDefPoint>& terrainDefinitionPoints)
{
	if (getEmberLocation()) {
		getEmberLocation()->updateTerrain(terrainDefinitionPoints);
	}
}

void EmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	if (str == "mode") {
		parsePositioningModeChange(v);
	} else if (str == "bbox") {
		Entity::onAttrChanged(str, v);
		onBboxChanged();
		return;
	} else ///check for terrain updates
	if (str == "terrain") {
		Terrain::TerrainParser terrainParser;
		updateTerrain(terrainParser.parseTerrain(v, getPredictedPos()));
	}

	///call this before checking for areas and terrainmods, since those instances created to handle that (TerrainMod and TerrainArea) will listen to the AttrChanged event, which would then be emitted after those have been created, causing duplicate regeneration from the same data
	Entity::onAttrChanged(str, v);

	///Only to this if the entity has been propely initialized, to avoid using incomplete entity data (for example an entity which haven't yet been moved to it's proper place, as well as avoiding duplicate parsing of the same data.
	if (mIsInitialized) {
		createDependentObject(str);
	}

}

void EmberEntity::parsePositioningModeChange(const Atlas::Message::Element& v)
{
	const std::string& mode = v.asString();
	PositioningMode newMode;
	if (mode.empty()) {
		newMode = PM_DEFAULT;
	} else if (mode == MODE_FLOATING) {
		newMode = PM_FLOATING;
	} else if (mode == MODE_FIXED) {
		newMode = PM_FIXED;
	} else if (mode == MODE_PROJECTILE) {
		newMode = PM_PROJECTILE;
	} else {
		newMode = PM_DEFAULT;
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
	if (visualization == "ErisBBox") {
		showErisBoundingBox(visualize);
	}
}

bool EmberEntity::getVisualize(const std::string& visualization) const
{
	if (visualization == "ErisBBox") {
		return getShowErisBoundingBox();
	} else if (mAttachment) {
		return mAttachment->getVisualize(visualization);
	}
	return false;
}

void EmberEntity::showErisBoundingBox(bool show)
{
	if (show) {
		EmberOgre::getSingleton().getWorld()->getAuthoringManager().displaySimpleEntityVisualization(*this);
	} else {
		EmberOgre::getSingleton().getWorld()->getAuthoringManager().hideSimpleEntityVisualization(*this);
	}
}

void EmberEntity::onBboxChanged()
{
	if (mAttachment) {
		mAttachment->updateScale();
	}
}

bool EmberEntity::getShowErisBoundingBox() const
{
	return EmberOgre::getSingleton().getWorld()->getAuthoringManager().hasSimpleEntityVisualization(*this);
}

EmberEntity* EmberEntity::getEmberLocation() const
{
	return static_cast<EmberEntity*> (getLocation());
}

EmberEntity* EmberEntity::getEmberContained(unsigned int index) const
{
	return static_cast<EmberEntity*> (getContained(index));
}

std::vector<std::string> EmberEntity::getActions()
{
	///get the actions from Eris and return them a simple vector of strings
	std::vector<std::string> actions;

	if (hasAttr("actions")) {
		const Atlas::Message::Element& operations = valueOfAttr("actions");
		if (operations.isList()) {
			const Atlas::Message::ListType& list = operations.asList();
			actions.reserve(list.size());
			Atlas::Message::ListType::const_iterator J = list.begin();
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
	///get the use operations from Eris and return them a simple vector of strings
	std::vector<std::string> operators;

	Eris::TypeInfoArray types = getUseOperations();

	for (Eris::TypeInfoArray::iterator I = types.begin(); I != types.end(); ++I) {
		operators.push_back((*I)->getName());
	}

	return operators;
}

void EmberEntity::dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const
{
	logOutstream << "Dumping attributes for entity " << getId() << "(" << getName() << ")" << std::endl;

	Atlas::Message::QueuedDecoder decoder;

	Atlas::Codecs::XML codec(outstream, decoder);
	Ember::MultiLineListFormatter formatter(outstream, codec);
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
		//We must delete the attachment before we delete the graphical representation.
		setAttachment(0);
		delete mGraphicalRepresentation;
		mGraphicalRepresentation = graphicalRepresentation;
		onLocationChanged(getLocation()); //This is needed to generate a new attachment.
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
			entity->onLocationChanged(entity->getEmberLocation());
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
		std::string id = idElement.asString();
		if (id != "") {
			for (unsigned int i = 0; i < numContained(); ++i) {
				EmberEntity* entity = getEmberContained(i);
				if (entity && entity->getId() == id) {
					return entity;
				}
			}
		}
	}

	return 0;
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

}
