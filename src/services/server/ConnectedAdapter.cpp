//
// C++ Implementation: ConnectedAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConnectedAdapter.h"
#include <Eris/Connection.h>
#include <Eris/Account.h>
#include <Eris/Lobby.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Eris/TypeInfo.h>
#include <Eris/Exceptions.h>
#include <Eris/Entity.h>

#include <Atlas/Objects/Operation.h>

#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"


namespace Ember {

ConnectedAdapter::ConnectedAdapter(Eris::Avatar* avatar, Eris::Connection* connection) : mAvatar(avatar), mConnection(connection)
{
}


ConnectedAdapter::~ConnectedAdapter()
{
}

void ConnectedAdapter::moveToPoint(const WFMath::Point<3>& dest) {
	try {
		mAvatar->moveToPoint(dest);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING( "Got Eris error on moving: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on moving: " << except.what());
	}

}

void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) {
	try {
		mAvatar->moveInDirection(velocity, orientation);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on moving: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on moving: " << except.what());
	}
}


void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity) {
	try {
		mAvatar->moveInDirection(velocity);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on moving: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on moving: " << except.what());
	}
}

// void ConnectedAdapter::teleportTo(const WFMath::Point<3>& dest)
// {
// 
// }


void ConnectedAdapter::touch(Eris::Entity* entity) 
{
	try {
		mAvatar->touch(entity);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on touching: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on touching: " << except.what());
	}
}   		
 
void ConnectedAdapter::emote(const std::string& emote)
{
	try {
		mAvatar->emote(emote);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on emoting: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on emoting: " << except.what());
	}
}

void ConnectedAdapter::take(Eris::Entity* entity) 
{
	try {
		mAvatar->take(entity);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on taking: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on taking: " << except.what());
	}
}   		

void ConnectedAdapter::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset) 
{
	try {
		mAvatar->drop(entity, offset);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on dropping: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on dropping: " << except.what());
	}
}   		
void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos)
{
	///use the existing orientation
	place(entity, target, pos, entity->getOrientation( ));
}

void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient)
{
	try {
		/// we want to do orientation too so we can't use the Avatar::place method until that's updated
		Atlas::Objects::Entity::Anonymous what;
		what->setLoc(target->getId());
		what->setPosAsList(Atlas::Message::Element(pos.toAtlas()).asList());
		what->setAttr("orientation", orient.toAtlas());			
		
		what->setId(entity->getId());
	
		Atlas::Objects::Operation::Move moveOp;
		moveOp->setFrom(mAvatar->getEntity()->getId());
		moveOp->setArgs1(what);
		
		///if the avatar is a "creator", i.e. and admin, we will set the TO property
		///this will bypass all of the server's filtering, allowing us to place any entity, unrelated to if it's too heavy or belong to someone else
		if (mAvatar->getEntity()->getType()->isA(mConnection->getTypeService()->getTypeByName("creator"))) {
			moveOp->setTo(entity->getId());
		}
	
		mConnection->send(moveOp);	
	
	
// 			mAvatar->place(entity, target, pos, orient);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on dropping: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on dropping: " << except.what());
	}
}   		

void ConnectedAdapter::wield(Eris::Entity* entity)
{
	try {
		mAvatar->wield(entity);
		
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on wielding: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on wielding: " << except.what());
	}
}   		

void ConnectedAdapter::use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation)
{
	try {
		S_LOG_VERBOSE("Using " << entity->getName() << " with operation '" << operation << "'.");
		mAvatar->useOn(entity, pos, operation);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on using: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on using: " << except.what());
	}
}   		

void ConnectedAdapter::useStop()
{
	try {
		mAvatar->useStop();
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on stopping using: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on stopping using: " << except.what());
	}
}   

void ConnectedAdapter::actuate(Eris::Entity* entity, const std::string& action)
{

	Atlas::Objects::Entity::Anonymous what;
	what->setId(entity->getId());
// 	what->setObjtype("obj");
	
	Atlas::Objects::Operation::RootOperation actionOp;
	actionOp->setObjtype("op");
	actionOp->setArgs1(what);
	std::list<std::string> actionParents;
	actionParents.push_back(action);
	actionOp->setParents(actionParents);
	
	Atlas::Objects::Operation::RootOperation actuateOp;
	actuateOp->setObjtype("op");
	actuateOp->setArgs1(actionOp);
	std::list<std::string> actuateParents;
	actuateParents.push_back("actuate");
	actuateOp->setParents(actuateParents);
	actuateOp->setFrom(mAvatar->getEntity()->getId());


	S_LOG_INFO("Actuating entity with id " << entity->getId() << ", named " << entity->getName() << " with action '" << action << "'.");
	mConnection->send(actuateOp);	
}

void ConnectedAdapter::deleteEntity(Eris::Entity* entity)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		
		Atlas::Objects::Operation::Delete deleteOp;
		deleteOp->setFrom(mAvatar->getEntity()->getId());
		deleteOp->setTo(entity->getId());
		deleteOp->setArgs1(what);
		
		S_LOG_INFO("Deleting entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection->send(deleteOp);	
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on deleting entity: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on deleting entity: " << except.what());
	}
}
	
void ConnectedAdapter::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		for(Atlas::Message::MapType::iterator I = elements.begin(); I != elements.end(); ++I) {
			what->setAttr(I->first, I->second);			
		}
		
		Atlas::Objects::Operation::Set setOp;
		setOp->setFrom(mAvatar->getEntity()->getId());
		//setOp->setTo(entity->getId());
		setOp->setArgs1(what);
		
		S_LOG_INFO("Setting attributes of entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection->send(setOp);	
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on setting attributes on entity: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on setting attributes on entity: " << except.what());
	}
}
	
	
void ConnectedAdapter::attack(Eris::Entity* entity)
{
	try {
		mAvatar->attack(entity);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on attack: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on attack: " << except.what());
	}
}

void ConnectedAdapter::eat(Eris::Entity* entity)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		
		Atlas::Objects::Operation::Generic op;
		op->setType("eat", -1);
		op->setFrom(mAvatar->getEntity()->getId());
		op->setArgs1(what);
		
		S_LOG_INFO("Eating entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection->send(op);	
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on eating entity: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on eating entity: " << except.what());
	}
}

		
void ConnectedAdapter::say(const std::string &message) 
{
	try {
		mAvatar->say(message);
		
		std::string msg;
		msg = "Saying: [" + message + "]. ";
		ConsoleBackend::getSingletonPtr()->pushMessage(msg);
		S_LOG_VERBOSE( msg);
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on say: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on say: " << except.what());
	}	
}

void ConnectedAdapter::adminTell(const std::string& entityId, const std::string& attribute, const std::string &value) 
{
	try {
		
		Atlas::Objects::Entity::Anonymous what;
		what->setAttr(attribute, value);
		Atlas::Objects::Operation::Talk talk;
		talk->setFrom(entityId);
		talk->setTo(entityId);
		talk->setArgs1(what);
		
		Atlas::Objects::Operation::Sound sound;
		sound->setFrom(mAvatar->getEntity()->getId());
		sound->setTo(entityId);
		sound->setArgs1(talk);
		
		mConnection->send(sound);
		
	}
	catch (const Eris::BaseException& except)
	{
		S_LOG_WARNING("Got Eris error on admin_tell: " << except._msg);
	}
	catch (const std::runtime_error& except)
	{
		S_LOG_WARNING("Got unknown error on admin_tell: " << except.what());
	}	
}

}
