/*
    Copyright (C) 2002  Martin Pollard (Xmp)

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

#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include <framework/Service.h>
#include <framework/ConsoleObject.h>
#include "OOGChat.h"

#include <Eris/Connection.h>
#include <Eris/Player.h>
#include <Eris/Lobby.h>
#include <Eris/World.h>
#include <Eris/Avatar.h>

#include <sigc++/object.h>

namespace dime {

/**
 * Dime Server Service
 *
 * @author Martin Pollard (Xmp)
 *
 * @see dime::Service
 * @see dime::MetaserverService
 * @see dime::ConsoleObject
 */
class ServerService : public Service, public ConsoleObject,
  virtual public SigC::Object
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /** 
     * Holds our connection to the server
     */
    Eris::Connection *myConn;

    /**
     * Holds the player object we are connected with
     */
    Eris::Player *myPlayer;

    /**
     * Holds the world object of this server
     */
    Eris::World *myWorld;

	/**
     * Holds the current avatar
     */
	Eris::Avatar *myAvatar;

    /**
     * Contains the class that controls Out of Game Chat
     */
    OOGChat *myOOGChat;

    /**
     * The host we are connected/ing to at present
     */
    std::string myHost;

    /**
     * The port we are using to connect
     */
    short myPort;

    /**
     * True if and only if we are successfully connected to the server
     */
    bool myConnected;


    //----------------------------------------------------------------------
    // Constructors & Destructor

  public:

    /** Creates a new ServerService using default values. */
    ServerService();


    /** Deletes a ServerService instance. */
    ~ServerService();

    //----------------------------------------------------------------------
    // Getters & Setters
    bool isConnected() const
      {
        return myConnected;
      }

    //----------------------------------------------------------------------
    // Methods

    Service::Status start();

    void stop(int code);

    bool connect(const std::string& host, short port = 6767);

    void reconnect();

    void disconnect();

    void runCommand(const std::string &, const std::string &);

    Eris::World* getWorld()
      {
	return myWorld;
      }

    //----------------------------------------------------------------------
    // Callbacks from Eris
  private:

    // Connection Callbacks

    void gotFailure(const std::string& msg);

    void connected();

    bool disconnecting();

    void disconnected();

    void statusChanged(Eris::BaseConnection::Status);

    void timeout(Eris::BaseConnection::Status);

    // Player Callbacks

    void gotCharacterInfo(const Atlas::Objects::Entity::GameEntity &);

    void gotAllCharacters();

    void loginFailure(Eris::LoginFailureType, const std::string &);

    void loginSuccess();

    void logoutComplete(bool);

    // List of ServerService's console commands
    static const char * const CONNECT = "connect";
    static const char * const RECONNECT = "reconnect";
    static const char * const DISCONNECT = "disconnect";
    static const char * const CREATEACC = "createacc";
    static const char * const LOGIN = "login";
    static const char * const LOGOUT = "logout";
    static const char * const CREATECHAR = "createchar";
    static const char * const TAKECHAR = "takechar";
    static const char * const LISTCHARS = "listchars";
}; //ServerService

} // namespace dime

#endif
