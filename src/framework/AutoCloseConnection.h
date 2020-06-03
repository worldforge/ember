/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_AUTOCLOSECONNECTION_H
#define EMBER_AUTOCLOSECONNECTION_H

#include <sigc++/connection.h>

namespace Ember {
/**
 * Wraps a sigc::connection and disconnects it when being deleted. Supports move semantics.
 * This helps with managing connections through RAII.
 */
struct AutoCloseConnection {

	AutoCloseConnection() = default;

	explicit AutoCloseConnection(const sigc::connection& connection_) : connection(connection_) {
	}

	AutoCloseConnection(AutoCloseConnection&& rhs) noexcept:
			connection(rhs.connection), owned(rhs.owned) {
		rhs.owned = false;
	}

	~AutoCloseConnection() {
		if (owned && connection) {
			connection.disconnect();
		}
	}

	AutoCloseConnection& operator=(const sigc::connection& connection_) {
		if (owned && connection) {
			connection.disconnect();
		}
		this->connection = connection_;
		return *this;
	}


	sigc::connection connection;
	bool owned = true;

	void disconnect() {
		connection.disconnect();
	}

};

}

#endif //EMBER_AUTOCLOSECONNECTION_H
