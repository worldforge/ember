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

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/bind.h>
#include <sigc++/object_slot.h>
#endif
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
  
  //HACK!!!
  //this is only for testing purposes
  Eris::Connection* getConnection() { return myConn; }

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
      
    void moveToPoint(const WFMath::Point<3>& dest); 
    void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation);
    void moveInDirection(const WFMath::Vector<3>& velocity);
   	void touch(Eris::Entity* entity);
   	
    //----------------------------------------------------------------------
   	// Signals
   	SigC::Signal1<void, Eris::Avatar*> GotAvatar;
   	SigC::Signal1<void, Eris::World*> GotWorld;
   	SigC::Signal1<void, Eris::Connection*> GotConnection;

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
    static const char * const CONNECT;
    static const char * const RECONNECT;
    static const char * const DISCONNECT;
    static const char * const CREATEACC;
    static const char * const LOGIN;
    static const char * const LOGOUT;
    static const char * const CREATECHAR;
    static const char * const TAKECHAR;
    static const char * const LISTCHARS;
    static const char * const TOUCH;	
}; //ServerService

} // namespace dime

#endif
