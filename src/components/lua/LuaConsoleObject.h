//
// C++ Interface: LuaConsoleObject
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifndef EMBEROGRELUACONSOLEOBJECT_H
#define EMBEROGRELUACONSOLEOBJECT_H
#include "framework/ConsoleObject.h"
#include "components/lua/Connectors.h"

#include <tolua++.h>

namespace Ember {
namespace Lua {

/**
 * @author Erik Hjortsberg
 *
 * @brief A console object which can easily be created in Lua, and will perform call back into Lua when activated.
 *
 * Use an instance of this whenever you want to register console commands from Lua.
 *
 */
class LuaConsoleObject : public Ember::ConsoleObject
{
public:
    
	/**
	 * @brief Ctor.
	 * @param command The console command to register.
	 * @param luaMethod A lua method to call when the command is executed.
	 * @param description An optional description of the console command.
	 */
    LuaConsoleObject(const std::string& command, const std::string& luaMethod, const std::string& description = "");

    /**
	 * @brief Ctor.
	 * @param command The console command to register.
	 * @param luaMethod A lua method to call when the command is executed.
	 * @param description An optional description of the console command.
	 */
    LuaConsoleObject(const std::string& command, lua_Object luaMethod, const std::string& description = "");

    /**
     * @brief Dtor.
     */
    ~LuaConsoleObject();


	/**
	 * @brief Reimplements the ConsoleObject::runCommand method.
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);

	/**
	 * @brief Sets a "self" reference which will be prepended to any lua call.
	 *
	 * @param selfIndex The lua index of the self reference.
	 */
	LuaConsoleObject* setSelf(lua_Object selfIndex);

private:

	/**
	 * @brief The connector which points to a Lua function.
	 *
	 * This is called when the command is executed.
	 */
	Ember::Lua::TemplatedConnectorBase<Ember::Lua::StringValueAdapter, Ember::Lua::StringValueAdapter>* mConnector;

	/**
	 * @brief The console command registered.
	 */
	const Ember::ConsoleCommandWrapper mCommandWrapper;
};

}
}

#endif
