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

#include "NonConnectedState.h"
#include "ConnectingState.h"

#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/LoggingInstance.h"

#include <Eris/TransferInfo.h>

#include <algorithm>
#include <iostream>

namespace Ember
{

NonConnectedState::NonConnectedState(ServerServiceSignals& signals) :
	Connect("connect", this, "Connect to a server."), mSignals(signals), mChildState(0)
{
}

NonConnectedState::~NonConnectedState()
{
}

void NonConnectedState::destroyChildState()
{
	//Make sure to sever the connection, so that we don't end up in an infinite loop if something goes wrong when shutting down.
	mDisconnectedConnection.disconnect();
	delete mChildState;
	mChildState = 0;
}

ServerServiceSignals& NonConnectedState::getSignals() const
{
	return mSignals;
}

IState& NonConnectedState::getTopState()
{
	if (mChildState) {
		return mChildState->getTopState();
	}
	return *this;
}

bool NonConnectedState::connect(const std::string& host, short port)
{
	destroyChildState();
	mChildState = new ConnectingState(*this, host, port);
	if (!mChildState->connect()) {
		destroyChildState();
	} else {
		mDisconnectedConnection.disconnect();
		mDisconnectedConnection = mChildState->getConnection().Disconnected.connect(sigc::mem_fun(*this, &NonConnectedState::disconnected));
	}

	return mChildState != 0;
}

void NonConnectedState::disconnected()
{
	S_LOG_INFO("Disconnected");

	ConsoleBackend::getSingleton().pushMessage("Disconnected from server.", "important");

	destroyChildState();
}

void NonConnectedState::runCommand(const std::string &command, const std::string &args)
{
	// Connect command
	if (Connect == command) {
		// Split string into server / port pair
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string server = tokeniser.nextToken();
		std::string port = tokeniser.remainingTokens();
		std::string msg;
		msg = "Connecting to: [" + server + "]";
		ConsoleBackend::getSingleton().pushMessage(msg, "info");
		if (port == "")
			connect(server);
		else
			connect(server, (short)atoi(port.c_str()));

		// Disonnect command
	}
}

IServerAdapter& NonConnectedState::getServerAdapter()
{
	return mAdapter;
}

void NonConnectedState::disconnect()
{
}

bool NonConnectedState::logout()
{
	return false;
}

void NonConnectedState::takeCharacter(const std::string &id)
{
}

void NonConnectedState::takeTransferredCharacter(const Eris::TransferInfo& transferInfo)
{
}

bool NonConnectedState::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties)
{
	return false;
}

void NonConnectedState::transfer(const Eris::TransferInfo& transferInfo)
{
	//Start by disconnecting from current server, and reconnecting to new server.
	destroyChildState();
	connect(transferInfo.getHost(), transferInfo.getPort());
}

}
