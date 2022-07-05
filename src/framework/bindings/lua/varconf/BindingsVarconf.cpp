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
	auto varconf = lua["varconf"].get_or_create<sol::table>();

	varconf["GLOBAL"] = varconf::GLOBAL;
	varconf["USER"] = varconf::USER;
	varconf["INSTANCE"] = varconf::INSTANCE;
	auto variable = varconf.new_usertype<Variable>("Variable",
												   sol::constructors<Variable(bool), Variable(int), Variable(double), Variable(const std::string&)>());
	variable["as_bool"] = [](Variable* self) { return (bool) *self; };
	variable["as_int"] = [](Variable* self) { return (int) *self; };
	variable["as_double"] = [](Variable* self) { return (double) *self; };
	variable["as_string"] = &Variable::as_string;
	variable["is_bool"] = &Variable::is_bool;
	variable["is_int"] = &Variable::is_int;
	variable["is_double"] = &Variable::is_double;
	variable["is_string"] = &Variable::is_string;
	variable["is_array"] = &Variable::is_array;


}