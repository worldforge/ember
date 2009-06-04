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
#include <framework/ConsoleCommandWrapper.h>

#include <Atlas/Objects/RootOperation.h>
#include <Eris/BaseConnection.h>

#include <sigc++/object_slot.h>

#include "IServerAdapter.h"

namespace Eris
{
	class Avatar;
	class Connection;
	class View;
	class Lobby;
	class Account;
}

namespace Ember {

	class OOGChat;

/**
 * Ember Server Service
 *
 * @author Martin Pollard (Xmp)
 *
 * @see Ember::Service
 * @see Ember::MetaserverService
 * @see Ember::ConsoleObject
 */
class ServerService : public Service, public ConsoleObject
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:

    /** 
     * Holds our connection to the server
     */
    Eris::Connection *mConn;

    /**
     * Holds the player object we are connected with
     */
    Eris::Account *mAccount;

    /**
     * Holds the world object of this server
     */
    Eris::View *mView;

	/**
     * Holds the current avatar
     */
	Eris::Avatar *mAvatar;

    /**
     * Contains the class that controls Out of Game Chat
     */
    OOGChat *mOOGChat;

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
    bool mConnected;


    //----------------------------------------------------------------------
    // Constructors & Destructor

  public:
  
  //HACK!!!
  //this is only for testing purposes
  Eris::Connection* getConnection() { return mConn; }

    /** Creates a new ServerService using default values. */
    ServerService();


    /** Deletes a ServerService instance. */
    ~ServerService();

    //----------------------------------------------------------------------
    // Getters & Setters
    bool isConnected() const;

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

	Eris::View* getView();
	Eris::Avatar* getAvatar();

      
	/**
	 *    Tells the server to try to move the user to the specified point.
	 * @param dest 
	 */
	void moveToPoint(const WFMath::Point<3>& dest); 
	/**
	 *    Moves the user.
	 * @param velocity The velocity with which to move the user.
	 * @param orientation The user's orientation.
	 */
	void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation);
	/**
	 *    Moves the user.
	 * @param velocity The velocity with which to move the user.
	 */
	void moveInDirection(const WFMath::Vector<3>& velocity);
	/**
	 *    Teleports the avatar to the specified location.
	 *    NOTE: This will only work if the user is logged in as admin.
	 * @param dest The destination coords.
	 * @param entity The location entity. In most cases this will be the world entity.
	 */
// 	void teleportTo(const WFMath::Point<3>& dest, Eris::Entity* entity);
	/**
	 *    Say something out loud.
	 * @param message 
	 */
	void say(const std::string &message);
	/**
	 *    Touch another entity.
	 * @param entity 
	 */
	void touch(Eris::Entity* entity);
	
	/**
	 *    Emotes something.
	 * @param entity 
	 */
	void emote(const std::string& emote);
	void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset = WFMath::Vector<3>(0,0,0));
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos = WFMath::Point<3>(0, 0, 0));
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient);
	void wield(Eris::Entity* entity);
	void take(Eris::Entity* entity);
	/**
	 *    Uses the currently wielded entity on the supplied entity.
	 * @param entity The entity on which the currently wielded entity will be used.
	 * @param pos The position on the entity to perform the action. In some cases this won't matter, whereas in others (such as digging on the world entity) it will.
	 * @param operation The operation to perform. If left blank the default will be used.
	 */
	void use(Eris::Entity* entity, WFMath::Point<3> pos = WFMath::Point<3>(0,0,0), const std::string& operation = "");
	/**
	 *    Stops the current use operation.
	 */
	void useStop();
	
	void actuate(Eris::Entity* entity,  const std::string& action);
	void attack(Eris::Entity* entity);
	void eat(Eris::Entity* entity);
	void deleteEntity(Eris::Entity* entity);
	void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes);
	void adminTell(const std::string& entityId, const std::string& attribute, const std::string &value);
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

	void gotAvatarDeactivated(Eris::Avatar* avatar);
	
    void gotCharacterInfo(const Atlas::Objects::Entity::RootEntity &);

    void gotAllCharacters();

//    void loginFailure(Eris::LoginFailureType, const std::string &);
    void loginFailure(const std::string &);

    void loginSuccess();

    void logoutComplete(bool);

	// List of ServerService's console commands
	
	const Ember::ConsoleCommandWrapper Connect;
	//const Ember::ConsoleCommandWrapper ReConnect;
	const Ember::ConsoleCommandWrapper DisConnect;
	const Ember::ConsoleCommandWrapper CreateAcc;
	const Ember::ConsoleCommandWrapper Login;
	const Ember::ConsoleCommandWrapper Logout;
	const Ember::ConsoleCommandWrapper CreateChar;
	const Ember::ConsoleCommandWrapper TakeChar;
	const Ember::ConsoleCommandWrapper ListChars;
	const Ember::ConsoleCommandWrapper Say;
	const Ember::ConsoleCommandWrapper Emote;
	const Ember::ConsoleCommandWrapper Delete;
	const Ember::ConsoleCommandWrapper AdminTell;
	
	IServerAdapter* mServerAdapter;
};

inline bool ServerService::isConnected() const
{
	return mConnected;
}

inline Eris::View* ServerService::getView()
{
	return mView;
}

inline Eris::Avatar* ServerService::getAvatar()
{
	return mAvatar;
}


} // namespace Ember

#endif
