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
#include "framework/ConsoleBackend.h"
#include "MotionManager.h"
#include "DimeEntity.h"
using namespace Ogre;

namespace DimeOgre {

/*eris 1.3
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, , Ogre::Entity* ogreEntity) : Eris::Entity(ge, ty, vw) 
*/
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager)
//, Ogre::Entity* ogreEntity) 
: mSceneManager(sceneManager),
mAnimationState_Walk(NULL),
mOgreEntity(NULL),
mOgreNode(NULL),
//mOgreNode(dynamic_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild(ge.getId()))),
Eris::Entity(ge, vw) 
{
	//mOgreEntity = ogreEntity;
	this->createOgreEntity(sceneManager);
	
	// set the Ogre node position and orientation based on Atlas data
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	std::cout << "Entity " << getID() << " placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")" << std::endl;
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	
	
}
DimeEntity::~DimeEntity()
{
	if (mAnimationState_Walk) {
		MotionManager::getSingleton().removeAnimation(mAnimationState_Walk);
	}
/*	mSceneManager->removeEntity(mOgreEntity);
	mSceneManager->removeEntity(mOgreEntity);
	
	delete mOgreEntity;
	delete mSceneNode;
	*/
}

void DimeEntity::scaleNode(Ogre::Real scalingFactor) {

	if (hasBBox()) {
		const WFMath::AxisBox<3> boundingBox = getBBox();	
		WFMath::CoordType WFHeight = boundingBox.upperBound(2);
		Ogre::Real ogreNodeHeight  = getSceneNode()->_getWorldAABB().getMaximum().y;
		Ogre::Real scaler = ((ogreNodeHeight  / scalingFactor) / WFHeight) ;
		getSceneNode()->setScale(scaler, scaler, scaler);
	}
	
	
}

void DimeEntity::createOgreEntity(Ogre::SceneManager* sceneManager) {
	// create the ogre node and the
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	
//	typeService->getTypeByName("world");
	
	//Eris::TypeInfo* type = getContainer()->getType();
//	Eris::StringSet parents = getContainer()->getInherits();
	DimeEntity* container = (DimeEntity*)getContainer();
	if (container == NULL) {
		mOgreNode = static_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild(getID()));
	} else {
		Ogre::SceneNode * node = container->getSceneNode();
		if (node) {
			mOgreNode = static_cast<Ogre::SceneNode*>(node->createChild(getID()));
		} else {
			mOgreNode = static_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild(getID()));
		}
		
	}		
	Ogre::String id = getID();
	id += "_scaleNode";
	mScaleNode = static_cast<Ogre::SceneNode*>(mOgreNode->createChild(id));
	//mScaleNode->setInheritScale(false);
	mScaleNode->setScale(Ogre::Vector3(0.01,0.01,0.01));
	mOgreNode->showBoundingBox(true);

/*
	// create the ogre entity
	if(!strcmp(ge.getID().c_str(),avatarID.c_str())) { // if it's the player
		dime::ConsoleBackend::getMainConsole()->pushMessage("Creating player entity");
		fprintf(stderr, "TRACE - CREATING PLAYER ENTITY\n");
		ogreEntity = mSceneManager->createEntity(ge.getID(), "dragon.mesh");
		//ogreNode->setScale(OGRESCALER);
	}
	else 
*/	
	if(!strcmp(getType()->getName().c_str(),"settler"))	// 0 if strings are equal
	{
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
		mScaleNode->setScale(Ogre::Vector3(0.02,0.02,0.02));
	}
	else if(!strcmp(getType()->getName().c_str(),"merchant"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
		mScaleNode->setScale(Ogre::Vector3(0.02,0.02,0.02));
	}
	else if(!strcmp(getType()->getName().c_str(),"pig"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "pig.mesh");
		mScaleNode->setScale(0.4,0.4,0.4);
		mAnimationState_Walk = mOgreEntity->getAnimationState("Walk");	
	}
	else if(!strcmp(getType()->getName().c_str(),"sty"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Sty.mesh");
		mScaleNode->setScale(1,1,1);
	}
	else if(!strcmp(getType()->getName().c_str(),"squirrel"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "squirrel.mesh");
	}
	else if(!strcmp(getType()->getName().c_str(),"fir"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Fir.mesh");
		mScaleNode->setScale(0.02,0.02,0.02);
	}
	else if(!strcmp(getType()->getName().c_str(),"oak"))
	{
		mOgreEntity = sceneManager->createEntity(getID(), "Oak.mesh");
		mScaleNode->setScale(0.04,0.04,0.04);
	}
	else
	{
		// TODO: razor should be a coin
		//fprintf(stderr, "TRACE - FOUND ANYTHING ELSE - RAZOR MESH: ");
		fprintf(stderr, getType()->getName().c_str());
		fprintf(stderr, "\n");

		mOgreEntity = sceneManager->createEntity(getID(), "razor.mesh");
		//ogreNode->setScale(1,1,1);
		//mOgreNode->setScale(0.1,0.1,0.1);
	}

	
	if (mAnimationState_Walk) {
		MotionManager::getSingleton().addAnimation(mAnimationState_Walk);
	}
	
	mOgreEntity->setVisible(false);


	// attach the node to the entity
	mScaleNode->attachObject(mOgreEntity);
	mOgreEntity->setUserObject(this);
	//scaleNode(0.01);

}

/*
 * return the scenenode to which this entity belongs
 */
SceneNode* DimeEntity::getSceneNode() {

	return mOgreNode;	
}

Ogre::Entity* DimeEntity::getOgreEntity() {
	return mOgreEntity;	
}

/*
 * TODO: extend this method to allow for smooth movement
 */
void DimeEntity::handleMove()
{
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	MotionManager* motionManager = &MotionManager::getSingleton();
	if (getVelocity() != WFMath::Vector<3>().zero()) {
		motionManager->addEntity(this);
		if (mAnimationState_Walk) {
			mAnimationState_Walk->setEnabled(true);
//			motionManager->addAnimation(mAnimationState_Walk);
		}
	} else {
		motionManager->removeEntity(this);
		if (mAnimationState_Walk) {
			mAnimationState_Walk->setEnabled(false);
//			motionManager->removeAnimation(mAnimationState_Walk);
		}
	}
	//Root::getSingleton().getAutoCreatedWindow()->setDebugText(std::string("Moved: " + _id) );
}

void DimeEntity::handleTalk(const std::string &msg)
{
	
    std::string message = "<";
    message.append(getName());
    message.append("> ");
    message.append(msg);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
}

void DimeEntity::setVisible(bool vis)
{
	mOgreEntity->setVisible(vis);	
}

/*
void DimeEntity::setContainer(Entity *pr)
{
		
	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(pr);
	if (dimeEntity) {
		//detach from our current object and add to the new entity
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		dimeEntity->getSceneNode()->addChild(getSceneNode());
				
	} else {
		//detach from our current object and add to the world
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
		getSceneNode()->getCreator()->getRootSceneNode()->addChild(getSceneNode());
	}		
	
}
*/

void DimeEntity::setContainer(Entity *pr)
{
		
	DimeEntity* dimeEntity = dynamic_cast<DimeEntity*>(pr);
	if (getSceneNode()) {
		//detach from our current object
		getSceneNode()->getParent()->removeChild(getSceneNode()->getName());
	}
	if (dimeEntity) {
		// add to the new entity
		dimeEntity->getSceneNode()->addChild(getSceneNode());
				
	} else {
		//add to the world
		getSceneNode()->getCreator()->getRootSceneNode()->addChild(getSceneNode());
	}		
	Entity::setContainer(pr);
}

void DimeEntity::adjustHeightPositionForContainedNode(DimeEntity* const entity) 
{
	//for generic entities we set the height to 0
	Ogre::SceneNode* sceneNode = entity->getSceneNode();
	Ogre::Vector3 position = sceneNode->getPosition();
	sceneNode->setPosition(position.x, 0,position.z);
}

void DimeEntity::adjustHeightPosition()
{
	DimeEntity* container = (DimeEntity*)getContainer();
	if (container) {
		container->adjustHeightPositionForContainedNode(this);
	}
	
}


/*
void DimeEntity::addMember(Entity *e) 
{
	try{
		SceneNode* sceneNode = mSceneManager->getSceneNode(e->getID());

	
		//sceneNode->getParent()->removeChild(sceneNode->getName());	
		getSceneNode()->addChild(sceneNode);
	} catch(Ogre::Exception ex) {
		int i = 1;
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}

	Entity::addMember(e);
	
}

void DimeEntity::rmvMember(Entity *e)
{
	try{
		getSceneNode()->removeChild(e->getID());
	} catch(Ogre::Exception ex) {
		int i = 1;
		//the contained item haven't been initialised yet
		//this is no problem though because when it do get initialized
		//it's container will be set
	}
	Entity::rmvMember(e);	
}
*/
/*
void DimeEntity::markAsMainAvatar(Ogre::SceneManager* sceneManager)
{
	mIsMainAvatar = true;
	if (mOgreEntity != NULL) {
		delete mOgreEntity;
	}
	mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
	mOgreNode->attachObject(mOgreEntity);
	mOgreEntity->setUserObject(this);
}

*/
}

