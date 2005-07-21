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



#include "model/Model.h"

using namespace Ogre;


namespace Ogre {
    #define POSITION_BINDING 0

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
mSceneManager(sceneManager)
, mIsInitialized(false)
, mView(vw)
, Eris::Entity(id, ty, vw) 
, mTerrainArea(this)
, mIsInMotionManager(false)
, mErisEntityBoundingBox(0)
{
	createSceneNode();
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
	if (mErisEntityBoundingBox)
	{
		EmberOgre::getSingleton().getWorldSceneNode()->removeAndDestroyChild(mErisEntityBoundingBox->getParentSceneNode()->getName());
	}
	
	//mSceneManager->destroySceneNode(getSceneNode()->getName());
}

void EmberEntity::init(const Atlas::Objects::Entity::GameEntity &ge)
{
	Eris::Entity::init(ge);
	
	
	// set the Ogre node position and orientation based on Atlas data
	if (getPosition().isValid()) {
		getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
		adjustHeightPosition();
	}
	if (getOrientation().isValid()) {
		getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	}
	std::stringstream ss;
	ss << "Entity " << getId() << "(" << getName() << ") placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")";
	S_LOG_INFO( ss.str());
	
	if (hasAttr("area")) {
		mTerrainArea.init();
		addArea(&mTerrainArea);
	}
	
	mIsInitialized = true;
	
}


void EmberEntity::createSceneNode()
{
	EmberEntity* container = dynamic_cast<EmberEntity*>(getLocation());
	if (container == 0) {
		S_LOG_INFO( "ENTITY CREATED IN LIMBO: "<< this->getId() << " (" << this->getName() << ") \n" )

		//mSceneManager->createSceneNode(getId());
		mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->createSceneNode(getId()));
//		mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getId()));
		
	} else {
		Ogre::SceneNode * node = container->getSceneNode();
		//node->createChild(getId());
		mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getId()));
/*		if (node) {
			mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getId()));
		} else {
			mOgreNode = static_cast<Ogre::SceneNode*>(mSceneManager->getRootSceneNode()->createChild(getId()));
		}*/
	}		
}

void EmberEntity::updateMotion(Ogre::Real timeSlice)
{
	getSceneNode()->setPosition(Atlas2Ogre(getPredictedPos()));
	adjustHeightPosition();


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

void EmberEntity::onTalk(const Atlas::Objects::Root& talkArgs)
{
	
    if (!talkArgs->hasAttr("say")) {
        Eris::Entity::onTalk(talkArgs);
		return;
    }
	
	mSuggestedResponses.clear();
	if (talkArgs->hasAttr("responses")) {
		if (talkArgs->getAttr("responses").isList()) {
			const Atlas::Message::ListType & responseList = talkArgs->getAttr("responses").asList();
			Atlas::Message::ListType::const_iterator I = responseList.begin();
			for(; I != responseList.end(); ++I) {
				mSuggestedResponses.push_back(I->asString());
			}
		
		}
	}
	
	std::string msg = talkArgs->getAttr("say").asString();
    


	
	std::string message = "<";
	message.append(getName());
	message.append(",");
	const std::string type = getType()->getName(); // Eris type as a string
	message.append(type);
	message.append("> ");
	message.append(msg);
	S_LOG_INFO( "TRACE - ENTITY SAYS: [" << message << "]\n" )
	Ember::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
	// Make the message appear in the chat box
	GUIManager::getSingleton().AppendIGChatLine.emit(msg, this);
	// Make a sound if it's the merchant speaking
	if(type.compare("merchant")==0) {
		S_LOG_INFO( "THE MERCHANT IS SPEAKING" )
		Ember::EmberServices::getInstance()->getSoundService()->playTestGYPH();
	}
	// Call the method of the base class (since we've overloaded it)
	Eris::Entity::onTalk(talkArgs);
}


void EmberEntity::onSoundAction( const Atlas::Objects::Operation::RootOperation & op )
{
	int i = 0;
}


void EmberEntity::onVisibilityChanged(bool vis)
{
	checkVisibility(vis);
	Eris::Entity::onVisibilityChanged(vis);
//	mOgreEntity->setVisible(vis);	
}

void EmberEntity::checkVisibility(bool vis)
{
	EmberEntity* container = dynamic_cast<EmberEntity*>(getLocation());
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


void EmberEntity::adjustHeightPosition()
{
	if (mMovementMode == MM_FIXED) {

	} else {
		EmberEntity* container = static_cast<EmberEntity*>(getLocation());
		if (container) {
			container->adjustHeightPositionForContainedNode(this);
		}
	}	
}

float EmberEntity::getHeightPositionForContainedNode(const TerrainPosition& position, EmberEntity* const entity)
{
	float height = 0;
	//send it upwards until we get a an entity which knows how to set the height
	EmberEntity* container = static_cast<EmberEntity*>(getLocation());
	if (container) {
		TerrainPosition derivedPosition(getPosition().x() + position.x(), getPosition().y() + position.y());
		height = container->getHeightPositionForContainedNode(derivedPosition, entity);
	}
	
	//adjust the height after our own height
	height -= getPosition().z();
	return height;
	
}



void EmberEntity::adjustHeightPositionForContainedNode(EmberEntity* const entity) 
{

	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	sceneNode->setPosition(position.x, getHeightPositionForContainedNode(TerrainPosition(entity->getPosition().x(), entity->getPosition().y()), entity), position.z);
	
}

void EmberEntity::onLocationChanged(Eris::Entity *oldLocation)
{
//	return Eris::Entity::onLocationChanged(oldLocation, newLocation);
	
	if (getLocation() == oldLocation)
	{
		S_LOG_INFO( "SAME NEW LOCATION AS OLD FOR ENTITY: " << this->getId() << " (" << this->getName() << ")" );
		return Eris::Entity::onLocationChanged(oldLocation);
	
	}
	Eris::Entity::onLocationChanged(oldLocation);
	
	//if we're attached to something, detach from it
	detachFromModel();

	if (!getLocation()) {
		return;	
	} else {
		EmberEntity* newLocationEntity = static_cast<EmberEntity*>(getLocation());
		
		Ogre::Vector3 oldWorldPosition = getSceneNode()->getWorldPosition();
		
		if (getSceneNode()->getParentSceneNode()) {
			//detach from our current object
			getSceneNode()->getParentSceneNode()->removeChild(getSceneNode()->getName());
		}
		if (newLocationEntity) { 
	
			
				// add to the new entity
				newLocationEntity->getSceneNode()->addChild(getSceneNode());
				S_LOG_INFO( "ENTITY: " << this->getId() << " (" << this->getName() << ") RELOCATED TO: "<< newLocationEntity->getId() << " (" << newLocationEntity->getName() << ")" )
				if (getPosition().isValid()) {
					getSceneNode()->setPosition(Atlas2Ogre(getPosition()));
					adjustHeightPosition();
				}
				if (getOrientation().isValid()) {
					getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
				}
	
		} else {
			//add to the world
			S_LOG_INFO( "ENTITY RELOCATED TO LIMBO: "<< this->getId() << " (" << this->getName() << ")" );
	//		mSceneManager->getRootSceneNode()->addChild(getSceneNode());
		}		
		
		checkVisibility(isVisible());
		std::stringstream ss;
		ss << "ENTITY HAS POSITION: " << getPosition() << " AND ORIENTATION: " <<  getOrientation();                                                      
		S_LOG_INFO( ss.str() )
	
		//we adjust the entity so it retains it's former position in the world
		Ogre::Vector3 newWorldPosition = getSceneNode()->getWorldPosition();
		getSceneNode()->translate(oldWorldPosition - newWorldPosition);
	}
	
}

void EmberEntity::onAction(const Atlas::Objects::Operation::Action& act)
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
	S_LOG_INFO( std::string("Entity (") + getName() + ":" + getId() + ") action: " + name);
}

void EmberEntity::onImaginary(const Atlas::Objects::Root& act)
{
	GUIManager::getSingleton().setDebugText(std::string("Entity (") + getName() + ":" + getId() + ") imaginary: "+act->getName());
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
        std::string mode = v.asString();
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
	Entity::onAttrChanged(str, v);
}

void EmberEntity::onModeChanged(MovementMode newMode)
{
	if (newMode == MM_FIXED) {
		adjustHeightPosition();
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
		
		boundingBoxNode->setPosition(Atlas2Ogre(getPosition()));
		boundingBoxNode->setOrientation(Atlas2Ogre(getOrientation()));
	}
	mErisEntityBoundingBox->setVisible(show);
	
//	
}

bool EmberEntity::getShowOgreBoundingBox()
{
	return getSceneNode()->getShowBoundingBox();
}
bool EmberEntity::getShowErisBoundingBox()
{
	return (mErisEntityBoundingBox && mErisEntityBoundingBox->isVisible());
	
}

inline Ogre::SceneNode* EmberEntity::getSceneNode() const 
{
	return mOgreNode;	
}

inline EmberEntity* EmberEntity::getEmberLocation() const
{ 
	return static_cast<EmberEntity*>(getLocation());
}

}





