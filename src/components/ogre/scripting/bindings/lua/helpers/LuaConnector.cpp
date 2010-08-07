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

#include "Connectors.h"
#include "Connectors_impl.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/EntityWorldPickListener.h"

using namespace Ember;

namespace EmberOgre
{

LuaConnector::~LuaConnector()
{
	delete mConnector;
}

void LuaConnector::setState(lua_State* state)
{
	LuaConnectors::ConnectorBase::setState(state);
}

lua_State* LuaConnector::getState()
{
	return LuaConnectors::ConnectorBase::getState();
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
		mConnector = new LuaConnectors::ConnectorOne<typename TSignal::result_type, TAdapter0, typename TAdapter0::value_type>(signal, adapter);
	} else {
		mConnector = 0;
	}
}

template <typename TSignal, typename TAdapter0, typename TAdapter1>
void LuaConnector::createConnector(TSignal& signal, const TAdapter0& adapter0, const TAdapter1& adapter1)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorTwo<typename TSignal::result_type, TAdapter0, TAdapter1, typename TAdapter0::value_type, typename TAdapter1::value_type>(signal, adapter0, adapter1);
	} else {
		mConnector = 0;
	}
}

LuaConnector::LuaConnector(sigc::signal<void>& signal)
{
	if (checkSignalExistence(&signal)) {
		mConnector = new LuaConnectors::ConnectorZero<void>(signal);
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
	createConnector(signal, LuaConnectors::StringValueAdapter());
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
