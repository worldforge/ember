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

#ifndef ENTEREDWORLDSTATE_H_
#define ENTEREDWORLDSTATE_H_

#include "StateBase.h"
#include "ConnectedAdapter.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <Atlas/Objects/ObjectsFwd.h>
#include <Atlas/Message/Element.h>
#include <wfmath/vector.h>
#include <wfmath/point.h>

namespace WFMath
{
class Quaternion;
}

namespace Eris
{
class Avatar;
class Account;
class View;
class Connection;
class Entity;
class TransferInfo;
}

namespace Ember
{

class TeleportRequestedState;

/**
 * @brief State for when the user has entered into the world. For most cases this is the desired state.
 */
class EnteredWorldState: public virtual StateBase<void>, public ConsoleObject
{
public:
	EnteredWorldState(IState& parentState, Eris::Avatar& avatar, Eris::Account& account);
	virtual ~EnteredWorldState();

	void runCommand(const std::string &, const std::string &);

	virtual IServerAdapter& getServerAdapter();

	virtual bool logout();

	const Ember::ConsoleCommandWrapper Say;
	const Ember::ConsoleCommandWrapper Emote;
	const Ember::ConsoleCommandWrapper Delete;
	const Ember::ConsoleCommandWrapper AdminTell;

private:

	/**
	 * @brief Holds the current avatar.
	 */
	Eris::Avatar& mAvatar;

	/**
	 * @brief Holds the account object we are connected with.
	 */
	Eris::Account& mAccount;

	ConnectedAdapter mAdapter;

	void avatar_transferRequest(const Eris::TransferInfo& transferInfo);

	void gotAvatarDeactivated(Eris::Avatar* avatar);

	Eris::Connection& getConnection() const;

	Eris::View& getView() const;

};

}

#endif /* ENTEREDWORLDSTATE_H_ */
