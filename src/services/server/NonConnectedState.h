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

#ifndef NONCONNECTEDSTATE_H_
#define NONCONNECTEDSTATE_H_

#include "IState.h"
#include "NonConnectedAdapter.h"
#include "framework/ConsoleObject.h"


#include <Eris/TimedEventService.h>


#include <sigc++/connection.h>
#include <sigc++/trackable.h>

namespace Ember
{

class ConnectingState;

/**
 * @brief State for when no connection yet has been made.
 */
class NonConnectedState: public virtual sigc::trackable, public IState, public ConsoleObject
{
public:
	NonConnectedState(ServerServiceSignals& signals);
	virtual ~NonConnectedState();

	virtual void destroyChildState();

	virtual ServerServiceSignals& getSignals() const;

	virtual IState& getTopState();

	virtual void disconnect();

	virtual bool logout();

	virtual void takeCharacter(const std::string &id);

	virtual void takeTransferredCharacter(const Eris::TransferInfo& transferInfo);

	virtual bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties);

	virtual void transfer(const Eris::TransferInfo& transferInfo);

	virtual IServerAdapter& getServerAdapter();

	void runCommand(const std::string &, const std::string &);

	bool connect(const std::string& host, short port = 6767);

	const Ember::ConsoleCommandWrapper Connect;

private:

	ServerServiceSignals& mSignals;

	ConnectingState* mChildState;

	NonConnectedAdapter mAdapter;

	/**
	 * @brief Track the Disconnected connection, so to sever it when aborting and thus avoiding infinite loops.
	 */
	sigc::connection mDisconnectedConnection;

	void disconnected();

};

}

#endif /* NONCONNECTEDSTATE_H_ */
