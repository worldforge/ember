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

#include "ServerServiceSignals.h"

#include "framework/Service.h"

#include <wfmath/vector.h>
#include <wfmath/point.h>
#include <wfmath/quaternion.h>

#include <Atlas/Message/Element.h>

#include <sigc++/object_slot.h>
#include <memory>


namespace Eris
{
class Avatar;
class Connection;
class View;
class Lobby;
class Account;
class Entity;
class TransferInfo;
}

namespace Ember
{

class OOGChat;
class NonConnectedState;
class IServerAdapter;

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
class ServerService: public Service, public ServerServiceSignals
{
public:

	/**
	 * @brief Ctor.
	 */
	ServerService();

	/**
	 * @brief Dtor.
	 */
	virtual ~ServerService();

	bool isConnected() const;

	Service::Status start();

	void stop(int code);

	bool connect(const std::string& host, short port = 6767);

	void reconnect();

	void disconnect();

	/**
	 * @brief Takes a transferred character.
	 * @param transferInfo Information needed about the transfer.
	 */
	void takeTransferredCharacter(const Eris::TransferInfo& transferInfo);

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
	 * @param extraProperties Any additional properties we want to set. Note that the server might reject any property, depending on rules and access rules.
	 */
	bool createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName, const Atlas::Message::MapType& extraProperties = Atlas::Message::MapType());

//	Eris::View* getView() const;
	Eris::Avatar* getAvatar() const;

	/**
	 * @brief Gets the underlying connection.
	 * @returns A connection instance, or null if we're currently not connected
	 */
	Eris::Connection* getConnection() const;

	/**
	 * @brief Gets the used account, if available.
	 * @return An account, or null if none has been created.
	 */
	Eris::Account* getAccount() const;

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
	 * @brief Say something out loud.
	 * @param message The message being said.
	 */
	void say(const std::string &message);

	/**
	 * @brief Says something, addressed to a list of entities.
	 *
	 * Note that the message will be heard by everyone within earshot, even those not addressed.
	 *
	 * @param message The message.
	 * @param entities A list of entities to which the message will be addressed.
	 */
	void sayTo(const std::string &message, const std::vector<const Eris::Entity*>& entities);

	/**
	 * @brief Says something, addressed to an entity.
	 *
	 * Note that the message will be heard by everyone within earshot, even those not addressed.
	 *
	 * @param message The message.
	 * @param entities The entity to which the message is addressed.
	 */
	void sayTo(const std::string &message, const Eris::Entity& entity);

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
	void drop(Eris::Entity* entity, const WFMath::Vector<3>& offset = WFMath::Vector<3>::ZERO(), const WFMath::Quaternion& orientation = WFMath::Quaternion());
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos = WFMath::Point<3>::ZERO());
	void place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orientation);
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

private:

	void gotConnection(Eris::Connection* connection);

	void gotAvatar(Eris::Avatar* avatar);

	void gotAccount(Eris::Account* account);

	void destroyedAccount();

	void destroyedAvatar();

	IServerAdapter& getAdapter();

	Eris::Connection* mConnection;
	Eris::Account* mAccount;
	Eris::Avatar* mAvatar;

	/**
	 * @brief Contains the class that controls Out of Game Chat
	 */
	OOGChat *mOOGChat;

	/**
	 * @brief The adapter which holds the actual implementation of any server communication.
	 * When the client isn't connected this will be represented by an instance of NonConnectedAdapter, whereas when it is connected to a server it will be represented by an instance of ConnectedAdapter.
	 */
	IServerAdapter* mServerAdapter;

	std::unique_ptr<NonConnectedState> mNonConnectedState;

};

} // namespace Ember

#endif
