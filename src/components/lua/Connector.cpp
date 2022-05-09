//
// C++ Implementation: LuaConnectors
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Connector.h"
#include <memory>
#include <utility>

namespace Ember {

namespace Lua {

Connector::~Connector() = default;

lua_State* Connector::getState() {
	return ConnectorBase::getState();
}

std::shared_ptr<Connector> Connector::connect(lua_Object luaMethod, lua_Object selfIndex) {
	sol::object method(getState(), luaMethod);
	if (method.get_type() == sol::type::function) {
		sol::object self(getState(), selfIndex);
		connect(sol::function(method), self);
	}
	return this->shared_from_this();
}

std::shared_ptr<Connector> Connector::connect(sol::function luaMethod, sol::object selfIndex) {
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect lua method to a non existent signal.");
	} else {
		if (selfIndex.valid() && selfIndex.get_type() == sol::type::table) {
			mConnector->setSelf(sol::table(selfIndex));
		}
		mConnector->connect(std::move(luaMethod));
	}
	return this->shared_from_this();
}

void Connector::disconnect() {
	if (mConnector) {
		mConnector->disconnect();
		mConnector.reset();
	}
}

bool Connector::checkSignalExistence(void* signal) {
	if (!signal) {
		S_LOG_WARNING("Tried to connect lua to a non existent signal.");
		return false;
	}
	return true;

}

Connector::Connector(sigc::signal<void>& signal) {
	if (checkSignalExistence(&signal)) {
		mConnector = std::make_unique<ConnectorZero<void>>(signal);
	}
}

Connector::Connector(std::unique_ptr<ConnectorBase> connector)
		: mConnector(std::move(connector)) {
}

Connector::Connector(const Connector& connector)
		: mConnector(std::move(connector.mConnector)) {
}


Connector* Connector::createConnector(sigc::signal<void>* signal) {
	return new Connector(*signal);
}

Connector* Connector::createConnector(sigc::signal<void>& signal) {
	return new Connector(signal);
}

}

}
