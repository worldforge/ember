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
#include "Connectors.h"

namespace EmberOgre {

/**
@author Erik Hjortsberg
*/
class LuaConsoleObject : public Ember::ConsoleObject
{
public:
    
    LuaConsoleObject(const std::string& command, const std::string& luaMethod, const std::string& description = "");

    ~LuaConsoleObject();


	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command 
	 * @param args 
	 */
	virtual	void runCommand(const std::string &command, const std::string &args);
private:
	std::string mCommand;
	std::string mLuaMethod;
	LuaConnectors::TemplatedConnectorBase<LuaConnectors::StringValueAdapter, LuaConnectors::StringValueAdapter>* mConnector;
	const Ember::ConsoleCommandWrapper mCommandWrapper;
};

}

#endif
