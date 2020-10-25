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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConnectingState.h"
#include "ServerServiceSignals.h"
#include "ServerServiceConnectionListener.h"

#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"
#include "Version.h"

#include <Eris/Exceptions.h>

namespace Ember {
ConnectingState::ConnectingState(IState& parentState, Eris::Session& session, const std::string& host, short port) :
		StateBase<ConnectedState>::StateBase(parentState),
		mConnection(session, "Ember " EMBER_VERSION, host, port, std::make_unique<ServerServiceConnectionListener>(getSignals())),
		mHasSignalledDisconnected(false) {
	// Bind signals
	mConnection.Connected.connect(sigc::mem_fun(*this, &ConnectingState::connected));
	mConnection.StatusChanged.connect(sigc::mem_fun(*this, &ConnectingState::statusChanged));
	mConnection.Disconnected.connect(sigc::mem_fun(*this, &ConnectingState::disconnected));
	mConnection.Failure.connect(sigc::mem_fun(*this, &ConnectingState::gotFailure));
	//mConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));
}

ConnectingState::ConnectingState(IState& parentState, Eris::Session& session, const std::string& socket) :
		StateBase<ConnectedState>::StateBase(parentState),
		mConnection(session, "Ember " EMBER_VERSION, socket, std::make_unique<ServerServiceConnectionListener>(getSignals())),
		mHasSignalledDisconnected(false) {
	// Bind signals
	mConnection.Connected.connect(sigc::mem_fun(*this, &ConnectingState::connected));
	mConnection.StatusChanged.connect(sigc::mem_fun(*this, &ConnectingState::statusChanged));
	mConnection.Disconnected.connect(sigc::mem_fun(*this, &ConnectingState::disconnected));
	mConnection.Failure.connect(sigc::mem_fun(*this, &ConnectingState::gotFailure));
	//mConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));
}

ConnectingState::~ConnectingState() {
	if (!mHasSignalledDisconnected) {
		//If we get a failure there's a great risk that the Disconnected signal isn't emitted. We'll therefore do it ourselves.
		mConnection.Disconnected.emit();
	}
}

bool ConnectingState::connect() {
	try {
		// If the connection fails here an errnumber is returned
		int errorno = mConnection.connect();
		if (errorno) {
			return false;
		}
	} catch (const Eris::BaseException& except) {
		S_LOG_WARNING("Got error on connect." << except);
		return false;
	} catch (...) {
		S_LOG_WARNING("Got unknown error on connect.");
		return false;
	}
	mHasSignalledDisconnected = false;
	return true;
}

void ConnectingState::connected() {
	S_LOG_INFO("Connected");
	getSignals().GotConnection.emit(&mConnection);

	setChildState(std::make_unique<ConnectedState>(*this, mConnection));
	// Set up the player object
	// Init OOGChat controller
	//     mOOGChat = new OOGChat(mAccount);


	ConsoleBackend::getSingleton().pushMessage("Connected to Server", "important");
}

void ConnectingState::statusChanged(Eris::BaseConnection::Status status) {
	getSignals().EventStatusChanged.emit(status);
	S_LOG_INFO("Status Changed to: " << status);
}

void ConnectingState::disconnected() {
	mHasSignalledDisconnected = true;
}

Eris::Connection& ConnectingState::getConnection() {
	return mConnection;
}

void ConnectingState::gotFailure(const std::string& msg) {
	std::ostringstream temp;

	temp << "Got Server error: " << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str(), "error");

	//Don't destroy ourselves here, since the connection will then be destroyed
	//(and the connection code which triggered this callback will do some stuff once this callback is done, hence we can't delete that)

//	mChildStateDeleter->
//
//	if (!mDeleteChildState) {
//		mDeleteChildState = new DestroyChildStateEvent(getParentState());
//		Eris::TimedEventService::instance()->registerEvent(mDeleteChildState);
//	}
}

}
