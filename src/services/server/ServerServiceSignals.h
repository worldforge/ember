/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#ifndef SERVERSERVICESIGNALS_H_
#define SERVERSERVICESIGNALS_H_

#include <sigc++/signal.h>

#include <Atlas/Objects/ObjectsFwd.h>
#include <Eris/BaseConnection.h>
#include <string>
#include <vector>

namespace Eris
{
class Avatar;
class Connection;
class View;
class Lobby;
class Account;
}

namespace Ember
{
class AvatarTransferInfo;
class ServerServiceSignals {
public:

	virtual ~ServerServiceSignals() = default;;

	sigc::signal<void, Eris::Avatar*> GotAvatar;
	sigc::signal<void, Eris::View*> GotView;
	sigc::signal<void, Eris::Connection*> GotConnection;
	sigc::signal<void, Eris::Account*> GotAccount;
	sigc::signal<void, Eris::Account*> LoginSuccess;
	sigc::signal<void, Eris::Account*, const std::string&> LoginFailure;
	sigc::signal<void, const Atlas::Objects::Entity::RootEntity&> GotCharacterInfo;
	sigc::signal<void, Eris::Account*> GotAllCharacters;

	/**
	 * @brief Emitted when the Account object has been destroyed.
	 */
	sigc::signal<void> DestroyedAccount;

	/**
	 * @brief Emitted when the Avatar instance has been destroyed.
	 */
	sigc::signal<void> DestroyedAvatar;

	/**
	 * @brief Emitted when the View instance has been destroyed.
	 */
	sigc::signal<void> DestroyedView;

	/**
	 * @brief Emitted when an object is about to be sent to the server.
	 */
	sigc::signal<void, const Atlas::Objects::Root&> EventSendingObject;

	/**
	 * @brief Emitted when an object has been received.
	 */
	sigc::signal<void, const Atlas::Objects::Root&> EventReceivedObject;

	/**
	 * @brief Emitted when the current connection status changes.
	 */
	sigc::signal<void, Eris::BaseConnection::Status> EventStatusChanged;

	/**
	 * @brief Emitted when there are transfer info objects available for the connected server.
	 */
	sigc::signal<void, const std::vector<AvatarTransferInfo>&> TransferInfoAvailable;

};
}

#endif /* SERVERSERVICESIGNALS_H_ */
