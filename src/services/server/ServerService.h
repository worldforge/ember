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

#include <Atlas/Objects/RootOperation.h>
#include <Eris/Connection.h>
#include <Eris/Account.h>
#include <Eris/Lobby.h>
#include <Eris/View.h>
#include <Eris/Avatar.h>

// #include <sigc++/object.h>
// #include <sigc++/signal.h>
// #include <sigc++/slot.h>
// #include <sigc++/bind.h>
#include <sigc++/object_slot.h>

namespace Ember {

/**
 * Ember Server Service
 *
 * @author Martin Pollard (Xmp)
 *
 * @see Ember::Service
 * @see Ember::MetaserverService
 * @see Ember::ConsoleObject
 */
class ServerService : public Service, public ConsoleObject, public sigc::trackable
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
    Eris::Account *myAccount;

    /**
     * Holds the world object of this server
     */
    Eris::View *myView;

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
	
    void takeCharacter(const std::string &id);

	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description);

    void runCommand(const std::string &, const std::string &);

    Eris::View* getView()
      {
	return myView;
      }
      
	void moveToPoint(const WFMath::Point<3>& dest); 
	void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation);
	void moveInDirection(const WFMath::Vector<3>& velocity);
	void say(const std::string &message);
	void touch(Eris::Entity* entity);
	void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset);
	void place(Eris::Entity* entity, Eris::Entity* target);
	void wield(Eris::Entity* entity);
	void take(Eris::Entity* entity);
	void use(Eris::Entity* entity, WFMath::Point<3> pos = WFMath::Point<3>(0,0,0));
	//void use(Eris::Entity* entity);

    //----------------------------------------------------------------------
	// Signals
	sigc::signal<void, Eris::Avatar*> GotAvatar;
	sigc::signal<void, Eris::View*> GotView;
	sigc::signal<void, Eris::Connection*> GotConnection;
	sigc::signal<void, Eris::Account*> GotAccount;
	sigc::signal<void, Eris::Account *> LoginSuccess;
	sigc::signal<void, Eris::Account *, const std::string &> LoginFailure;
	sigc::signal<void, const Atlas::Objects::Entity::RootEntity &> GotCharacterInfo;
	sigc::signal<void, Eris::Account *> GotAllCharacters;


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

    // Account Callbacks

	void gotAvatarSuccess(Eris::Avatar* avatar);

    void gotCharacterInfo(const Atlas::Objects::Entity::RootEntity &);

    void gotAllCharacters();

//    void loginFailure(Eris::LoginFailureType, const std::string &);
    void loginFailure(const std::string &);

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
	static const char * const SAY;
	static const char * const TOUCH;	
}; //ServerService

} // namespace Ember

#endif
