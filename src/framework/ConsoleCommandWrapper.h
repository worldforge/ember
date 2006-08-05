//
// C++ Interface: ConsoleCommandWrapper
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#ifndef EMBERCONSOLECOMMANDWRAPPER_H
#define EMBERCONSOLECOMMANDWRAPPER_H

#include <string>

namespace Ember {
class ConsoleObject;
/**
	@author Erik Hjortsberg <erik@katastrof.nu>
	
	A utility wrapper object for a console command. Use this on classes that have console commands. It will register and deregister itself on construction and destruction.
*/
class ConsoleCommandWrapper {
public:

    /**
     * Constructor.
     * @param command The console command.
     * @param object The object instance.
     * @return 
     */
    ConsoleCommandWrapper(std::string command, ConsoleObject *object);
    
    /**
     * Constructor.
     * @param command The console command.
     * @param object The object instance.
     * @param description Description of the command.
     * @return 
     */
    ConsoleCommandWrapper(std::string command, ConsoleObject *object, std::string description);

    ~ConsoleCommandWrapper();
    
    /**
     * Gets the command.
     * @return 
     */
    inline const std::string& getCommand() const;
    
    /**
     * Gets the description of the command.
     * @return 
     */
    inline const std::string& getDescription() const;
    
	inline bool operator==( const std::string& command) const;
    
private:

	std::string mCommand;
	std::string mDescription;
	ConsoleObject* mObject;
};
    
    
const std::string& ConsoleCommandWrapper::getCommand() const {return mCommand;}
const std::string& ConsoleCommandWrapper::getDescription() const {return mDescription;}
bool ConsoleCommandWrapper::operator==( const std::string& command) const { return command == mCommand; }


}

#endif
