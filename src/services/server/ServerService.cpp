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

#include "NonConnectedState.h"
#include "services/config/ConfigService.h"

#include <Eris/Session.h>
#include <framework/LoggingInstance.h>

//Needed for the "access" function.
#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#endif

#include "framework/FileSystemObserver.h"

namespace Ember {

ServerService::ServerService(Eris::Session& session) :
		Service("Server"),
		mSession(session),
		mConnection(nullptr),
		mAccount(nullptr),
		mAvatar(nullptr),
		mOOGChat(nullptr),
		mServerAdapter(nullptr),
		mNonConnectedState(new NonConnectedState(*this, session)) {
	GotAccount.connect(sigc::mem_fun(*this, &ServerService::gotAccount));
	GotAvatar.connect(sigc::mem_fun(*this, &ServerService::gotAvatar));
	GotConnection.connect(sigc::mem_fun(*this, &ServerService::gotConnection));

	DestroyedAccount.connect(sigc::mem_fun(*this, &ServerService::destroyedAccount));
	DestroyedAvatar.connect(sigc::mem_fun(*this, &ServerService::destroyedAvatar));
}

ServerService::~ServerService() = default;

/* Method for starting this service 	*/
bool ServerService::start() {
	setRunning(true);

	return true;

}

/* Interface method for stopping this service 	*/
void ServerService::stop() {
	if (isRunning()) {
		auto directory = boost::filesystem::path(mLocalSocketPath).remove_filename().string();
		Ember::FileSystemObserver::getSingleton().remove_directory(directory);

		disconnect();
	}
	Service::stop();
}

void ServerService::gotConnection(Eris::Connection* connection) {
	mConnection = connection;
}

void ServerService::gotAvatar(Eris::Avatar* avatar) {
	mAvatar = avatar;
}

void ServerService::gotAccount(Eris::Account* account) {
	mAccount = account;
}

void ServerService::destroyedAccount() {
	mAccount = nullptr;
}

void ServerService::destroyedAvatar() {
	mAvatar = nullptr;
}

Eris::Connection* ServerService::getConnection() const {
	return mConnection;
}

Eris::Account* ServerService::getAccount() const {
	return mAccount;
}

Eris::Avatar* ServerService::getAvatar() const {
	return mAvatar;
}

/* Interface method for connecting to host */
bool ServerService::connect(const std::string& host, short port) {
	return mNonConnectedState->connect(host, port);
}

bool ServerService::connectLocal() {
	return mNonConnectedState->connectLocal(mLocalSocketPath.string());
}

bool ServerService::hasLocalSocket() {
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

void ServerService::disconnect() {
	mNonConnectedState->getTopState().disconnect();
}

bool ServerService::logout() {
	return mNonConnectedState->getTopState().logout();
}

void ServerService::takeTransferredCharacter(const Eris::TransferInfo& transferInfo) {
	mNonConnectedState->getTopState().takeTransferredCharacter(transferInfo);
}

void ServerService::takeCharacter(const std::string& id) {
	mNonConnectedState->getTopState().takeCharacter(id);
}

bool ServerService::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties) {
	return mNonConnectedState->getTopState().createCharacter(name, sex, type, description, spawnName, extraProperties);
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

	if (boost::filesystem::is_directory(directory)) {
		try {
			Ember::FileSystemObserver::getSingleton().add_directory(directory, [this](const Ember::FileSystemObserver::FileSystemEvent& event) {
				if (event.ev.path == mLocalSocketPath) {
					S_LOG_VERBOSE("Local server socket directory changed.");
					EventLocalSocketChanged.emit();
				}
			});
		} catch (...) {
			S_LOG_INFO("Could not observe the directory '" << mLocalSocketPath.string() << "' which means Ember won't detect if a local server is started.");
		}
		S_LOG_VERBOSE("Observing directory " << directory << " for local server socket.");
	} else {
		S_LOG_INFO("Could not observe the directory '" << mLocalSocketPath.string() << "' which means Ember won't detect if a local server is started.");
	}
}

} // namespace Ember
