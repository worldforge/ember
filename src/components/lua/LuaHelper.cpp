//
// C++ Implementation: LuaHelper
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LuaHelper.h"
#include "framework/Exception.h"
#include <vector>
#include <sstream>

namespace Ember {
namespace Lua {

int LuaHelper::luaErrorHandler(lua_State *L) {
#if LUA_VERSION_NUM >= 501
	///see if we have the debug library loaded
	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1)) {
		lua_pop(L, 1);
		return 1;
	}
	///if so, call the traceback method
	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1)) {
		lua_pop(L, 2);
		return 1;
	}
	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);
#endif
	return 1;
}

/*************************************************************************
    Pushes a named function on the stack
    This whole method is pilfered from the CEGUI project. (CEGUILuaFunctor.cpp)
*************************************************************************/
void LuaHelper::pushNamedFunction(lua_State* state, const std::string luaMethod)
{
//     int top = lua_gettop(state);

    // do we have any dots in the handler name? if so we grab the function as a table field
    std::string::size_type i = luaMethod.find_first_of('.');
    if (i!=std::string::npos)
    {
        // split the rest of the string up in parts seperated by '.'
        // TODO: count the dots and size the vector accordingly from the beginning.
        std::vector<std::string> parts;
        std::string::size_type start = 0;
        do
        {
            parts.push_back(luaMethod.substr(start,i-start));
            start = i+1;
            i = luaMethod.find_first_of('.',start);
        } while(i!=std::string::npos);

        // add last part
        parts.push_back(luaMethod.substr(start));

        // first part is the global
        lua_getglobal(state, parts[0].c_str());
        if (!lua_istable(state,-1))
        {
//             lua_settop(state,top);
            throw Ember::Exception("Unable to get the Lua event handler: '"+luaMethod+"' as first part is not a table");
        }

        // if there is more than two parts, we have more tables to go through
        std::vector<std::string>::size_type visz = parts.size();
        if (visz-- > 2) // avoid subtracting one later on
        {
            // go through all the remaining parts to (hopefully) have a valid Lua function in the end
            std::vector<std::string>::size_type vi = 1;
            while (vi<visz)
            {
                // push key, and get the next table
                lua_pushstring(state,parts[vi].c_str());
                lua_gettable(state,-2);
                if (!lua_istable(state,-1))
                {
//                     lua_settop(L,top);
					std::stringstream ss;
					ss <<(vi+1);
                    throw  Ember::Exception("Unable to get the Lua event handler: '"+luaMethod+"' as part #"+ss.str()+" ("+parts[vi]+") is not a table");
                }
                // get rid of the last table and move on
                lua_remove(state,-2);
                vi++;
            }
        }

        // now we are ready to get the function to call ... phew :)
        lua_pushstring(state,parts[visz].c_str());
        lua_gettable(state,-2);
        lua_remove(state,-2); // get rid of the table
    }
    // just a regular global function
    else
    {
        lua_getglobal(state, luaMethod.c_str());
    }

    // is it a function
	if ( !lua_isfunction(state,-1) )
	{
		throw Ember::Exception( "\"" + luaMethod + "\" does not represent a Lua function" );
	}
}

}

}
