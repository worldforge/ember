/*
 Copyright (C) 2002  Martin Pollard, Simon
	
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

#ifndef CONSOLEBACKEND_H
#define CONSOLEBACKEND_H

// Included headers from the current project
#include "ConsoleObject.h"
#include "Tokeniser.h"

// Included custom library headers

// Included system headers
#include <string>
#include <list>
#include <map>

namespace dime {

/**
 * Backend storage class for Console's.
 *
 * More detailed description of the class, it's purpose, what it does,
 * how to use it and so on.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * @see dime::Console
 * @see dime::ConsoleObject
 *
 */

class ConsoleBackend : public ConsoleObject
{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:


    //======================================================================
    // Public Constants
    //======================================================================
    public:

    //======================================================================
    // Private Constants
    //======================================================================
    private:
    
    static const unsigned int MAX_MESSAGES = 7;

    // List of ConsoleBackend's console commands
    static const char * const LIST_CONSOLE_COMMANDS;

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

    /**
     * Instance variable for singleton main dime console.
     */
    static ConsoleBackend* theMainConsole;

    /**
     * Mapping of registered commands to associated object.
     */
    std::map<std::string, ConsoleObject*> myRegisteredCommands;

    /**
     * Current console messages
     */
    std::list<std::string> myConsoleMessages;
  
    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new ConsoleBackend using default values.
     */
     ConsoleBackend() :
       myRegisteredCommands(std::map<std::string, ConsoleObject*>()),
       myConsoleMessages(std::list<std::string>())
     {
       // Register console commands
       registerCommand(LIST_CONSOLE_COMMANDS, this);
     }

    /**
     * Copy constructor.
     */
    ConsoleBackend( const ConsoleBackend &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    ConsoleBackend &operator= ( const ConsoleBackend &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a ConsoleBackend instance.
     */
    virtual ~ConsoleBackend ()
    {
        // TODO: Free any allocated resources here.
    }


    //----------------------------------------------------------------------
    // Getters

    /**
     * Gets an instance of the main dime console
     */
    static ConsoleBackend* getMainConsole()
    {
      if ( !theMainConsole )
        theMainConsole = new ConsoleBackend();
      return theMainConsole;
    }

    const std::list<std::string>& getConsoleMessages() const
      {
	return myConsoleMessages;
      }


    //----------------------------------------------------------------------
    // Setters


    //----------------------------------------------------------------------
    // Other public methods

    /**
     * Registers a command with the console
     * command is the command to register
     * object is the originating object
     */ 
    void registerCommand(const std::string &command, ConsoleObject *object);

    /**
     * Deregisters a command with the console
     * command is the command to deregister
     */ 
    void deregisterCommand(const std::string &command);

    /**
     * This is the method the determines what object the pass the command onto
     * command is the command string to process
     */ 
    void runCommand(const std::string &command);
    
    /**
     * Add a message to the console message queue.
     * message is the message string
     */ 
    void pushMessage(const std::string &message);

    /**
     * This is the ConsoleObject method.
     * command is the command to run
     * args is the commands arguments
     */ 
    void runCommand(const std::string &command, const std::string &args);

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of ConsoleBackend

} // End of dime namespace

#endif
