//
// C++ Implementation: ConnectedAdapter
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#include "Connection.h"
#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"

#include <Eris/Account.h>
#include <Eris/Lobby.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>
#include <Eris/TypeInfo.h>
#include <Eris/Exceptions.h>
#include <Eris/Entity.h>

#include <Atlas/Objects/Operation.h>
#include <wfmath/stream.h>

#include <sstream>

namespace Ember
{

ConnectedAdapter::ConnectedAdapter(Eris::Account& account, Eris::Avatar& avatar, Eris::Connection& connection) :
	mAccount(account), mAvatar(avatar), mConnection(connection)
{
}

ConnectedAdapter::~ConnectedAdapter()
{
}

void ConnectedAdapter::moveToPoint(const WFMath::Point<3>& dest)
{
	try {
		mAvatar.moveToPoint(dest);
	} catch (const std::exception& ex) {
		S_LOG_WARNING( "Got error on moving." << ex);
	}
}

void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation)
{
	try {
		mAvatar.moveInDirection(velocity, orientation);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on moving." << ex);
	}
}

void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity)
{
	try {
		mAvatar.moveInDirection(velocity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on moving." << ex);
	}
}

// void ConnectedAdapter::teleportTo(const WFMath::Point<3>& dest)
// {
//
// }


void ConnectedAdapter::touch(Eris::Entity* entity)
{
	try {
		mAvatar.touch(entity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on touching." << ex);
	}
}

void ConnectedAdapter::emote(const std::string& emote)
{
	try {
		mAvatar.emote(emote);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on emoting." << ex);
	}
}

void ConnectedAdapter::take(Eris::Entity* entity)
{
	try {
		mAvatar.take(entity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on taking." << ex);
	}
}

void ConnectedAdapter::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset)
{
	try {
		mAvatar.drop(entity, offset);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on dropping." << ex);
	}
}
void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos)
{
	///use the existing orientation
	place(entity, target, pos, entity->getOrientation());
}

void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient)
{
	try {
		std::stringstream ss;
		ss << "Placing " << entity->getName() << " inside " << target->getName() << " at position " << pos << " and orientation " << orient << ".";
		S_LOG_VERBOSE(ss.str());
		/// we want to do orientation too so we can't use the Avatar::place method until that's updated
		Atlas::Objects::Entity::Anonymous what;
		what->setLoc(target->getId());
		what->setPosAsList(Atlas::Message::Element(pos.toAtlas()).asList());
		what->setAttr("orientation", orient.toAtlas());

		what->setId(entity->getId());

		Atlas::Objects::Operation::Move moveOp;
		moveOp->setFrom(mAvatar.getEntity()->getId());
		moveOp->setArgs1(what);

		///if the avatar is a "creator", i.e. and admin, we will set the TO property
		///this will bypass all of the server's filtering, allowing us to place any entity, unrelated to if it's too heavy or belong to someone else
		if (mAvatar.getEntity()->getType()->isA(mConnection.getTypeService()->getTypeByName("creator"))) {
			moveOp->setTo(entity->getId());
		}

		mConnection.send(moveOp);

		// 			mAvatar.place(entity, target, pos, orient);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on dropping." << ex);
	}
}

void ConnectedAdapter::wield(Eris::Entity* entity, const std::string& outfitSlot)
{

	try {

		if (entity->getLocation() != mAvatar.getEntity()) {
			S_LOG_WARNING("Can't wield an Entity which is not located in the avatar.");

			return;
		}

		Atlas::Objects::Entity::Anonymous arguments;
		arguments->setId(entity->getId());
		if (outfitSlot != "") {
			arguments->setAttr("outfit", outfitSlot);
		}
		Atlas::Objects::Operation::Wield wield;
		wield->setFrom(mAvatar.getEntity()->getId());
		wield->setArgs1(arguments);

		mConnection.send(wield);

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on wielding." << ex);
	}
}

void ConnectedAdapter::use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation)
{
	try {
		std::stringstream ss;
		ss << pos;
		S_LOG_VERBOSE("Using " << entity->getName() << " with operation '" << operation << "' at position "<< ss.str() << ".");
		mAvatar.useOn(entity, pos, operation);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on using." << ex);
	}
}

void ConnectedAdapter::useStop()
{
	try {
		mAvatar.useStop();
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on stopping using." << ex);
	}
}

void ConnectedAdapter::actuate(Eris::Entity* entity, const std::string& action)
{
	try {

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
		actuateOp->setFrom(mAvatar.getEntity()->getId());

		S_LOG_INFO("Actuating entity with id " << entity->getId() << ", named " << entity->getName() << " with action '" << action << "'.");
		mConnection.send(actuateOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on actuating." << ex);
	}
}

void ConnectedAdapter::deleteEntity(Eris::Entity* entity)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());

		Atlas::Objects::Operation::Delete deleteOp;
		deleteOp->setFrom(mAvatar.getEntity()->getId());
		deleteOp->setTo(entity->getId());
		deleteOp->setArgs1(what);

		S_LOG_INFO("Deleting entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection.send(deleteOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on deleting entity." << ex);
	}
}

void ConnectedAdapter::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		//We'll use this flag to make sure that nothing gets sent in the case that the only thing changed was immutable attributes (like "pos").
		bool areAttributesToSend = false;
		for (Atlas::Message::MapType::iterator I = elements.begin(); I != elements.end(); ++I) {
			//The "pos" attribute is immutable and cannot be altered by a "set" op. Instead we must use a "move" op.
			if (I->first == "pos") {
				place(entity, entity->getLocation(), WFMath::Point<3>(I->second));
			} else {
				what->setAttr(I->first, I->second);
				areAttributesToSend = true;
			}
		}

		if (areAttributesToSend) {
			Atlas::Objects::Operation::Set setOp;
			setOp->setFrom(mAvatar.getEntity()->getId());
			//setOp->setTo(entity->getId());
			setOp->setArgs1(what);

			S_LOG_INFO("Setting attributes of entity with id " << entity->getId() << ", named " << entity->getName());
			mConnection.send(setOp);
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on setting attributes on entity." << ex);
	}
}

void ConnectedAdapter::attack(Eris::Entity* entity)
{
	try {
		mAvatar.attack(entity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on attack." << ex);
	}
}

void ConnectedAdapter::eat(Eris::Entity* entity)
{
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());

		Atlas::Objects::Operation::Generic op;
		op->setType("eat", -1);
		op->setFrom(mAvatar.getEntity()->getId());
		op->setArgs1(what);

		S_LOG_INFO("Eating entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection.send(op);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on eating entity." << ex);
	}
}

void ConnectedAdapter::say(const std::string &message)
{
	try {
		mAvatar.say(message);

		std::string msg;
		msg = "Saying: [" + message + "]. ";
		ConsoleBackend::getSingletonPtr()->pushMessage(msg);
		S_LOG_VERBOSE( msg);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on say." << ex);
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
		sound->setFrom(mAvatar.getEntity()->getId());
		sound->setTo(entityId);
		sound->setArgs1(talk);

		mConnection.send(sound);

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on admin_tell." << ex);
	}
}

void ConnectedAdapter::createTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	try {
		Atlas::Objects::Operation::Create createOp;
		createOp->setFrom(mAccount.getId());
		createOp->setArgs1(typeInfo);

		S_LOG_INFO("Sending new type info data.");
		mConnection.send(createOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on sending new type info data." << ex);
	}
}

void ConnectedAdapter::setTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	try {
		Atlas::Objects::Operation::Set setOp;
		setOp->setFrom(mAccount.getId());
		setOp->setArgs1(typeInfo);

		S_LOG_INFO("Sending updated type info data.");
		mConnection.send(setOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on sending updated type info data." << ex);
	}
}

}
