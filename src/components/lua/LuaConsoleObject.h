//
// C++ Interface: LuaConsoleObject
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
#ifndef EMBEROGRELUACONSOLEOBJECT_H
#define EMBEROGRELUACONSOLEOBJECT_H

#include "framework/ConsoleCommandWrapper.h"
#include "sol2/sol.hpp"

#include <memory>

namespace Ember {
namespace Lua {

/**
 * @author Erik Ogenvik
 *
 * @brief A console object which can easily be created in Lua, and will perform call back into Lua when activated.
 *
 * Use an instance of this whenever you want to register console commands from Lua.
 *
 */
class LuaConsoleObject {
public:

	/**
	 * @brief Ctor.
	 * @param command The console command to register.
	 * @param luaMethod A lua method to call when the command is executed.
	 * @param description An optional description of the console command.
	 */
	LuaConsoleObject(const std::string& command, const sol::function& luaMethod, const std::string& description = "");


private:

	/**
	 * @brief The console command registered.
	 */
	const ConsoleCommandWrapper mCommandWrapper;
};

}
}

#endif
