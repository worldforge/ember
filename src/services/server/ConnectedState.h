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

#ifndef CONNECTEDSTATE_H_
#define CONNECTEDSTATE_H_

#include "StateBase.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <Eris/Account.h>

namespace Eris
{
class Connection;
}

namespace Ember
{

class AccountAvailableState;

/**
 * @brief State for when a connection to a server has been made.
 *
 * This is a very transient state, as the next state, AccountAvailableState, is loaded immediately.
 */
class ConnectedState: public virtual StateBase<AccountAvailableState>, public ConsoleObject
{
public:
	ConnectedState(IState& parentState, Eris::Connection& connection);
	virtual ~ConnectedState();

	void runCommand(const std::string &, const std::string &);

	void disconnect();

	const ConsoleCommandWrapper DisConnect;

private:

	/**
	 * @brief Holds our connection to the server
	 */
	Eris::Connection& mConnection;

	bool disconnecting();


};

}

#endif /* CONNECTEDSTATE_H_ */
