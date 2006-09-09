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

#include <sigc++/signal.h>
#include <sigc++/object_slot.h>

// Included system headers
#include <deque>
#include <list>
#include <map>
#include <set>
#include <string>

namespace Ember {

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
 * @see Ember::Console
 * @see Ember::ConsoleObject
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
    
	struct ConsoleObjectEntry
	{
		ConsoleObject* Object;
		std::string Description;
	};

	typedef std::map<std::string, ConsoleObjectEntry> ConsoleObjectEntryStore;

    static const unsigned int MAX_MESSAGES;

    // List of ConsoleBackend's console commands
    static const char * const LIST_CONSOLE_COMMANDS;

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

    /**
     * Instance variable for singleton main Ember console.
     */
    static ConsoleBackend* theMainConsole;

    /**
     * Mapping of registered commands to associated object.
     */
    ConsoleObjectEntryStore myRegisteredCommands;

    /**
     * Current console messages
     */
    std::list< std::string > myConsoleMessages;
	
	/**
	 * Message history.
	 **/
	std::deque< std::string > mHistory;
	
	/**
	 * History iterator.
	 **/
	size_t mHistoryPosition;
	
    /**
     * Prefix map for commands.
     **/
    std::map< std::string, std::set< std::string > > mPrefixes;
    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new ConsoleBackend using default values.
     */
	ConsoleBackend(void);

    /**
     * Copy constructor.
     */
    ConsoleBackend( const ConsoleBackend &source );


    /**
     * Assignment operator.
     */
    ConsoleBackend &operator= ( const ConsoleBackend &source );


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a ConsoleBackend instance.
     */
    virtual ~ConsoleBackend ();


    //----------------------------------------------------------------------
    // Getters

    /**
     * Gets an instance of the main Ember console
     */
    inline static ConsoleBackend* getMainConsole();

    inline const std::list<std::string>& getConsoleMessages() const;

	
	inline size_t getHistoryPosition() const;
	
	const std::set< std::string > & getPrefixes(const std::string & prefix) const;
	
	/**
	 * Get the current history string.
	 * The history position 0 is managed in the ConsoleWidget.
	 **/
	const std::string & getHistoryString();
	
	void changeHistory(size_t stHistoryIndex, const std::string & sCommand);


    //----------------------------------------------------------------------
    // Setters
	
	/**
	 * Moves the history iterator backwards (in time).
	 **/
	void moveBackwards(void);
	
	/**
	 * Moves the history iterator forwards (in time).
	 **/
	void moveForwards(void);


    //----------------------------------------------------------------------
    // Other public methods

    /**
     * Registers a command with the console
     * command is the command to register
     * object is the originating object
     */ 
    void registerCommand(const std::string &command, ConsoleObject *object, const std::string& description = "");

    /**
     * Deregisters a command with the console
     * command is the command to deregister
     */ 
    void deregisterCommand(const std::string &command);

    /**
     * This is the method the determines what object the pass the command onto
	 * 
     * @param command the command string to process
     */ 
    void runCommand(const std::string &command, bool addToHistory = true);
	
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
    

    //------------------------------------
    // Events
 
    
    /**
     * This event is raised every time a message is pushed to the console.
     * If True is returned, the message won't be saved in myConsoleMessages 
     */ 
    sigc::signal<bool, const std::string&> GotMessage;

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:
    
    virtual bool onGotMessage(const std::string &message);


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of ConsoleBackend

ConsoleBackend* ConsoleBackend::getMainConsole()
{
	if ( !theMainConsole )
	theMainConsole = new ConsoleBackend();
	return theMainConsole;
}

const std::list<std::string>& ConsoleBackend::getConsoleMessages() const
{
	return myConsoleMessages;
}

size_t ConsoleBackend::getHistoryPosition() const
{
	return mHistoryPosition;
}


} // End of Ember namespace

#endif
