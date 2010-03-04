//
// C++ Interface: LuaHelper
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBEROGRE_SCRIPTINGLUAHELPER_H
#define EMBEROGRE_SCRIPTINGLUAHELPER_H


#include <string>

// include Lua libs
extern "C" {
#include "lua.h"
}

namespace Ember {

namespace Lua {

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Helper functions for Lua scripting.
 */
class LuaHelper{
public:
	/**
	 * @brief Handles lua error by printing out a stracktrace.
	 * @param L The lua state.
	 */
	static int luaErrorHandler(lua_State *L);

	/**
	 * @brief Pushes the lua method onto the stack.
	 * @param state The lua state.
	 * @param luaMethod A lua method name.
	 */
	static void pushNamedFunction(lua_State* state, const std::string luaMethod);

};

}

}

#endif
