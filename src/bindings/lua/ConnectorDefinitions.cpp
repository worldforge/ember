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

#include "ConnectorDefinitions.h"
#include "TypeResolving.h"
#include "components/lua/Connector.h"
#include <sigc++/signal.h>
#include <sigc++/trackable.h>
#include <sigc++/connection.h>

struct Foo {
	Foo(int bar) : i(bar) {
	}

	int i;
	sigc::signal<void> aSignal;
};

struct FooConnect {
	sigc::signal<void>& signal;

	Ember::Lua::Connector* connect() {
		return Ember::Lua::Connector::createConnector(signal);
	}
};

void registerConnectorDefinitions(sol::state_view& lua) {

	lua.set_function("foobar", [](sol::object o) {
		auto type = o.get_type();
		auto isType = o.is<sigc::signal<void>>();
		int i = 0;
	});

//	lua.set_function("_createConnector", [](sol::object o){o.get_type()>()});
	//lua.new_simple_usertype<sigc::signal<void, Eris::Avatar*>>("sigc::signal<void, Eris::Avatar*>");
	lua.new_usertype<Foo>("Foo",
						  sol::constructors<Foo(int)>(),
						  "i", &Foo::i,
						  "aSignal", sol::property([](Foo* self) {
				return std::make_shared<Ember::Lua::Connector>(self->aSignal);
			}),
						  "emitSignal", [](Foo* foo) {
				foo->aSignal();
			});

//	lua.new_usertype<sigc::signal<void, Eris::Avatar*>>("sigc::signal<void, Eris::Avatar*>", "connect", [](){});
}