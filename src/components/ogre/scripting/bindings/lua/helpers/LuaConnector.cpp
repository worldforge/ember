//
// C++ Implementation: LuaConnectors
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "LuaConnector.h"

#include "framework/Exception.h"
#include "services/logging/LoggingService.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberPhysicalEntity.h"
// #include "components/ogre/PersonEmberEntity.h"
#include "components/ogre/AvatarEmberEntity.h"
// 
#include "components/ogre/MousePicker.h"
#include "components/ogre/EntityWorldPickListener.h"
// #include "../EmberEntityFactory.h"
// #include "../jesus/Jesus.h"



#include <CEGUIExceptions.h>

#include <Eris/ServerInfo.h>

namespace EmberOgre {

namespace LuaConnectors {

ConnectorBase::ConnectorBase()
{
}


ConnectorBase::ConnectorBase(const LuaTypeStore& luaTypeNames)
: mLuaTypeNames(luaTypeNames)
{
}


void ConnectorBase::connect(const std::string & luaMethod)
{
	mLuaMethod = luaMethod;
}

//template<> void ConnectorBase::callLuaMethod(std::string t0, std::string t1, Empty t2, Empty t3);
template <typename T0, typename T1, typename T2, typename T3> void ConnectorBase::callLuaMethod(T0 t0, T1 t1, T2 t2, T3 t3)
{
	try {
		size_t numberOfArguments = mLuaTypeNames.size();
		lua_State* state = EmberOgre::LuaConnector::getState();
		
		lua_getglobal(state, mLuaMethod.c_str());
	
		// is it a function
		if ( !lua_isfunction(state,-1) )
		{
			throw Ember::Exception( "\"" + mLuaMethod + "\" does not represent a Lua function" );
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
			
		// call it
		int error = lua_pcall(state,numberOfArguments,0,0);
			
		// handle errors
		if ( error )
		{
			const std::string& msg = lua_tostring(state,-1);
			lua_pop(state,numberOfArguments);
			throw Ember::Exception(msg);
		}

	}
	catch(const Ember::Exception& ex )
	{
		lua_settop( EmberOgre::LuaConnector::getState(), 0 );
		S_LOG_FAILURE("Unable to execute scripted event handler: " << mLuaMethod << "\n" << ex.getError());
	}
	catch( const CEGUI::String& str )
	{
		lua_settop( EmberOgre::LuaConnector::getState(), 0 );
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: "<<mLuaMethod<<"\n"<<str.c_str());
	}
	catch( const CEGUI::Exception& ex )
	{
		lua_settop( EmberOgre::LuaConnector::getState(), 0 );
		S_LOG_FAILURE("(LuaScriptModule) Unable to execute scripted event handler: "<<mLuaMethod<<"\n"<<ex.getMessage().c_str());

	} catch (...) 
	{
		lua_settop( EmberOgre::LuaConnector::getState(), 0 );
		S_LOG_FAILURE("Unspecified error when executing: " << mLuaMethod  );
	}
}



ConnectorZero::ConnectorZero(sigc::signal<void>& signal) :  ConnectorBase(), mSignal(signal)
{
	mSignal.connect(sigc::mem_fun(*this, &ConnectorZero::signal_recieve));
}

template <typename T0> 
ConnectorOne<T0>::ConnectorOne(sigc::signal<void, T0>& signal, const LuaTypeStore& luaTypeNames) :  ConnectorBase(luaTypeNames), mSignal(signal)
{
	mSignal.connect(sigc::mem_fun(*this, &ConnectorOne<T0>::signal_recieve));
}

template <typename T0> 
ConnectorOne<T0>::ConnectorOne(SigC::Signal1<void, T0>& signal, const LuaTypeStore& luaTypeNames) :
 ConnectorBase(luaTypeNames), mSignal_old(signal)
{
	mSignal_old.connect(sigc::mem_fun(*this, &ConnectorOne<T0>::signal_recieve));
}

template <typename T0, typename T1> 
ConnectorTwo<T0, T1>::ConnectorTwo(sigc::signal<void, T0, T1>& signal, const LuaTypeStore& luaTypeNames) :  ConnectorBase(luaTypeNames), mSignal(signal)
{
	mSignal.connect(sigc::mem_fun(*this, &ConnectorTwo<T0, T1>::signal_recieve));
}

template <typename T0, typename T1, typename T2> 
ConnectorThree<T0, T1, T2>::ConnectorThree(sigc::signal<void, T0, T1, T2>& signal, const LuaTypeStore& luaTypeNames) :  ConnectorBase(luaTypeNames), mSignal(signal)
{
	mSignal.connect(sigc::mem_fun(*this, &ConnectorThree<T0, T1, T2>::signal_recieve));
}

template <typename T0, typename T1, typename T2, typename T3> 
ConnectorFour<T0, T1, T2, T3>::ConnectorFour(sigc::signal<void, T0, T1, T2, T3>& signal, const LuaTypeStore& luaTypeNames) :  ConnectorBase(luaTypeNames), mSignal(signal)
{
	mSignal.connect(sigc::mem_fun(*this, &ConnectorFour<T0, T1, T2, T3>::signal_recieve));
}


void ConnectorZero::signal_recieve()
{
	callLuaMethod(Empty(), Empty(), Empty(), Empty());
}

template <typename T0>
void ConnectorOne<T0>::signal_recieve(T0 t0)
{
	callLuaMethod(t0, Empty(), Empty(), Empty());
}

template <typename T0, typename T1>
void ConnectorTwo<T0, T1>::signal_recieve(T0 t0, T1 t1)
{
	callLuaMethod(t0, t1, Empty(), Empty());
}

template <typename T0, typename T1, typename T2>
void ConnectorThree<T0, T1, T2>::signal_recieve(T0 t0, T1 t1, T2 t2)
{
	callLuaMethod(t0, t1, t2, Empty());
}

template <typename T0, typename T1, typename T2, typename T3>
void ConnectorFour<T0, T1, T2, T3>::signal_recieve(T0 t0, T1 t1, T2 t2, T3 t3)
{
	callLuaMethod(t0, t1, t2, t3);
}


};

template <typename T>
void LuaConnector::pushValue(T theValue, const std::string& luaTypename)
{
//	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)&theValue, luaTypename.c_str());
	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)theValue, luaTypename.c_str());
}

void LuaConnector::pushValue(const Eris::ServerInfo& theValue, const std::string& luaTypename)
{
	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)&theValue, luaTypename.c_str());
}

void LuaConnector::pushValue(const std::string& theValue, const std::string& luaTypename) 
{
	tolua_pushstring(EmberOgre::LuaConnector::getState(),theValue.c_str());
}

void LuaConnector::pushValue(LuaConnectors::Empty theValue, const std::string& luaTypename) 
{
}

void LuaConnector::pushValue(const float& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(),theValue);
}

void LuaConnector::pushValue(const long& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(),theValue);
}

void LuaConnector::pushValue(const unsigned long& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(),theValue);
}

void LuaConnector::pushValue(const unsigned int& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(),theValue);
}


void LuaConnector::pushValue(const int& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(),theValue);
}

void LuaConnector::pushValue(const EntityPickResult& theValue, const std::string& luaTypename)
{
	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)&theValue, luaTypename.c_str());
}

void LuaConnector::pushValue(const MousePickerArgs& theValue, const std::string& luaTypename)
{
	tolua_pushusertype(EmberOgre::LuaConnector::getState(),(void*)&theValue, luaTypename.c_str());
}

void LuaConnector::pushValue(const Input::MouseButton& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
}
void LuaConnector::pushValue(const Input::InputMode& theValue, const std::string& luaTypename)
{
	tolua_pushnumber(EmberOgre::LuaConnector::getState(), theValue);
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

void LuaConnector::connect(const std::string& luaMethod)
{
	mConnector->connect(luaMethod);
}








LuaConnector::LuaConnector(sigc::signal<void, const std::string&, EmberEntity*> & signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("string");
	luaTypes.push_back("EmberOgre::EmberEntity");
	mConnector = new LuaConnectors::ConnectorTwo<const std::string&, EmberEntity*>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::Connection*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("Eris::Connection");
	mConnector = new LuaConnectors::ConnectorOne<Eris::Connection*>(signal, luaTypes);
}

LuaConnector::LuaConnector(SigC::Signal1<void, const Eris::ServerInfo&>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("Eris::ServerInfo");
	mConnector = new LuaConnectors::ConnectorOne<const Eris::ServerInfo&>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, float>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("float");
	mConnector = new LuaConnectors::ConnectorOne<float>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::EntityPickResult");
	luaTypes.push_back("EmberOgre::MousePickerArgs");
	mConnector = new LuaConnectors::ConnectorTwo<const EntityPickResult&, const MousePickerArgs&>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::MousePickerArgs");
	mConnector = new LuaConnectors::ConnectorOne<const MousePickerArgs&>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, Input::MouseButton, Input::InputMode>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::Input::MouseButton");
	luaTypes.push_back("EmberOgre::Input::InputMode");
	mConnector = new LuaConnectors::ConnectorTwo<Input::MouseButton, Input::InputMode>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntityFactory*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::EmberEntityFactory");
	mConnector = new LuaConnectors::ConnectorOne<EmberEntityFactory*>(signal, luaTypes);
}
LuaConnector::LuaConnector(sigc::signal<void, AvatarEmberEntity*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::AvatarEmberEntity");
	mConnector = new LuaConnectors::ConnectorOne<AvatarEmberEntity*>(signal, luaTypes);
}
LuaConnector::LuaConnector(sigc::signal<void, Jesus*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::Jesus");
	mConnector = new LuaConnectors::ConnectorOne<Jesus*>(signal, luaTypes);
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::EmberEntity");
	mConnector = new LuaConnectors::ConnectorOne<EmberEntity*>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("string");
	mConnector = new LuaConnectors::ConnectorOne<const std::string&>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("string");
	luaTypes.push_back("string");
	mConnector = new LuaConnectors::ConnectorTwo<const std::string&, const std::string&>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, BasePointUserObject*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::BasePointUserObject");
	mConnector = new LuaConnectors::ConnectorOne<BasePointUserObject*>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void, TerrainEditAction*>& signal)
{
	LuaTypeStore luaTypes;
	luaTypes.push_back("EmberOgre::TerrainEditAction");
	mConnector = new LuaConnectors::ConnectorOne<TerrainEditAction*>(signal, luaTypes);
}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	mConnector = new LuaConnectors::ConnectorZero(signal);
}


};
