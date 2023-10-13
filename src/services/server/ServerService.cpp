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

#include "ServerService.h"

#include "LoggedInState.h"
#include "NonConnectedState.h"
#include "ConnectingState.h"
#include "ConnectedState.h"
#include "AccountAvailableState.h"
#include "EnteredWorldState.h"
#include "StateBase.h"

#include "services/config/ConfigService.h"

#include "framework/Session.h"
#include <framework/LoggingInstance.h>

//Needed for the "access" function.
#ifdef _WIN32
#include "platform/platform_windows.h"
#else
#endif

#include "framework/FileSystemObserver.h"

namespace Ember {

ServerService::ServerService(Session& session) :
		Service("Server"),
		mSession(session),
		mConnection(nullptr),
		mAccount(nullptr),
		mAvatar(nullptr),
		mOOGChat(nullptr),
		mNonConnectedState(std::make_unique<NonConnectedState>(*this, session)) {
	GotAccount.connect(sigc::mem_fun(*this, &ServerService::gotAccount));
	GotAvatar.connect(sigc::mem_fun(*this, &ServerService::gotAvatar));
	GotConnection.connect(sigc::mem_fun(*this, &ServerService::gotConnection));

	DestroyedAccount.connect(sigc::mem_fun(*this, &ServerService::destroyedAccount));
	DestroyedAvatar.connect(sigc::mem_fun(*this, &ServerService::destroyedAvatar));
}

ServerService::~ServerService() {
	auto directory = boost::filesystem::path(mLocalSocketPath).remove_filename().string();
	Ember::FileSystemObserver::getSingleton().remove_directory(directory);

	disconnect();
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

	boost::asio::local::stream_protocol::socket socket(mSession.m_io_service);
	boost::system::error_code ec;
	socket.connect(boost::asio::local::stream_protocol::endpoint(mLocalSocketPath.string()), ec);
	if (ec) {
		return false;
	} else {
		return socket.is_open();
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

void ServerService::setupLocalServerObservation(ConfigService& configService) {

	mLocalSocketPath = configService.getHomeDirectory(BaseDirType_DATA).remove_filename() / "cyphesis" / "tmp" / "cyphesis_cyphesis.sock";
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
