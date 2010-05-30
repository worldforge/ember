/*
 Copyright (C) 2002 Martin Pollard (Xmp)

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ServerService.h"

#include "Connection.h"
#include "ServerServiceConnectionListener.h"

#include "ConnectedAdapter.h"
#include "NonConnectedAdapter.h"

#include "OOGChat.h"

#include "framework/LoggingInstance.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Anonymous.h>
#include <wfmath/atlasconv.h>

#include <sigc++/object_slot.h>
#include <sigc++/object.h>
#include <Eris/Connection.h>
#include <Eris/Person.h>
#include <Eris/Avatar.h>
#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include <Eris/Exceptions.h>
#include <Eris/View.h>

#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace Ember
{

ServerService::ServerService() :
	mConn(0), mAccount(0), mView(0), mAvatar(0), mOOGChat(0), mConnected(false), Connect("connect", this, "Connect to a server."), DisConnect("disconnect", this, "Disconnect from the server."),
	//	ReConnect("reconnect", this, "Reconnect to the server"),
			CreateAcc("create", this, "Create an account on the server."), Login("login", this, "Login to the connected server."), Logout("logout", this, "Logout from the connected server."), CreateChar("add", this, "Create a character on the server."), TakeChar("take", this, "Take control of one of your characters."), ListChars("list", this, "List you available characters on the server."), Say("say", this, "Say something."), Emote("me", this, "Emotes something."), Delete("delete", this, "Deletes an entity."), AdminTell("admin_tell", this, "Uses admin mode to directly tell a NPC something. Usage: /admin_tell <entityid> <key> <value>"), mServerAdapter(new NonConnectedAdapter())
{
	setName("Server Service");
	setDescription("Service for Server session");
}

/* dtor */
ServerService::~ServerService()
{
	delete mConn;
	//delete mAccount;
	//delete mView;
	delete mServerAdapter;
}

/* Method for starting this service 	*/
Service::Status ServerService::start()
{
	setStatus(Service::OK);
	setRunning(true);

	return Service::OK;

}

/* Interface method for stopping this service 	*/
void ServerService::stop(int code)
{
	setStatus(Service::OK);
	setRunning(false);

	disconnect();
}

/* Interface method for connecting to host */
bool ServerService::connect(const std::string& host, short port)
{
	myHost = host;
	myPort = port;
	try {
		// Create new instance of mConn the constructor sets the
		// singleton instance up.  Do _not_ use Connection::Instance()
		// this does not create a new connection.
		// We are connected without debuging enabled thus the false
		mConn = new Connection(std::string("Ember ") + VERSION, myHost, port, false, new ServerServiceConnectionListener(*this));

		// Bind signals
		mConn->Failure.connect(sigc::mem_fun(*this, &ServerService::gotFailure));
		mConn->Connected.connect(sigc::mem_fun(*this, &ServerService::connected));
		mConn->Disconnected.connect(sigc::mem_fun(*this, &ServerService::disconnected));
		mConn->Disconnecting.connect(sigc::mem_fun(*this, &ServerService::disconnecting));
		mConn->StatusChanged.connect(sigc::mem_fun(*this, &ServerService::statusChanged));
		//mConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));
		// If the connection fails here an errnumber is returned
		int errorno = mConn->connect();
		if (errorno) {
			return false;
		}
	} catch (const Eris::BaseException& except) {
		S_LOG_WARNING("Got error on connect:" << except._msg);
		return false;
	} catch (...) {
		S_LOG_WARNING("Got unknown error on connect.");
		return false;
	}

	return true;
}

//   void ServerService::reconnect()
//   {
//     if (!mConn) return;
//     try {
// 		const std::string host = mConn->get
//         mConn->reconnect();
//       }
//     catch (const Eris::BaseException& except)
//     {
//         S_LOG_WARNING("Got error on reconnect:" << except._msg );
//         return;
//     }
//     catch (...)
//       {
//         S_LOG_WARNING("Got unknown error on reconnect" );
//         return;
//       }
//   }


void ServerService::disconnect()
{
	if (!mConn)
		return;
	try {
		Eris::Account* tempAccount = mAccount;
		mAccount = 0;
		delete tempAccount;
		DestroyedAccount.emit();
	} catch (const std::exception& e) {
		S_LOG_WARNING("Got error on account deletion." << e);
		return;
	} catch (...) {
		S_LOG_WARNING( "Got unknown error on account deletion.");
		return;
	}
	try {
		mConn->disconnect();
	} catch (const std::exception& e) {
		S_LOG_WARNING("Got error on disconnect." << e);
		return;
	} catch (...) {
		S_LOG_WARNING("Got unknown error on disconnect");
		return;
	}

}

void ServerService::gotFailure(const std::string & msg)
{
	std::ostringstream temp;

	temp << "Got Server error: " << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str());
}

void ServerService::connected()
{
	S_LOG_INFO("Connected");
	mConnected = true;
	GotConnection.emit(mConn);

	// Set up the player object
	mAccount = new Eris::Account(mConn);
	mAccount->GotCharacterInfo.connect(sigc::mem_fun(*this, &ServerService::gotCharacterInfo));
	mAccount->GotAllCharacters.connect(sigc::mem_fun(*this, &ServerService::gotAllCharacters));
	mAccount->LoginFailure.connect(sigc::mem_fun(*this, &ServerService::loginFailure));
	mAccount->LoginSuccess.connect(sigc::mem_fun(*this, &ServerService::loginSuccess));
	mAccount->LogoutComplete.connect(sigc::mem_fun(*this, &ServerService::logoutComplete));
	mAccount->AvatarSuccess.connect(sigc::mem_fun(*this, &ServerService::gotAvatarSuccess));
	mAccount->AvatarDeactivated.connect(sigc::mem_fun(*this, &ServerService::gotAvatarDeactivated));

	GotAccount.emit(mAccount);
	// Init OOGChat controller
	//     mOOGChat = new OOGChat(mAccount);


	ConsoleBackend::getSingleton().pushMessage("Connected to Server");
}

bool ServerService::disconnecting()
{
	S_LOG_INFO("Disconnecting");
	Eris::Account* tempAccount = mAccount;
	mAccount = 0;
	delete tempAccount;
	return true;
}

void ServerService::disconnected()
{
	S_LOG_INFO("Disconnected");

	// NULL out OOGChat & player so noone gets tempted to play with an unconnected lobby/player
	/*	delete mAccount;
	 mAccount=NULL;*/
	mConnected = false;
	delete mOOGChat;
	mOOGChat = NULL;

	ConsoleBackend::getSingleton().pushMessage("Disconnected from server.");
}

void ServerService::statusChanged(Eris::BaseConnection::Status status)
{
	S_LOG_INFO("Status Changed to: "<<status);
}

void ServerService::timeout(Eris::BaseConnection::Status status)
{
	S_LOG_INFO( "Connection Timed Out");
	ConsoleBackend::getSingleton().pushMessage("Connection to server timed out");
}

void ServerService::gotCharacterInfo(const Atlas::Objects::Entity::RootEntity & info)
{
	S_LOG_INFO("Got Character Info");
	ConsoleBackend::getSingleton().pushMessage("Got character info");

	GotCharacterInfo.emit(info);
}

void ServerService::gotAllCharacters()
{
	S_LOG_INFO("Got All Characters");
	ConsoleBackend::getSingleton().pushMessage("Got all characters");
	Eris::CharacterMap cm = mAccount->getCharacters();
	Eris::CharacterMap::iterator i;
	for (i = cm.begin(); i != cm.end(); i++) {
		std::string msg;
		msg = "Character ID: [" + (*i).first + "].";
		ConsoleBackend::getSingleton().pushMessage(msg);
	}
	GotAllCharacters.emit(mAccount);

}

void ServerService::loginFailure(const std::string& msg)
{
	std::ostringstream temp;

	temp << "Login Failure:" << msg;
	S_LOG_WARNING(temp.str());

	ConsoleBackend::getSingleton().pushMessage(temp.str());
	LoginFailure.emit(mAccount, msg);
}

void ServerService::loginSuccess()
{
	//mView = new Eris::View(mAccount, mConn);

	S_LOG_INFO("Login Success.");
	ConsoleBackend::getSingleton().pushMessage("Login Successful");
	LoginSuccess.emit(mAccount);
}

void ServerService::takeCharacter(const std::string &id)
{
	mAccount->takeCharacter(id);
}

void ServerService::gotAvatarSuccess(Eris::Avatar* avatar)
{
	//if we already have a avatar, do nothing
	//TODO: perhaps signal an error?
	if (!mAvatar) {
		mAvatar = avatar;
		mView = mAvatar->getView();
		GotAvatar.emit(mAvatar);
		GotView.emit(mView);
	}
	delete mServerAdapter;
	mServerAdapter = new ConnectedAdapter(*mAccount, *mAvatar, *mConn);

}

void ServerService::gotAvatarDeactivated(Eris::Avatar* avatar)
{
	mAvatar = 0;
	mView = 0;
	delete mServerAdapter;
	mServerAdapter = new NonConnectedAdapter();
}

void ServerService::logoutComplete(bool clean)
{
	//     delete mView;
	mView = 0;

	S_LOG_INFO("Logout Complete cleanness=" << clean);
	ConsoleBackend::getSingleton().pushMessage("Logged out from server");
}

void ServerService::runCommand(const std::string &command, const std::string &args)
{
	// Connect command
	if (Connect == command) {
		// Split string into server / port pair
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string server = tokeniser.nextToken();
		std::string port = tokeniser.remainingTokens();
		std::string msg;
		msg = "Connecting to: [" + server + "]";
		ConsoleBackend::getSingleton().pushMessage(msg);
		if (port == "")
			connect(server);
		else
			connect(server, (short)atoi(port.c_str()));

		// Disonnect command
	} else if (DisConnect == command) {
		disconnect();

		// Create Account command
	} else if (CreateAcc == command) {
		if (!mAccount)
			return;
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string uname = tokeniser.nextToken();
		std::string password = tokeniser.nextToken();
		std::string realname = tokeniser.remainingTokens();

		std::string msg;
		msg = "Creating account: Name: [" + uname + "], Password: [" + password + "], Real Name: [" + realname + "]";

		try {
			mAccount->createAccount(uname, realname, password);
		} catch (const std::exception& except) {
			S_LOG_WARNING("Got error on account creation." << except);
			return;
		} catch (...) {
			S_LOG_WARNING("Got unknown error on account creation.");
			return;
		}

		// Login command
	} else if (Login == command) {

		// TODO: put this in a separate method
		if (mAccount) {
			// Split string into userid / password pair
			Tokeniser tokeniser = Tokeniser();
			tokeniser.initTokens(args);
			std::string userid = tokeniser.nextToken();
			std::string password = tokeniser.remainingTokens();

			mAccount->login(userid, password);

			std::string msg;
			msg = "Login: [" + userid + "," + password + "]";
			ConsoleBackend::getSingleton().pushMessage(msg);
		} else {
			ConsoleBackend::getSingleton().pushMessage("not connected");
		}

		// Logout command
	} else if (Logout == command) {
		ConsoleBackend::getSingleton().pushMessage("Logging out...");
		if (mAccount) {
			mAccount->logout();
		}

		// Create Character command
	} else if (CreateChar == command) {
		// Split string into name/type/sex/description
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string name = tokeniser.nextToken();
		std::string sex = tokeniser.nextToken();
		std::string type = tokeniser.nextToken();
		std::string spawnPoint = tokeniser.nextToken();
		std::string description = tokeniser.remainingTokens();

		if (!createCharacter(name, sex, type, description, spawnPoint)) {
			return;
		}

		// Take Character Command
	} else if (TakeChar == command) {
		if (mAccount) {
			takeCharacter(args);
		}

		// List Characters Command
	} else if (ListChars == command) {
		if (mAccount) {
			mAccount->refreshCharacterInfo();
		}

		// Say (In-Game chat) Command
	} else if (Say == command) {
		say(args);
	} else if (Emote == command) {
		emote(args);
	} else if (Delete == command) {
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			Eris::Entity* entity = getView()->getEntity(entityId);
			if (entity) {
				deleteEntity(entity);
			}
		}

		/*		// Touch Command
		 } else if (command==TOUCH) {
		 // TODO: make this switch call the touch method
		 // TODO: polish this rough check
		 S_LOG_VERBOSE("Touching");
		 if(!mAvatar) {
		 S_LOG_WARNING("No avatar.");
		 return;
		 }

		 Atlas::Objects::Operation::Touch touch;
		 Atlas::Message::MapType opargs;

		 opargs["id"] = args;
		 touch->setFrom(mAvatar->getId());
		 touch->setArgsAsList(Atlas::Message::ListType(1, opargs));

		 mConn->send(touch);*/
	} else if (AdminTell == command) {
		Tokeniser tokeniser = Tokeniser();
		tokeniser.initTokens(args);
		std::string entityId = tokeniser.nextToken();
		if (entityId != "") {
			std::string key = tokeniser.nextToken();
			if (key != "") {
				std::string value = tokeniser.nextToken();
				if (value != "") {
					adminTell(entityId, key, value);
				}
			}
		}
	}
}

bool ServerService::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description, const std::string& spawnName)
{
	ConsoleBackend::getSingleton().pushMessage("Creating char...");
	if (mAccount) {
		std::string msg;
		msg = "Creating character: Name: [" + name + "], Sex: [" + sex + "], Type: [" + type + "], Desc: [" + description + "]";
		ConsoleBackend::getSingleton().pushMessage(msg);

		S_LOG_INFO("Creating character.");
		Atlas::Objects::Entity::RootEntity character;
		character->setParentsAsList(Atlas::Message::ListType(1, type));
		character->setName(name);
		character->setAttr("sex", sex);
		character->setAttr("description", description);
		if (spawnName != "") {
			character->setAttr("spawn_name", spawnName);
		}
		try {
			mAccount->createCharacter(character);
		} catch (const std::exception& except) {
			S_LOG_WARNING("Got Eris error on character creation." << except);
			return false;
		} catch (...) {
			S_LOG_WARNING("Got unknown error on character creation.");
			return false;
		}
		S_LOG_INFO("Done creating character.");
		if (mAvatar) {
			GotAvatar.emit(mAvatar);
			mView = mAvatar->getView();
			if (mView) {
				GotView.emit(mView);
			}
		}
	} else {
		ConsoleBackend::getSingleton().pushMessage("Not logged in. Can't create char...");
	}

	return true;
}

void ServerService::moveToPoint(const WFMath::Point<3>& dest)
{
	mServerAdapter->moveToPoint(dest);
}

void ServerService::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation)
{
	mServerAdapter->moveInDirection(velocity, orientation);
}

void ServerService::moveInDirection(const WFMath::Vector<3>& velocity)
{
	mServerAdapter->moveInDirection(velocity);
}

// 	void ServerService::teleportTo(const WFMath::Point<3>& dest)
// 	{
// 		mServerAdapter->teleportTo(dest);
// 	}


void ServerService::touch(Eris::Entity* entity)
{
	mServerAdapter->touch(entity);
}

void ServerService::take(Eris::Entity* entity)
{
	mServerAdapter->take(entity);
}

void ServerService::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset)
{
	mServerAdapter->drop(entity, offset);
}

void ServerService::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos)
{
	mServerAdapter->place(entity, target, pos);
}

void ServerService::place(Eris::Entity* entity, Eris::Entity* target, const WFMath::Point<3>& pos, const WFMath::Quaternion& orient)
{
	mServerAdapter->place(entity, target, pos, orient);
}

void ServerService::wield(Eris::Entity* entity, const std::string& outfitSlot)
{
	mServerAdapter->wield(entity, outfitSlot);
}

void ServerService::use(Eris::Entity* entity, WFMath::Point<3> pos, const std::string& operation)
{
	mServerAdapter->use(entity, pos, operation);
}

void ServerService::useStop()
{
	mServerAdapter->useStop();
}

void ServerService::actuate(Eris::Entity* entity, const std::string& action)
{
	mServerAdapter->actuate(entity, action);
}

void ServerService::attack(Eris::Entity* entity)
{
	mServerAdapter->attack(entity);
}

void ServerService::eat(Eris::Entity* entity)
{
	mServerAdapter->eat(entity);
}

void ServerService::say(const std::string &message)
{
	mServerAdapter->say(message);
}

void ServerService::emote(const std::string &message)
{
	mServerAdapter->emote(message);
}

void ServerService::deleteEntity(Eris::Entity* entity)
{
	mServerAdapter->deleteEntity(entity);
}

void ServerService::setAttributes(Eris::Entity* entity, Atlas::Message::MapType& attributes)
{
	mServerAdapter->setAttributes(entity, attributes);
}

void ServerService::adminTell(const std::string& entityId, const std::string& attribute, const std::string &value)
{
	mServerAdapter->adminTell(entityId, attribute, value);
}

void ServerService::createTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	mServerAdapter->createTypeInfo(typeInfo);
}

void ServerService::setTypeInfo(const Atlas::Objects::Root& typeInfo)
{
	mServerAdapter->setTypeInfo(typeInfo);
}


} // namespace Ember
