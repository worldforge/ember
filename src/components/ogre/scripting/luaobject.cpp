// luaobject.h
//
// Copyright 2008 by Nigel Atkinson suprapilot+LuaCode@gmail.com 
//
//    This library is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    A copy of theGNU General Public License is availible at:
//    <http://www.gnu.org/licenses/>.
// 
// Makes interfacing with Lua easier.  
// Tested with Lua 5.1.3 and 5.1.2.
// MSVC 7.12 (2003) and GCC (g++) 4.1.2

// 8th May 2008 Nigel
// Added LuaException class and changed all throws to use it.  User code can use it too.
// It grabs any string, if any, from the top of the Lua stack.


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "luaobject.h"

void LuaVal::push( lua_State *L )
{
	switch( mType )
	{
	case LUA_TNIL:
		lua_pushnil( L );
		break;

	case LUA_TNUMBER:
		lua_pushnumber( L, d );
		break;

	case LUA_TSTRING:
		lua_pushstring( L, str.c_str() );
		break;

	case LUA_TFUNCTION:
		lua_pushcfunction( L, func );
		break;

	case LUA_TLIGHTUSERDATA:
		obj->push();
		break;

	default:
		throw LuaException( L, "LuaVal got messed up somehow.", __FILE__, __LINE__ );
	}
}

