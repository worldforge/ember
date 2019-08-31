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

//

#ifndef ENTITY_IMPORTER_H
#define ENTITY_IMPORTER_H

#include "EntityImporterBase.h"
#include <Atlas/Objects/RootOperation.h>
#include <boost/filesystem/path.hpp>

namespace Eris {
class Account;
}
namespace Ember {

/**
 * @brief Imports a previously exported entity.
 */
class EntityImporter : public EntityImporterBase {
public:
	explicit EntityImporter(Eris::Account& account);

	~EntityImporter() override;

	/**
	 * @brief Reads a directory and gets information on any entity dumps found.
	 *
	 * This is not recursive.
	 * @param directoryPath The path to the directory.
	 * @return Information on any entity dumps found.
	 */
	static std::vector<ShortInfo> getInfoFromDirectory(const boost::filesystem::path& directoryPath);

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
	 * @brief Loads from a file.
	 * @param filename The full path to the file.
	 * @return The top level object stored in the file.
	 */
	Atlas::Objects::Root loadFromFile(const std::string& filename) override;

	void operationResult(const Atlas::Objects::Operation::RootOperation& op);

};

}
#endif // ENTITY_IMPORTER_H
