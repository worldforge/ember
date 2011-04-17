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
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"

#include <Atlas/Objects/ObjectsFwd.h>

namespace Eris
{
class Account;
class Avatar;
}

namespace Ember
{

class TransferEvent;
class EnteredWorldState;

/**
 * @brief State for when the user is logged in to a server account, but has yet not taken a character in the world.
 */
class LoggedInState: public virtual StateBase<EnteredWorldState> , public ConsoleObject
{
public:
	LoggedInState(IState& parentState, Eris::Account& account);
	virtual ~LoggedInState();

	void runCommand(const std::string &, const std::string &);

	void takeCharacter(const std::string &id);

	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName);

	virtual bool logout();

	const Ember::ConsoleCommandWrapper Logout;
	const Ember::ConsoleCommandWrapper CreateChar;
	const Ember::ConsoleCommandWrapper TakeChar;
	const Ember::ConsoleCommandWrapper ListChars;

private:

	/**
	 * @brief Holds the account object we are connected with.
	 */
	Eris::Account& mAccount;

	TransferEvent* mTransferEvent;

	void gotAvatarSuccess(Eris::Avatar* avatar);

	void gotAvatarDeactivated(Eris::Avatar* avatar);

	void gotCharacterInfo(const Atlas::Objects::Entity::RootEntity& characterInfo);

	void gotAllCharacters();

	void avatar_transferRequest(const Eris::TransferInfo& transferInfo);

	void checkTransfer();
};

}
#endif /* LOGGEDINSTATE_H_ */
