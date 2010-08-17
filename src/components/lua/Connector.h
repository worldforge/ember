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

	Connector(const Connector& connector);

	/**
	 * @brief Ctor.
	 * @param signal The signal to connect to.
	 */
	Connector(sigc::signal<void>& signal);

	/**
	 * @brief Ctor.
	 * @param signal The signal to connect to.
	 */
	template <typename TAdapter0, typename TSignal>
	Connector(TSignal& signal);

	template <typename TAdapter0, typename TSignal>
	Connector(TSignal& signal, TAdapter0& adapter0);

	/**
	 * @brief Ctor.
	 * @param signal The signal to connect to.
	 */
	template <typename TAdapter0, typename TAdapter1, typename TSignal>
	Connector(TSignal& signal);
	template <typename TAdapter0, typename TAdapter1, typename TSignal>
	Connector(TSignal& signal, TAdapter0& adapter0, TAdapter1& adapter1);

	/**
	 * @brief Dtor.
	 * The internal connector will be deleted, which will disconnect any connection.
	 */
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

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	static Connector createConnector(sigc::signal<void>* signal);

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	static Connector createConnector(sigc::signal<void>& signal);

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	template <typename TAdapter0, typename TSignal>
	static Connector createConnector(TSignal* signal);

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	template <typename TAdapter0, typename TSignal>
	static Connector createConnector(TSignal& signal);

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	template <typename TAdapter0, typename TAdapter1, typename TSignal>
	static Connector createConnector(TSignal* signal);

	/**
	 * @brief Creates a new connector.
	 * @param signal The signal to connect to.
	 * @returns A new connector instance.
	 */
	template <typename TAdapter0, typename TAdapter1, typename TSignal>
	static Connector createConnector(TSignal& signal);

private:

	/**
	 * @brief The internal connector which will handle the actual lua binding.
	 * @note This is mutable so that it can be set to null when the copy constructor is invoked.
	 */
	mutable Ember::Lua::ConnectorBase* mConnector;

	/**
	 * @brief Ctor.
	 * @param connector The underlying connector instance.
	 */
	Connector(Ember::Lua::ConnectorBase* connector);


	/**
	 * @brief Checks that the signal submitted isn't null. If so, mConnector will be set to null and no connection will occur.
	 *
	 * @param signal A pointer to a signal.
	 * @return True if the supplied signal isn't null.
	 */
	bool checkSignalExistence(void* signal);

};

template <typename TAdapter0, typename TSignal>
class ConnectorOne_ : public Connector
{
public:
	ConnectorOne_(TSignal& signal)
	: Connector::Connector(signal, TAdapter0())
	{

	}
};

template <typename TAdapter0, typename TAdapter1, typename TSignal>
class ConnectorTwo_ : public Connector
{
public:
	ConnectorTwo_(TSignal& signal)
	: Connector::Connector(signal, TAdapter0(), TAdapter1())
	{

	}
};

template <typename TSignal, typename TAdapter0, typename TAdapter1 = EmptyValueAdapter>
class Connector_ : public Connector
{
public:
	Connector_(TSignal& signal)
	: Connector::Connector(signal, TAdapter0(), TAdapter1())
	{

	}
};

template <typename TAdapter0, typename TSignal>
Connector::Connector(TSignal& signal)
: mConnector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, TAdapter0()))
{
}

template <typename TAdapter0, typename TSignal>
Connector::Connector(TSignal& signal, TAdapter0& adapter0)
: mConnector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, adapter0))
{
}

template <typename TAdapter0, typename TAdapter1, typename TSignal>
Connector::Connector(TSignal& signal)
: mConnector(new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, TAdapter0(), TAdapter1()))
{
}

template <typename TAdapter0, typename TAdapter1, typename TSignal>
Connector::Connector(TSignal& signal, TAdapter0& adapter0, TAdapter1& adapter1)
: mConnector(new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, adapter0, adapter1))
{
}


template <typename TAdapter0, typename TSignal>
Connector Connector::createConnector(TSignal* signal)
{
	return Connector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(*signal, TAdapter0()));
}

template <typename TAdapter0, typename TSignal>
Connector Connector::createConnector(TSignal& signal)
{
	return Connector(new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, TAdapter0()));
}

template <typename TAdapter0, typename TAdapter1, typename TSignal>
Connector Connector::createConnector(TSignal* signal)
{
	return Connector(new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(*signal, TAdapter0(), TAdapter1()));
}

template <typename TAdapter0, typename TAdapter1, typename TSignal>
Connector Connector::createConnector(TSignal& signal)
{
	return Connector(new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, TAdapter0(), TAdapter1()));
}

}

}

#endif
