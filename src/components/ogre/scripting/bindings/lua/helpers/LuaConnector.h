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
// You should have received a copy ofthe GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGRELUACONNECTORS_H
#define EMBEROGRELUACONNECTORS_H

#include "tolua++.h"
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <string>
#include <vector>

#include "components/ogre/input/Input.h"

namespace Eris {

class Connection;
class Account;
class View;
class ServerInfo;
class Task;
}

namespace EmberOgre {

class EmberEntity;
struct EntityPickResult;
struct MousePickerArgs;
class EmberEntityFactory;
class AvatarEmberEntity;
class Jesus;
class BasePointUserObject;
class TerrainEditAction;

typedef std::vector<std::string> LuaTypeStore;

namespace Gui {
class EntityIcon;
}
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
			ConnectorBase();
			ConnectorBase(const LuaTypeStore& luaTypeNames);
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
			
			template <typename Treturn, typename T0, typename T1, typename T2, typename T3> Treturn callLuaMethod(T0 t0, T1 t1, T2 t2, T3 t3);
	
		protected:
			
			/**
			The lua method to call.
			*/
			std::string mLuaMethod;
			
			/**
			A vector of the lua type names of the arguments, in order called.
			*/
			std::vector<std::string> mLuaTypeNames;
			
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
			
			template<typename Treturn> Treturn returnValueFromLua(lua_State* state);
/*			void returnValueFromLua(lua_State* state, bool& returnValueHolder);
			void returnValueFromLua(lua_State* state);*/
	
	};

template <typename Treturn>
	class ConnectorZero : public ConnectorBase
	{
		public:
			ConnectorZero(sigc::signal<Treturn>& signal);
			ConnectorZero(SigC::Signal0<Treturn>& signal);
			
		private:
			sigc::signal<Treturn> mSignal;
			SigC::Signal0<Treturn> mSignal_old;
			Treturn signal_recieve();
	};


template <typename Treturn, typename T0>
	class ConnectorOne : public ConnectorBase
	{
		public:
			ConnectorOne(sigc::signal<Treturn, T0>& signal,  const LuaTypeStore& luaTypeNames);
			ConnectorOne(SigC::Signal1<Treturn, T0>& signal, const LuaTypeStore& luaTypeNames);
			
		private:
			sigc::signal<Treturn, T0> mSignal;
			SigC::Signal1<Treturn, T0> mSignal_old;
			Treturn signal_recieve(T0 t0);
	
	};

template <typename Treturn, typename T0, typename T1>
	class ConnectorTwo : public ConnectorBase
	{
		public:
			ConnectorTwo(sigc::signal<Treturn, T0, T1>& signal, const LuaTypeStore& luaTypeNames);
		
		private:
			sigc::signal<Treturn, T0, T1> mSignal;
			Treturn signal_recieve(T0 t0, T1 t1);
	
	};

template <typename Treturn, typename T0, typename T1, typename T2>
	class ConnectorThree : public ConnectorBase
	{
		public:
			ConnectorThree(sigc::signal<Treturn, T0, T1, T2>& signal, const LuaTypeStore& luaTypeNames);
		
		
		private:
			sigc::signal<Treturn, T0, T1, T2> mSignal;
			Treturn signal_recieve(T0 t0, T1 t1, T2 t2);
	
	};

template <typename Treturn, typename T0, typename T1, typename T2, typename T3>
	class ConnectorFour : public ConnectorBase
	{
		public:
			ConnectorFour(sigc::signal<Treturn, T0, T1, T2, T3>& signal, const LuaTypeStore& luaTypeNames);
		
		
		private:
			sigc::signal<Treturn, T0, T1, T2, T3> mSignal;
			Treturn signal_recieve(T0 t0, T1 t1, T2 t2, T3 t3);
	
	};
}


/**
@author Erik Hjortsberg

Class used for connecting sigc signals to lua. Acts as an adapter for the signals, recieving them from the c++ environment and sending them to the lua environment.

To use them in lua, use code like this:
<code>
	--connect the lua method "lua_foo" to the event "EventFoo" of the object "emitter" and keeps a reference to the adapter in the variable "fooConnector"
	local emitter = EmberOgre.Emitter:new()
	fooConnector = EmberOgre.LuaConnector:new_local(emitter.EventFoo):connect("lua_foo")
	
	function lua_foo()
		--do something here
	end

</code>

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
	static void pushValue(const std::set<std::string>& theValue, const std::string& luaTypename);
	
	
 	LuaConnector(sigc::signal<void>& signal);
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
 	LuaConnector(sigc::signal<bool, const std::string&>& signal);
 	LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal);
 	LuaConnector(sigc::signal<void, BasePointUserObject*>& signal);
 	LuaConnector(sigc::signal<void, TerrainEditAction*>& signal);
 	LuaConnector(sigc::signal<void, Eris::Task*>& signal);
 	LuaConnector(sigc::signal<void, const std::set<std::string>&>& signal);
 	LuaConnector(sigc::signal<void, EmberOgre::Gui::EntityIcon*>& signal);
   
   
    ~LuaConnector();
    
	/**
	 *    Connects to the named lua method.
	 * @param luaMethod The fully qualified name of the method.
	 * @return 
	 */
	LuaConnector* connect(const std::string& luaMethod);
	
	/**
	 *    Connects to the lua method.
	 * @param luaMethod The lua method.
	 * @return 
	 */
	LuaConnector* connect(int luaMethod);
	
	/**
	Disconnects from the signal.
	*/
	void disconnect();
	
private:
	LuaConnectors::ConnectorBase* mConnector;
};

}

#endif
