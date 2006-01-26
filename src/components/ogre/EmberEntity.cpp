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

#include "EmberOgrePrerequisites.h"
#include "framework/Service.h"
#include "framework/ConsoleBackend.h"
#include "services/EmberServices.h"
#include "services/sound/SoundService.h"
#include "EmberEntityFactory.h"
#include "MotionManager.h"
#include "GUIManager.h"
#include "EmberEntity.h"
#include "TerrainArea.h"

#include "EmberOgre.h"
#include <OgreWireBoundingBox.h>
#include <Mercator/Area.h>
//#include <Atlas/Objects/ObjectsFwd.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>



#include "model/Model.h"

using namespace Ogre;


namespace Ogre {

	/**
	This is just like a WireBoundBox but not aligned to the axes
	*/
	class OOBBWireBoundingBox  : public WireBoundingBox
	{
		public:
	
		void OOBBWireBoundingBox::getWorldTransforms( Matrix4* xform ) const
		{
			SimpleRenderable::getWorldTransforms(xform);
		}
		//-----------------------------------------------------------------------
		const Quaternion& OOBBWireBoundingBox::getWorldOrientation(void) const
		{
			return SimpleRenderable::getWorldOrientation();
		}
		//-----------------------------------------------------------------------
		const Vector3& OOBBWireBoundingBox::getWorldPosition(void) const
		{
			return SimpleRenderable::getWorldPosition();
		}				

	};

};














namespace EmberOgre {


const char * const EmberEntity::MODE_STANDING = "standing";
const char * const EmberEntity::MODE_RUNNING = "running";
const char * const EmberEntity::MODE_WALKING = "walking";
const char * const EmberEntity::MODE_SWIMMING = "swimming";
const char * const EmberEntity::MODE_FLOATING = "floating";
const char * const EmberEntity::MODE_FIXED = "fixed";


EmberEntity::EmberEntity(const std::string& id, Eris::TypeInfo* ty, Eris::View* vw,Ogre::SceneManager* sceneManager)
:
mIsInitialized(false)
, Eris::Entity(id, ty, vw) 
, mTerrainArea(this)
, mIsInMotionManager(false)
, mErisEntityBoundingBox(0)
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
	//if (mErisEntityBoundingBox)
	//{
	//	EmberOgre::getSingleton().getWorldSceneNode()->removeAndDestroyChild(mErisEntityBoundingBox->getParentSceneNode()->getName());
	//}
	
	//mSceneManager->destroySceneNode(getSceneNode()->getName());
}

void EmberEntity::init(const Atlas::Objects::Entity::RootEntity &ge, bool fromCreateOp)
{
	Eris::Entity::init(ge, fromCreateOp);
	
	
	// set the Ogre node position and orientation based on Atlas data
	if (getPosition().isValid()) {
		getSceneNode()->setPosition(Atlas2Ogre(getPredictedPos()));
		adjustPosition();
	}
	if (getOrientation().isValid()) {
		getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	}
	std::stringstream ss;
	ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPredictedPos().x() << "," << getPredictedPos().y() << "," << getPredictedPos().x() << ")";
	S_LOG_VERBOSE( ss.str());
	
	if (hasAttr("area")) {
		mTerrainArea.init();
		addArea(&mTerrainArea);
	}
		
	mIsInitialized = true;
	
}


void EmberEntity::createSceneNode(Ogre::SceneManager* sceneManager)
{
	EmberEntity* container = static_cast<EmberEntity*>(getLocation());
	if (container == 0) {
		S_LOG_VERBOSE( "Entity created in limbo: "<< this->getId() << " (" << this->getName() << ") \n" )

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
	WFMath::Quaternion orient = getOrientation();
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
		
	Atlas::Objects::Root talk = args.front();
	
	
    if (!talk->hasAttr("say")) {
        Eris::Entity::onTalk(talkArgs);
		return;
    }
	
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
	
	std::string msg = talk->getAttr("say").asString();
    


	
	std::string message = "<";
	message.append(getName());
	message.append(",");
	const std::string type = getType()->getName(); // Eris type as a string
	message.append(type);
	message.append("> ");
	message.append(msg);
	S_LOG_VERBOSE( "Entity says: [" << message << "]\n" )

	// Make the message appear in the chat box
	GUIManager::getSingleton().AppendIGChatLine.emit(msg, this);

	// Make a sound in OpenAL
	Ember::EmberServices::getSingleton().getSoundService()->playTalk(msg,
		getPosition(),getOrientation());

	// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}


void EmberEntity::onSoundAction( const Atlas::Objects::Operation::RootOperation & op )
{
}


void EmberEntity::onVisibilityChanged(bool vis)
{
	checkVisibility(vis);
	Eris::Entity::onVisibilityChanged(vis);
//	mOgreEntity->setVisible(vis);	
}

void EmberEntity::checkVisibility(bool vis)
{
	EmberEntity* container = static_cast<EmberEntity*>(getLocation());
	if (container) {
		//check with the parent first if we should show ourselves
		if (vis && container->allowVisibilityOfMember(this)) {
			//don't cascade, only change the top node
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
	getSceneNode()->setVisible(visible && getLocation(), false);	
}


void EmberEntity::adjustPosition()
{
	if (mMovementMode == MM_FIXED) {

	} else {
		EmberEntity* container = getEmberLocation();
		if (container) {
			container->adjustPositionForContainedNode(this);
		}
	}	
}

Ogre::Vector3 EmberEntity::getOffsetForContainedNode(const Ogre::Vector3& localPosition, EmberEntity* const entity)
{
	Ogre::Vector3 offset = Ogre::Vector3::ZERO;
	///send it upwards until we get a an entity which knows how to set the position (we'll in most cases end up in the world which will adjust the height a bit
	EmberEntity* container = getEmberLocation();
	if (container) {
		//TerrainPosition derivedPosition(getPredictedPos().x() + position.x(), getPredictedPos().y() + position.y());
		offset = container->getOffsetForContainedNode(localPosition + getSceneNode()->getPosition(), entity);
	}
	
	///adjust the height after our own height
	//newPosition.y -= getSceneNode()->getPosition().y;
	return offset;
	
}



void EmberEntity::adjustPositionForContainedNode(EmberEntity* const entity) 
{

	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	//Ogre::Vector3 position = sceneNode->getPosition();
	Ogre::Vector3 offset = getOffsetForContainedNode(sceneNode->getPosition(), entity);
	sceneNode->translate(offset);
	
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
		
		if (getSceneNode()->getParentSceneNode()) {
			//detach from our current object
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
			const Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
			getSceneNode()->translate(oldWorldPosition - newWorldPosition);
		}
	}
	
}

void EmberEntity::onAction(const Atlas::Objects::Operation::RootOperation& act)
{
	
/*	std::string allattribs;
	
	//Atlas::Objects::BaseObjectData::const_iterator I = act->begin();
	std::list< std::string >::const_iterator I = act->getParents().begin();

	for (; I != act->getParents().end(); ++I) 
	{
		//const Atlas::Message::Element e = (const Atlas::Message::Element)(*I).second;
		allattribs.append((*I) + " : ");
	
	}*/
	
	std::string name = act->getName();
	
	//GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") action: " + name);
	S_LOG_VERBOSE( "Entity: " << this->getId() << " (" << this->getName() << ") action: " << name);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	S_LOG_VERBOSE("Entity: " << this->getId() << " (" << this->getName() << ") imaginary: " << act->getName());
	//GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") imaginary: "+act->getName());
}


bool EmberEntity::allowVisibilityOfMember(EmberEntity* entity) {
	return true;
}

std::vector< std::string > EmberEntity::getSuggestedResponses( ) const
{
	return mSuggestedResponses;
}

bool EmberEntity::hasSuggestedResponses( ) const
{
	return mSuggestedResponses.size() > 0;
}


void EmberEntity::addArea(TerrainArea* area)
{
//just pass it on
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

void EmberEntity::showOgreBoundingBox(bool show)
{
	getSceneNode()->showBoundingBox(show);
}

void EmberEntity::showErisBoundingBox(bool show)
{
	if (!mErisEntityBoundingBox) {
		//mErisEntityBoundingBox = new Ogre::WireBoundingBox();
		mErisEntityBoundingBox = new Ogre::OOBBWireBoundingBox();
		Ogre::SceneNode* boundingBoxNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
		boundingBoxNode->attachObject(mErisEntityBoundingBox);
		Ogre::AxisAlignedBox aabb(Atlas2Ogre(getBBox().highCorner()), Atlas2Ogre(getBBox().lowCorner()));
		mErisEntityBoundingBox->setupBoundingBox(aabb);
/*		Model* model = new Model(getName() + "fdsfs");
		model->create("placeholder");
		boundingBoxNode->attachObject(model);*/
		
		boundingBoxNode->setPosition(Atlas2Ogre(getPredictedPos()));
		boundingBoxNode->setOrientation(Atlas2Ogre(getOrientation()));
	}
	mErisEntityBoundingBox->setVisible(show);
	
//	
}

void EmberEntity::onBboxChanged()
{
	if (mErisEntityBoundingBox) {
		Ogre::AxisAlignedBox aabb(Atlas2Ogre(getBBox().highCorner()), Atlas2Ogre(getBBox().lowCorner()));
		mErisEntityBoundingBox->setupBoundingBox(aabb);
	}
}


bool EmberEntity::getShowOgreBoundingBox()
{
	return getSceneNode()->getShowBoundingBox();
}
bool EmberEntity::getShowErisBoundingBox()
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

std::vector<std::string> EmberEntity::getDefaultUseOperators()
{
	std::vector<std::string> operators;
	
	const Eris::Entity::AttrMap::const_iterator I_attribute = this->getAttributes().find("operations");
	if (I_attribute != this->getAttributes().end()) {
		Atlas::Message::Element operations = I_attribute->second;
		if (operations.isList()) {
			const Atlas::Message::ListType & plist = operations.asList();
			Atlas::Message::ListType::const_iterator J = plist.begin();
			for (; J != plist.end(); ++J) {
				if (J->isString()) {
					operators.push_back(J->asString());
				}
			}				
		}
	} 
	return operators;
}

Ogre::SceneManager* EmberEntity::getSceneManager()
{
	assert(mOgreNode);
	return mOgreNode->getCreator();
}


}





