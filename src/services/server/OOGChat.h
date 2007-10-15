/*
 Copyright (C) 2002  Martin Pollard
	
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

#ifndef OOGCHAT_H
#define OOGCHAT_H

// Included headers from the current project
#include "framework/ConsoleObject.h"

// Included custom library headers
#include <Eris/Lobby.h>
#include <Eris/Room.h>
#include <Eris/Account.h>
#include <sigc++/object.h>
#include <sigc++/bind.h>

// Included system headers

namespace Ember {

/**
 * Eris Lobby management object.
 *
 * This class holds the Eris::Lobby object for the current session
 * and handles it's callback, etc.
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
 * @see Ember::ServerService
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class OOGChat : public ConsoleObject
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

    // List of OOGChat's console commands
    static const char * const CMD_TALK;
    static const char * const CMD_EMOTE;
    static const char * const CMD_ME;
    static const char * const CMD_JOIN;
    static const char * const CMD_PART;
    static const char * const CMD_MSG;

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

    /**
     * Holds the lobby of this server
     */
    Eris::Lobby* myLobby;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new OOGChat using default values.
     */
    OOGChat(Eris::Account* account);

    /**
     * Copy constructor.
     */
    OOGChat( const OOGChat &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    OOGChat &operator= ( const OOGChat &source )
    {
        // Copy fields from source class to this class here.
        myLobby = source.myLobby;

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a OOGChat instance.
     */
    virtual ~OOGChat ();


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of Lobby of this OOGChat
     */
    Eris::Lobby* getLobby() const
    {
        return myLobby;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Lobby of this OOGChat
     */
    void setLobby( Eris::Lobby* lobby )
    {
        myLobby = lobby;
    }


    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and crate a separator comment like above for them.
    
    //======================================================================
    // Protected Methods
    //======================================================================
    protected:

    /**
     * Command handler for console backend.
     */
    void runCommand(const std::string &command, const std::string &args);

    //----------------------------------------------------------------------
    // Callbacks from Eris

    // Lobby Callbacks

    void sightPerson(Eris::Person*);

    void privateTalk(Eris::Person* person, const std::string&); 

    void loggedIn( const Atlas::Objects::Entity::Player& );

    void entered(Eris::Room *room);

    void talk(Eris::Room *room, Eris::Person* person, const std::string& msg);

    void emote(Eris::Room *room, Eris::Person* person, const std::string& msg);

    void appearance(Eris::Room *room,  Eris::Person* person);

    void disappearance(Eris::Room *room,  Eris::Person* person);

    void changed(const Eris::StringSet& sset, Eris::Room *room);


    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of OOGChat

} // End of Ember namespace

#endif
