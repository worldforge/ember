// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).

// Rewritten for Dime by Martin Pollard (Xmp)

// Originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002 

#ifndef CONSOLEOBJECT_H
#define CONSOLEOBJECT_H

#include <string>

namespace dime {
/**
 * The ConsoleObject is a interface used to allow objects to register commands
 * with the console. Any object wishing to register a command, must implement 
 * this interface.
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
   * command is a command that has been previously registered with the console
   * args is the argument string that has been provided for the command
   */ 
  virtual void runCommand(const std::string &command, const std::string &args) = 0;
};

} /* namespace dime */
#endif /* CONSOLEOBJECT_H */
