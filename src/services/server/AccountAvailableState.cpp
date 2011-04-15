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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AccountAvailableState.h"
#include "LoggedInState.h"
#include "ServerServiceSignals.h"
#include "TransferInfoStringSerializer.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"
#include "framework/LoggingInstance.h"

#include "services/config/ConfigService.h"

#include <Eris/Connection.h>
#include <Eris/TransferInfo.h>

#include <iostream>
#include <sstream>
#include <fstream>

namespace Ember
{

AccountAvailableState::AccountAvailableState(IState& parentState, Eris::Connection& connection) :
	StateBase<LoggedInState>::StateBase(parentState), CreateAcc("create", this, "Create an account on the server."), Login("login", this, "Login to the connected server."), mAccount(&connection)
{
	mAccount.LoginFailure.connect(sigc::mem_fun(*this, &AccountAvailableState::loginFailure));
	mAccount.LoginSuccess.connect(sigc::mem_fun(*this, &AccountAvailableState::loginSuccess));
	mAccount.LogoutComplete.connect(sigc::mem_fun(*this, &AccountAvailableState::logoutComplete));
	getSignals().GotAccount.emit(&mAccount);
	checkTransfer();
}

AccountAvailableState::~AccountAvailableState()
{
	getSignals().DestroyedAccount.emit();
}

void AccountAvailableState::checkTransfer()
{
	TransferInfoStringSerializer serializer;
	std::string teleportFilePath(EmberServices::getSingleton().getConfigService()->getHomeDirectory() + "/teleports");
	std::ifstream teleportsFile(teleportFilePath.c_str());
	TransferInfoStringSerializer::TransferInfoStore transferObjects;
	if (teleportsFile.good()) {
		serializer.deserialize(transferObjects, teleportsFile);
	}
	teleportsFile.close();

	for (TransferInfoStringSerializer::TransferInfoStore::const_iterator I = transferObjects.begin(); I != transferObjects.end(); ++I) {
		const Eris::TransferInfo& transferInfo(*I);
		if (transferInfo.getHost() == mAccount.getConnection()->getHost() && transferInfo.getPort() == mAccount.getConnection()->getPort()) {

		}
	}

}

void AccountAvailableState::loginFailure(const std::string& msg)
{
	std::ostringstream temp;

	temp << "Login Failure:" << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str());
	getSignals().LoginFailure.emit(&mAccount, msg);
}

void AccountAvailableState::loginSuccess()
{
	S_LOG_INFO("Login Success.");
	ConsoleBackend::getSingleton().pushMessage("Login Successful");
	setChildState(new LoggedInState(*this, mAccount));
	getSignals().LoginSuccess.emit(&mAccount);
}

void AccountAvailableState::logoutComplete(bool clean)
{
	S_LOG_INFO("Logout Complete cleanness=" << clean);
	ConsoleBackend::getSingleton().pushMessage("Logged out from server");
	destroyChildState();
}

void AccountAvailableState::runCommand(const std::string &command, const std::string &args)
{
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
		ConsoleBackend::getSingleton().pushMessage(msg);
	}
}
}
