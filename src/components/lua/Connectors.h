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

#ifndef EMBEROGRE_LUACONNECTORS_CONNECTORS_H_
#define EMBEROGRE_LUACONNECTORS_CONNECTORS_H_

#include <string>
#include <sigc++/connection.h>

struct lua_State;

namespace Ember
{

namespace Lua
{

template <typename T>
const char* resolveLuaTypename();

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Marker class for empty values.
 */
struct Empty{};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Base class for value adapters which are mapped to a lua user type, and therefore needs to have the name of this type carried with them.
 */
class ValueAdapterBase
{
public:

	/**
	 * @brief Ctor.
	 * @param luaTypeName The lua name of the enveloped type. For example "Ember::Foo".
	 */
	ValueAdapterBase(const std::string& luaTypeName)
	: mLuaTypeName(luaTypeName)
	{}

protected:

	/**
	 * @brief The lua name of the enveloped type.
	 */
	const std::string mLuaTypeName;
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for reference types.
 */
template <typename T>
class RefValueAdapter : public ValueAdapterBase
{
public:
	typedef T &value_type;

	/**
	 * @brief Ctor.
	 * @param luaTypeName The lua name of the enveloped type. For example "Ember::Foo".
	 */
	RefValueAdapter(const std::string& luaTypeName) : ValueAdapterBase::ValueAdapterBase(luaTypeName)
	{}
	RefValueAdapter() : ValueAdapterBase::ValueAdapterBase(resolveLuaTypename<T>())
	{}
	/**
	 * @brief Tries to push a value onto the lua stack.
	 * @param state The lua state.
	 * @param value The value to push.
	 * @returns True if a value was pushed onto the stack.
	 */
	bool pushValue(lua_State* state, const T& value);

};


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for pointer types.
 */
template <typename T>
class PtrValueAdapter : public ValueAdapterBase
{
public:
	typedef T *value_type;

	/**
	 * @brief Ctor.
	 * @param luaTypeName The lua name of the enveloped type. For example "Ember::Foo".
	 */
	PtrValueAdapter(const std::string& luaTypeName) : ValueAdapterBase::ValueAdapterBase(luaTypeName)
	{}

	PtrValueAdapter() : ValueAdapterBase::ValueAdapterBase(resolveLuaTypename<T>())
	{}
	/**
	 * @brief Tries to push a value onto the lua stack.
	 * @param state The lua state.
	 * @param value The value to push.
	 * @returns True if a value was pushed onto the stack.
	 */
	bool pushValue(lua_State* state, const T* value);
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for number types.
 */
template <typename T>
class NumberValueAdapter
{
public:
	typedef T value_type;

	/**
	 * @brief Tries to push a value onto the lua stack.
	 * @param state The lua state.
	 * @param value The value to push.
	 * @returns True if a value was pushed onto the stack.
	 */
	bool pushValue(lua_State* state, const T& value);
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for strings.
 */
class StringValueAdapter
{
public:
	typedef const std::string& value_type;

	/**
	 * @brief Tries to push a value onto the lua stack.
	 * @param state The lua state.
	 * @param value The value to push.
	 * @returns True if a value was pushed onto the stack.
	 */
	bool pushValue(lua_State* state, value_type value);

};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for booleans.
 */
class BooleanValueAdapter
{
public:
	typedef bool& value_type;

	/**
	 * @brief Tries to push a value onto the lua stack.
	 * @param state The lua state.
	 * @param value The value to push.
	 * @returns True if a value was pushed onto the stack.
	 */
	bool pushValue(lua_State* state, const bool& value);

};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A value adapter for empty values.
 */
class EmptyValueAdapter
{
public:

	typedef Empty value_type;

	bool pushValue(lua_State* state, const Empty& value);
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Base class for connectors which mainly tie sigc signals to lua method calls.
 */
class ConnectorBase: public sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 */
	ConnectorBase();

	/**
	 * @brief Dtor.
	 */
	virtual ~ConnectorBase();

	/**
	 * @brief Connects to a specified lua method.
	 * @param luaMethod The fully qualified name of the method.
	 * A lookup of the lua method will be carried out when the signal being listened to is emitted.
	 */
	void connect(const std::string & luaMethod);

	/**
	 * @brief Connects to a specified lua method.
	 * @param luaMethod The lua method.
	 */
	void connect(int luaMethod);

	/**
	 * @brief Disconnects from the signal.
	 */
	void disconnect();

	/**
	 * @brief Sets a "self" index, which is a table which will be prepended as an argument to the call into Lua.
	 *
	 * @param selfIndex Index of a lua table.
	 */
	void setSelfIndex(int selfIndex);

	/**
	 * @brief Sets the common lua state.
	 *
	 * @param state The common lua state.
	 */
	static void setState(lua_State* state);

	/**
	 * @brief Gets the common lua state.
	 *
	 * @return The common lua state.
	 */
	static lua_State* getState();

	/**
	 * @brief Returns a value from the lua state.
	 * The type of value is specified in the template type.
	 * We'll use a simple function for this, rather than a system with adapter such as we use for pushing values onto Lua.
	 * The main reason is that in pretty much all cases void is returned by the signals we listen to, and so far the only other value returned is bool.
	 * @returns A value extracted from the lua environment.
	 */
	template<typename Treturn>
	static Treturn returnValueFromLua();

protected:

	/**
	 * @brief The lua method to call described as a fully qualified string.
	 * This value is only used if the lua object being called has been resolved and mLuaFunctionIndex set.
	 */
	std::string mLuaMethod;

	/**
	 * @brief The lua object to call.
	 * If the connection has been done against a string (see mLuaMethod) instead of a lua object this will initially be set to LUA_NOREF.
	 * When a call into lua is required a resolution of the named method will be carried out, and this value set to the resulting object.
	 */
	int mLuaFunctionIndex;

	/**
	 * @brief The connection to the signal being listened to.
	 */
	sigc::connection mConnection;

	/**
	 * @brief An optional "self" index, which is a table which will be prepended to any lua call.
	 *
	 * This is useful for providing object oriented features to Lua.
	 */
	int mLuaSelfIndex;

	/**
	 * @brief The global lua state.
	 */
	static lua_State* sState;

	/**
	 * @brief Pushes the lua method onto the stack.
	 * @param state The lua state.
	 */
	void pushNamedFunction(lua_State* state);

	/**
	 * @brief Resolves the lua function object to use, and pushed onto the stack.
	 * @param The lua state.
	 * @returns The number of arguments pushed onto the stack.
	 */
	int resolveLuaFunction(lua_State* state);

	/**
	 * @brief Calls the lua function.
	 * @param state The lua state.
	 * @param numberOfArguments The number of arguments on the stack.
	 */
	void callFunction(lua_State* state, int numberOfArguments);



};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Templated connector base class.
 * In this current version this only supports two adapters. This is because so far we haven't in Ember had any need for more, as no signal emitted uses more than two arguments.
 */
template <typename TAdapter0 = EmptyValueAdapter, typename TAdapter1 = EmptyValueAdapter>
class TemplatedConnectorBase: public ConnectorBase
{
public:
	/**
	 * @brief Ctor.
	 * @param adapter0 The first adapter.
	 * @param adapter1 The second adapter.
	 */
	TemplatedConnectorBase(const TAdapter0& adapter0, const TAdapter1& adapter1);

	/**
	 * @brief Calls the lua method with the supplied values.
	 * The value types should be inferred from the adapters used.
	 * @param t0 The first value.
	 * @param t1 The second value.
	 */
	template <typename Tvalue_type0, typename Tvalue_type1>
	void callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1);

protected:

	/**
	 * @brief The first adapter.
	 */
	TAdapter0 mAdapter0;

	/**
	 * @brief The second adapter.
	 */
	TAdapter1 mAdapter1;

};


/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A connector for a signal without any arguments.
 */
template <typename TReturn = void>
class ConnectorZero: public TemplatedConnectorBase<EmptyValueAdapter, EmptyValueAdapter>
{
public:

	/**
	 * @brief Ctor.
	 * @param signal The signal which should be listened to.
	 */
	ConnectorZero(sigc::signal<TReturn>& signal);

private:

	/**
	 * @brief The signal which should be listened to.
	 */
	sigc::signal<TReturn> mSignal;

	/**
	 * @brief Called when the signal is emitted.
	 */
	TReturn signal_receive();
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A connector for a signal with one argument.
 */
template <typename TReturn, typename TAdapter0, typename T0 = typename TAdapter0::value_type >
class ConnectorOne: public TemplatedConnectorBase<TAdapter0, EmptyValueAdapter>
{
public:

	/**
	 * @brief Ctor.
	 * @param signal The signal which should be listened to.
	 * @param adapter0 The first adapter to use.
	 */
	ConnectorOne(sigc::signal<TReturn, T0>& signal, const TAdapter0& adapter0);

private:

	/**
	 * @brief The signal which should be listened to.
	 */
	sigc::signal<TReturn, T0> mSignal;

	/**
	 * @brief Called when the signal is emitted.
	 * @param t0 The first argument.
	 */
	TReturn signal_receive(const T0& t0);
};

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A connector for a signal with two arguments.
 */
template <typename TReturn, typename TAdapter0, typename TAdapter1, typename T0 = typename TAdapter0::value_type, typename T1 = typename TAdapter1::value_type >
class ConnectorTwo: public TemplatedConnectorBase<TAdapter0, TAdapter1>
{
public:

	/**
	 * @brief Ctor.
	 * @param signal The signal which should be listened to.
	 * @param adapter0 The first adapter to use.
	 * @param adapter1 The second adapter to use.
	 */
	ConnectorTwo(sigc::signal<TReturn, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1);

private:

	/**
	 * @brief The signal which should be listened to.
	 */
	sigc::signal<TReturn, T0, T1> mSignal;

	/**
	 * @brief Called when the signal is emitted.
	 * @param t0 The first argument.
	 * @param t1 The second argument.
	 */
	TReturn signal_receive(const T0& t0, const T1& t1);

};
//template<typename Treturn>
//Treturn ConnectorBase::returnValueFromLua(lua_State* state)
//{
//	return static_cast<Treturn> (lua_touserdata(state, -1));
//}

}
}
#endif /* EMBEROGRE_LUACONNECTORS_CONNECTORS_H_ */
