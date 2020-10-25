//
// Copyright (C) 2009 Alistair Riddoch
// Copyright (C) 2012 Erik Ogenvik
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef EMBER_ENTITYEXPORTER_H
#define EMBER_ENTITYEXPORTER_H

#include "EntityExporterBase.h"

#include <unordered_map>

namespace Eris
{
class Account;
}

namespace Ember
{

/**
 * @author Erik Ogenvik
 *
 * @brief Ember specific subclass of EntityExporter.
 *
 */
class EntityExporter: public EntityExporterBase
{
public:
	/**
	 * @brief Ctor.
	 * @param account The account connected to the server. Must be an admin account.
	 */
	explicit EntityExporter(Eris::Account& account);

	/**
	 * @brief Dtor.
	 */
	~EntityExporter() override;

protected:


	Eris::Account& mAccount;

	std::unordered_map<long int, CallbackFunction> mCallbacks;

	/**
	 * @brief Create a new unique serial number for operations.
	 * @return A new serial number.
	 */
	long int newSerialNumber() override;

	/**
	 * @brief Send an object to the server.
	 * @param op An object which is to be sent.
	 */
	void send(const Atlas::Objects::Operation::RootOperation& op) override;

	/**
	 * @brief Send an object to the server and await a response.
	 * @param op An object which is to be sent.
	 * @param callback A callback, called when a response is received.
	 */
	void sendAndAwaitResponse(const Atlas::Objects::Operation::RootOperation& op, CallbackFunction& callback) override;

	/**
	 * @brief Creates a multi line formatter.
	 * @param s A stream.
	 * @param b A bridge.
	 * @return A formatter instance. Ownership is transferred.
	 */
	std::unique_ptr<Atlas::Formatter> createMultiLineFormatter(std::iostream& s, Atlas::Bridge& b) override;

	/**
	 * @brief Fills the supplied map with server meta data.
	 * @param serverMap An empty map.
	 */
	void fillWithServerData(Atlas::Message::MapType& serverMap) override;

	void operationResult(const Atlas::Objects::Operation::RootOperation& op);

	/**
	 * @brief Gets the current time as a unit timestamp string.
	 * @return A time string.
	 */
	static std::string getCurrentTime();


};

}
#endif
