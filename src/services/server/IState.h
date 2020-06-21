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

#ifndef ISTATE_H_
#define ISTATE_H_

#include <Atlas/Message/Element.h>
#include <string>

namespace Eris {
class TransferInfo;
}

namespace Ember {

class ServerServiceSignals;

/**
 * @brief Represents one state in the server connection process, from a connection is created, to the user has entered into the world (and beyond, when the user might be teleporting to another server).
 *
 * Each state can have a child state, which represents the next possible connection state (for example going from "connecting" to "connected"). When a state is destroyed it must also destroy its child state.
 */
struct IState
{

	/**
	 * @brief Dtor.
	 * When the state is destroyed, any child state needs to be destroyed as well.
	 */
	virtual ~IState() = default;

	/**
	 * @brief Destroys any child state.
	 */
	virtual void destroyChildState() = 0;

	/**
	 * @brief Gets the server service signals.
	 * @return The server service signals.
	 */
	virtual ServerServiceSignals& getSignals() const = 0;

	/**
	 * @brief Gets the top state in the state hierarchy.
	 * @return The top state.
	 */
	virtual IState& getTopState() = 0;

	/**
	 * @brief Disconnects from the server.
	 */
	virtual void disconnect() = 0;

	/**
	 * @brief Logs out the user.
	 *
	 * If the user has an avatar, that will be logged out. If the user doesn't have an avatar yet but is logged in through the account, the account will be logged out.
	 */
	virtual bool logout() = 0;


	/**
	 * @brief Takes a transferred character.
	 * @param transferInfo Information needed about the transfer.
	 */
	virtual void takeTransferredCharacter(const Eris::TransferInfo& transferInfo) = 0;

	/**
	 * @brief Transfers the user to another server, to claim a character on it.
	 * @param transferInfo Information needed about the transfer.
	 */
	virtual void transfer(const Eris::TransferInfo& transferInfo) = 0;


};

}
#endif /* ISTATE_H_ */
