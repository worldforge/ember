/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef SERVERSERVICESIGNALS_H_
#define SERVERSERVICESIGNALS_H_

#include <sigc++/signal.h>

#include <Atlas/Objects/ObjectsFwd.h>
#include <string>

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
class ServerServiceSignals {
public:

	virtual ~ServerServiceSignals() {};

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

};
}

#endif /* SERVERSERVICESIGNALS_H_ */
