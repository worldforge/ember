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
#ifndef EMBEROGRE_LUACONNECTOR_H
#define EMBEROGRE_LUACONNECTOR_H

#include "services/input/Input.h"
#include "components/ogre/MovementController.h"
#include "components/lua/luaobject.h"

#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>
#include <string>
#include <vector>
#include <Atlas/Objects/ObjectsFwd.h>
#include <tolua++.h>

namespace Atlas
{
namespace Message
{
class Element;
}
}

namespace Eris
{

class Connection;
class Account;
class View;
class ServerInfo;
class Task;
class Avatar;
}

namespace EmberOgre
{

class EmberEntity;
struct EntityPickResult;
struct MousePickerArgs;
class EmberEntityFactory;
class Jesus;
class World;
namespace Terrain
{
class BasePointUserObject;
class TerrainEditAction;
class TerrainPage;
class TerrainEditorOverlay;
class TerrainManager;
}

namespace Authoring
{
class EntityMover;
}
typedef std::vector<std::string> LuaTypeStore;

namespace Gui
{
class EntityIcon;
}
//template<> void ConnectorBase::callLuaMethod(std::string t0, std::string t1, Empty t2, Empty t3);

namespace LuaConnectors
{
class ConnectorBase;
}

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
 fooConnector = EmberOgre.LuaConnector:new_local(emitter.EventFoo):connect(lua_foo)



 </code>

 */
class LuaConnector
{
public:

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

	LuaConnector(sigc::signal<void>& signal);
	LuaConnector(sigc::signal<void, const std::string&, EmberEntity*>& signal);
	LuaConnector(sigc::signal<void, Eris::Connection*>& signal);
	LuaConnector(sigc::signal<void, const Eris::ServerInfo&>& signal);
	LuaConnector(sigc::signal<void, float>& signal);
	LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal);
	LuaConnector(sigc::signal<void, const std::vector<EntityPickResult>&, const MousePickerArgs&>& signal);
	LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal);
	LuaConnector(sigc::signal<void, Ember::Input::MouseButton, Ember::Input::InputMode>& signal);
	LuaConnector(sigc::signal<void, Ember::Input::InputMode>& signal);
	LuaConnector(sigc::signal<void, EmberEntityFactory&>& signal);
	LuaConnector(sigc::signal<void, Jesus*>& signal);
	LuaConnector(sigc::signal<void, EmberEntity*>& signal);
	LuaConnector(sigc::signal<void, EmberEntity&>& signal);
	LuaConnector(sigc::signal<void, EmberEntity&, Authoring::EntityMover&>& signal);
	LuaConnector(sigc::signal<void, const std::string&>& signal);
	LuaConnector(sigc::signal<bool, const std::string&>& signal);
	LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal);
	LuaConnector(sigc::signal<void, Terrain::BasePointUserObject*>& signal);
	LuaConnector(sigc::signal<void, Terrain::TerrainEditAction*>& signal);
	LuaConnector(sigc::signal<void, Terrain::TerrainEditorOverlay&>& signal);
	LuaConnector(sigc::signal<void, Terrain::TerrainManager&>& signal);
	LuaConnector(sigc::signal<void, Eris::Task*>& signal);
	LuaConnector(sigc::signal<void, const std::set<std::string>&>& signal);
	LuaConnector(sigc::signal<void, Gui::EntityIcon*>& signal);
	LuaConnector(sigc::signal<void, const Atlas::Message::Element&>& signal);
	LuaConnector(sigc::signal<void, MovementControllerMode::Mode>& signal);
	LuaConnector(sigc::signal<void, Terrain::TerrainPage&>& signal);
	LuaConnector(sigc::signal<void, const Atlas::Objects::Root&>& signal);
	LuaConnector(sigc::signal<void, Eris::Avatar*>& signal);
	LuaConnector(sigc::signal<void, Eris::View*>& signal);
	LuaConnector(sigc::signal<void, World&>& signal);

	~LuaConnector();

	/**
	 * @brief Connects to the named lua method.
	 *
	 * @param luaMethod The fully qualified name of the method.
	 * @param selfIndex An optional lua table index to be used as a "self" parameter.
	 *
	 * @return This instance.
	 */
	LuaConnector* connect(const std::string& luaMethod, lua_Object selfIndex = LUA_NOREF);

	/**
	 * @brief Connects to the lua method.
	 *
	 * @param luaMethod The lua method.
	 * @param selfIndex An optional lua table index to be used as a "self" parameter.
	 *
	 * @return This instance.
	 */
	LuaConnector* connect(lua_Object luaMethod, lua_Object selfIndex = LUA_NOREF);

	/**
	 * @brief Disconnects from the signal.
	 */
	void disconnect();

	/**
	 * @brief Sets a "self" reference which will be prepended to any lua call.
	 *
	 * @param selfIndex The lua index of the self reference.
	 */
	LuaConnector* setSelf(int selfIndex);

private:

	/**
	 * @brief The internal connector which will handle the actual lua binding.
	 */
	LuaConnectors::ConnectorBase* mConnector;

	/**
	 * @brief Checks that the signal submitted isn't null. If so, mConnector will be set to null and no connection will occur.
	 *
	 * @param signal A pointer to a signal.
	 * @return True if the supplied signal isn't null.
	 */
	bool checkSignalExistence(void* signal);


	/**
	 * @brief Creates a new connector instance.
	 * @param signal The signal.
	 * @param adapter The first adapter.
	 */
	template <typename TSignal, typename TAdapter0>
	void createConnector(TSignal& signal, const TAdapter0& adapter);

	/**
	 * @brief Creates a new connector instance.
	 * @param signal The signal.
	 * @param adapter0 The first adapter.
	 * @param adapter1 The second adapter.
	 */
	template <typename TSignal, typename TAdapter0, typename TAdapter1>
	void createConnector(TSignal& signal, const TAdapter0& adapter0, const TAdapter1& adapter1);

};

}

#endif
