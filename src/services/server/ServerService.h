/*
 Copyright (C) 2002  Martin Pollard (Xmp)

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

#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include "ServerServiceSignals.h"

#include "framework/Service.h"

#include <wfmath/vector.h>
#include <wfmath/point.h>
#include <wfmath/quaternion.h>

#include <Atlas/Message/Element.h>


#include <boost/filesystem/path.hpp>
#include <memory>

namespace Eris {
class Avatar;

class Connection;

class View;

class Lobby;

class Account;

class Entity;

class TransferInfo;

struct Session;
}

namespace Ember {

class ConfigService;

class OOGChat;

class NonConnectedState;

/**
 * @brief Ember Server Service
 *
 * Handles most interaction with the server.
 *
 * @author Martin Pollard (Xmp)
 *
 * @see Ember::Service
 * @see Ember::MetaserverService
 * @see Ember::ConsoleObject
 */
class ServerService : public Service, public ServerServiceSignals {
public:

	/**
	 * @brief Ctor.
	 */
	explicit ServerService(Eris::Session& session);

	/**
	 * @brief Dtor.
	 */
	~ServerService() override;

	void setupLocalServerObservation(ConfigService& configService);

	/**
	 * @brief Connects to a remote host.
	 * @param host The hostname of the remote host.
	 * @param port The port on the remote host.
	 * @return True if connection was successful.
	 */
	bool connect(const std::string& host, short port = 6767);

	/**
	 * @brief Connects to a local socket.
	 * @return True if connection was successful.
	 */
	bool connectLocal();

	/**
	 * @brief Checks if there's a local socket available.
	 * @return True if a local socket exists.
	 */
	bool hasLocalSocket();

	void disconnect();

	/**
	 * @brief Takes a transferred character.
	 * @param transferInfo Information needed about the transfer.
	 */
	void takeTransferredCharacter(const Eris::TransferInfo& transferInfo);

	/**
	 * @brief Logs out the user.
	 *
	 * If the user has an avatar, that will be logged out. If the user doesn't have an avatar yet but is logged in through the account, the account will be logged out.
	 */
	bool logout();

	Eris::Avatar* getAvatar() const;

	/**
	 * @brief Gets the underlying connection.
	 * @returns A connection instance, or null if we're currently not connected
	 */
	Eris::Connection* getConnection() const;

	/**
	 * @brief Gets the used account, if available.
	 * @return An account, or null if none has been created.
	 */
	Eris::Account* getAccount() const;

	sigc::signal<void> EventLocalSocketChanged;

private:

	void gotConnection(Eris::Connection* connection);

	void gotAvatar(Eris::Avatar* avatar);

	void gotAccount(Eris::Account* account);

	void destroyedAccount();

	void destroyedAvatar();

	Eris::Session& mSession;

	Eris::Connection* mConnection;
	Eris::Account* mAccount;
	Eris::Avatar* mAvatar;

	/**
	 * @brief Contains the class that controls Out of Game Chat
	 */
	OOGChat* mOOGChat;

	std::unique_ptr<NonConnectedState> mNonConnectedState;

	boost::filesystem::path mLocalSocketPath;

};

} // namespace Ember

#endif
