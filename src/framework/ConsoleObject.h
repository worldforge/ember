/*
    Copyright (C) 2002  Martin Pollard (Xmp), Simon Goodall

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

// Rewritten for Ember by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#ifndef CONSOLEOBJECT_H
#define CONSOLEOBJECT_H

// Included headers from the current project
#include "framework/ConsoleCommandWrapper.h"

// Included custom library headers

// Included system headers
#include <string>

namespace Ember {
/**
 * ConsoleObject
 *
 * The ConsoleObject is an interface used to allow objects to register commands
 * with the console. Any object wishing to register a command, must implement 
 * this interface.
 *
 * class SomeClass : public ConsoleObject {
 * void runCommand(const std::string &command, const std::string &args);
 * }
 *
 * @author Xmp (Martin Pollard)
 *
 * @see Console
 */ 
class ConsoleObject {
public:
  /**
   * Default constructor
   */ 	
  ConsoleObject() {}

  /**
   * Default destructor
   */ 
  virtual ~ConsoleObject() {}

  /**
   * This is the function that needs to be extended to use the console.
   * "command" is a command that has been previously registered with the console
   * "args" is the argument string that has been provided for the command
   */ 
  virtual void runCommand(const std::string& command, const std::string& args) = 0;
};

} /* namespace Ember */
#endif /* CONSOLEOBJECT_H */
