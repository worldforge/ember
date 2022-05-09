/*
 Copyright (C) 2022 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "BindingsVarconf.h"
#include "components/lua/Connector.h"

#include <varconf/variable.h>

using namespace varconf;
using namespace Ember::Lua;

void registerBindingsVarconf(sol::state_view& lua) {

	lua.new_usertype<Variable>("varconf::Variable",
							   sol::constructors<Variable(bool), Variable(int), Variable(double), Variable(const std::string&)>(),
							   "as_bool", [](Variable* self) { return (bool) *self; },
							   "as_int", [](Variable* self) { return (int) *self; },
							   "as_double", [](Variable* self) { return (double) *self; },
							   "as_string",&Variable::as_string,
							   "is_bool",&Variable::is_bool,
							   "is_int",&Variable::is_int,
							   "is_double",&Variable::is_double,
							   "is_string",&Variable::is_string,
							   "is_array",&Variable::is_array
	);


}