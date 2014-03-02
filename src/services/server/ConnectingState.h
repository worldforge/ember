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

#ifndef CONNECTINGSTATE_H_
#define CONNECTINGSTATE_H_

#include "StateBase.h"

#include "Connection.h"
#include "ConnectedState.h"

namespace Eris
{
class Session;
}

namespace Ember
{

class DestroyChildStateEvent;

/**
 * @brief State for when a connection to a server is in progress.
 */
class ConnectingState: public virtual StateBase<ConnectedState>
{
public:

	/**
	 * @brief Ctor. for connecting to a remote host.
	 *
	 * @param parentState The parent state.
	 * @param host The hostname of the remote host.
	 * @param port The port on the remote host.
	 */
	ConnectingState(IState& parentState, Eris::Session& session, const std::string& host, short port);

	/**
	 * @brief Ctor. for connecting to a local socket.
	 * @param parent State The parent state.
	 * @param socket The path to the local socket.
	 */
	ConnectingState(IState& parentState, Eris::Session& session, const std::string& socket);
	virtual ~ConnectingState();

	bool connect();

	Eris::Connection& getConnection();

private:

	/**
	 * @brief Holds our connection to the server
	 */
	Connection mConnection;

	/**
	 * @brief Keeps track of whether the Disconnected of the Eris Connection has been signalled.
	 *
	 * Since that won't happen if we disconnect because of a failure we need to do that ourselves when shutting down.
	 */
	bool mHasSignalledDisconnected;

	void connected();

	void disconnected();

	void statusChanged(Eris::BaseConnection::Status status);

	void gotFailure(const std::string& msg);

};

}

#endif /* CONNECTINGSTATE_H_ */
