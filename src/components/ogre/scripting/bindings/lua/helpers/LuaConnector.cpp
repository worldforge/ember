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
	const std::string mLuaTypeName;
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



class ConnectorBase: public sigc::trackable
{
public:
	ConnectorBase();
	virtual ~ConnectorBase();

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
class TemplatedConnectorBase: public ConnectorBase
{
public:
	TemplatedConnectorBase(const TAdapter0& adapter0, const TAdapter1& adapter1);

	template <typename Tvalue_type0, typename Tvalue_type1>
	void callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1);

protected:
	TAdapter0 mAdapter0;
	TAdapter1 mAdapter1;


};


//template<typename Treturn>
//Treturn ConnectorBase::returnValueFromLua(lua_State* state)
//{
//	return static_cast<Treturn> (lua_touserdata(state, -1));
//}


ConnectorBase::ConnectorBase() :
	mLuaFunctionIndex(LUA_NOREF), mLuaSelfIndex(LUA_NOREF)
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

template <typename TAdapter0, typename TAdapter1>
TemplatedConnectorBase<TAdapter0, TAdapter1>::TemplatedConnectorBase(const TAdapter0& adapter0, const TAdapter1& adapter1) :
	mAdapter0(adapter0), mAdapter1(adapter1)
{
}

template <typename TAdapter0, typename TAdapter1> template <typename Tvalue_type0, typename Tvalue_type1>
void TemplatedConnectorBase<TAdapter0, TAdapter1>::callLuaMethod(const Tvalue_type0& t0, const Tvalue_type1& t1)
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







class ConnectorZero: public TemplatedConnectorBase<EmptyValueAdapter, EmptyValueAdapter>
{
public:
	ConnectorZero(sigc::signal<void>& signal);

private:
	sigc::signal<void> mSignal;

	void signal_recieve();
};

template <typename TAdapter0, typename T0 = typename TAdapter0::value_type >
class ConnectorOne: public TemplatedConnectorBase<TAdapter0, EmptyValueAdapter>
{
public:
	ConnectorOne(sigc::signal<void, T0>& signal, const TAdapter0& adapter0);

private:
	sigc::signal<void, T0> mSignal;

	void signal_recieve(const T0& t0);
};

template <typename TAdapter0, typename TAdapter1, typename T0 = typename TAdapter0::value_type, typename T1 = typename TAdapter1::value_type >
class ConnectorTwo: public TemplatedConnectorBase<TAdapter0, TAdapter1>
{
public:
	ConnectorTwo(sigc::signal<void, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1);

private:
	sigc::signal<void, T0, T1> mSignal;

	void signal_recieve(T0 t0, T1 t1);
};



ConnectorZero::ConnectorZero(sigc::signal<void>& signal) :
		TemplatedConnectorBase<>::TemplatedConnectorBase(EmptyValueAdapter(), EmptyValueAdapter()), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorZero::signal_recieve));
}

template <typename TAdapter0, typename T0>
ConnectorOne<TAdapter0, T0>::ConnectorOne(sigc::signal<void, T0>& signal, const TAdapter0& adapter0) :
	TemplatedConnectorBase<TAdapter0>::TemplatedConnectorBase(adapter0, EmptyValueAdapter()), mSignal(signal)
{
	ConnectorBase::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorOne::signal_recieve));
}

template <typename TAdapter0, typename TAdapter1, typename T0, typename T1>
ConnectorTwo<TAdapter0, TAdapter1, T0, T1>::ConnectorTwo(sigc::signal<void, T0, T1>& signal, const TAdapter0& adapter0, const TAdapter1& adapter1) :
	TemplatedConnectorBase<TAdapter0, TAdapter1>::TemplatedConnectorBase(adapter0, adapter1), mSignal(signal)
{
	ConnectorBase::mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorTwo::signal_recieve));
}

void ConnectorZero::signal_recieve()
{
	callLuaMethod(Empty(), Empty());
}


template <typename TAdapter0, typename T0>
void ConnectorOne<TAdapter0, T0>::signal_recieve(const T0& t0)
{
	callLuaMethod(t0, Empty());
}

template <typename TAdapter0, typename TAdapter1, typename T0, typename T1>
void ConnectorTwo<TAdapter0, TAdapter1, T0, T1>::signal_recieve(T0 t0, T1 t1)
{
	callLuaMethod(t0, t1);
}

}

LuaConnector::~LuaConnector()
{
	delete mConnector;
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
	return this;
}

void LuaConnector::disconnect()
{
	if (mConnector) {
		mConnector->disconnect();
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
	return this;
}


bool LuaConnector::checkSignalExistence(void* signal)
{
	if (!signal) {
		S_LOG_WARNING("Tried to connect lua to a non existent signal.");
		return false;
	}
	return true;

}


template <typename TSignal, typename TAdapter0>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorOne<TAdapter0, typename TAdapter0::value_type>(signal, adapter);
	} else {
		mConnector = 0;
	}
}

template <typename TSignal, typename TAdapter0, typename TAdapter1>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter0, const TAdapter1& adapter1)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorTwo<TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, adapter0, adapter1);
	} else {
		mConnector = 0;
	}
}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorZero(signal);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, EmberEntity*>& signal)
{
	createConnector(signal, LuaConnectors::StringValueAdapter(), LuaConnectors::PtrValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
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
	mConnector = 0;
//	if (checkSignalExistence(&signal)) {
//		LuaTypeStore luaTypes;
//		luaTypes.push_back("string");
//		mConnector = new LuaConnectors::ConnectorOne<bool, const std::string&>(signal, luaTypes);
//	}
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
