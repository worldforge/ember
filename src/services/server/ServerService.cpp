/*
 Copyright (C) 2002 Martin Pollard (Xmp)
 Copyright (C) 2013 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ServerService.h"

#include "NonConnectedAdapter.h"
#include "NonConnectedState.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include <Eris/Session.h>
#include <framework/LoggingInstance.h>

//Needed for the "access" function.
#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#endif

#include "framework/FileSystemObserver.h"

namespace Ember
{

ServerService::ServerService(Eris::Session& session) :
		Service("Server"),
		mSession(session),
		mConnection(nullptr),
		mAccount(nullptr),
		mAvatar(nullptr),
		mOOGChat(nullptr),
		mNonConnectedState(new NonConnectedState(*this, session))
{
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
bool ServerService::start()
{
	setRunning(true);

	return true;

}

/* Interface method for stopping this service 	*/
void ServerService::stop()
{
	Service::stop();

	Ember::FileSystemObserver::getSingleton().remove_directory(mLocalSocketPath.string());

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

bool ServerService::connectLocal()
{
	return mNonConnectedState->connectLocal(mLocalSocketPath.string());
}

bool ServerService::hasLocalSocket()
{
	//No socket support on win32
#ifdef _WIN32
	return false;
#else

	//check that the socket file is available and that we can connect to it
	if (!boost::filesystem::exists(mLocalSocketPath)) {
		return false;
	}

	try {
		boost::asio::local::stream_protocol::socket socket(mSession.getIoService());
		socket.connect(boost::asio::local::stream_protocol::endpoint(mLocalSocketPath.string()));
		return socket.is_open();
	} catch (...) {
		return false;
	}
#endif
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

void ServerService::setupLocalServerObservation(ConfigService& configService) {

	mLocalSocketPath = boost::filesystem::path(configService.getPrefix() + "/var/tmp/cyphesis_cyphesis.sock");
	if (configService.itemExists("general", "socket")) {
		auto setting = configService.getValue("general", "socket");
		if (setting.is_string()) {
			mLocalSocketPath = boost::filesystem::path(setting.as_string());
		}
	}

	auto directory = boost::filesystem::path(mLocalSocketPath).remove_filename().string();
	S_LOG_VERBOSE("Observing directory " << directory << " for local server socket.");

	Ember::FileSystemObserver::getSingleton().add_directory(directory, [this](const Ember::FileSystemObserver::FileSystemEvent& event) {
		if (event.ev.path == mLocalSocketPath) {
			S_LOG_VERBOSE("Local server socket directory changed.");
			EventLocalSocketChanged.emit();
		}
	});
}


} // namespace Ember
