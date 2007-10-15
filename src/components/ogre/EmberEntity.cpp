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

#include "EmberEntity.h"

#include "framework/Service.h"
#include "framework/ConsoleBackend.h"
#include "services/EmberServices.h"
#include "services/sound/SoundService.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "terrain/TerrainArea.h"
#include "MathConverter.h"

#include "EmberOgre.h"
#include <OgreWireBoundingBox.h>
#include <OgreException.h>

#include <Mercator/Area.h>
//#include <Atlas/Objects/ObjectsFwd.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>
#include <Atlas/Formatter.h>
#include <Atlas/Objects/Decoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Message/MEncoder.h>
#include <Atlas/Message/QueuedDecoder.h>


#include "model/Model.h"

using namespace Ogre;


namespace Ogre {

	/**
	This is just like a WireBoundBox but not aligned to the axes, hence it will correctly line up according to it's orientation.
	*/
	class OOBBWireBoundingBox  : public WireBoundingBox
	{
		public:
	
		void getWorldTransforms( Matrix4* xform ) const
		{
			SimpleRenderable::getWorldTransforms(xform);
		}
		//-----------------------------------------------------------------------
		const Quaternion& getWorldOrientation(void) const
		{
			return SimpleRenderable::getWorldOrientation();
		}
		//-----------------------------------------------------------------------
		const Vector3& getWorldPosition(void) const
		{
			return SimpleRenderable::getWorldPosition();
		}				

	};

};














namespace EmberOgre {


const std::string EmberEntity::MODE_STANDING("standing");
const std::string EmberEntity::MODE_RUNNING("running");
const std::string EmberEntity::MODE_WALKING("walking");
const std::string EmberEntity::MODE_SWIMMING("swimming");
const std::string EmberEntity::MODE_FLOATING("floating");
const std::string EmberEntity::MODE_FIXED("fixed");

const std::string EmberEntity::BboxMaterialName("BaseYellowNoLightning");


EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager)
:
Eris::Entity(id, ty, vw) 
, mIsInitialized(false)
, mIsInMotionManager(false)
, mErisEntityBoundingBox(0)
, mOgreNode(0)
, mTerrainArea(0)
{
	createSceneNode(sceneManager);
}

EmberEntity::~EmberEntity()
{
	//detach all children so we don't destroy them
	while (getSceneNode()->numChildren()) {
		getSceneNode()->removeChild((short unsigned int)0);
	}
	Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*>(getSceneNode()->getParent());
	if (parent) {
		parent->removeAndDestroyChild(getSceneNode()->getName());
	} else {
		getSceneNode()->getCreator()->destroySceneNode(getSceneNode()->getName());
	}
	///make sure it's not in the MotionManager
	///TODO: keep a marker in the entity so we don't need to call this for all entities
	MotionManager::getSingleton().removeEntity(this);
	
	delete mErisEntityBoundingBox;
	
	//mSceneManager->destroySceneNode(getSceneNode()->getName());
}

void EmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	Eris::Entity::init(ge, fromCreateOp);
	
	synchronizeWithServer();
	
	// set the Ogre node position and orientation based on Atlas data
	std::stringstream ss;
	ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
	S_LOG_VERBOSE( ss.str());
	
	if (hasAttr("area")) {
		mTerrainArea = std::auto_ptr<TerrainArea>(new TerrainArea(this));
		if (mTerrainArea->init()) {
			addArea(mTerrainArea.get());
		}
	}
		
	mIsInitialized = true;
	
}

void EmberEntity::synchronizeWithServer()
{
	if (getPosition().isValid()) {
		getSceneNode()->setPosition(Atlas2Ogre(getPredictedPos()));
		adjustPosition();
	}
	if (getOrientation().isValid()) {
		getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	}

}


void EmberEntity::createSceneNode(Ogre::SceneManager* sceneManager)
{
	EmberEntity* container = getEmberLocation();
	if (container == 0) {
		//S_LOG_VERBOSE( "Entity created in limbo: "<< this->getId() << " (" << this->getName() << ")" )

		mOgreNode = sceneManager->createSceneNode(getId());
		
	} else {
		Ogre::SceneNode * node = container->getSceneNode();
		mOgreNode = node->createChildSceneNode(getId());
	}		
}

void EmberEntity::updateMotion(Ogre::Real timeSlice)
{
	getSceneNode()->setPosition(Atlas2Ogre(getPredictedPos()));
	adjustPosition();
	
	//if there's a debug bounding box for the eris entity, update it's position
	if (mErisEntityBoundingBox) {
		mErisEntityBoundingBox->getParentSceneNode()->setPosition(getSceneNode()->getPosition());
		mErisEntityBoundingBox->getParentSceneNode()->setOrientation(getSceneNode()->getOrientation());
	}

}




void EmberEntity::onMoved()
{
	Eris::Entity::onMoved();
	const WFMath::Quaternion& orient = getOrientation();
	getSceneNode()->setOrientation(Atlas2Ogre(orient));
	updateMotion(0);
}

void EmberEntity::setMoving(bool moving)
{
	// Call the overridden method 
	Eris::Entity::setMoving(moving);
	
	MotionManager* motionManager = &MotionManager::getSingleton();
	if (moving) {
		//the entity is moving
		if (!mIsInMotionManager) {
			motionManager->addEntity(this);
			mIsInMotionManager = true;
		}
	} else {
		//the entity has stopped moving
		if (mIsInMotionManager) {
			motionManager->removeEntity(this);
			mIsInMotionManager = false;
		}
	}
	


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
			for(; I != responseList.end(); ++I) {
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
	S_LOG_VERBOSE( "Entity says: [" << message << "]\n" )

	/// Make the message appear in the chat box
	GUIManager::getSingleton().AppendIGChatLine.emit(msg, this);

	/// Make a sound in OpenAL
//	Ember::EmberServices::getSingleton().getSoundService()->playTalk(msg,
//		getPosition(),getOrientation());

	/// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}


void EmberEntity::onSoundAction( const Atlas::Objects::Operation::RootOperation & op )
{
}


void EmberEntity::onVisibilityChanged(bool vis)
{
	checkVisibility(vis);
	Eris::Entity::onVisibilityChanged(vis);
}

void EmberEntity::checkVisibility(bool vis)
{
	///since we don't want to show all entities solely by their server flags (for example, an inventory item belonging to a character might not be shown even though the server thinks it's visible) we have to some more checks before we decide whether to show this or not
	EmberEntity* container = static_cast<EmberEntity*>(getLocation());
	if (container) {
		///check with the parent first if we should show ourselves
		if (vis && container->allowVisibilityOfMember(this)) {
			///don't cascade, only change the top node
			setVisible(true);	
		} else {
			setVisible(false);	
		}
		
	} else {
		setVisible(vis);
	}
}


void EmberEntity::setVisible(bool visible)
{
	///when entities are hidden, we detach them from the rendering scene graph altogether. this speeds up Ogre since it doesn't have to calculate visibility for nodes that are hidden anyway
	if (!visible) {
		if (getSceneNode()->getParent()) {
			getSceneNode()->getParent()->removeChild(getSceneNode());
		}
	} else {
		if (getLocation()) {
			if (!getSceneNode()->getParent()) {
				getEmberLocation()->getSceneNode()->addChild(getSceneNode());
			}
		}
	}
	
	getSceneNode()->setVisible(visible && getLocation(), false);	
}


void EmberEntity::adjustPosition()
{
	if (getPredictedPos().isValid()) {
		adjustPosition(Atlas2Ogre( getPredictedPos() ));
	}
}

void EmberEntity::adjustPosition(const Ogre::Vector3& position)
{
	if (mMovementMode == MM_FIXED) {

	} else {
		EmberEntity* container = getEmberLocation();
		if (container) {
			container->adjustPositionForContainedNode(this, position);
		}
	}	
}

const Ogre::Vector3& EmberEntity::getOffsetForContainedNode(const Ogre::Vector3& localPosition, EmberEntity* const entity)
{
	///send it upwards until we get a an entity which knows how to set the position (we'll in most cases end up in the world which will adjust the height a bit)
	EmberEntity* container = getEmberLocation();
	if (container) {
		//TerrainPosition derivedPosition(getPredictedPos().x() + position.x(), getPredictedPos().y() + position.y());
		return container->getOffsetForContainedNode(localPosition + getSceneNode()->getPosition(), entity);
	} else {
		return Ogre::Vector3::ZERO;
	}
	
	
}



void EmberEntity::adjustPositionForContainedNode(EmberEntity* const entity, const Ogre::Vector3& position) 
{

	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	//Ogre::Vector3 position = sceneNode->getPosition();
	const Ogre::Vector3& offset = getOffsetForContainedNode(position, entity);
	if (offset != Ogre::Vector3::ZERO) {
		sceneNode->setPosition(position + offset);
	}
	
}

void EmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
	
	if (getLocation() == oldLocation)
	{
		S_LOG_WARNING( "Same new location as old for entity: " << this->getId() << " (" << this->getName() << ")" );
		return Eris::Entity::onLocationChanged(oldLocation);
	
	}
	Eris::Entity::onLocationChanged(oldLocation);
	
	///if we're attached to something, detach from it
	detachFromModel();

	if (!getLocation()) {
		return;	
	} else {
		EmberEntity* newLocationEntity = getEmberLocation();
		
		const Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
// 		const Ogre::Quaternion oldWorldOrientation = getSceneNode()->getWorldOrientation();
		
		if (getSceneNode()->getParentSceneNode()) {
			///detach from our current object
			getSceneNode()->getParentSceneNode()->removeChild(getSceneNode()->getName());
		}
		if (newLocationEntity) { 
			// add to the new entity
			newLocationEntity->getSceneNode()->addChild(getSceneNode());
			S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") relocated to: "<< newLocationEntity->getId() << " (" << newLocationEntity->getName() << ")" );
			if (getPosition().isValid()) {
				///note that in some instances, for instance when the avatar enters the sty, the position isn't updated yet, which will make the avatar "snap" to an incorrect position (since the parent node has changed) until next frame, when the position should have been updated
				getSceneNode()->setPosition(Atlas2Ogre(getPredictedPos()));
				adjustPosition();
				std::stringstream ss;
				ss << getPredictedPos();
				S_LOG_VERBOSE("New position for entity: "  << this->getId() << " (" << this->getName() << " ) :" << ss.str());
			}
			if (getOrientation().isValid()) {
				getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
				std::stringstream ss;
				ss << getOrientation();
				S_LOG_VERBOSE("New orientation for entity: "  << this->getId() << " (" << this->getName() << " ) :" << ss.str());
			}
//				getSceneNode()->rotate(newLocationEntity->getSceneNode()->getWorldOrientation() - oldWorldOrientation);
	
		} else {
			///the entity has no current parent, and should be placed in limbo (hopefully a more correct parent will be submitted in a future LocationChanged event
			S_LOG_VERBOSE( "Entity relocated to limbo: "<< this->getId() << " (" << this->getName() << ")" );
	//		mSceneManager->getRootSceneNode()->addChild(getSceneNode());
		}		
		
		checkVisibility(isVisible());
	
		///we'll adjust the entity so it retains it's former position in the world, but only for moving entities
		///since else we'll get a "gap" when we're waiting on updated positions from the server
		///this isn't optimal
		if (isMoving()) {
			const Ogre::Vector3& newWorldPosition = getSceneNode()->getWorldPosition();
			getSceneNode()->translate(oldWorldPosition - newWorldPosition);
		}
	}
	
}

void EmberEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	const std::list<std::string> &p = act->getParents();
	std::list<std::string>::const_iterator I = p.begin();
	
	if (I == p.end()) return;
	
	const std::string& name = *I;
	std::string message = getName() + " performs a " + name;
	
	Ember::ConsoleBackend::getMainConsole()->pushMessage(message);
	
	S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") action: " << name);
	Entity::onAction(act);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
    Atlas::Message::Element attr;
    if (act->copyAttr("description", attr) != 0 || !attr.isString()) {
        return;
    }
		
	S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") imaginary: " << attr.String());
}


bool EmberEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return true;
}

const std::vector< std::string >& EmberEntity::getSuggestedResponses( ) const
{
	return mSuggestedResponses;
}

bool EmberEntity::hasSuggestedResponses( ) const
{
	return mSuggestedResponses.size() > 0;
}


void EmberEntity::addArea(TerrainArea* area)
{
///just pass it on to the parent until we get to someone who knows how to handle this (preferrably the terrain)
	if (getEmberLocation()) {
		getEmberLocation()->addArea(area);
	}
}

void EmberEntity::onAttrChanged(const std::string& str, const Atlas::Message::Element& v)
{
    if (str == "mode") {
    	parseModeChange(v);
	} else if (str == "bbox") {
		Entity::onAttrChanged(str, v);
		onBboxChanged();
		return;
	}
	Entity::onAttrChanged(str, v);
}

void EmberEntity::parseModeChange(const Atlas::Message::Element& v)
{
	const std::string& mode = v.asString();
	MovementMode newMode;
	if (mode.empty()) {
		newMode = MM_DEFAULT;
	} else if (mode == MODE_STANDING) {
		newMode = MM_STANDING;
	} else if (mode == MODE_RUNNING) {
		newMode = MM_RUNNING;
	} else if (mode == MODE_WALKING) {
		newMode = MM_WALKING;
	} else if (mode == MODE_SWIMMING) {
		newMode = MM_SWIMMING;
	} else if (mode == MODE_FLOATING) {
		newMode = MM_FLOATING;
	} else if (mode == MODE_FIXED) {
		newMode = MM_FIXED;
	} else {
		newMode = MM_DEFAULT;
	}
	
	onModeChanged(newMode);
}

void EmberEntity::onModeChanged(MovementMode newMode)
{
	if (newMode == MM_FIXED) {
		adjustPosition();
	}
	mMovementMode = newMode;
}

void EmberEntity::setVisualize(const std::string& visualization, bool visualize)
{
	if (visualization == "OgreBBox") {
		showOgreBoundingBox(visualize);
	} else if (visualization == "ErisBBox") {
		showErisBoundingBox(visualize);
	}
}

bool EmberEntity::getVisualize(const std::string& visualization) const
{
	if (visualization == "OgreBBox") {
		return getShowOgreBoundingBox();
	} else if (visualization == "ErisBBox") {
		return getShowErisBoundingBox();
	}
	return false;
}


void EmberEntity::showOgreBoundingBox(bool show)
{
	getSceneNode()->showBoundingBox(show);
}

void EmberEntity::showErisBoundingBox(bool show)
{

	createErisBboxMaterial();
	
	///if there's no bounding box, create one now
	///allowing for some lazy loading
	if (!mErisEntityBoundingBox) {
		mErisEntityBoundingBox = new Ogre::OOBBWireBoundingBox();
		mErisEntityBoundingBox->setMaterial(BboxMaterialName);
		Ogre::SceneNode* boundingBoxNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
		boundingBoxNode->attachObject(mErisEntityBoundingBox);
		Ogre::AxisAlignedBox aabb = Atlas2Ogre(getBBox());
		mErisEntityBoundingBox->setupBoundingBox(aabb);
		
		boundingBoxNode->setPosition(Atlas2Ogre(getPredictedPos()));
		boundingBoxNode->setOrientation(Atlas2Ogre(getOrientation()));
	}
	mErisEntityBoundingBox->setVisible(show);
	
}

void EmberEntity::createErisBboxMaterial()
{
	if (!Ogre::MaterialManager::getSingleton().resourceExists(BboxMaterialName)) {
		Ogre::MaterialPtr baseYellowNoLighting = Ogre::MaterialManager::getSingleton().create(BboxMaterialName, 
			Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
 		baseYellowNoLighting->setLightingEnabled(false);
		baseYellowNoLighting->setAmbient(Ogre::ColourValue(1, 1, 0.7));
		baseYellowNoLighting->setDiffuse(Ogre::ColourValue(1, 1, 0.7));
	}
}

void EmberEntity::onBboxChanged()
{
	if (mErisEntityBoundingBox) {
		mErisEntityBoundingBox->setupBoundingBox(Atlas2Ogre(getBBox()));
	}
}


bool EmberEntity::getShowOgreBoundingBox() const
{
	return getSceneNode()->getShowBoundingBox();
}
bool EmberEntity::getShowErisBoundingBox() const
{
	return (mErisEntityBoundingBox && mErisEntityBoundingBox->isVisible());
	
}

//inline 
Ogre::SceneNode* EmberEntity::getSceneNode() const 
{
	return mOgreNode;	
}

EmberEntity* EmberEntity::getEmberLocation() const
{ 
	return static_cast<EmberEntity*>(getLocation());
}


const Ogre::AxisAlignedBox& EmberEntity::getWorldBoundingBox(bool derive) const
{
	static Ogre::AxisAlignedBox boundingBox(0,0,0,0,0,0);
	return boundingBox;
}
	
const Ogre::Sphere & EmberEntity::getWorldBoundingSphere (bool derive) const
{
	static Ogre::Sphere sphere;
	return sphere;
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

Ogre::SceneManager* EmberEntity::getSceneManager()
{
	assert(mOgreNode);
	return mOgreNode->getCreator();
}

static void dumpElement(const std::string &prefix, const std::string &name, const Atlas::Message::Element &e, std::ostream& outstream, std::ostream& logOutstream) 
{

  if (e.isMap()) {
    logOutstream << prefix << name << ": Dumping Map" << std::endl;
    Atlas::Message::MapType::const_iterator itr = e.asMap().begin();
    Atlas::Message::MapType::const_iterator end = e.asMap().end();
    outstream << prefix << name << ":" << std::endl;
    for (; itr != end; ++itr) {
      dumpElement(prefix + "  ", itr->first, itr->second, outstream, logOutstream);
    }
    logOutstream << prefix << "Finished Dumping Map" << std::endl;
  } else if (e.isList()) {
    logOutstream << prefix << name << ": Dumping List" << std::endl;
    Atlas::Message::ListType::const_iterator itr = e.asList().begin();
    Atlas::Message::ListType::const_iterator end = e.asList().end();
    outstream << prefix << name << ":" << std::endl;
    for (; itr != end; ++itr) {
      dumpElement(prefix + "  ", "", *itr, outstream, logOutstream);
    }
    logOutstream << prefix << "Finished Dumping List" << std::endl;
  } else {
    if (e.isString()) outstream << prefix << name << ": " << e.asString() << std::endl;
    if (e.isNum()) outstream << prefix << name << ": " << e.asNum() << std::endl;
  }
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
  
  
//   const Eris::Entity::AttrMap &attribs = getAttributes();

//   Eris::Entity::AttrMap::const_iterator itr = attribs.begin();
//   Eris::Entity::AttrMap::const_iterator end = attribs.end();
//   for (; itr != end; ++itr) {
//     dumpElement("",itr->first, itr->second, outstream, logOutstream);
//   }
}

}





