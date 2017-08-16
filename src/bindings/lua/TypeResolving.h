//
// C++ Interface: LuaConnectors
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2005
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#ifndef TYPERESOLVING_H
#define TYPERESOLVING_H
#include "components/lua/Connector.h"

#include "services/input/Input.h"
#include "components/ogre/MovementController.h"

#include <string>
#include <vector>
#include <Atlas/Objects/ObjectsFwd.h>

#include <Eris/BaseConnection.h>

#include <MeshLodGenerator/OgreLodConfig.h>

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
class TypeInfo;
}

namespace Ember
{
struct MouseMotion;
class EntityTalk;
class EmberEntity;
namespace OgreView
{
struct EntityPickResult;
struct MousePickerArgs;
class EmberEntityFactory;
class World;
class MotionManager;
class GUIManager;
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
class ActionBarIcon;
class HelpMessage;
}

}
}

#endif
