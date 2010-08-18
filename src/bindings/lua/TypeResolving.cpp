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

#include "TypeResolving.h"
#include "components/lua/Connectors.h"

#include "components/ogre/MousePicker.h"
#include "components/ogre/EntityWorldPickListener.h"


namespace Ember
{

namespace Lua
{

/**
 * For every type you want to move between C++ and Lua through either an Ember::Lua::RefValueAdapter or an Ember::Lua::PtrValueAdapter you must provide a suitable method here.
 */

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
template <>
const char* resolveLuaTypename<const Atlas::Message::Element >(){return "Atlas::Message::Element";}
template <>
const char* resolveLuaTypename<const Atlas::Objects::Root >(){return "Atlas::Object::Root";}
}
}

