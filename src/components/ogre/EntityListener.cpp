/*
	EntityListener.cpp by Miguel Guzman (Aglanor)
	Copyright (C) 2003 Miguel Guzman & The Worldforge Project

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

//TODO: reorder headers, etc

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "framework/ConsoleBackend.h"

#include "MathConverter.h"
#include "DimeEntity.h"
#include "Avatar.h"
#include "EntityListener.h"


EntityListener* EntityListener::_instance = 0;

EntityListener::EntityListener()
{

}

EntityListener::~EntityListener()
{
#if 0
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();
    w->EntityCreate.disconnect();
#endif
}

EntityListener & EntityListener::getSingleton(void)
{
	//fprintf(stderr, "TRACE - ENTITY LISTENER - SINGLETON ENTERING\n");
	if(_instance == 0)
		_instance = new EntityListener;
	return *_instance;
}

void EntityListener::setSceneManager(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr=sceneMgr;
	dime::ConsoleBackend::getMainConsole()->pushMessage("Scene Manager set on Entity Listener");
}

void EntityListener::connectWorldSignals(void) {

	dime::ConsoleBackend::getMainConsole()->pushMessage("Connecting world signals");

    /* Find out where the Eris world instance resides... */
    Eris::World *w = dime::DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    //w->EntityCreate.connect( SigC::slot( *this, &EntityListener::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &EntityListener::entityDelete ) );

    //w->Entered.connect( SigC::slot( *mDimeAvatar, &Avatar::enteredWorld ) );

    w->Appearance.connect( SigC::slot( *this, &EntityListener::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &EntityListener::disappearance ) );

	dime::ConsoleBackend::getMainConsole()->pushMessage("World signals connected");

}

/* Eris::World entity signals */
/*
void EntityListener::entityCreate( Eris::Entity *e )
{

	dime::ConsoleBackend::getMainConsole()->pushMessage("Adding entity...");

	// create the ogre node and the
	// TODO: use Eris entity hierarchy for the node hierarchy !!
	Ogre::SceneNode* ogreNode = dynamic_cast<Ogre::SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
	Ogre::Entity* ogreEntity;

	// create the ogre entity
	if(!strcmp(e->getID().c_str(),avatarID.c_str())) { // if it's the player
		dime::ConsoleBackend::getMainConsole()->pushMessage("Creating player entity");
		fprintf(stderr, "TRACE - CREATING PLAYER ENTITY\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "dragon.mesh");
		//ogreNode->setScale(OGRESCALER);
	}
	else if(!strcmp(e->getType()->getName().c_str(),"settler"))	// 0 if strings are equal
	{
		fprintf(stderr, "TRACE - FOUND A SETTLER - MALEBUILDER MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "robot.mesh");
		//ogreNode->setScale(1,1,1);
		//ogreNode->setScale(OGRESCALER);
	}
	else if(!strcmp(e->getType()->getName().c_str(),"merchant"))
	{
		fprintf(stderr, "TRACE - FOUND A MERCHANT - ROBOT MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "robot.mesh");
		//ogreNode->setScale(1,1,1);
		//ogreNode->setScale(OGRESCALER);
	}
	else if(!strcmp(e->getType()->getName().c_str(),"pig"))
	{
		fprintf(stderr, "TRACE - FOUND A PIG - OGREHEAD MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "ogrehead.mesh");
		//ogreNode->setScale(OGRESCALER);
	}
	else
	{
		// TODO: razor should be a coin
		fprintf(stderr, "TRACE - FOUND ANYTHING ELSE - RAZOR MESH\n");
		ogreEntity = mSceneMgr->createEntity(e->getID(), "razor.mesh");
		//ogreNode->setScale(1,1,1);
		//ogreNode->setScale(0.001,0.001,0.001);
	}
	ogreEntity->setVisible(false);

	// set the Ogre node position and orientation based on Atlas data
	ogreNode->setPosition(WF2OGRE_VECTOR3(1,1,1) * Atlas2Ogre(e->getPosition()));
	std::cout << "Entity placed at (" << e->getPosition().x() << "," << e->getPosition().y() << "," << e->getPosition().x() << ")" << std::endl;
	//WFMath::Quaternion wq = e->getOrientation();
	//WFMath::Vector<3> wv = wq.vector();
	//Ogre::Quaternion oq = Ogre::Quaternion(Atlas2Ogre(e-getOrientation()));
	ogreNode->setOrientation(Atlas2Ogre(e->getOrientation()));

	// old code, just for the record
	/** WFMath::Point<3> position = e->getPosition();
	ogreNode->setPosition(position.x(),position.z(),-position.y());*/

/*	// scale HACK. This is very hacky. Fix this.
        if(!strcmp(e->getType()->getName().c_str(),"settler")) {	// 0 if strings are equal
				// robots are bigger :P
	}
	else {

	}
*/
/*
	dimeEntity = new DimeEntity(ogreEntity, e);
	ogreEntity->setUserObject(dimeEntity);
	// attach the node to the entity
	ogreNode->attachObject(ogreEntity);

	fprintf(stderr, "TRACE - ENTITY ADDED TO THE GAMEVIEW\n");

    // Whenever a new entity is created, make sure to connect to those signals too

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *dimeEntity, &DimeEntity::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *dimeEntity, &DimeEntity::removedMember ) );

    e->Recontainered.connect( SigC::slot( *dimeEntity, &DimeEntity::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *dimeEntity, &DimeEntity::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *dimeEntity, &DimeEntity::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *dimeEntity, &DimeEntity::say ), e ) );
}
*/

void EntityListener::entityDelete( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY DELETED\n");
}

void EntityListener::entered( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - PLAYER ENTERED THE WORLD\n");
	std::cout << "THE PLAYER IS ENTITY " <<  e->getID() << std::endl;
	avatarID = e->getID();
	dime::ConsoleBackend::getMainConsole()->pushMessage("Entering the world...");
	// Set the Player camera accordingly
	// TODO: do this when the avatar moves too
	/*mCamera->setPosition(Atlas2Ogre(e->getPosition()));
	Ogre::Vector3 height(0,1.75,0);
	mCamera->move(height);
	mCamera->setOrientation(Atlas2Ogre(e->getOrientation()));
        mWindow->setDebugText("Avatar entered the world");
	*/

}

void EntityListener::appearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY APPEARS\n");
	dime::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY APPEARS");
}

void EntityListener::disappearance( Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY DISAPPPEARS\n");
}

/* Eris::Entity signals */
/*
void EntityListener::recontainered( Eris::Entity *e, Eris::Entity *c )
{
	fprintf(stderr, "TRACE - ENTITY RECONTAINERED\n");
}

void EntityListener::changed( const Eris::StringSet &s, Eris::Entity *e  )
{
	std::cout << "TRACE - ENTITY CHANGED - PROCESSING CHANGES FOR "  << e->getID() << std::endl;
	/*
        TODO: ENTITY CHANGES HERE
	*/
/*
}

void EntityListener::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{
	fprintf(stderr, "TRACE - ENTITY MOVED\n");

	Ogre::Entity* o = mSceneMgr->getEntity(e->getID());
	o->getParentNode()->setPosition(Atlas2Ogre(e->getPosition()));
	o->getParentNode()->setOrientation(Atlas2Ogre(e->getOrientation()));
}

void EntityListener::say( const std::string &s, Eris::Entity *e )
{
        std::string message = "<";
        message.append(e->getName());
        message.append("> ");
        message.append(s);
	std::cout << "TRACE - ENTITY SAYS: [" << message << "]\n" << std::endl;
	dime::ConsoleBackend::getMainConsole()->pushMessage("TRACE - ENTITY SPEAKS");
        //mWindow->setDebugText(message);

	// TODO: fix this one
	/*
    dime::LoggingService::getInstance()->slog(__FILE__, __LINE__, dime::LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
	*/
	/*
}

void EntityListener::addedMember(Eris::Entity *e)
{}

void EntityListener::removedMember(Eris::Entity *e)
{}
*/
Ogre::SceneManager* EntityListener::getSceneManager()
{
	return mSceneMgr;
}






