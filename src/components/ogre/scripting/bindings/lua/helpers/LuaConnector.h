//
// C++ Interface: LuaConnectors
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
#ifndef EMBEROGRELUACONNECTORS_H
#define EMBEROGRELUACONNECTORS_H

#include "tolua++.h"
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <string>
#include <vector>

#include "components/ogre/input/Input.h"

namespace Eris {

class Connection;
class Account;
class View;
class ServerInfo;
}

namespace EmberOgre {

class EmberEntity;
class EntityPickResult;
class MousePickerArgs;
class EmberEntityFactory;
class AvatarEmberEntity;
class Jesus;
class BasePointUserObject;
class TerrainEditAction;

typedef std::vector<std::string> LuaTypeStore;


//template<> void ConnectorBase::callLuaMethod(std::string t0, std::string t1, Empty t2, Empty t3);

namespace LuaConnectors {

	
	class Empty
	{
		public:
		Empty() {}
	};
	
	class ConnectorBase 
	: public sigc::trackable
	{
		public:
			void connect(const std::string & luaMethod);
			ConnectorBase();
			ConnectorBase(const LuaTypeStore& luaTypeNames);
			template <typename T0, typename T1, typename T2, typename T3> void callLuaMethod(T0 t0, T1 t1, T2 t2, T3 t3);
			//void callLuaMethod(const std::string& t0, const std::string& t1, LuaConnectors::Empty t2, LuaConnectors::Empty t3) ;
			protected:
			std::string mLuaMethod;
			unsigned int mNumberOfArguments;
			std::vector<std::string> mLuaTypeNames;
	};

	class ConnectorZero : public ConnectorBase
	{
		public:
			ConnectorZero(sigc::signal<void>& signal);
			ConnectorZero(SigC::Signal0<void>& signal);
			
		private:
			sigc::signal<void> mSignal;
			SigC::Signal0<void> mSignal_old;
			void signal_recieve();
	};


template <typename T0>
	class ConnectorOne : public ConnectorBase
	{
		public:
			ConnectorOne(sigc::signal<void, T0>& signal,  const LuaTypeStore& luaTypeNames);
			ConnectorOne(SigC::Signal1<void, T0>& signal, const LuaTypeStore& luaTypeNames);
			
		private:
			sigc::signal<void, T0> mSignal;
			SigC::Signal1<void, T0> mSignal_old;
			void signal_recieve(T0 t0);
	
	};

template <typename T0, typename T1>
	class ConnectorTwo : public ConnectorBase
	{
		public:
			ConnectorTwo(sigc::signal<void, T0, T1>& signal, const LuaTypeStore& luaTypeNames);
		
		private:
			sigc::signal<void, T0, T1> mSignal;
			void signal_recieve(T0 t0, T1 t1);
	
	};

template <typename T0, typename T1, typename T2>
	class ConnectorThree : public ConnectorBase
	{
		public:
			ConnectorThree(sigc::signal<void, T0, T1, T2>& signal, const LuaTypeStore& luaTypeNames);
		
		
		private:
			sigc::signal<void, T0, T1, T2> mSignal;
			void signal_recieve(T0 t0, T1 t1, T2 t2);
	
	};

template <typename T0, typename T1, typename T2, typename T3>
	class ConnectorFour : public ConnectorBase
	{
		public:
			ConnectorFour(sigc::signal<void, T0, T1, T2, T3>& signal, const LuaTypeStore& luaTypeNames);
		
		
		private:
			sigc::signal<void, T0, T1, T2, T3> mSignal;
			void signal_recieve(T0 t0, T1 t1, T2 t2, T3 t3);
	
	};
}


/**
@author Erik Hjortsberg
*/
class LuaConnector{
public:
    static void setState(lua_State* state);
    static lua_State* getState();
	static lua_State* sState;
	template <typename T> static void pushValue(T theValue, const std::string& luaTypename);
	static void pushValue(const std::string& theValue, const std::string& luaTypename);
	static void pushValue(const float& theValue, const std::string& luaTypename);
	static void pushValue(const int& theValue, const std::string& luaTypename);
	static void pushValue(const unsigned int& theValue, const std::string& luaTypename);
	static void pushValue(const long& theValue, const std::string& luaTypename);
	static void pushValue(const unsigned long& theValue, const std::string& luaTypename);
	
	static void pushValue(const Eris::ServerInfo& theValue, const std::string& luaTypename);
	static void pushValue(const EntityPickResult& theValue, const std::string& luaTypename);
	static void pushValue(const MousePickerArgs& theValue, const std::string& luaTypename);
	static void pushValue(LuaConnectors::Empty theValue, const std::string& luaTypename);
	static void pushValue(const Input::MouseButton& theValue, const std::string& luaTypename);
	static void pushValue(const Input::InputMode& theValue, const std::string& luaTypename);
	
	
	LuaConnector(sigc::signal<void, const std::string&, EmberEntity*>& signal);
	LuaConnector(sigc::signal<void, Eris::Connection*>& signal);
	LuaConnector(SigC::Signal1<void, const Eris::ServerInfo&>& signal);
 	LuaConnector(sigc::signal<void, float>& signal);
 	LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal);
	LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal);
	LuaConnector(sigc::signal<void, Input::MouseButton, Input::InputMode>& signal);
	LuaConnector(sigc::signal<void, EmberEntityFactory*>& signal);
	LuaConnector(sigc::signal<void, AvatarEmberEntity*>& signal);
	LuaConnector(sigc::signal<void, Jesus*>& signal);
	LuaConnector(sigc::signal<void, EmberEntity*>& signal);
 	LuaConnector(sigc::signal<void, const std::string&>& signal);
 	LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal);
 	LuaConnector(sigc::signal<void, BasePointUserObject*>& signal);
 	LuaConnector(sigc::signal<void, TerrainEditAction*>& signal);
 	LuaConnector(sigc::signal<void>& signal);
   
   
   
    ~LuaConnector();
    
    void connect(const std::string& luaMethod);
private:
	LuaConnectors::ConnectorBase* mConnector;
};

}

#endif
