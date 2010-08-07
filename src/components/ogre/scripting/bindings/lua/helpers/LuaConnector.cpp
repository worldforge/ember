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

#include "LuaConnector.h"
#include "components/lua/LuaHelper.h"
#include "components/lua/luaobject.h"

#include "framework/Exception.h"
#include "services/EmberServices.h"
#include "framework/LoggingInstance.h"
#include "services/scripting/ScriptingService.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/EntityWorldPickListener.h"

#include <Eris/Task.h>

#include <CEGUIExceptions.h>

#include <Eris/ServerInfo.h>

using namespace Ember;

namespace EmberOgre
{

namespace LuaConnectors
{

class ValueAdapterBase
{
public:

	ValueAdapterBase(const std::string& luaTypeName)
	: mLuaTypeName(luaTypeName)
	{

	}
protected:
	std::string mLuaTypeName;
};

template <typename T>
class RefValueAdapter : public ValueAdapterBase
{
public:
	typedef T &value_type;

	RefValueAdapter(const std::string& luaTypeName) : ValueAdapterBase::ValueAdapterBase(luaTypeName)
	{}

	bool pushValue(const T& value)
	{
		tolua_pushusertype(EmberOgre::LuaConnector::getState(), (void*)&value, mLuaTypeName.c_str());
		return true;
	}

};

template <typename T>
class PtrValueAdapter : public ValueAdapterBase
{
public:
	typedef T *value_type;

	PtrValueAdapter(const std::string& luaTypeName) : ValueAdapterBase::ValueAdapterBase(luaTypeName)
	{}

	bool pushValue(const T* value)
	{
		tolua_pushusertype(EmberOgre::LuaConnector::getState(), (void*)value, mLuaTypeName.c_str());
		return true;
	}
};

template <typename T>
class NumberValueAdapter
{
public:
	typedef T value_type;

	bool pushValue(const T& value)
	{
		tolua_pushnumber(EmberOgre::LuaConnector::getState(), value);
		return true;
	}
};

class StringValueAdapter
{
public:
	typedef const std::string& value_type;

	bool pushValue(value_type value)
	{
		tolua_pushstring(EmberOgre::LuaConnector::getState(), value.c_str());
		return true;
	}
};

class EmptyValueAdapter
{
public:

	typedef Empty value_type;

	bool pushValue(const Empty& value)
	{
		return false;
	}
};



class ConnectorBase_: public sigc::trackable
{
public:
	ConnectorBase_();
	virtual ~ConnectorBase_();

	/**
	 * Connects to a specified lua method.
	 * @param luaMethod The fully qualified name of the method.
	 */
	void connect(const std::string & luaMethod);

	/**
	 * Connects to a specified lua method.
	 * @param luaMethod The lua method
	 */
	void connect(int luaMethod);

	/**
	 Disconnects from the signal.
	 */
	void disconnect();

	/**
	 * @brief Sets a "self" index, which is a table which will be prepended as an argument to the call into Lua.
	 *
	 * @param selfIndex Index of a lua table.
	 */
	void setSelfIndex(int selfIndex);

protected:

	/**
	 The lua method to call.
	 */
	std::string mLuaMethod;

	/**
	 pushes the lua method onto the stack
	 */
	void pushNamedFunction(lua_State* state);

	/**
	 After the lua method has been bound, we don't need to do any more lookups and can instead just use the function index, which is stored in this variable.
	 */
	int mLuaFunctionIndex;

	/**
	 The connection.
	 */
	sigc::connection mConnection;

	/**
	 * @brief An optional "self" index, which is a table which will be prepended to any lua call.
	 *
	 * This is useful for providing object oriented features to Lua.
	 */
	int mLuaSelfIndex;

};

template <typename TAdapter0 = EmptyValueAdapter, typename TAdapter1 = EmptyValueAdapter>
class TemplatedConnectorBase_: public ConnectorBase_
{
public:
	TemplatedConnectorBase_(const TAdapter0& adapter0, const TAdapter1& adapter1);

	template <typename Tvalue_type0, typename Tvalue_type1>
	void callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1);

protected:
	TAdapter0 mAdapter0;
	TAdapter1 mAdapter1;


};


ConnectorBase::ConnectorBase() :
	mLuaFunctionIndex(LUA_NOREF), mLuaSelfIndex(LUA_NOREF)
{
}

ConnectorBase::ConnectorBase(const LuaTypeStore& luaTypeNames) :
	mLuaTypeNames(luaTypeNames), mLuaFunctionIndex(LUA_NOREF), mLuaSelfIndex(LUA_NOREF)
{
}

ConnectorBase::~ConnectorBase()
{
	mConnection.disconnect();
}

void ConnectorBase::disconnect()
{
	mConnection.disconnect();
}

void ConnectorBase::connect(const std::string & luaMethod)
{
	mLuaMethod = luaMethod;
}

void ConnectorBase::connect(int luaMethod)
{
	mLuaFunctionIndex = luaMethod;
}

void ConnectorBase::pushNamedFunction(lua_State* state)
{
	Lua::LuaHelper::pushNamedFunction(state, mLuaMethod);
}

void ConnectorBase::setSelfIndex(int selfIndex)
{
	mLuaSelfIndex = selfIndex;
}


//template<> void ConnectorBase::callLuaMethod(std::string t0, std::string t1, Empty t2, Empty t3);
template<typename Treturn, typename T0, typename T1, typename T2, typename T3> Treturn ConnectorBase::callLuaMethod(T0 t0, T1 t1, T2 t2, T3 t3)
{
	int numberOfArguments = static_cast<int> (mLuaTypeNames.size());
	lua_State* state = EmberOgre::LuaConnector::getState();
	int top = lua_gettop(state);
	try {

		if (mLuaFunctionIndex == LUA_NOREF || Ember::EmberServices::getSingleton().getScriptingService()->getAlwaysLookup()) {
			pushNamedFunction(state);
			mLuaFunctionIndex = luaL_ref(state, LUA_REGISTRYINDEX);
		}

		///get the lua function
		lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaFunctionIndex);

		//Check if there's a "self" table specified. If so, prepend that as the first argument and increase the number of arguments counter.
		if (mLuaSelfIndex != LUA_NOREF) {
			lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaSelfIndex);
			numberOfArguments++;
		}

		LuaTypeStore::const_iterator I = mLuaTypeNames.begin();
		if (I != mLuaTypeNames.end())
			EmberOgre::LuaConnector::pushValue(t0, (*I++));
		if (I != mLuaTypeNames.end())
			EmberOgre::LuaConnector::pushValue(t1, (*I++));
		if (I != mLuaTypeNames.end())
			EmberOgre::LuaConnector::pushValue(t2, (*I++));
		if (I != mLuaTypeNames.end())
			EmberOgre::LuaConnector::pushValue(t3, (*I++));

		///push our error handling method before calling the code
		int error_index = lua_gettop(state) - numberOfArguments;
#if LUA51
		lua_pushcfunction(state, Ember::Lua::LuaHelper::luaErrorHandler);
#else
		lua_pushliteral(state, "_TRACEBACK");
		lua_rawget(state, LUA_GLOBALSINDEX); /* get traceback function */
#endif
		lua_insert(state, error_index);/* put it under chunk and args */

		luaPop pop(state, 1); // pops error handler on exit

		/// call it
		int error = lua_pcall(state, numberOfArguments, LUA_MULTRET, error_index);

		/// handle errors
		if (error) {
			const std::string& msg = lua_tostring(state,-1);
			lua_settop(state, top);
			// 			lua_pop(state,numberOfArguments);
			throw Ember::Exception(msg);
		}

		//		Treturn& returnValue(0);
		//return (Treturn)returnValueFromLua(state);
		//return returnValue;

	} catch (const CEGUI::String& str) {
		lua_settop(state, top);
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: " << mLuaMethod<<".\n" << str.c_str());
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

template<typename Treturn>
Treturn ConnectorBase::returnValueFromLua(lua_State* state)
{
	return static_cast<Treturn> (lua_touserdata(state, -1));
}


ConnectorBase_::ConnectorBase_() :
	mLuaFunctionIndex(LUA_NOREF), mLuaSelfIndex(LUA_NOREF)
{
}

ConnectorBase_::~ConnectorBase_()
{
	mConnection.disconnect();
}

void ConnectorBase_::disconnect()
{
	mConnection.disconnect();
}

void ConnectorBase_::connect(const std::string & luaMethod)
{
	mLuaMethod = luaMethod;
}

void ConnectorBase_::connect(int luaMethod)
{
	mLuaFunctionIndex = luaMethod;
}

void ConnectorBase_::pushNamedFunction(lua_State* state)
{
	Lua::LuaHelper::pushNamedFunction(state, mLuaMethod);
}

void ConnectorBase_::setSelfIndex(int selfIndex)
{
	mLuaSelfIndex = selfIndex;
}

template <typename TAdapter0, typename TAdapter1>
TemplatedConnectorBase_<TAdapter0, TAdapter1>::TemplatedConnectorBase_(const TAdapter0& adapter0, const TAdapter1& adapter1) :
	mAdapter0(adapter0), mAdapter1(adapter1)
{
}

template <typename TAdapter0, typename TAdapter1> template <typename Tvalue_type0, typename Tvalue_type1>
void TemplatedConnectorBase_<TAdapter0, TAdapter1>::callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1)
{
	int numberOfArguments(0);
	lua_State* state = EmberOgre::LuaConnector::getState();
	int top = lua_gettop(state);
	try {

		if (mLuaFunctionIndex == LUA_NOREF || Ember::EmberServices::getSingleton().getScriptingService()->getAlwaysLookup()) {
			pushNamedFunction(state);
			mLuaFunctionIndex = luaL_ref(state, LUA_REGISTRYINDEX);
		}

		///get the lua function
		lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaFunctionIndex);

		//Check if there's a "self" table specified. If so, prepend that as the first argument and increase the number of arguments counter.
		if (mLuaSelfIndex != LUA_NOREF) {
			lua_rawgeti(state, LUA_REGISTRYINDEX, mLuaSelfIndex);
			numberOfArguments++;
		}

		if (mAdapter0.pushValue(t0)) {
			numberOfArguments++;
		}
		if (mAdapter1.pushValue(t1)) {
			numberOfArguments++;
		}

		///push our error handling method before calling the code
		int error_index = lua_gettop(state) - numberOfArguments;
#if LUA51
		lua_pushcfunction(state, Ember::Lua::LuaHelper::luaErrorHandler);
#else
		lua_pushliteral(state, "_TRACEBACK");
		lua_rawget(state, LUA_GLOBALSINDEX); /* get traceback function */
#endif
		lua_insert(state, error_index);/* put it under chunk and args */

		luaPop pop(state, 1); // pops error handler on exit

		/// call it
		int error = lua_pcall(state, numberOfArguments, LUA_MULTRET, error_index);

		/// handle errors
		if (error) {
			const std::string& msg = lua_tostring(state,-1);
			lua_settop(state, top);
			// 			lua_pop(state,numberOfArguments);
			throw Ember::Exception(msg);
		}

		//		Treturn& returnValue(0);
		//return (Treturn)returnValueFromLua(state);
		//return returnValue;

	} catch (const CEGUI::String& str) {
		lua_settop(state, top);
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: " << mLuaMethod<<".\n" << str.c_str());
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







class ConnectorZero_: public TemplatedConnectorBase_<EmptyValueAdapter, EmptyValueAdapter>
{
public:
	ConnectorZero_(sigc::signal<void>& signal);

private:
	sigc::signal<void> mSignal;

	void signal_recieve();
};

template <typename TAdapter0, typename T0 = typename TAdapter0::value_type >
class ConnectorOne_: public TemplatedConnectorBase_<TAdapter0, EmptyValueAdapter>
{
public:
	ConnectorOne_(sigc::signal<void, T0>& signal, const TAdapter0& adapter0);

private:
	sigc::signal<void, T0> mSignal;

	void signal_recieve(const T0& t0);
};

template <typename TAdapter0, typename TAdapter1, typename T0 = typename TAdapter0::value_type, typename T1 = typename TAdapter1::value_type >
class ConnectorTwo_: public TemplatedConnectorBase_<TAdapter0, TAdapter1>
{
public:
	ConnectorTwo_(sigc::signal<void, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1);

private:
	sigc::signal<void, T0, T1> mSignal;

	void signal_recieve(T0 t0, T1 t1);
};


template<typename Treturn>
ConnectorZero<Treturn>::ConnectorZero(sigc::signal<Treturn>& signal) :
	ConnectorBase(), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorZero<Treturn>::signal_recieve));
}


ConnectorZero_::ConnectorZero_(sigc::signal<void>& signal) :
		TemplatedConnectorBase_<>::TemplatedConnectorBase_(EmptyValueAdapter(), EmptyValueAdapter()), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorZero_::signal_recieve));
}

template <typename TAdapter0, typename T0>
ConnectorOne_<TAdapter0, T0>::ConnectorOne_(sigc::signal<void, T0>& signal, const TAdapter0& adapter0) :
	TemplatedConnectorBase_<TAdapter0>::TemplatedConnectorBase_(adapter0, EmptyValueAdapter()), mSignal(signal)
{
	ConnectorBase_::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorOne_::signal_recieve));
}

template <typename TAdapter0, typename TAdapter1, typename T0, typename T1>
ConnectorTwo_<TAdapter0, TAdapter1, T0, T1>::ConnectorTwo_(sigc::signal<void, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1) :
	TemplatedConnectorBase_<TAdapter0, TAdapter1>::TemplatedConnectorBase_(adapter0, adapter1), mSignal(signal)
{
	ConnectorBase_::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorTwo_::signal_recieve));
}

template<typename Treturn, typename T0>
ConnectorOne<Treturn, T0>::ConnectorOne(sigc::signal<Treturn, T0>& signal, const LuaTypeStore& luaTypeNames) :
	ConnectorBase(luaTypeNames), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorOne<Treturn, T0>::signal_recieve));
}

// template <typename Treturn, typename T0>
// ConnectorOne<Treturn, T0>::ConnectorOne(SigC::Signal1<Treturn, T0>& signal, const LuaTypeStore& luaTypeNames) :
//  ConnectorBase(luaTypeNames), mSignal_old(signal)
// {
// 	mConnection = mSignal_old.connect(sigc::mem_fun(*this, &ConnectorOne<Treturn, T0>::signal_recieve));
// }




template<typename Treturn, typename T0, typename T1>
ConnectorTwo<Treturn, T0, T1>::ConnectorTwo(sigc::signal<Treturn, T0, T1>& signal, const LuaTypeStore& luaTypeNames) :
	ConnectorBase(luaTypeNames), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorTwo<Treturn, T0, T1>::signal_recieve));
}

template<typename Treturn, typename T0, typename T1, typename T2>
ConnectorThree<Treturn, T0, T1, T2>::ConnectorThree(sigc::signal<Treturn, T0, T1, T2>& signal, const LuaTypeStore& luaTypeNames) :
	ConnectorBase(luaTypeNames), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorThree<Treturn, T0, T1, T2>::signal_recieve));
}

template<typename Treturn, typename T0, typename T1, typename T2, typename T3>
ConnectorFour<Treturn, T0, T1, T2, T3>::ConnectorFour(sigc::signal<Treturn, T0, T1, T2, T3>& signal, const LuaTypeStore& luaTypeNames) :
	ConnectorBase(luaTypeNames), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorFour<Treturn, T0, T1, T2, T3>::signal_recieve));
}

template<typename Treturn>
Treturn ConnectorZero<Treturn>::signal_recieve()
{
	return callLuaMethod<Treturn, Empty, Empty, Empty, Empty> (Empty(), Empty(), Empty(), Empty());
}

void ConnectorZero_::signal_recieve()
{
	callLuaMethod(Empty(), Empty());
}


template <typename TAdapter0, typename T0>
void ConnectorOne_<TAdapter0, T0>::signal_recieve(const T0& t0)
{
	callLuaMethod(t0, Empty());
}

template <typename TAdapter0, typename TAdapter1, typename T0, typename T1>
void ConnectorTwo_<TAdapter0, TAdapter1, T0, T1>::signal_recieve(T0 t0, T1 t1)
{
	callLuaMethod(t0, t1);
}


template<typename Treturn, typename T0>
Treturn ConnectorOne<Treturn, T0>::signal_recieve(T0 t0)
{
	return callLuaMethod<Treturn, T0, Empty, Empty, Empty> (t0, Empty(), Empty(), Empty());
}

template<typename Treturn, typename T0, typename T1>
Treturn ConnectorTwo<Treturn, T0, T1>::signal_recieve(T0 t0, T1 t1)
{
	return callLuaMethod<Treturn, T0, T1, Empty, Empty> (t0, t1, Empty(), Empty());
}

template<typename Treturn, typename T0, typename T1, typename T2>
Treturn ConnectorThree<Treturn, T0, T1, T2>::signal_recieve(T0 t0, T1 t1, T2 t2)
{
	return callLuaMethod<Treturn, T0, T1, T2, Empty> (t0, t1, t2, Empty());
}

template<typename Treturn, typename T0, typename T1, typename T2, typename T3>
Treturn ConnectorFour<Treturn, T0, T1, T2, T3>::signal_recieve(T0 t0, T1 t1, T2 t2, T3 t3)
{
	return callLuaMethod<Treturn, T0, T1, T2, T3> (t0, t1, t2, t3);
}

}
;

template<typename T>
void LuaConnector::pushValue(T theValue, const std::string& luaTypename)
{
	//	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)&theValue, luaTypename.c_str());
	tolua_pushusertype(EmberOgre::LuaConnector::getState(), (void*)theValue, luaTypename.c_str());
}

void LuaConnector::pushValue(const Eris::ServerInfo& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const std::string& theValue, const std::string& luaTypename)
{
	tolua_pushstring(EmberOgre::LuaConnector::getState(), theValue.c_str());
}

void LuaConnector::pushValue(LuaConnectors::Empty theValue, const std::string& luaTypename)
{
}

void LuaConnector::pushValue(const float& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const long& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const unsigned long& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const unsigned int& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const int& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const EntityPickResult& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const MousePickerArgs& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const Input::MouseButton& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}
void LuaConnector::pushValue(const Input::InputMode& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const std::set<std::string>& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const Atlas::Message::Element& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const EmberOgre::MovementControllerMode::Mode& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}

void LuaConnector::pushValue(const EmberOgre::Terrain::TerrainPage& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}
void LuaConnector::pushValue(const EmberOgre::EmberEntity& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}
void LuaConnector::pushValue(const Atlas::Objects::Root& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const std::vector<EmberOgre::EntityPickResult>& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const EmberOgre::Authoring::EntityMover& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const Terrain::TerrainEditorOverlay& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const Terrain::TerrainManager& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const World& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}

void LuaConnector::pushValue(const EmberEntityFactory& theValue, const std::string& luaTypename)
{
	pushUserTypeValue(theValue, luaTypename);
}


template<typename T>
void LuaConnector::pushUserTypeValue(T& theValue, const std::string& luaTypename)
{
	tolua_pushusertype(EmberOgre::LuaConnector::getState(), (void*)&theValue, luaTypename.c_str());
}

LuaConnector::~LuaConnector()
{
	delete mConnector;
	delete mConnector_;
}

lua_State* LuaConnector::sState = 0;

void LuaConnector::setState(lua_State* state)
{
	sState = state;
}

lua_State* LuaConnector::getState()
{
	return sState;
}

LuaConnector* LuaConnector::connect(const std::string& luaMethod, lua_Object selfIndex)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect the lua method '" << luaMethod << "' to a non existent signal.");
	} else {
		setSelf(selfIndex);
		mConnector->connect(luaMethod);
	}
	if (!mConnector_) {
		S_LOG_WARNING("Tried to connect the lua method '" << luaMethod << "' to a non existent signal.");
	} else {
		setSelf(selfIndex);
		mConnector_->connect(luaMethod);
	}
	return this;
}

LuaConnector* LuaConnector::connect(lua_Object luaMethod, lua_Object selfIndex)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect lua method to a non existent signal.");
	} else {
		setSelf(selfIndex);
		///we need to get the correct lua function
		int luaType = lua_type(sState, -1);
		if (luaType == LUA_TFUNCTION) {
			int index = luaL_ref(sState, LUA_REGISTRYINDEX);
			mConnector->connect(index);
		} else {
			S_LOG_WARNING("No valid lua function sent as argument to LuaConnector::connect");
		}
	}
	if (!mConnector_) {
		S_LOG_WARNING("Tried to connect lua method to a non existent signal.");
	} else {
		setSelf(selfIndex);
		///we need to get the correct lua function
		int luaType = lua_type(sState, -1);
		if (luaType == LUA_TFUNCTION) {
			int index = luaL_ref(sState, LUA_REGISTRYINDEX);
			mConnector_->connect(index);
		} else {
			S_LOG_WARNING("No valid lua function sent as argument to LuaConnector::connect");
		}
	}
	return this;
}

void LuaConnector::disconnect()
{
	if (mConnector) {
		mConnector->disconnect();
	}
	if (mConnector_) {
		mConnector_->disconnect();
	}
}

LuaConnector* LuaConnector::setSelf(int selfIndex)
{
	if (mConnector) {
		if (selfIndex != LUA_NOREF) {
			///we need to get the correct lua table
			int luaType = lua_type(sState, -1);
			int index = luaL_ref(sState, LUA_REGISTRYINDEX);
			if (luaType == LUA_TTABLE) {
				mConnector->setSelfIndex(index);
			}
		}
	}
	if (mConnector_) {
		if (selfIndex != LUA_NOREF) {
			///we need to get the correct lua table
			int luaType = lua_type(sState, -1);
			int index = luaL_ref(sState, LUA_REGISTRYINDEX);
			if (luaType == LUA_TTABLE) {
				mConnector_->setSelfIndex(index);
			}
		}
	}
	return this;
}


bool LuaConnector::checkSignalExistence(void* signal)
{
	mConnector = 0;
	mConnector_ = 0;
	if (!signal) {
		S_LOG_WARNING("Tried to connect lua to a non existent signal.");
		return false;
	}
	return true;

}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector_ = new LuaConnectors::ConnectorZero_(signal);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, EmberEntity*> & signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("string");
		luaTypes.push_back("EmberOgre::EmberEntity");
		mConnector = new LuaConnectors::ConnectorTwo<void, const std::string&, EmberEntity*>(signal, luaTypes);
	}
}

template <typename TSignal, typename TAdapter0>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter)
{
	if (checkSignalExistence(&signal)) {
		mConnector_= new LuaConnectors::ConnectorOne_<TAdapter0, typename TAdapter0::value_type>(signal, adapter);
	} else {
		mConnector_ = 0;
	}
}

template <typename TSignal, typename TAdapter0, typename TAdapter1>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter0, const TAdapter1& adapter1)
{
	if (checkSignalExistence(&signal)) {
		mConnector_ = new LuaConnectors::ConnectorTwo_<TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, adapter0, adapter1);
	} else {
		mConnector_ = 0;
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::Connection*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Eris::Connection>("Eris::Connection"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Eris::ServerInfo&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const Eris::ServerInfo>("Eris::ServerInfo"));
}

LuaConnector::LuaConnector(sigc::signal<void, float>& signal)
{
	createConnector(signal, LuaConnectors::NumberValueAdapter<float>());
}

LuaConnector::LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const EntityPickResult>("EmberOgre::EntityPickResult"), LuaConnectors::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::vector<EmberOgre::EntityPickResult>&, const MousePickerArgs&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const std::vector<EmberOgre::EntityPickResult> >("std::vector<EmberOgre::EntityPickResult>"), LuaConnectors::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, Input::MouseButton, Input::InputMode>& signal)
{
	createConnector(signal, LuaConnectors::NumberValueAdapter<Input::MouseButton>(), LuaConnectors::NumberValueAdapter<Input::InputMode>());
}

LuaConnector::LuaConnector(sigc::signal<void, Input::InputMode>& signal)
{
	createConnector(signal, LuaConnectors::NumberValueAdapter<Input::InputMode>());
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntityFactory&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<EmberEntityFactory>("EmberOgre::EmberEntityFactory"));
}
LuaConnector::LuaConnector(sigc::signal<void, Jesus*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Jesus>("EmberOgre::Jesus"));
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&, EmberOgre::Authoring::EntityMover&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<EmberEntity>("EmberOgre::EmberEntity"), LuaConnectors::RefValueAdapter<EmberOgre::Authoring::EntityMover>("EmberOgre::Authoring::EntityMover"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&>& signal)
{
	createConnector(signal, LuaConnectors::StringValueAdapter());
}

LuaConnector::LuaConnector(sigc::signal<bool, const std::string&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("string");
		mConnector = new LuaConnectors::ConnectorOne<bool, const std::string&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal)
{
	createConnector(signal, LuaConnectors::StringValueAdapter(), LuaConnectors::StringValueAdapter());
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::BasePointUserObject*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Terrain::BasePointUserObject>("EmberOgre::Terrain::BasePointUserObject"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainEditAction*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Terrain::TerrainEditAction>("EmberOgre::Terrain::TerrainEditAction"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainEditorOverlay&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<Terrain::TerrainEditorOverlay>("EmberOgre::Terrain::TerrainEditorOverlay"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainManager&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<Terrain::TerrainManager>("EmberOgre::Terrain::TerrainManager"));
}


LuaConnector::LuaConnector(sigc::signal<void, Eris::Task*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Eris::Task>("Eris::Task"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::set<std::string>&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const std::set<std::string>&>("std::set<std::string>"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Gui::EntityIcon*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<EmberOgre::Gui::EntityIcon>("EmberOgre::Gui::EntityIcon"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Message::Element&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const Atlas::Message::Element>("Atlas::Message::Element"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::MovementControllerMode::Mode>& signal)
{
	createConnector(signal, LuaConnectors::NumberValueAdapter<EmberOgre::MovementControllerMode::Mode>());
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Terrain::TerrainPage&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<EmberOgre::Terrain::TerrainPage>("EmberOgre::Terrain::TerrainPage"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Objects::Root&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<const Atlas::Objects::Root>("Atlas::Objects::Root"));
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::Avatar*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Eris::Avatar>("Eris::Avatar"));
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::View*>& signal)
{
	createConnector(signal, LuaConnectors::PtrValueAdapter<Eris::View>("Eris::View"));
}

LuaConnector::LuaConnector(sigc::signal<void, World&>& signal)
{
	createConnector(signal, LuaConnectors::RefValueAdapter<World>("EmberOgre::World"));
}




}
;
