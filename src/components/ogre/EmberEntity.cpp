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
 */
void DimeEntity::connectSignals()
{
    this->AddedMember.connect( SigC::slot( *this, &DimeEntity::addedMember ) );

    this->RemovedMember.connect( SigC::slot( *this, &DimeEntity::removedMember ) );

    this->Recontainered.connect( SigC::slot( *this, &DimeEntity::recontainered ) );

    this->Changed.connect( SigC::bind( SigC::slot( *this, &DimeEntity::changed ), this ) );

    this->Moved.connect( SigC::bind( SigC::slot( *this, &DimeEntity::moved ), this ) );

    this->Say.connect( SigC::bind( SigC::slot( *this, &DimeEntity::say ), this ) );
	
}

/*
 * return the scenenode to which this entity belongs
 */
SceneNode* DimeEntity::getSceneNode() {

	return static_cast<Ogre::SceneNode*>(mOgreEntity->getParentNode());	
}


void DimeEntity::entityCreate( Eris::Entity *e ) {}

/**
 * Called on entity deletion. You should remove all information you
 * hold about the entity.
 *
 * @param e A pointer to the Eris entity that has been deleted.
 *
 */
void DimeEntity::entityDelete( Eris::Entity *e ) {}

/**
 * Called only once, when the player enters the game world. It's
 * possible that you won't need this one.
 *
 * @param e A pointer to the Eris entity
 *
 */
void DimeEntity::entered( Eris::Entity *e ) {}

/** Called when an entity become visible. You'll probably want to add
 * a media pointer to your world model at this point.
 *
 * @param e A pointer to the Eris entity
 *
 */
void DimeEntity::appearance( Eris::Entity *e )
{
	mOgreEntity->setVisible(true);	
}

/**
 * Called when an entity becomes invisible. You should remove the media
 * pointer corresponding to the entity from your world view, but retain
 * any additional data you're holding about the entity.
 *
 * @param e A pointer to the Eris entity
 *
 */
void DimeEntity::disappearance( Eris::Entity *e )
{
	mOgreEntity->setVisible(false);	
		
}


/* Eris::Entity signals  (see eris\src\entity.h for more info)*/

/**
 * Called when an entity changes its container. This may require
 * changes to your world model, but some gameviews can safely ignore
 * this signal.
 *
 * @param e A pointer to the Eris entity that has been recontainered
 * @param c A pointer to the Eris entity that is the new container for e
 */
void DimeEntity::recontainered( Eris::Entity *e, Eris::Entity *c ) {}

/**
 * I'm not sure what this does. Let's ignore it until I can track down
 * James and bop him on the head for writing unhelpful comments ;)
 * NOTES: I suspect this is when an attribute of the object is changed.
 */
void DimeEntity::changed( const Eris::StringSet &s, Eris::Entity *e ) {}

/**
 * Called when the entity moves. Here you should alter the position
 * of the media pointer in your world model... this may involve
 * removing it from where it was before the entity moved and
 * placing it in the new position, in which case you'll need
 * a reverse-lookup of some kinda- WFMath::Point<3> is the new
 * entity coordinate, the old one is only known if stored by you.
 *
 * @param c The new coordinates of the entity
 * @param e A pointer to the Eris entity that has moved
 */
void DimeEntity::moved( const WFMath::Point< 3 > & point, Eris::Entity *e )
{
	getSceneNode()->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(point));
	getSceneNode()->setOrientation(Atlas2Ogre(e->getOrientation()));
}

/**
 * Called when the entity speaks. You'll probably want to display the
 * speech on the screen somehow.
 *
 * @param s A string containing the speech
 * @param e A pointer to the Eris entity
 */
void DimeEntity::say( const std::string &s, Eris::Entity *e )
{
        std::string message = "<";
        message.append(e->getName());
        message.append("> ");
        message.append(s);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
	
}

/**
* Sadly undocumented
 */
void DimeEntity::addedMember(Eris::Entity *e) {}

/**
 * Also sadly undocumented
 */
void DimeEntity::removedMember(Eris::Entity *e) {}

