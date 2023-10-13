//
// C++ Implementation: LuaConsoleObject
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
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "LuaConsoleObject.h"
#include "framework/ConsoleBackend.h"

namespace Ember::Lua {

LuaConsoleObject::LuaConsoleObject(const std::string& command, const sol::function& luaMethod, const std::string& description) :
		mCommandWrapper(ConsoleBackend::getSingleton(), command, [=](const std::string&, const std::string& args) {
			if (luaMethod) {
				luaMethod(args);
			}
		}, description) {
}

}
