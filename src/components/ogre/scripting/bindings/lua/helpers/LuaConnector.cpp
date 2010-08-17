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

#include "components/lua/Connectors.h"
#include "components/lua/Connectors_impl.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/EntityWorldPickListener.h"

using namespace Ember;

namespace EmberOgre
{

LuaConnector::~LuaConnector()
{
	delete mConnector;
}

lua_State* LuaConnector::getState()
{
	return Ember::Lua::ConnectorBase::getState();
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
		int luaType = lua_type(getState(), -1);
		if (luaType == LUA_TFUNCTION) {
			int index = luaL_ref(getState(), LUA_REGISTRYINDEX);
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
			int luaType = lua_type(getState(), -1);
			int index = luaL_ref(getState(), LUA_REGISTRYINDEX);
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
		mConnector = new Ember::Lua::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, adapter);
	} else {
		mConnector = 0;
	}
}

template <typename TSignal, typename TAdapter0, typename TAdapter1>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter0, const TAdapter1& adapter1)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new Ember::Lua::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, adapter0, adapter1);
	} else {
		mConnector = 0;
	}
}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new Ember::Lua::ConnectorZero<void>(signal);
	}
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, EmberEntity*>& signal)
{
	createConnector(signal, Ember::Lua::StringValueAdapter(), Ember::Lua::PtrValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
}


LuaConnector::LuaConnector(sigc::signal<void, Eris::Connection*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Eris::Connection>("Eris::Connection"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Eris::ServerInfo&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const Eris::ServerInfo>("Eris::ServerInfo"));
}

LuaConnector::LuaConnector(sigc::signal<void, float>& signal)
{
	createConnector(signal, Ember::Lua::NumberValueAdapter<float>());
}

LuaConnector::LuaConnector(sigc::signal<void, const EntityPickResult&, const MousePickerArgs&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const EntityPickResult>("EmberOgre::EntityPickResult"), Ember::Lua::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::vector<EmberOgre::EntityPickResult>&, const MousePickerArgs&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const std::vector<EmberOgre::EntityPickResult> >("std::vector<EmberOgre::EntityPickResult>"), Ember::Lua::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, const MousePickerArgs&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const MousePickerArgs>("EmberOgre::MousePickerArgs"));
}

LuaConnector::LuaConnector(sigc::signal<void, Input::MouseButton, Input::InputMode>& signal)
{
	createConnector(signal, Ember::Lua::NumberValueAdapter<Input::MouseButton>(), Ember::Lua::NumberValueAdapter<Input::InputMode>());
}

LuaConnector::LuaConnector(sigc::signal<void, Input::InputMode>& signal)
{
	createConnector(signal, Ember::Lua::NumberValueAdapter<Input::InputMode>());
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntityFactory&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<EmberEntityFactory>("EmberOgre::EmberEntityFactory"));
}
LuaConnector::LuaConnector(sigc::signal<void, Jesus*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Jesus>("EmberOgre::Jesus"));
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
}
LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<EmberEntity>("EmberOgre::EmberEntity"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberEntity&, EmberOgre::Authoring::EntityMover&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<EmberEntity>("EmberOgre::EmberEntity"), Ember::Lua::RefValueAdapter<EmberOgre::Authoring::EntityMover>("EmberOgre::Authoring::EntityMover"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&>& signal)
{
	createConnector(signal, Ember::Lua::StringValueAdapter());
}

LuaConnector::LuaConnector(sigc::signal<bool, const std::string&>& signal)
{
	createConnector(signal, Ember::Lua::StringValueAdapter());
}

LuaConnector::LuaConnector(sigc::signal<void, const std::string&, const std::string&>& signal)
{
	createConnector(signal, Ember::Lua::StringValueAdapter(), Ember::Lua::StringValueAdapter());
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::BasePointUserObject*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Terrain::BasePointUserObject>("EmberOgre::Terrain::BasePointUserObject"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainEditAction*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Terrain::TerrainEditAction>("EmberOgre::Terrain::TerrainEditAction"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainEditorOverlay&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<Terrain::TerrainEditorOverlay>("EmberOgre::Terrain::TerrainEditorOverlay"));
}

LuaConnector::LuaConnector(sigc::signal<void, Terrain::TerrainManager&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<Terrain::TerrainManager>("EmberOgre::Terrain::TerrainManager"));
}


LuaConnector::LuaConnector(sigc::signal<void, Eris::Task*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Eris::Task>("Eris::Task"));
}

LuaConnector::LuaConnector(sigc::signal<void, const std::set<std::string>&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const std::set<std::string>&>("std::set<std::string>"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Gui::EntityIcon*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<EmberOgre::Gui::EntityIcon>("EmberOgre::Gui::EntityIcon"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Message::Element&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const Atlas::Message::Element>("Atlas::Message::Element"));
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::MovementControllerMode::Mode>& signal)
{
	createConnector(signal, Ember::Lua::NumberValueAdapter<EmberOgre::MovementControllerMode::Mode>());
}

LuaConnector::LuaConnector(sigc::signal<void, EmberOgre::Terrain::TerrainPage&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<EmberOgre::Terrain::TerrainPage>("EmberOgre::Terrain::TerrainPage"));
}

LuaConnector::LuaConnector(sigc::signal<void, const Atlas::Objects::Root&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<const Atlas::Objects::Root>("Atlas::Objects::Root"));
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::Avatar*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Eris::Avatar>("Eris::Avatar"));
}

LuaConnector::LuaConnector(sigc::signal<void, Eris::View*>& signal)
{
	createConnector(signal, Ember::Lua::PtrValueAdapter<Eris::View>("Eris::View"));
}

LuaConnector::LuaConnector(sigc::signal<void, World&>& signal)
{
	createConnector(signal, Ember::Lua::RefValueAdapter<World>("EmberOgre::World"));
}




}


namespace Ember
{

namespace Lua
{

template <>
const char* resolveLuaTypename<const EmberOgre::EmberEntity>(){return "EmberOgre::EmberEntity";}
template <>
const char* resolveLuaTypename<const Ogre::Camera>(){return "Ogre::Camera";}
template <>
const char* resolveLuaTypename<const Eris::Connection>(){return "Eris::Connection";}
template <>
const char* resolveLuaTypename<const Eris::ServerInfo>(){return "Eris::ServerInfo";}
template <>
const char* resolveLuaTypename<const EmberOgre::EntityPickResult>(){return "EmberOgre::EntityPickResult";}
template <>
const char* resolveLuaTypename<const EmberOgre::MousePickerArgs>(){return "EmberOgre::MousePickerArgs";}
template <>
const char* resolveLuaTypename<const EmberOgre::EmberEntityFactory>(){return "EmberOgre::EmberEntityFactory";}
template <>
const char* resolveLuaTypename<const EmberOgre::Authoring::EntityMover>(){return "EmberOgre::Authoring::EntityMover";}
template <>
const char* resolveLuaTypename<const EmberOgre::Terrain::BasePointUserObject>(){return "EmberOgre::Terrain::BasePointUserObject";}
template <>
const char* resolveLuaTypename<const EmberOgre::Terrain::TerrainEditAction>(){return "EmberOgre::Terrain::TerrainEditAction";}
template <>
const char* resolveLuaTypename<const EmberOgre::Terrain::TerrainEditorOverlay>(){return "EmberOgre::Terrain::TerrainEditorOverlay";}
template <>
const char* resolveLuaTypename<const EmberOgre::Terrain::TerrainManager>(){return "EmberOgre::Terrain::TerrainManager";}
template <>
const char* resolveLuaTypename<const Eris::Task>(){return "Eris::Task";}
template <>
const char* resolveLuaTypename<const EmberOgre::Gui::EntityIcon>(){return "EmberOgre::Gui::EntityIcon";}
template <>
const char* resolveLuaTypename<const EmberOgre::Terrain::TerrainPage>(){return "EmberOgre::Terrain::TerrainPage";}
template <>
const char* resolveLuaTypename<const Eris::Avatar>(){return "Eris::Avatar";}
template <>
const char* resolveLuaTypename<const Eris::View>(){return "Eris::View";}
template <>
const char* resolveLuaTypename<const EmberOgre::World>(){return "EmberOgre::World";}
template <>
const char* resolveLuaTypename<const std::vector<EmberOgre::EntityPickResult> >(){return "std::vector<EmberOgre::EntityPickResult>";}
template <>
const char* resolveLuaTypename<const std::set<std::string> >(){return "std::set<std::string>";}

}
}

