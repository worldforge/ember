//
// C++ Implementation: ConnectedAdapter
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
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
#include <Eris/Response.h>

#include <Atlas/Objects/Operation.h>
#include <wfmath/stream.h>

namespace Ember {

ConnectedAdapter::ConnectedAdapter(Eris::Account& account, Eris::Avatar& avatar) :
		mAccount(account), mAvatar(avatar), mConnection(*avatar.getConnection()) {
}

void ConnectedAdapter::moveToPoint(const WFMath::Point<3>& dest) {
	try {
		mAvatar.moveToPoint(dest, WFMath::Quaternion());
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on moving." << ex);
	}
}

void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) {
	try {
		mAvatar.moveInDirection(velocity, orientation);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on moving." << ex);
	}
}

void ConnectedAdapter::moveInDirection(const WFMath::Vector<3>& velocity) {
	try {
		mAvatar.moveInDirection(velocity, WFMath::Quaternion());
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on moving." << ex);
	}
}

void ConnectedAdapter::touch(Eris::Entity* entity, WFMath::Point<3> pos) {
	try {
		mAvatar.touch(entity, pos);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on touching." << ex);
	}
}

void ConnectedAdapter::emote(const std::string& emote) {
	try {
		mAvatar.emote(emote);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on emoting." << ex);
	}
}

void ConnectedAdapter::take(Eris::Entity* entity) {
	try {
		mAvatar.take(entity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on taking." << ex);
	}
}

void ConnectedAdapter::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset, const WFMath::Quaternion& orientation) {
	try {
		mAvatar.drop(entity, offset, orientation);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on dropping." << ex);
	}
}

void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos) {
	//use the existing orientation
	place(entity, target, pos, entity->getOrientation());
}

void ConnectedAdapter::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient) {
	try {
		mAvatar.place(entity, target, pos, orient);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on dropping." << ex);
	}
}

void ConnectedAdapter::wield(Eris::Entity* entity, const std::string& attachPoint) {

	try {

		Atlas::Objects::Entity::Anonymous arguments;
		if (entity) {
			arguments->setId(entity->getId());
		}
		arguments->setAttr("attachment", attachPoint);
		Atlas::Objects::Operation::Wield wield;
		wield->setFrom(mAvatar.getId());
		wield->setArgs1(arguments);

		mConnection.send(wield);

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on wielding." << ex);
	}
}

void ConnectedAdapter::use(Eris::Entity* entity, const std::string& operation, WFMath::Point<3> pos) {
//	try {
//		std::stringstream ss;
//		ss << pos;
//		S_LOG_VERBOSE("Using " << entity->getName() << " with operation '" << operation << "' at position " << ss.str() << ".");
//		mAvatar.useOn(entity, pos, operation);
//	} catch (const std::exception& ex) {
//		S_LOG_WARNING("Got error on using." << ex);
//	}
}

void ConnectedAdapter::useStop() {
	try {
		mAvatar.useStop();
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on stopping using." << ex);
	}
}

void ConnectedAdapter::actuate(Eris::Entity* entity, const std::string& action) {
	try {

		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		// 	what->setObjtype("obj");

		Atlas::Objects::Operation::RootOperation actionOp;
		actionOp->setObjtype("op");
		actionOp->setArgs1(what);
		actionOp->setParent(action);

		Atlas::Objects::Operation::RootOperation actuateOp;
		actuateOp->setObjtype("op");
		actuateOp->setArgs1(actionOp);
		actuateOp->setParent("actuate");
		actuateOp->setFrom(mAvatar.getId());

		S_LOG_INFO("Actuating entity with id " << entity->getId() << ", named " << entity->getName() << " with action '" << action << "'.");
		mConnection.send(actuateOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on actuating." << ex);
	}
}

void ConnectedAdapter::deleteEntity(Eris::Entity* entity) {
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());

		Atlas::Objects::Operation::Delete deleteOp;
		deleteOp->setFrom(mAvatar.getId());
		deleteOp->setTo(entity->getId());
		deleteOp->setArgs1(what);
		deleteOp->setSerialno(Eris::getNewSerialno());
		mConnection.getResponder()->await(deleteOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
			//For now ignore; perhaps we should do some error checking instead?
			return Eris::Router::IGNORED;
		});

		S_LOG_INFO("Deleting entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection.send(deleteOp);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on deleting entity." << ex);
	}
}

void ConnectedAdapter::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& elements) {
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());
		//We'll use this flag to make sure that nothing gets sent in the case that the only thing changed was immutable attributes (like "pos").
		bool areAttributesToSend = false;
		Atlas::Message::MapType moveAttributes;
		for (auto& element : elements) {
			if (element.first == "pos" ||
				element.first == "mode" ||
				element.first == "orientation" ||
				element.first == "planted-offset" ||
				element.first == "loc" ||
				element.first == "velocity") {
				moveAttributes.insert(element);
			} else {
				what->setAttr(element.first, element.second);
				areAttributesToSend = true;
			}
		}

		//Some attributes can only be changed through a Move op.
		if (!moveAttributes.empty()) {
			Atlas::Objects::Entity::Anonymous moveArgs;
			if (entity->getLocation()) {
				moveArgs->setLoc(entity->getLocation()->getId());
			}
			for (auto& element : moveAttributes) {
				moveArgs->setAttr(element.first, element.second);
			}

			moveArgs->setId(entity->getId());

			Atlas::Objects::Operation::Move moveOp;
			moveOp->setFrom(mAvatar.getId());
			moveOp->setArgs1(moveArgs);

			moveOp->setSerialno(Eris::getNewSerialno());
			mConnection.getResponder()->await(moveOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
				//For now ignore; perhaps we should do some error checking instead?
				return Eris::Router::IGNORED;
			});

			//if the avatar is an admin, we will set the TO property
			//this will bypass all of the server's filtering, allowing us to place any
			//entity, unrelated to if it's too heavy or belong to someone else
			if (mAvatar.getIsAdmin()) {
				moveOp->setTo(entity->getId());
			}

			mConnection.send(moveOp);
		}


		if (areAttributesToSend) {
			Atlas::Objects::Operation::Set setOp;
			setOp->setFrom(mAvatar.getId());
			if (entity->getId() != mAvatar.getId()) {
				setOp->setTo(entity->getId());
			}
			setOp->setArgs1(what);

			setOp->setSerialno(Eris::getNewSerialno());
			mConnection.getResponder()->await(setOp->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
				//For now ignore; perhaps we should do some error checking instead?
				return Eris::Router::IGNORED;
			});
			S_LOG_INFO("Setting attributes of entity with id " << entity->getId() << ", named " << entity->getName());
			mConnection.send(setOp);
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on setting attributes on entity." << ex);
	}
}

void ConnectedAdapter::attack(Eris::Entity* entity) {
	try {
		mAvatar.attack(entity);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on attack." << ex);
	}
}

void ConnectedAdapter::eat(Eris::Entity* entity) {
	try {
		Atlas::Objects::Entity::Anonymous what;
		what->setId(entity->getId());

		Atlas::Objects::Operation::Generic op;
		op->setType("eat", -1);
		op->setFrom(mAvatar.getId());
		op->setArgs1(what);

		S_LOG_INFO("Eating entity with id " << entity->getId() << ", named " << entity->getName());
		mConnection.send(op);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on eating entity." << ex);
	}
}

void ConnectedAdapter::say(const std::string& message) {
	try {
		mAvatar.say(message);

		std::string msg;
		msg = "Saying: [" + message + "]. ";
		S_LOG_VERBOSE(msg);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on say." << ex);
	}
}

void ConnectedAdapter::sayTo(const std::string& message, const std::vector<std::string>& entities) {
	try {

		mAvatar.sayTo(message, entities);

		std::string msg;
		if (entities.size() > 1) {
			msg = "Saying to many entities: [" + message + "]. ";
		} else if (entities.size() == 1) {
			msg = "Saying to entity " + entities[0] + ": [" + message + "]. ";
		} else {
			msg = "Saying to no entity: [" + message + "]. ";
		}
		S_LOG_VERBOSE(msg);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on say." << ex);
	}
}

void ConnectedAdapter::sayTo(const std::string& message, const Eris::Entity& entity) {
	try {
		std::vector<std::string> entities;
		entities.push_back(entity.getId());

		mAvatar.sayTo(message, entities);

		std::string msg;
		msg = "Saying to " + entity.getName() + ": [" + message + "]. ";
		S_LOG_VERBOSE(msg);
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on say." << ex);
	}

}

void ConnectedAdapter::adminTell(const std::string& entityId, const std::string& attribute, const std::string& value) {
	try {

		Atlas::Objects::Entity::Anonymous what;
		what->setAttr(attribute, value);
		Atlas::Objects::Operation::Talk talk;
		talk->setFrom(entityId);
		talk->setTo(entityId);
		talk->setArgs1(what);

		Atlas::Objects::Operation::Sound sound;
		sound->setFrom(mAvatar.getId());
		sound->setTo(entityId);
		sound->setArgs1(talk);

		sound->setSerialno(Eris::getNewSerialno());
		mConnection.getResponder()->await(sound->getSerialno(), [&](const Atlas::Objects::Operation::RootOperation& op) -> Eris::Router::RouterResult {
			//Handle it here, so it does not propagate into the regular system.
			return Eris::Router::HANDLED;
		});
		S_LOG_INFO("Admin telling entity" << entityId << ": " << attribute << ": " << value);
		mConnection.send(sound);

	} catch (const std::exception& ex) {
		S_LOG_WARNING("Got error on admin_tell." << ex);
	}
}

void ConnectedAdapter::createTypeInfo(const Atlas::Objects::Root& typeInfo) {
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

void ConnectedAdapter::setTypeInfo(const Atlas::Objects::Root& typeInfo) {
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
