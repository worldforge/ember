//
// C++ Implementation: LuaConnectorHelper
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *   Copyright (C) 2207 Erik Hjortsberg <erik.hjortsberg@iteam.se>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LuaConnectorHelper.h"
#include "framework/Exception.h"
#include <sstream>

namespace EmberOgre {

namespace LuaConnectors {

// LuaConnectorHelper::LuaConnectorHelper()
// {
// }
// 
// 
// LuaConnectorHelper::~LuaConnectorHelper()
// {
// }

/*************************************************************************
    Pushes a named function on the stack
    This whole method is pilfered from the CEGUI project. (CEGUILuaFunctor.cpp)
*************************************************************************/
void LuaConnectorHelper::pushNamedFunction(lua_State* state, const std::string luaMethod)
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
