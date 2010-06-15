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

namespace Ember
{

class OOGChat;

/**
 * @brief Ember Server Service
 *
 * Handles most interaction with the server.
 *
 * @author Martin Pollard (Xmp)
 *
 * @see Ember::Service
 * @see Ember::MetaserverService
 * @see Ember::ConsoleObject
 */
class ServerService: public Service, public ConsoleObject
{
public:

	/**
	 * @brief Ctor.
	 */
	ServerService();

	/**
	 * @brief Dtor.
	 */
	~ServerService();

	bool isConnected() const;

	Service::Status start();

	void stop(int code);

	bool connect(const std::string& host, short port = 6767);

	void reconnect();

	void disconnect();

	/**
	 * @brief Tries to log the account out from the server.
	 *
	 * @returns True if successful.
	 */
	bool logoutAccount();

	/**
	 * @brief Tries to log the avatar out from the server.
	 *
	 * @returns True if successful.
	 */
	bool logoutAvatar();

	/**
	 * @brief Logs out the user.
	 *
	 * If the user has an avatar, that will be logged out. If the user doesn't have an avatar yet but is logged in through the account, the account will be logged out.
	 */
	bool logout();

	/**
	 * @brief Takes the character with the specified id.
	 *
	 * @param id The id of the character to take.
	 */
	void takeCharacter(const std::string &id);

	/**
	 * @brief Creates a new character on the server.
	 * @param name The name of the new character.
	 * @param sex The sex. In most cases either "male" or "female".
	 * @param type The type. This should correspond to one of the available types as sent from the server earlier.
	 * @param description A short description of the character.
	 * @param spawnName The name of the spawn area to spawn in. This should have been sent from the server earlier. Make sure that the type specified is available for the spawn point. If an empty string is specified the earlier spawn mechanism on the server will be used, which isn't guaranteed to always work.
	 */
	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName);

	void runCommand(const std::string &, const std::string &);

	Eris::View* getView() const;
	Eris::Avatar* getAvatar() const;

	/**
	 * @brief Gets the underlying connection.
	 * @returns A connection instance, or null if we're currently not connected
	 */
	Eris::Connection* getConnection() const;

	/**
	 *    Tells the server to try to move the user to the specified point.
	 * @param dest
	 */
	void moveToPoint(const WFMath::Point<3>& dest);

	/**
	 * @brief Moves the user.
	 * @param velocity The velocity with which to move the user.
	 * @param orientation The user's orientation.
	 */
	void moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation);

	/**
	 * @brief Moves the user.
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
	 * @brief Say something out loud.
	 * @param message
	 */
	void say(const std::string &message);
	/**
	 * @brief Touch another entity.
	 * @param entity
	 */
	void touch(Eris::Entity* entity);

	/**
	 * @brief Emotes something.
	 * @param entity
	 */
	void emote(const std::string& emote);
	void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset = WFMath::Vector<3>::ZERO());
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos = WFMath::Point<3>::ZERO());
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient);
	void wield(Eris::Entity* entity, const std::string& outfitSlot = "");
	void take(Eris::Entity* entity);
	/**
	 * @brief Uses the currently wielded entity on the supplied entity.
	 * @param entity The entity on which the currently wielded entity will be used.
	 * @param pos The position on the entity to perform the action. In some cases this won't matter, whereas in others (such as digging on the world entity) it will.
	 * @param operation The operation to perform. If left blank the default will be used.
	 */
	void use(Eris::Entity* entity, WFMath::Point<3> pos = WFMath::Point<3>::ZERO(), const std::string& operation = "");

	/**
	 * @brief Stops the current use operation.
	 */
	void useStop();

	void actuate(Eris::Entity* entity, const std::string& action);
	void attack(Eris::Entity* entity);
	void eat(Eris::Entity* entity);
	void deleteEntity(Eris::Entity* entity);
	void setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes);
	void adminTell(const std::string& entityId, const std::string& attribute, const std::string &value);

	/**
	 * @brief Creates new type data on the server.
	 * This will only be allowed if you have admin rights on the server.
	 * @param typeInfo The new type info.
	 */
	void createTypeInfo(const Atlas::Objects::Root& typeInfo);

	/**
	 * @brief Update existing type data on the server.
	 * This will only be allowed if you have admin rights on the server.
	 * @param typeInfo The new type info.
	 */
	void setTypeInfo(const Atlas::Objects::Root& typeInfo);

	//void use(Eris::Entity* entity);

	//----------------------------------------------------------------------
	// Signals
	sigc::signal<void, Eris::Avatar*> GotAvatar;
	sigc::signal<void, Eris::View*> GotView;
	sigc::signal<void, Eris::Connection*> GotConnection;
	sigc::signal<void, Eris::Account*> GotAccount;
	sigc::signal<void, Eris::Account*> LoginSuccess;
	sigc::signal<void, Eris::Account*, const std::string&> LoginFailure;
	sigc::signal<void, const Atlas::Objects::Entity::RootEntity&> GotCharacterInfo;
	sigc::signal<void, Eris::Account*> GotAllCharacters;

	/**
	 * @brief Emitted when the Account object has been destroyed.
	 */
	sigc::signal<void> DestroyedAccount;

	/**
	 * @brief Emitted when the Avatar instance has been destroyed.
	 */
	sigc::signal<void> DestroyedAvatar;

	/**
	 * @brief Emitted when the View instance has been destroyed.
	 */
	sigc::signal<void> DestroyedView;

	/**
	 * @brief Emitted when an object is about to be sent to the server.
	 */
	sigc::signal<void, const Atlas::Objects::Root&> EventSendingObject;

	/**
	 * @brief Emitted when an object has been received.
	 */
	sigc::signal<void, const Atlas::Objects::Root&> EventReceivedObject;

private:

	/**
	 * @brief Holds our connection to the server
	 */
	Eris::Connection *mConn;

	/**
	 * @brief Holds the player object we are connected with
	 */
	Eris::Account *mAccount;

	/**
	 * @brief Holds the world object of this server
	 */
	Eris::View *mView;

	/**
	 * @brief Holds the current avatar
	 */
	Eris::Avatar *mAvatar;

	/**
	 * @brief Contains the class that controls Out of Game Chat
	 */
	OOGChat *mOOGChat;

	/**
	 * @brief The host we are connected/ing to at present
	 */
	std::string myHost;

	/**
	 * @brief The port we are using to connect
	 */
	short myPort;

	/**
	 * @brief True if and only if we are successfully connected to the server
	 */
	bool mConnected;

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

	/**
	 * @brief The adapter which holds the actual implementation of any server communication.
	 * When the client isn't connected this will be represented by an instance of NonConnectedAdapter, whereas when it is connected to a server it will be represented by an instance of ConnectedAdapter.
	 */
	IServerAdapter* mServerAdapter;

	void gotFailure(const std::string& msg);

	void connected();

	bool disconnecting();

	void disconnected();

	void statusChanged(Eris::BaseConnection::Status status);

	void timeout(Eris::BaseConnection::Status status);

	void gotAvatarSuccess(Eris::Avatar* avatar);

	void gotAvatarDeactivated(Eris::Avatar* avatar);

	void gotCharacterInfo(const Atlas::Objects::Entity::RootEntity& characterInfo);

	void gotAllCharacters();

	//    void loginFailure(Eris::LoginFailureType, const std::string &);
	void loginFailure(const std::string& message);

	void loginSuccess();

	void logoutComplete(bool clean);

	/**
	 * @brief Destroys the account object and emits suitable signals.
	 */
	void cleanUpAccount();

};

inline bool ServerService::isConnected() const
{
	return mConnected;
}

inline Eris::View* ServerService::getView() const
{
	return mView;
}

inline Eris::Avatar* ServerService::getAvatar() const
{
	return mAvatar;
}

inline Eris::Connection* ServerService::getConnection() const
{
	return mConn;
}

} // namespace Ember

#endif
