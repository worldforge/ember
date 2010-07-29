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

#include "ConnectingState.h"
#include "ConnectedState.h"
#include "ServerServiceSignals.h"
#include "ServerServiceConnectionListener.h"

#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"

#include <Eris/Connection.h>
#include <Eris/Exceptions.h>
#include <iostream>
#include <sstream>

namespace Ember
{
ConnectingState::ConnectingState(IState& parentState, const std::string& host, short port) :
	StateBase<ConnectedState>::StateBase(parentState), mConnection(std::string("Ember ") + VERSION, host, port, false, new ServerServiceConnectionListener(getSignals()))
{
	// Bind signals
	mConnection.Failure.connect(sigc::mem_fun(*this, &ConnectingState::gotFailure));
	mConnection.Connected.connect(sigc::mem_fun(*this, &ConnectingState::connected));
	mConnection.StatusChanged.connect(sigc::mem_fun(*this, &ConnectingState::statusChanged));
	//mConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));


}

ConnectingState::~ConnectingState()
{
}

void ConnectingState::destroyChildState()
{
	StateBase<ConnectedState>::destroyChildState();
	getParentState().destroyChildState();
}

void ConnectingState::connect()
{
	try {
		// If the connection fails here an errnumber is returned
		int errorno = mConnection.connect();
		if (errorno) {
			getParentState().destroyChildState();
		}
	} catch (const Eris::BaseException& except) {
		S_LOG_WARNING("Got error on connect:" << except._msg);
		getParentState().destroyChildState();
	} catch (...) {
		S_LOG_WARNING("Got unknown error on connect.");
		getParentState().destroyChildState();
	}
}

void ConnectingState::gotFailure(const std::string & msg)
{
	std::ostringstream temp;

	temp << "Got Server error: " << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str());

}

void ConnectingState::connected()
{
	S_LOG_INFO("Connected");
	getSignals().GotConnection.emit(&mConnection);

	setChildState(new ConnectedState(*this, mConnection));
	// Set up the player object
	// Init OOGChat controller
	//     mOOGChat = new OOGChat(mAccount);


	ConsoleBackend::getSingleton().pushMessage("Connected to Server");
}

void ConnectingState::statusChanged(Eris::BaseConnection::Status status)
{
	getSignals().EventStatusChanged.emit(status);
	S_LOG_INFO("Status Changed to: " << status);
}

}
