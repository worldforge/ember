//
// C++ Interface: LuaConnectors
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
// You should have received a copy ofthe GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBER_LUA_CONNECTOR_H
#define EMBER_LUA_CONNECTOR_H

#include "luaobject.h"

#include "Connectors.h"
#include "Connectors_impl.h"

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <string>
#include <tolua++.h>




namespace Ember
{

namespace Lua
{


/**
 @author Erik Hjortsberg

 Class used for connecting sigc signals to lua. Acts as an adapter for the signals, recieving them from the c++ environment and sending them to the lua environment.

 To use them in lua, use code like this:
 <code>

 function lua_foo()
 --do something here
 end

 --connect the lua method "lua_foo" to the event "EventFoo" of the object "emitter" and keeps a reference to the adapter in the variable "fooConnector"
 local emitter = EmberOgre.Emitter:new()
 fooConnector = EmberOgre.Connector:new_local(emitter.EventFoo):connect(lua_foo)



 </code>

 */
class Connector
{
public:

	/**
	 * @brief Gets the common lua state.
	 *
	 * @return The common lua state.
	 */
	static lua_State* getState();

	~Connector();

	/**
	 * @brief Connects to the named lua method.
	 *
	 * @param luaMethod The fully qualified name of the method.
	 * @param selfIndex An optional lua table index to be used as a "self" parameter.
	 *
	 * @return This instance.
	 */
	Connector* connect(const std::string& luaMethod, lua_Object selfIndex = LUA_NOREF);

	/**
	 * @brief Connects to the lua method.
	 *
	 * @param luaMethod The lua method.
	 * @param selfIndex An optional lua table index to be used as a "self" parameter.
	 *
	 * @return This instance.
	 */
	Connector* connect(lua_Object luaMethod, lua_Object selfIndex = LUA_NOREF);

	/**
	 * @brief Disconnects from the signal.
	 */
	void disconnect();

	/**
	 * @brief Sets a "self" reference which will be prepended to any lua call.
	 *
	 * @param selfIndex The lua index of the self reference.
	 */
	Connector* setSelf(int selfIndex);

	static Connector* createLuaConnector(sigc::signal<void>* signal);
	static Connector* createLuaConnector(sigc::signal<void>& signal);

	template <typename TAdapter0, typename TSignal>
	static Connector* createLuaConnector(TSignal* signal);
	template <typename TAdapter0, typename TSignal>
	static Connector* createLuaConnector(TSignal& signal);

	/**
	 * @brief Creates a new connector instance.
	 * @param signal The signal.
	 * @param adapter0 The first adapter.
	 * @param adapter1 The second adapter.
	 */
	template <typename TSignal, typename TAdapter0, typename TAdapter1>
	static Connector* createLuaConnector(TSignal& signal);

private:

	/**
	 * @brief The internal connector which will handle the actual lua binding.
	 */
	Ember::Lua::ConnectorBase* mConnector;

	Connector(Ember::Lua::ConnectorBase* connector);
	Connector(sigc::signal<void>& signal);


	/**
	 * @brief Checks that the signal submitted isn't null. If so, mConnector will be set to null and no connection will occur.
	 *
	 * @param signal A pointer to a signal.
	 * @return True if the supplied signal isn't null.
	 */
	bool checkSignalExistence(void* signal);

};


inline Connector* Connector::createLuaConnector(sigc::signal<void>* signal)
{
	return new Connector(*signal);
}

inline Connector* Connector::createLuaConnector(sigc::signal<void>& signal)
{
	return new Connector(signal);
}

template <typename TAdapter0, typename TSignal>
Connector* Connector::createLuaConnector(TSignal* signal)
{
	return new Connector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(*signal, TAdapter0()));
}

template <typename TAdapter0, typename TSignal>
Connector* Connector::createLuaConnector(TSignal& signal)
{
	return new Connector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, TAdapter0()));
}

/**
 * @brief Creates a new connector instance.
 * @param signal The signal.
 * @param adapter0 The first adapter.
 * @param adapter1 The second adapter.
 */
template <typename TSignal, typename TAdapter0, typename TAdapter1>
Connector* Connector::createLuaConnector(TSignal& signal)
{
	return new Connector(new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, TAdapter0(), TAdapter1()));
}

}

}

#endif
