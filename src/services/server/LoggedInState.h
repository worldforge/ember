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

#ifndef LOGGEDINSTATE_H_
#define LOGGEDINSTATE_H_

#include "StateBase.h"
#include "EnteredWorldState.h"

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"

#include <Atlas/Objects/ObjectsFwd.h>

namespace Eris
{
class Account;
class Avatar;
class TimedEvent;
}

namespace Ember
{

class TransferEvent;
class AvatarTransferInfo;

/**
 * @brief State for when the user is logged in to a server account, but has yet not taken a character in the world.
 */
class LoggedInState: public virtual StateBase<EnteredWorldState> , public ConsoleObject
{
public:
	LoggedInState(IState& parentState, Eris::Account& account);
	virtual ~LoggedInState();

	void runCommand(const std::string &, const std::string &);

	void takeTransferredCharacter(const Eris::TransferInfo& transferInfo);

	void takeCharacter(const std::string &id);

	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties);

	virtual bool logout();

	const Ember::ConsoleCommandWrapper Logout;
	const Ember::ConsoleCommandWrapper CreateChar;
	const Ember::ConsoleCommandWrapper TakeChar;
	const Ember::ConsoleCommandWrapper ListChars;

private:

	typedef std::vector<AvatarTransferInfo> AvatarTransferInfoStore;

	/**
	 * @brief Holds the account object we are connected with.
	 */
	Eris::Account& mAccount;

	/**
	 * @brief Handles any transfer operation.
	 *
	 * This needs to occur separate since it's required that it's implemented through Eris TimedEvent events.
	 */
	Eris::TimedEvent* mTransferEvent;

	/**
	 * @brief Any optional transfer infos attached to this server.
	 */
	AvatarTransferInfoStore mTransferInfos;

	void gotAvatarSuccess(Eris::Avatar* avatar);

	void gotAvatarDeactivated(Eris::Avatar* avatar);

	void gotCharacterInfo(const Atlas::Objects::Entity::RootEntity& characterInfo);

	void gotAllCharacters();

	/**
	 * @brief Listen to server transfer requests and write the transfer info data to persistent storage.
	 * @param transferInfo The transfer info data.
	 * @param avatar The current avatar.
	 */
	void avatar_transferRequest(const Eris::TransferInfo& transferInfo, const Eris::Avatar* avatar);

	/**
	 * @brief Checks if there are any transfer info instances available for this server.
	 *
	 * This is done by deserializing all transfer infos from the persistent storage (currently a file)
	 * and checking if any of them refer to the server which we're connected to.
	 *
	 * If so, the ServerServiceSignals::TransferInfoAvailable signal will be emitted.
	 */
	void checkTransfer();

	/**
	 * @brief Removes the supplied transfer info from the persistent storage.
	 * @param transferInfo The transfer info which should be removed.
	 */
	void removeTransferInfo(const AvatarTransferInfo& transferInfo);

};

}
#endif /* LOGGEDINSTATE_H_ */
