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

ConnectorBase::ConnectorBase() :
	mLuaFunctionIndex(LUA_NOREF)
{
}

ConnectorBase::ConnectorBase(const LuaTypeStore& luaTypeNames) :
	mLuaTypeNames(luaTypeNames), mLuaFunctionIndex(LUA_NOREF)
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
	LuaHelper::pushNamedFunction(state, mLuaMethod);
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
		lua_pushcfunction(state, LuaHelper::luaErrorHandler);
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
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: "<<mLuaMethod<<"\n"<<str.c_str());
	} catch (const CEGUI::Exception& ex) {
		lua_settop(state, top);
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: "<<mLuaMethod<<"\n"<<ex.getMessage().c_str());
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

// void ConnectorBase::returnValueFromLua(lua_State* state, bool& returnValueHolder)
// {
// 	returnValueHolder = lua_toboolean(state,-1);
// }
//
// void returnValueFromLua(lua_State* state)
// {
// }

template<typename Treturn>
ConnectorZero<Treturn>::ConnectorZero(sigc::signal<Treturn>& signal) :
	ConnectorBase(), mSignal(signal)
{
	mConnection = mSignal.connect(sigc::mem_fun(*this, &ConnectorZero<Treturn>::signal_recieve));
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

template<typename T>
void LuaConnector::pushUserTypeValue(T& theValue, const std::string& luaTypename)
{
	tolua_pushusertype(EmberOgre::LuaConnector::getState(), (void*)&theValue, luaTypename.c_str());
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

LuaConnector* LuaConnector::connect(const std::string& luaMethod)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect the lua method '" << luaMethod << "' to a non existent signal.");
	} else {
		mConnector->connect(luaMethod);
	}
	return this;
}

LuaConnector* LuaConnector::connect(int luaMethod)
{
	if (!mConnector) {
		S_LOG_WARNING("Tried to connect lua method to a non existent signal.");
	} else {
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

bool LuaConnector::checkSignalExistence(void* signal)
{
	if (!signal) {
		S_LOG_WARNING("Tried to connect lua to a non existent signal.");
		mConnector = 0;
		return false;
	}
	return true;

}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorZero<void>(signal);
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

LuaConnector::LuaConnector(sigc::signal<void, Eris::Connection*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("Eris::Connection");
		mConnector = new LuaConnectors::ConnectorOne<void, Eris::Connection*>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const Eris::ServerInfo&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("Eris::ServerInfo");
		mConnector = new LuaConnectors::ConnectorOne<void, const Eris::ServerInfo&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, float>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("float");
		mConnector = new LuaConnectors::ConnectorOne<void, float>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::EntityPickResult");
		luaTypes.push_back("EmberOgre::MousePickerArgs");
		mConnector = new LuaConnectors::ConnectorTwo<void, const EntityPickResult&, const MousePickerArgs&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::vector<EmberOgre::EntityPickResult>&, const MousePickerArgs&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("std::vector<EmberOgre::EntityPickResult>");
		luaTypes.push_back("EmberOgre::MousePickerArgs");
		mConnector = new LuaConnectors::ConnectorTwo<void, const std::vector<EmberOgre::EntityPickResult>&, const MousePickerArgs&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::MousePickerArgs");
		mConnector = new LuaConnectors::ConnectorOne<void, const MousePickerArgs&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Input::MouseButton, Input::InputMode>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Input::MouseButton");
		luaTypes.push_back("EmberOgre::Input::InputMode");
		mConnector = new LuaConnectors::ConnectorTwo<void, Input::MouseButton, Input::InputMode>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Input::InputMode>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Input::InputMode");
		mConnector = new LuaConnectors::ConnectorOne<void, Input::InputMode>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntityFactory*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::EmberEntityFactory");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberEntityFactory*>(signal, luaTypes);
	}
}
LuaConnector::LuaConnector(sigc::signal<void, Jesus*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Jesus");
		mConnector = new LuaConnectors::ConnectorOne<void, Jesus*>(signal, luaTypes);
	}
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::EmberEntity");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberEntity*>(signal, luaTypes);
	}
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::EmberEntity");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberEntity&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&, EmberOgre::Authoring::EntityMover&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::EmberEntity");
		luaTypes.push_back("EmberOgre::Authoring::EntityMover");
		mConnector = new LuaConnectors::ConnectorTwo<void, EmberEntity&, EmberOgre::Authoring::EntityMover&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("string");
		mConnector = new LuaConnectors::ConnectorOne<void, const std::string&>(signal, luaTypes);
	}
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
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("string");
		luaTypes.push_back("string");
		mConnector = new LuaConnectors::ConnectorTwo<void, const std::string&, const std::string&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::BasePointUserObject*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Terrain::BasePointUserObject");
		mConnector = new LuaConnectors::ConnectorOne<void, Terrain::BasePointUserObject*>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainEditAction*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Terrain::TerrainEditAction");
		mConnector = new LuaConnectors::ConnectorOne<void, Terrain::TerrainEditAction*>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::Task*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("Eris::Task");
		mConnector = new LuaConnectors::ConnectorOne<void, Eris::Task*>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::set<std::string>&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("std::set<std::string>");
		mConnector = new LuaConnectors::ConnectorOne<void, const std::set<std::string>&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Gui::EntityIcon*>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Gui::EntityIcon");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberOgre::Gui::EntityIcon*>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Message::Element&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("Atlas::Message::Element");
		mConnector = new LuaConnectors::ConnectorOne<void, const Atlas::Message::Element&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::MovementControllerMode::Mode>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::AvatarMovementMode::Mode");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberOgre::MovementControllerMode::Mode>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Terrain::TerrainPage&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("EmberOgre::Terrain::TerrainPage");
		mConnector = new LuaConnectors::ConnectorOne<void, EmberOgre::Terrain::TerrainPage&>(signal, luaTypes);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Objects::Root&>& signal)
{
	if (checkSignalExistence(&signal)) {
		LuaTypeStore luaTypes;
		luaTypes.push_back("Atlas::Objects::Root");
		mConnector = new LuaConnectors::ConnectorOne<void, const Atlas::Objects::Root&>(signal, luaTypes);
	}
}

}
;
