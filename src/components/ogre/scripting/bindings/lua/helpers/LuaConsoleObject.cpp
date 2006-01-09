//
// C++ Implementation: LuaConsoleObject
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
#include "LuaConsoleObject.h"
#include "framework/ConsoleBackend.h"
#include "LuaConnector.h"

namespace EmberOgre {

LuaConsoleObject::LuaConsoleObject(const std::string& command, const std::string& luaMethod):
mCommand(command), mLuaMethod(luaMethod)
{
	Ember::ConsoleBackend::getMainConsole()->registerCommand(command,this);
	LuaTypeStore typenames;
	typenames.push_back("string");
	typenames.push_back("string");
	mConnector = new LuaConnectors::ConnectorBase(typenames);
	mConnector->connect(luaMethod);
}


LuaConsoleObject::~LuaConsoleObject()
{
	Ember::ConsoleBackend::getMainConsole()->deregisterCommand(mCommand);
	delete mConnector;
}

void LuaConsoleObject::runCommand(const std::string &command, const std::string &args)
{
	mConnector->callLuaMethod(command, args, EmberOgre::LuaConnectors::Empty(), EmberOgre::LuaConnectors::Empty());
}

}
