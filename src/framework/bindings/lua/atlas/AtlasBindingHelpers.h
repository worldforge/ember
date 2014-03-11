//
// C++ Interface: required
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#define __operator_dereference  operator*
#define __operator_index  operator[]

#include <Atlas/Message/Element.h>
#include <cstdlib>

/**
Utility method for the lua bindings which looks for a entry in a const map. If no entry is found we'll return null, which will be translated to "nil" in lua.
*/
const Atlas::Message::Element& _MapType_findInMap(const Atlas::Message::MapType* map, const std::string& key) {
	Atlas::Message::MapType::const_iterator I = map->find(key);
	if (I != map->end()) {
		return I->second;
	} else {
		///we'll avoid compiler warnings by doing it this way
		static Atlas::Message::Element* element(0);
		return *element;
	}
}

/**
 * Method that checks, is Lua variable has number type exactly, not a string
 */
TOLUA_API int tolua_isnumber_type (lua_State* L, int lo, int def, tolua_Error* err)
{
	if (def && lua_gettop(L)<abs(lo))
		return 1;
	if (lua_type(L,lo) == LUA_TNUMBER)
		return 1;
	err->index = lo;
	err->array = 0;
	err->type = "number";
	return 0;
}

/**
 * Method that checks, is Lua variable has string type exactly, not a number
 */
TOLUA_API int tolua_isstring_type (lua_State* L, int lo, int def, tolua_Error* err)
{
	if (def && lua_gettop(L)<abs(lo))
		return 1;
	if (lua_type(L,lo) == LUA_TSTRING)
		return 1;
	err->index = lo;
	err->array = 0;
	err->type = "string";
	return 0;
}

#define tolua_iscppstring_type   tolua_isstring_type
