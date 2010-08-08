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

#ifndef EMBEROGRE_LUACONNECTORS_CONNECTORS_IMPL_H_
#define EMBEROGRE_LUACONNECTORS_CONNECTORS_IMPL_H_

#include "LuaHelper.h"
#include "luaobject.h"
#include "framework/LoggingInstance.h"


#include <tolua++.h>
#include <string>
#include <sigc++/connection.h>

namespace Ember
{

namespace Lua
{


template <typename T>
bool RefValueAdapter<T>::pushValue(lua_State* state, const T& value)
{
	tolua_pushusertype(state, (void*)&value, mLuaTypeName.c_str());
	return true;
}

template <typename T>
bool PtrValueAdapter<T>::pushValue(lua_State* state, const T* value)
{
	tolua_pushusertype(state, (void*)value, mLuaTypeName.c_str());
	return true;
}

template <typename T>
bool NumberValueAdapter<T>::pushValue(lua_State* state, const T& value)
{
	tolua_pushnumber(state, value);
	return true;
}

template <typename TAdapter0, typename TAdapter1>
TemplatedConnectorBase<TAdapter0, TAdapter1>::TemplatedConnectorBase(const TAdapter0& adapter0, const TAdapter1& adapter1) :
	mAdapter0(adapter0), mAdapter1(adapter1)
{
}

template <typename TAdapter0, typename TAdapter1> template <typename Tvalue_type0, typename Tvalue_type1>
void TemplatedConnectorBase<TAdapter0, TAdapter1>::callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1)
{
	int numberOfArguments(0);
	lua_State* state = ConnectorBase::getState();
	int top = lua_gettop(state);
	try {
		numberOfArguments += resolveLuaFunction(state);

		if (mAdapter0.pushValue(state, t0)) {
			numberOfArguments++;
		}
		if (mAdapter1.pushValue(state, t1)) {
			numberOfArguments++;
		}

		callFunction(state, numberOfArguments);
		//		Treturn& returnValue(0);
		//return (Treturn)returnValueFromLua(state);
		//return returnValue;

	} catch (const std::exception& ex) {
		lua_settop(state, top);
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler '" << mLuaMethod << "'." << ex);
	} catch (...) {
		lua_settop(state, top);
		S_LOG_FAILURE("Unspecified error when executing: " << mLuaMethod );
	}
	/*	void* test(0);
	 return (Treturn)*test;*/
}










template <typename TReturn>
ConnectorZero<TReturn>::ConnectorZero(sigc::signal<TReturn>& signal) :
		TemplatedConnectorBase<>::TemplatedConnectorBase(EmptyValueAdapter(), EmptyValueAdapter()), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorZero::signal_recieve));
}

template <typename TReturn, typename TAdapter0, typename T0>
ConnectorOne<TReturn, TAdapter0, T0>::ConnectorOne(sigc::signal<TReturn, T0>& signal, const TAdapter0& adapter0) :
	TemplatedConnectorBase<TAdapter0>::TemplatedConnectorBase(adapter0, EmptyValueAdapter()), mSignal(signal)
{
	ConnectorBase::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorOne::signal_recieve));
}

template <typename TReturn, typename TAdapter0, typename TAdapter1, typename T0, typename T1>
ConnectorTwo<TReturn, TAdapter0, TAdapter1, T0, T1>::ConnectorTwo(sigc::signal<TReturn, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1) :
	TemplatedConnectorBase<TAdapter0, TAdapter1>::TemplatedConnectorBase(adapter0, adapter1), mSignal(signal)
{
	ConnectorBase::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorTwo::signal_recieve));
}

template <typename TReturn>
void ConnectorZero<TReturn>::signal_recieve()
{
	return callLuaMethod(Empty(), Empty());
}


template <typename TReturn, typename TAdapter0, typename T0>
TReturn ConnectorOne<TReturn, TAdapter0, T0>::signal_recieve(const T0& t0)
{
	callLuaMethod(t0, Empty());
}

template <typename TReturn, typename TAdapter0, typename TAdapter1, typename T0, typename T1>
TReturn ConnectorTwo<TReturn, TAdapter0, TAdapter1, T0, T1>::signal_recieve(const T0& t0, const T1& t1)
{
	callLuaMethod(t0, t1);
}
//template<typename Treturn>
//Treturn ConnectorBase::returnValueFromLua(lua_State* state)
//{
//	return static_cast<Treturn> (lua_touserdata(state, -1));
//}


}
}
#endif /* EMBEROGRE_LUACONNECTORS_CONNECTORS_IMPL_H_ */
