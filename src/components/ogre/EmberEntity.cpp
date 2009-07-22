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

#include "framework/Service.h"
#include "framework/ConsoleBackend.h"
#include "services/EmberServices.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "terrain/TerrainArea.h"
#include "terrain/TerrainMod.h"
#include "Convert.h"
#include "EmberEntityActionCreator.h"
#include "IGraphicalRepresentation.h"
#include "mapping/EmberEntityMappingManager.h"
#include "IEntityAttachment.h"

#include "EmberOgre.h"
#include <OgreWireBoundingBox.h>
#include <OgreException.h>
#include <OgreMaterialManager.h>


//#include <Atlas/Objects/ObjectsFwd.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Formatter.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>


using namespace Ogre;

namespace Ogre
{

/**
 This is just like a WireBoundBox but not aligned to the axes, hence it will correctly line up according to it's orientation.
 */
class OOBBWireBoundingBox: public WireBoundingBox
{
public:

	void getWorldTransforms(Matrix4* xform) const
	{
		SimpleRenderable::getWorldTransforms(xform);
	}
};

}
;

namespace EmberOgre
{

const std::string EmberEntity::MODE_FLOATING("floating");
const std::string EmberEntity::MODE_FIXED("fixed");
const std::string EmberEntity::MODE_PROJECTILE("projectile");


const std::string EmberEntity::BboxMaterialName("BaseYellowNoLightning");

EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw, Ogre::SceneManager* sceneManager) :
	Eris::Entity(id, ty, vw), mIsInitialized(false), mIsInMotionManager(false), mErisEntityBoundingBox(0), mTerrainArea(0), mTerrainMod(0), mMovementMode(MM_DEFAULT), mGraphicalRepresentation(0), mEntityMapping(0), mAttachment(0), mAttachmentControlDelegate(0)
{
//	createSceneNode(sceneManager);
}

EmberEntity::~EmberEntity()
{
	delete mAttachment;
	delete mGraphicalRepresentation;

	if (mErisEntityBoundingBox)
	{
		mErisEntityBoundingBox->getParentSceneNode()->getCreator()->destroySceneNode(mErisEntityBoundingBox->getParentSceneNode()->getName());
	}
	OGRE_DELETE mErisEntityBoundingBox;
	delete mEntityMapping;
}

void EmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{

	/// we need a model mapping
	createEntityMapping();

	assert(mEntityMapping);

	///calling this will result in a call to setModel(...)
	mEntityMapping->initialize();

	// Setup Sounds
//	setSounds();

	///start out with the default movement mode
	onMovementModeChanged(EmberEntity::MM_DEFAULT);

	Eris::Entity::init(ge, fromCreateOp);

	// set the Ogre node position and orientation based on Atlas data
	std::stringstream ss;
	if (getPredictedPos().isValid())
	{
		ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
	}
	S_LOG_VERBOSE( ss.str());

	mIsInitialized = true;

	///Delay the checking and creation of area and terrainmod, since if we do it in onAttrChanged before the entity is properly initialized we'll get problems with multiple reparsings, as well as problems with placing the areas or terrainmod before the entity has been moved to it's proper place.
	///Another way of doing this would be by attaching listeners, but that require more memory. It might be in the end that that's a better approach, depending on how much memory is needed, contrasted with the extra cycles spent here for every entity.
	if (hasAttr("area"))
	{
		createDependentObject("area");
	}

	if (hasAttr("terrainmod"))
	{
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
	if (attributeName == "area" && !mTerrainArea.get())
	{
		mTerrainArea = std::auto_ptr<Terrain::TerrainArea>(new Terrain::TerrainArea(*this));
		if (mTerrainArea->init())
		{
			addArea(mTerrainArea.get());
			return true;
		}
		else
		{
			///if we couldn't properly initialize, delete the instance now, and then hopefully the next time the "area" attribute is changed we'll be able to properly create an area
			mTerrainArea.reset();
		}
	}

	///if the area attribute has changed, and we _don't_ have any mTerrainMod instance, try to create one such.
	///if we do have an mTerrainMod instance, all updates will be taken care of by that instead and we can ignore this
	if (attributeName == "terrainmod" && !mTerrainMod.get())
	{
		mTerrainMod = std::auto_ptr<Terrain::TerrainMod>(new Terrain::TerrainMod(this));
		if (mTerrainMod->init())
		{
			addTerrainMod(mTerrainMod.get());
			return true;
		}
		else
		{
			///if we couldn't properly initialize, delete the instance now, and then hopefully the next time the "area" attribute is changed we'll be able to properly create a mod
			mTerrainMod.reset();
		}
	}
	return false;
}

void EmberEntity::createEntityMapping()
{
	delete mEntityMapping;
	///the creator binds the model mapping and this instance together by creating instance of EmberEntityModelAction and EmberEntityPartAction which in turn calls the setModel(..) and show/hideModelPart(...) methods.
	EmberEntityActionCreator creator(*this);
	mEntityMapping = ::EmberOgre::Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(this, &creator);
}

void EmberEntity::onMoved()
{
	parseMovementMode();
	Eris::Entity::onMoved();
}

void EmberEntity::adjustPosition()
{
	if (mAttachment) {
		mAttachment->updatePosition();
	}
}

void EmberEntity::onTalk(const Atlas::Objects::Operation::RootOperation& talkArgs)
{
	const std::vector<Atlas::Objects::Root>& args = talkArgs->getArgs();
	if (args.empty())
	{
		Eris::Entity::onTalk(talkArgs);
		return;
	}

	const Atlas::Objects::Root& talk = args.front();

	if (!talk->hasAttr("say"))
	{
		Eris::Entity::onTalk(talkArgs);
		return;
	}

	///some talk operations come with a predefined set of suitable responses, so we'll store those so that they can later on be queried by the GUI for example
	mSuggestedResponses.clear();
	if (talk->hasAttr("responses"))
	{
		if (talk->getAttr("responses").isList())
		{
			const Atlas::Message::ListType & responseList = talk->getAttr("responses").asList();
			Atlas::Message::ListType::const_iterator I = responseList.begin();
			for (; I != responseList.end(); ++I)
			{
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
	S_LOG_VERBOSE( "Entity says: [" << message << "]\n" );

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

	if (I != p.end())
	{

		const std::string& name = *I;
		std::string message = getName() + " emits a " + name + ".";

		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);

		S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") sound action: " << name);
	}

	Eris::Entity::onSoundAction(op);
}

void EmberEntity::setAttachmentControlDelegate(IAttachmentControlDelegate* delegate)
{
	mAttachmentControlDelegate = delegate;
	if (mAttachment) {
		mAttachment->setControlDelegate(delegate);
	}
}

IAttachmentControlDelegate* EmberEntity::getAttachmentControlDelegate() const
{
	return mAttachmentControlDelegate;
}
void EmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
	if (getLocation() == oldLocation)
	{
		///If it's the same location, don't do anything, but do add a warning to the log since this isn't supposed to happen.
		S_LOG_WARNING( "Same new location as old for entity: " << this->getId() << " (" << this->getName() << ")" );
		return Eris::Entity::onLocationChanged(oldLocation);
	}
	Eris::Entity::onLocationChanged(oldLocation);

	///Get the new location. We use getEmberLocation() since we always know that all entities are of type EmberEntity.
	EmberEntity* newLocationEntity = getEmberLocation();

	if (newLocationEntity) {
		IEntityAttachment* newAttachment = newLocationEntity->getAttachment()->attachEntity(*this);
		setAttachment(newAttachment);
	} else {
		setAttachment(0);
	}
}


void EmberEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();

	if (I != p.end())
	{

		const std::string& name = *I;
		std::string message = getName() + " performs a " + name + ".";

		Ember::ConsoleBackend::getSingletonPtr()->pushMessage(message);

		S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") action: " << name);
	}
	Entity::onAction(act);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	Atlas::Message::Element attr;
	if (act->copyAttr("description", attr) && attr.isString())
	{
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

void EmberEntity::addArea(Terrain::TerrainArea* area)
{
	///A normal EmberEntity shouldn't know anything about the terrain, so we can't handle the area here.
	///Intead we just pass it on to the parent until we get to someone who knows how to handle this (preferrably the terrain).
	if (getEmberLocation())
	{
		getEmberLocation()->addArea(area);
	}
}

void EmberEntity::addTerrainMod(Terrain::TerrainMod* mod)
{
	///Same as addArea: pass it on to the parent until it gets to someone who knows how to handle this
	if (getEmberLocation())
	{
		getEmberLocation()->addTerrainMod(mod);
	}
}

void EmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
	if (str == "mode")
	{
		parsePositioningModeChange(v);
	}
	else if (str == "bbox")
	{
		Entity::onAttrChanged(str, v);
		onBboxChanged();
		return;
	}

	///call this before checking for areas and terrainmods, since those instances created to handle that (TerrainMod and TerrainArea) will listen to the AttrChanged event, which would then be emitted after those have been created, causing duplicate regeneration from the same data
	Entity::onAttrChanged(str, v);

	///Only to this if the entity has been propely initialized, to avoid using incomplete entity data (for example an entity which haven't yet been moved to it's proper place, as well as avoiding duplicate parsing of the same data.
	if (mIsInitialized)
	{
		createDependentObject(str);
	}

}

void EmberEntity::parseMovementMode()
{
	MovementMode newMode = MM_DEFAULT;
	if (isMoving()) {
		const WFMath::Vector<3> velocity = getPredictedVelocity();
		if (velocity.isValid()) {
			if (velocity.mag() > 2.6) {
				newMode = MM_RUNNING;
			} else {
				newMode = MM_WALKING;
			}
		}
	}
	if (newMode != mMovementMode) {
		onMovementModeChanged(newMode);
	}
}


void EmberEntity::parsePositioningModeChange(const Atlas::Message::Element& v)
{
	const std::string& mode = v.asString();
	PositioningMode newMode;
	if (mode.empty())
	{
		newMode = PM_DEFAULT;
	}
	else if (mode == MODE_FLOATING)
	{
		newMode = PM_FLOATING;
	}
	else if (mode == MODE_FIXED)
	{
		newMode = PM_FIXED;
	}
	else if (mode == MODE_PROJECTILE)
	{
		newMode = PM_PROJECTILE;
	}
	else
	{
		newMode = PM_DEFAULT;
	}

	onPositioningModeChanged(newMode);
}

void EmberEntity::onPositioningModeChanged(PositioningMode newMode)
{
	if (newMode != mPositioningMode)
	{
		adjustPosition();
	}
	EventPositioningModeChanged.emit(newMode);
	mPositioningMode = newMode;
}

void EmberEntity::onMovementModeChanged(MovementMode newMode)
{
	EventMovementModeChanged.emit(newMode);
	mMovementMode = newMode;
}

void EmberEntity::setVisualize(const std::string& visualization, bool visualize)
{
	if (mGraphicalRepresentation) {
		mGraphicalRepresentation->setVisualize(visualization, visualize);
	}
//	if (visualization == "OgreBBox")
//	{
//		showOgreBoundingBox(visualize);
//	}
//	else if (visualization == "ErisBBox")
//	{
//		showErisBoundingBox(visualize);
//	}
}

bool EmberEntity::getVisualize(const std::string& visualization) const
{
	if (mGraphicalRepresentation) {
		return mGraphicalRepresentation->getVisualize(visualization);
	}
	return false;
//	if (visualization == "OgreBBox")
//	{
//		return getShowOgreBoundingBox();
//	}
//	else if (visualization == "ErisBBox")
//	{
//		return getShowErisBoundingBox();
//	}
//	return false;
}

void EmberEntity::showErisBoundingBox(bool show)
{

	createErisBboxMaterial();

	///if there's no bounding box, create one now
	///allowing for some lazy loading
	if (!mErisEntityBoundingBox)
	{
		mErisEntityBoundingBox = OGRE_NEW Ogre::OOBBWireBoundingBox();
		mErisEntityBoundingBox->setMaterial(BboxMaterialName);
		Ogre::SceneNode* boundingBoxNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
		boundingBoxNode->attachObject(mErisEntityBoundingBox);

		///if there's no bounding box defined for this entity, show one that is 0.2 meters across in all direction
		if (hasBBox())
		{
			mErisEntityBoundingBox->setupBoundingBox(Convert::toOgre(getBBox()));
		}
		else
		{
			mErisEntityBoundingBox->setupBoundingBox(Ogre::AxisAlignedBox(-0.1, -0.1, -0.1, 0.1, 0.1, 0.1));
		}

		boundingBoxNode->setPosition(Convert::toOgre(getPredictedPos()));
		boundingBoxNode->setOrientation(Convert::toOgre(getOrientation()));
	}
	mErisEntityBoundingBox->setVisible(show);

}

void EmberEntity::createErisBboxMaterial()
{
	if (!Ogre::MaterialManager::getSingleton().resourceExists(BboxMaterialName))
	{
		Ogre::MaterialPtr baseYellowNoLighting = Ogre::MaterialManager::getSingleton().create(BboxMaterialName, Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
		baseYellowNoLighting->setLightingEnabled(false);
		baseYellowNoLighting->setAmbient(Ogre::ColourValue(1, 1, 0.7));
		baseYellowNoLighting->setDiffuse(Ogre::ColourValue(1, 1, 0.7));
	}
}

void EmberEntity::onBboxChanged()
{
	if (mAttachment) {
		mAttachment->updateScale();
	}
	if (mErisEntityBoundingBox)
	{
		mErisEntityBoundingBox->setupBoundingBox(Convert::toOgre(getBBox()));
	}
}

bool EmberEntity::getShowErisBoundingBox() const
{
	return (mErisEntityBoundingBox && mErisEntityBoundingBox->isVisible());
}

EmberEntity* EmberEntity::getEmberLocation() const
{
	return static_cast<EmberEntity*> (getLocation());
}

std::vector<std::string> EmberEntity::getActions()
{
	///get the actions from Eris and return them a simple vector of strings
	std::vector<std::string> actions;

	if (hasAttr("actions"))
	{
		const Atlas::Message::Element& operations = valueOfAttr("actions");
		if (operations.isList())
		{
			const Atlas::Message::ListType& list = operations.asList();
			actions.reserve(list.size());
			Atlas::Message::ListType::const_iterator J = list.begin();
			for (; J != list.end(); ++J)
			{
				if (J->isString())
				{
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

	for (Eris::TypeInfoArray::iterator I = types.begin(); I != types.end(); ++I)
	{
		operators.push_back((*I)->getName());
	}

	return operators;
}

void EmberEntity::dumpAttributes(std::iostream& outstream, std::ostream& logOutstream) const
{
	logOutstream << "Dumping attributes for entity " << getId() << "(" << getName() << ")" << std::endl;

	Atlas::Message::QueuedDecoder decoder;
	//std::fstream file;

	Atlas::Codecs::XML codec(outstream, decoder);
	Atlas::Formatter formatter(outstream, codec);
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
	if (graphicalRepresentation != mGraphicalRepresentation)
	{
		delete mGraphicalRepresentation;
	}
	mGraphicalRepresentation = graphicalRepresentation;
}

void EmberEntity::setAttachment(IEntityAttachment* attachment)
{
	if (attachment != mAttachment)
	{
		delete mAttachment;
	}
	mAttachment = attachment;

}
IEntityAttachment* EmberEntity::getAttachment() const
{
	return mAttachment;
}

EmberEntity* EmberEntity::getAttachedEntity(const std::string& namedPoint)
{
	//HACK: this is just a temporary solution
	std::string id;
	if (hasAttr("right_hand_wield"))
	{
		const Atlas::Message::Element& idElement = valueOfAttr("right_hand_wield");
		id = idElement.asString();

	}
	else if (hasAttr("left_hand_wield"))
	{
		const Atlas::Message::Element& idElement = valueOfAttr("left_hand_wield");
		id = idElement.asString();
	}
	if (id != "") {
		for (int i = 0; i < numContained(); ++i) {
			Eris::Entity* entity = getContained(i);
			if (entity && entity->getId() == id) {
				return static_cast<EmberEntity*>(entity);
			}
		}
	}
	return 0;
}


}
