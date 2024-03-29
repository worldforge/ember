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

#include "ConnectedState.h"

#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"
#include "AssetsSyncState.h"

#include <Eris/Connection.h>

namespace Ember {

ConnectedState::ConnectedState(IState& parentState, Eris::Connection& connection) :
		StateBase<AssetsSyncState>::StateBase(parentState),
		DisConnect("disconnect", this, "Disconnect from the server."),
		mConnection(connection) {
	mConnection.Disconnecting.connect(sigc::mem_fun(*this, &ConnectedState::disconnecting));
	mConnection.Failure.connect(sigc::mem_fun(*this, &ConnectedState::gotFailure));
	setChildState(std::make_unique<AssetsSyncState>(*this, connection));
}

void ConnectedState::disconnect() {
	try {
		mConnection.disconnect();
	} catch (const std::exception& e) {
		S_LOG_WARNING("Got error on disconnect." << e);
	} catch (...) {
		S_LOG_WARNING("Got unknown error on disconnect");
	}
}

bool ConnectedState::disconnecting() {
	S_LOG_INFO("Disconnecting");
	destroyChildState();
	return true;
}

void ConnectedState::runCommand(const std::string& command, const std::string& args) {
	if (DisConnect == command) {
		disconnect();
	}
}

void ConnectedState::gotFailure(const std::string& msg) {
	//If we got a failure we should release the account directly.
	//The handling of deleting the connection and so on is handled by the NonConnectedState.
	if (getChildState()) {
		destroyChildState();
	}
}

}
