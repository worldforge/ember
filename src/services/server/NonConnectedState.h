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

#ifndef NONCONNECTEDSTATE_H_
#define NONCONNECTEDSTATE_H_

#include "IState.h"
#include "framework/ConsoleObject.h"

#include <sigc++/connection.h>
#include <sigc++/trackable.h>

namespace Eris
{
struct Session;
}

namespace Ember
{

class ConnectingState;

/**
 * @brief State for when no connection yet has been made.
 */
class NonConnectedState: public virtual sigc::trackable, public IState, public ConsoleObject
{
public:
	NonConnectedState(ServerServiceSignals& signals, Eris::Session& session);

	~NonConnectedState() override = default;

	void destroyChildState() override;

	ServerServiceSignals& getSignals() const override;

	IState& getTopState() override;

	void disconnect() override;

	bool logout() override;

	void takeCharacter(const std::string &id) override;

	void takeTransferredCharacter(const Eris::TransferInfo& transferInfo) override;

	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties) override;

	void transfer(const Eris::TransferInfo& transferInfo) override;

	void runCommand(const std::string &, const std::string &) override;

	/**
	 * @brief Connects to a remote host.
	 * @param host The hostname of the remote host.
	 * @param port The port on the remote host.
	 * @return True if connection was successful.
	 */
	bool connect(const std::string& host, short port = 6767);

	/**
	 * @brief Connects to a local socket.
	 * @param socket The path to the local socket.
	 * @return True if connection was successful.
	 */
	bool connectLocal(const std::string& socket);

	const Ember::ConsoleCommandWrapper Connect;

private:

	ServerServiceSignals& mSignals;

	Eris::Session& mSession;

	ConnectingState* mChildState;

	/**
	 * @brief Track the Disconnected connection, so to sever it when aborting and thus avoiding infinite loops.
	 */
	sigc::connection mDisconnectedConnection;

	void disconnected();

};

}

#endif /* NONCONNECTEDSTATE_H_ */
