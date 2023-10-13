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

#include "AccountAvailableState.h"
#include "ServerServiceSignals.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include "services/config/ConfigService.h"


namespace Ember {

AccountAvailableState::AccountAvailableState(IState& parentState, Eris::Connection& connection) :
		StateBase<LoggedInState>::StateBase(parentState),
		CreateAcc("create", this, "Create an account on the server."),
		Login("login", this, "Login to the connected server."),
		mAccount(connection) {
	mAccount.LoginFailure.connect(sigc::mem_fun(*this, &AccountAvailableState::loginFailure));
	mAccount.LoginSuccess.connect(sigc::mem_fun(*this, &AccountAvailableState::loginSuccess));
	mAccount.LogoutComplete.connect(sigc::mem_fun(*this, &AccountAvailableState::logoutComplete));
	mAccount.ErrorMessage.connect([&](const std::string& message) {
		ConsoleBackend::getSingleton().pushMessage("Error from server: " + message, "error");
	});
	getSignals().GotAccount.emit(&mAccount);
}

AccountAvailableState::~AccountAvailableState() {
	getSignals().DestroyedAccount.emit();
}

void AccountAvailableState::loginFailure(const std::string& msg) {
	std::ostringstream temp;

	temp << "Login Failure:" << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str(), "error");
	getSignals().LoginFailure.emit(&mAccount, msg);
}

void AccountAvailableState::loginSuccess() {
	S_LOG_INFO("Login Success.");
	ConsoleBackend::getSingleton().pushMessage("Login Successful", "important");
	setChildState(std::make_unique<LoggedInState>(*this, mAccount));
	getSignals().LoginSuccess.emit(&mAccount);
}

void AccountAvailableState::logoutComplete(bool clean) {
	S_LOG_INFO("Logout Complete cleanness=" << clean);
	ConsoleBackend::getSingleton().pushMessage("Logged out from server", "important");

	destroyChildState();
}

void AccountAvailableState::runCommand(const std::string& command, const std::string& args) {
	if (CreateAcc == command) {

		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string uname = tokeniser.nextToken();
		std::string password = tokeniser.nextToken();
		std::string realname = tokeniser.remainingTokens();

		std::string msg;
		msg = "Creating account: Name: [" + uname + "], Password: [" + password + "], Real Name: [" + realname + "]";

		try {
			mAccount.createAccount(uname, realname, password);
		} catch (const std::exception& except) {
			S_LOG_WARNING("Got error on account creation." << except);
			return;
		} catch (...) {
			S_LOG_WARNING("Got unknown error on account creation.");
			return;
		}

	} else if (Login == command) {

		// Split string into userid / password pair
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string userid = tokeniser.nextToken();
		std::string password = tokeniser.remainingTokens();

		mAccount.login(userid, password);

		std::string msg;
		msg = "Login: [" + userid + "," + password + "]";
		ConsoleBackend::getSingleton().pushMessage(msg, "info");
	}
}
}
