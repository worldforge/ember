//
// C++ Interface: ConsoleCommandWrapper
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBERCONSOLECOMMANDWRAPPER_H
#define EMBERCONSOLECOMMANDWRAPPER_H

#include <string>
#include "ConsoleObject.h"

namespace Ember {
class ConsoleBackend;
/**
@author Erik Ogenvik <erik@ogenvik.org>

@brief An utility wrapper object for a console command.
Use this on classes that have console commands. It will register and deregister itself on construction and destruction.
*/
class ConsoleCommandWrapper {
public:


    /**
     * Constructor.
     * @param command The console command. There are mainly two different kinds of command: normal commands and "toggle" commands. The latter are identified by always starting with "+", such as "+move_forward". If a "toggle" command is used, an "inverse command" will automatically be generated, accessable through getInverseCommand(). This is meant to be used to act as an inverse of the main command.
     * @param object The object instance. This will receive notifications when the command (and if available the inverse command) is used.
     * @param description Description of the command, to be presented to the end user.
     * @param suppressLogging If true, logging of the registration and deregistration will be suppressed. This is useful if you have a command which you'll add and remove often.
    */
	ConsoleCommandWrapper(std::string command, ConsoleObject *object, std::string description = "", bool suppressLogging = false);

	ConsoleCommandWrapper(ConsoleBackend& consoleBackend, std::string command, ConsoleObject *object, std::string description = "", bool suppressLogging = false);
	ConsoleCommandWrapper(ConsoleBackend& consoleBackend, std::string command, ConsoleCallback callback, const std::string& description = "", bool suppressLogging = false);

    /**
     * @brief Dtor.
     */
    virtual ~ConsoleCommandWrapper();

    /**
     * @brief Gets the command.
     * @return The command which this wrapper represents.
     */
    const std::string& getCommand() const;

    /**
     * @brief Gets the inverse command, i.e. the command used to invert the action of the command.
     * This is only relevant when dealing with commands prefixed with "+", i.e. commands that will require a similiar command prefixed by "-" to stop what they're doing. An example of this is the "+move_forward" command which will move the avatar forward until a "-move_forward" command is received.
     * This is most often used in conjunction with the InputCommandMapper class, but it can of course be used by other mechanisms as well.
     *
     * Note that an inverse command is automatically generated whenever a "+" command is used.
     * @return The inverse command for the current command.
     */
    const std::string& getInverseCommand() const;

    /**
     * Gets the description of the command.
     * @return
     */
    const std::string& getDescription() const;

	bool operator==( const std::string& command) const;

private:

	/**
	The command to which this wrapper is bound to.
	*/
	std::string mCommand;

	/**
	The inverse command which could be used to stop the action invoked by the main command.
	This is optional since not all commands will have inverse commands.
	*/
	std::string mInverseCommand;

	/**
	A description of the command, to be shown for the end user.
	*/
	std::string mDescription;

	/**
	 * @brief If true, logging of the registration and deregistration will be suppressed.
	 * This is useful if you have a command which you'll add and remove often.
	 */
	bool mSuppressLogging;
};


inline const std::string& ConsoleCommandWrapper::getCommand() const {return mCommand;}
inline const std::string& ConsoleCommandWrapper::getInverseCommand() const {return mInverseCommand;}
inline const std::string& ConsoleCommandWrapper::getDescription() const {return mDescription;}
inline bool ConsoleCommandWrapper::operator==( const std::string& command) const { return command == mCommand; }


}

#endif
