//
// C++ Implementation: LuaConnectors
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Connector.h"

#include "Connectors.h"
#include "Connectors_impl.h"



namespace Ember
{

namespace Lua
{

Connector::~Connector()
{
	delete mConnector;
}

lua_State* Connector::getState()
{
	return Ember::Lua::ConnectorBase::getState();
}

Connector* Connector::connect(const std::string& luaMethod, lua_Object selfIndex)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect the lua method '" << luaMethod << "' to a non existent signal.");
	} else {
		setSelf(selfIndex);
		mConnector->connect(luaMethod);
	}
	return this;
}

Connector* Connector::connect(lua_Object luaMethod, lua_Object selfIndex)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect lua method to a non existent signal.");
	} else {
		setSelf(selfIndex);
		///we need to get the correct lua function
		int luaType = lua_type(getState(), -1);
		if (luaType == LUA_TFUNCTION) {
			int index = luaL_ref(getState(), LUA_REGISTRYINDEX);
			mConnector->connect(index);
		} else {
			S_LOG_WARNING("No valid lua function sent as argument to Connector::connect");
		}
	}
	return this;
}

void Connector::disconnect()
{
	if (mConnector) {
		mConnector->disconnect();
	}
}

Connector* Connector::setSelf(int selfIndex)
{
	if (mConnector) {
		if (selfIndex != LUA_NOREF) {
			///we need to get the correct lua table
			int luaType = lua_type(getState(), -1);
			int index = luaL_ref(getState(), LUA_REGISTRYINDEX);
			if (luaType == LUA_TTABLE) {
				mConnector->setSelfIndex(index);
			}
		}
	}
	return this;
}


bool Connector::checkSignalExistence(void* signal)
{
	if (!signal) {
		S_LOG_WARNING("Tried to connect lua to a non existent signal.");
		return false;
	}
	return true;

}

Connector::Connector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new Ember::Lua::ConnectorZero<void>(signal);
	}
}

Connector::Connector(Ember::Lua::ConnectorBase* connector)
{
	mConnector = connector;
}

Connector::Connector(const Connector& connector)
: mConnector(connector.mConnector)
{
	connector.mConnector = 0;
}


Connector Connector::createConnector(sigc::signal<void>* signal)
{
	return Connector(*signal);
}

Connector Connector::createConnector(sigc::signal<void>& signal)
{
	return Connector(signal);
}

}

}
