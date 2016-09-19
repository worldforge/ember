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
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef ACCOUNTAVAILABLESTATE_H_
#define ACCOUNTAVAILABLESTATE_H_

#include "StateBase.h"
#include "LoggedInState.h"
#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
#include <Eris/Account.h>

namespace Ember
{

/**
 * @brief State for when a connection to a server has been made, but the user has not yet logged in.
 *
 * The user can either create a new account, or login using an existing one.
 */
class AccountAvailableState: public virtual StateBase<LoggedInState> , public ConsoleObject
{
public:
	AccountAvailableState(IState& parentState, Eris::Connection& connection);
	virtual ~AccountAvailableState();

	void runCommand(const std::string &, const std::string &);

	/**
	 * @brief Creates a new account.
	 *
	 * Format: create <username> <password> <real name>
	 */
	const Ember::ConsoleCommandWrapper CreateAcc;

	/**
	 * @brief Logs in to an existing account.
	 *
	 * Format: login <username> <password>
	 */
	const Ember::ConsoleCommandWrapper Login;

private:

	/**
	 * @brief Holds the account object we are connected with.
	 */
	Eris::Account mAccount;

	/**
	 * @brief Callback for login failures.
	 * @param msg Login failure message.
	 */
	void loginFailure(const std::string& msg);

	/**
	 * @brief Callback for login success.
	 */
	void loginSuccess();

	void logoutComplete(bool clean);

};

}

#endif /* ACCOUNTAVAILABLESTATE_H_ */
