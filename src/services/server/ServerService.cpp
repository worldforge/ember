/*
 Copyright (C) 2002 Martin Pollard (Xmp)

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ServerService.h"

//#include "ConnectedAdapter.h"
#include "NonConnectedAdapter.h"
#include "NonConnectedState.h"
#include <wfmath/point.h>
//#include "OOGChat.h"

namespace Ember
{

ServerService::ServerService() :
	mConnection(0), mAccount(0), mAvatar(0), mOOGChat(0), mNonConnectedState(new NonConnectedState(*this))
{
	setName("Server Service");
	setDescription("Service for Server session");

	GotAccount.connect(sigc::mem_fun(*this, &ServerService::gotAccount));
	GotAvatar.connect(sigc::mem_fun(*this, &ServerService::gotAvatar));
	GotConnection.connect(sigc::mem_fun(*this, &ServerService::gotConnection));

	DestroyedAccount.connect(sigc::mem_fun(*this, &ServerService::destroyedAccount));
	DestroyedAvatar.connect(sigc::mem_fun(*this, &ServerService::destroyedAvatar));

}

ServerService::~ServerService()
{

}

/* Method for starting this service 	*/
Service::Status ServerService::start()
{
	setStatus(Service::OK);
	setRunning(true);

	return Service::OK;

}

/* Interface method for stopping this service 	*/
void ServerService::stop(int code)
{
	Service::stop(code);
	setStatus(Service::OK);

	disconnect();
}

void ServerService::gotConnection(Eris::Connection* connection)
{
	mConnection = connection;
}

void ServerService::gotAvatar(Eris::Avatar* avatar)
{
	mAvatar = avatar;
}

void ServerService::gotAccount(Eris::Account* account)
{
	mAccount = account;
}

void ServerService::destroyedAccount()
{
	mAccount = 0;
}

void ServerService::destroyedAvatar()
{
	mAvatar = 0;
}

Eris::Connection* ServerService::getConnection() const
{
	return mConnection;
}

Eris::Account* ServerService::getAccount() const
{
	return mAccount;
}

Eris::Avatar* ServerService::getAvatar() const
{
	return mAvatar;
}

/* Interface method for connecting to host */
bool ServerService::connect(const std::string& host, short port)
{
	return mNonConnectedState->connect(host, port);
}

void ServerService::disconnect()
{
	mNonConnectedState->getTopState().disconnect();
}

bool ServerService::logout()
{
	return mNonConnectedState->getTopState().logout();
}

void ServerService::takeTransferredCharacter(const Eris::TransferInfo& transferInfo)
{
	mNonConnectedState->getTopState().takeTransferredCharacter(transferInfo);
}

void ServerService::takeCharacter(const std::string &id)
{
	mNonConnectedState->getTopState().takeCharacter(id);
}

bool ServerService::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties)
{
	return mNonConnectedState->getTopState().createCharacter(name, sex, type, description, spawnName, extraProperties);
}

void ServerService::moveToPoint(const WFMath::Point<3>& dest)
{
	getAdapter().moveToPoint(dest);
}

void ServerService::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation)
{
	getAdapter().moveInDirection(velocity, orientation);
}

void ServerService::moveInDirection(const WFMath::Vector<3>& velocity)
{
	getAdapter().moveInDirection(velocity);
}

void ServerService::touch(Eris::Entity* entity)
{
	getAdapter().touch(entity);
}

void ServerService::take(Eris::Entity* entity)
{
	getAdapter().take(entity);
}

void ServerService::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset, const WFMath::Quaternion& orientation)
{
	getAdapter().drop(entity, offset, orientation);
}

void ServerService::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos)
{
	getAdapter().place(entity, target, pos);
}

void ServerService::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient)
{
	getAdapter().place(entity, target, pos, orient);
}

void ServerService::wield(Eris::Entity* entity, const std::string& outfitSlot)
{
	getAdapter().wield(entity, outfitSlot);
}

void ServerService::use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation)
{
	getAdapter().use(entity, pos, operation);
}

void ServerService::useStop()
{
	getAdapter().useStop();
}

void ServerService::actuate(Eris::Entity* entity, const std::string& action)
{
	getAdapter().actuate(entity, action);
}

void ServerService::attack(Eris::Entity* entity)
{
	getAdapter().attack(entity);
}

void ServerService::eat(Eris::Entity* entity)
{
	getAdapter().eat(entity);
}

void ServerService::say(const std::string &message)
{
	getAdapter().say(message);
}

void ServerService::sayTo(const std::string &message, const std::vector<const Eris::Entity*>& entities)
{
	getAdapter().sayTo(message, entities);
}

void ServerService::sayTo(const std::string &message, const Eris::Entity& entity)
{
	getAdapter().sayTo(message, entity);
}

void ServerService::emote(const std::string &message)
{
	getAdapter().emote(message);
}

void ServerService::deleteEntity(Eris::Entity* entity)
{
	getAdapter().deleteEntity(entity);
}

void ServerService::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes)
{
	getAdapter().setAttributes(entity, attributes);
}

void ServerService::adminTell(const std::string& entityId, const std::string& attribute, const std::string &value)
{
	getAdapter().adminTell(entityId, attribute, value);
}

void ServerService::createTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	getAdapter().createTypeInfo(typeInfo);
}

void ServerService::setTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	getAdapter().setTypeInfo(typeInfo);
}

IServerAdapter& ServerService::getAdapter()
{
	return mNonConnectedState->getTopState().getServerAdapter();
}

} // namespace Ember
