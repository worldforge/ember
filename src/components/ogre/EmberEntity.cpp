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
#include "DimeEntity.h"
using namespace Ogre;

/*eris 1.3
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::TypeInfo* ty, Eris::View* vw, , Ogre::Entity* ogreEntity) : Eris::Entity(ge, ty, vw) 
*/
DimeEntity::DimeEntity(const Atlas::Objects::Entity::GameEntity &ge, Eris::World* vw, Ogre::SceneManager* sceneManager)
//, Ogre::Entity* ogreEntity) 
: Eris::Entity(ge, vw) 
{
	//mOgreEntity = ogreEntity;
	createOgreEntity(sceneManager);
	
	// set the Ogre node position and orientation based on Atlas data
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(getPosition()));
	std::cout << "Entity placed at (" << getPosition().x() << "," << getPosition().y() << "," << getPosition().x() << ")" << std::endl;
	//WFMath::Quaternion wq = e->getOrientation();
	//WFMath::Vector<3> wv = wq.vector();
	//Ogre::Quaternion oq = Ogre::Quaternion(Atlas2Ogre(e-getOrientation()));
	getSceneNode()->setOrientation(Atlas2Ogre(getOrientation()));
	
	mOgreEntity->setUserObject(this);
	
}
DimeEntity::~DimeEntity()
{
	mOgreEntity = NULL;
}

void DimeEntity::createOgreEntity(Ogre::SceneManager* sceneManager) {
	// create the ogre node and the
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());


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
		//fprintf(stderr, "TRACE - FOUND A SETTLER - MALEBUILDER MESH\n");
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
	}
	else if(!strcmp(getType()->getName().c_str(),"merchant"))
	{
		//fprintf(stderr, "TRACE - FOUND A MERCHANT - ROBOT MESH\n");
		mOgreEntity = sceneManager->createEntity(getID(), "robot.mesh");
	}
	else if(!strcmp(getType()->getName().c_str(),"pig"))
	{
		//fprintf(stderr, "TRACE - FOUND A PIG - OGREHEAD MESH\n");
		mOgreEntity = sceneManager->createEntity(getID(), "ogrehead.mesh");
	}
	else
	{
		// TODO: razor should be a coin
		fprintf(stderr, "TRACE - FOUND ANYTHING ELSE - RAZOR MESH: ");
		fprintf(stderr, getType()->getName().c_str());
		fprintf(stderr, "\n");

		mOgreEntity = sceneManager->createEntity(getID(), "razor.mesh");
		//ogreNode->setScale(1,1,1);
		ogreNode->setScale(0.1,0.1,0.1);
	}
	//mOgreEntity->setVisible(false);


	// attach the node to the entity
	ogreNode->attachObject(mOgreEntity);
}


/*
 * this is temporary until we can better subclass Eris::Entity
 *
void DimeEntity::connectSignals()
{
    this->AddedMember.connect( SigC::slot( *this, &DimeEntity::addedMember ) );

    this->RemovedMember.connect( SigC::slot( *this, &DimeEntity::removedMember ) );

    this->Recontainered.connect( SigC::slot( *this, &DimeEntity::recontainered ) );

    this->Changed.connect( SigC::bind( SigC::slot( *this, &DimeEntity::changed ), this ) );

    this->Moved.connect( SigC::bind( SigC::slot( *this, &DimeEntity::moved ), this ) );

    this->Say.connect( SigC::bind( SigC::slot( *this, &DimeEntity::say ), this ) );
	
}
*/
/*
 * return the scenenode to which this entity belongs
 */
SceneNode* DimeEntity::getSceneNode() {

	return static_cast<Ogre::SceneNode*>(mOgreEntity->getParentNode());	
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
	Root::getSingleton().getAutoCreatedWindow()->setDebugText(std::string("Moved: " + _id) );
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


