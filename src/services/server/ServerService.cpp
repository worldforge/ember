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

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Anonymous.h>
#include "ServerService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"
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

#include "ConnectedAdapter.h"
#include "NonConnectedAdapter.h"

#include "OOGChat.h"

#include "framework/ConsoleCommandWrapper.h"



namespace Ember
{

	// List of ServerService's console commands
// 	const char * const ServerService::CONNECT    = "connect";
// 	const char * const ServerService::RECONNECT  = "reconnect";
// 	const char * const ServerService::DISCONNECT = "disconnect";
// 	const char * const ServerService::CREATEACC  = "create";
// 	const char * const ServerService::LOGIN      = "login";
// 	const char * const ServerService::LOGOUT     = "logout";
// 	const char * const ServerService::CREATECHAR = "add";
// 	const char * const ServerService::TAKECHAR   = "take";
// 	const char * const ServerService::LISTCHARS  = "list";
// 	const char * const ServerService::SAY        = "say";
// 	const char * const ServerService::TOUCH      = "touch";

  /* ctor */
  ServerService::ServerService() : myConn(0), myAccount(0),
				   myView(0), myOOGChat(0),
				   myConnected(false), myAvatar(0), mServerAdapter(new NonConnectedAdapter()),
	Connect("connect", this, "Connect to a server."),
	DisConnect("disconnect", this, "Disconnect from the server."),
//	ReConnect("reconnect", this, "Reconnect to the server"),
	CreateAcc("create", this, "Create an account on the server."),
	Login("login", this, "Login to the connected server."),
	Logout("logout", this, "Logout from the connected server."),
	CreateChar("add", this, "Create a character on the server."),
	TakeChar("take", this, "Take control of one of your characters."),
	ListChars("list", this, "List you available characters on the server."),
	Say("say", this, "Say something.")
  {
    setName("Server Service");
    setDescription("Service for Server session");


/*	ConsoleBackend::getMainConsole()->registerCommand(CONNECT,this, "Connect to a server.");
	ConsoleBackend::getMainConsole()->registerCommand(DISCONNECT,this, "Disconnect from the server.");
	ConsoleBackend::getMainConsole()->registerCommand(CREATEACC,this, "Create an account on the server.");
	ConsoleBackend::getMainConsole()->registerCommand(LOGIN,this, "Login to the connected server.");
	ConsoleBackend::getMainConsole()->registerCommand(LOGOUT,this, "Logout from the connected server.");
	ConsoleBackend::getMainConsole()->registerCommand(CREATECHAR,this, "Create a character on the server.");
	ConsoleBackend::getMainConsole()->registerCommand(LISTCHARS,this, "List you available characters on the server.");
	ConsoleBackend::getMainConsole()->registerCommand(TAKECHAR,this, "Take control of one of your characters.");
	ConsoleBackend::getMainConsole()->registerCommand(SAY,this, );*/
// 	ConsoleBackend::getMainConsole()->registerCommand(TOUCH,this, ");

	  }

  /* dtor */
  ServerService::~ServerService()
  {
    delete myConn;
    delete myAccount;
    delete myView;
  }
	
  /* Method for starting this service 	*/
  Service::Status ServerService::start()
  {
    setStatus(Service::OK);
    setRunning( true );

    return Service::OK;
	
  }

  /* Interface method for stopping this service 	*/
  void ServerService::stop(int code)
  {
    setStatus(Service::OK);
    setRunning( false );

	if (myConn) {
		myConn->disconnect();
    }
	myConnected = false;
  }

  /* Interface method for connecting to host */
  bool ServerService::connect(const std::string& host, short port)
  {
    myHost = host;
    myPort = port;
    try {
		// Create new instance of myConn the constructor sets the
		// singleton instance up.  Do _not_ use Connection::Instance()
		// this does not create a new connection.
		// We are connected without debuging enabled thus the false
		myConn = new Eris::Connection(std::string("Ember ") + VERSION,myHost, port, false);
		
		// Bind signals
		myConn->Failure.connect(sigc::mem_fun(*this, &ServerService::gotFailure));
		myConn->Connected.connect(sigc::mem_fun(*this, &ServerService::connected));
		myConn->Disconnected.connect(sigc::mem_fun(*this, &ServerService::disconnected));
		myConn->Disconnecting.connect(sigc::mem_fun(*this, &ServerService::disconnecting));
		myConn->StatusChanged.connect(sigc::mem_fun(*this, &ServerService::statusChanged));
		//myConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));
      // If the connection fails here an errnumber is returned
      int errorno = myConn->connect();
		if (errorno) 
		{
			return false;
		}
    }
    catch (const Eris::BaseException& except)
    {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on connect:" << except._msg << ENDM;
        return false;
    }
    catch (...)
      {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on connect" << ENDM;
        return false;
      }

    return true;
  }


//   void ServerService::reconnect()
//   {
//     if (!myConn) return;
//     try {
// 		const std::string host = myConn->get
//         myConn->reconnect();
//       }
//     catch (const Eris::BaseException& except)
//     {
//         LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on reconnect:" << except._msg << ENDM;
//         return;
//     }
//     catch (...)
//       {
//         LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on reconnect" << ENDM;
//         return;
//       }
//   }


  void ServerService::disconnect()
  {
    if (!myConn) return;
    try {
      myConn->disconnect();
      }
    catch (const Eris::BaseException& except)
    {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on disconnect:" << except._msg << ENDM;
        return;
    }
    catch (...)
      {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on disconnect" << ENDM;
        return;
      }
  }
	
  void ServerService::gotFailure(const std::string & msg)
  {
    std::ostringstream temp;

    temp << "Got Server error: " << msg;
    S_LOG_WARNING(temp.str());
    
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }
	
  void ServerService::connected()
  {
  	S_LOG_INFO("Connected");
    myConnected = true;
    GotConnection.emit(myConn);

    // Set up the player object
    myAccount=new Eris::Account(myConn);
    myAccount->GotCharacterInfo.connect(sigc::mem_fun(*this,&ServerService::gotCharacterInfo));
    myAccount->GotAllCharacters.connect(sigc::mem_fun(*this,&ServerService::gotAllCharacters));
    myAccount->LoginFailure.connect(sigc::mem_fun(*this,&ServerService::loginFailure));
    myAccount->LoginSuccess.connect(sigc::mem_fun(*this,&ServerService::loginSuccess));
    myAccount->LogoutComplete.connect(sigc::mem_fun(*this,&ServerService::logoutComplete));
	myAccount->AvatarSuccess.connect(sigc::mem_fun(*this,&ServerService::gotAvatarSuccess));
	
	GotAccount.emit(myAccount);
    // Init OOGChat controller
//     myOOGChat = new OOGChat(myAccount);
    

    ConsoleBackend::getMainConsole()->pushMessage("Connected to Server");
  }

  bool ServerService::disconnecting()
  {
	S_LOG_INFO("Disconnecting");
    return true;
  }

  void ServerService::disconnected()
  {
	S_LOG_INFO("Disconnected");

    // NULL out OOGChat & player so noone gets tempted to play with an unconnected lobby/player
    delete myAccount;
    myAccount=NULL;
    delete myOOGChat;
    myOOGChat = NULL;

    ConsoleBackend::getMainConsole()->pushMessage("Disconnected from server.");
  }

  void ServerService::statusChanged(Eris::BaseConnection::Status status)
  {
	S_LOG_INFO("Status Changed to: "<<status);
  }

  void ServerService::timeout(Eris::BaseConnection::Status status)
  {
	S_LOG_INFO( "Connection Timed Out");
    ConsoleBackend::getMainConsole()->pushMessage("Connection to server timed out");
  }

void ServerService::gotCharacterInfo(const Atlas::Objects::Entity::RootEntity & info)
{
	S_LOG_INFO("Got Character Info");
	ConsoleBackend::getMainConsole()->pushMessage("Got character info");
	
	GotCharacterInfo.emit(info);
}

  void ServerService::gotAllCharacters()
  {
	S_LOG_INFO("Got All Characters");
	ConsoleBackend::getMainConsole()->pushMessage("Got all characters");
	Eris::CharacterMap cm = myAccount->getCharacters();
	Eris::CharacterMap::iterator i;
	for(i=cm.begin();i!=cm.end();i++) {
		std::string msg;
		msg = "Character ID: [" + (*i).first + "].";
		ConsoleBackend::getMainConsole()->pushMessage(msg);
	}
	GotAllCharacters.emit(myAccount);

  }

//  void ServerService::loginFailure(Eris::LoginFailureType, const std::string &msg) 
  void ServerService::loginFailure(const std::string &msg) 
  {
    std::ostringstream temp;

    temp<< "Login Failure:"<<msg;
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << temp.str() << ENDM;

    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
	LoginFailure.emit(myAccount, msg);
  }

  void ServerService::loginSuccess(){
    //myView = new Eris::View(myAccount, myConn);

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Login Success."<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Login Successful");
	LoginSuccess.emit(myAccount);
  }
  
  void ServerService::takeCharacter(const std::string &id){
		myAccount->takeCharacter(id);
	}
	
	void ServerService::gotAvatarSuccess(Eris::Avatar* avatar) {
		//if we already have a avatar, do nothing
		//TODO: perhaps signal an error?
		if (!myAvatar) {
			myAvatar = avatar;
			GotAvatar.emit(myAvatar);
			myView = myAvatar->getView();
			GotView.emit(myView);
		}
		delete mServerAdapter;
		mServerAdapter = new ConnectedAdapter(myAvatar, myConn);
	
	}


  void ServerService::logoutComplete(bool clean) {
    delete myView;
    myView = 0;

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Logout Complete cleanness="<<clean<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Logged out from server");
  }

	void ServerService::runCommand(const std::string &command, const std::string &args)
	{
		// Connect command
		if(Connect == command){
			// Split string into server / port pair
			Tokeniser tokeniser = Tokeniser();
			tokeniser.initTokens(args);
			std::string server = tokeniser.nextToken();
			std::string port = tokeniser.remainingTokens();
			std::string msg;
			msg = "Connecting to: [" + server + "]";
			ConsoleBackend::getMainConsole()->pushMessage(msg);
			if (port=="")
				connect(server);
			else
			connect(server, (short)atoi(port.c_str()));

		// Disonnect command
		} else if (DisConnect == command){
			disconnect();

		// Create Account command
		} else if (CreateAcc == command) {	
			if (!myAccount) return;
			Tokeniser tokeniser = Tokeniser();
			tokeniser.initTokens(args);
			std::string uname = tokeniser.nextToken();
			std::string password = tokeniser.nextToken();
			std::string realname = tokeniser.remainingTokens();
	
			std::string msg;
			msg = "Creating account: Name: [" + uname + "], Password: [" + password + "], Real Name: [" + realname + "]";
			
			try {
				myAccount->createAccount(uname,realname,password);
			} 
			catch (const Eris::BaseException& except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on account creation: " << except._msg << ENDM;
				return;
			}
			catch (const std::runtime_error& except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on account creation: " << except.what() << ENDM;
				return;
			}      
			
		// Login command
		} else if (Login == command) {
	
		// TODO: put this in a separate method
			if (myAccount)
			{
				// Split string into userid / password pair
				Tokeniser tokeniser = Tokeniser();
				tokeniser.initTokens(args);
				std::string userid = tokeniser.nextToken();
				std::string password = tokeniser.remainingTokens();
	
				myAccount->login(userid,password);
				
				std::string msg;
				msg = "Login: [" + userid + "," + password + "]";
				ConsoleBackend::getMainConsole()->pushMessage(msg);
			} else {
				ConsoleBackend::getMainConsole()->pushMessage("not connected");
			}

		// Logout command
		} else if (Logout == command) {
			ConsoleBackend::getMainConsole()->pushMessage("Loggin out...");
			if (myAccount)
			{
				myAccount->logout();
			}
		
		// Create Character command
		} else if (CreateChar == command) {
			// Split string into name/type/sex/description
			Tokeniser tokeniser = Tokeniser();
			tokeniser.initTokens(args);
			std::string name = tokeniser.nextToken();
			std::string sex  = tokeniser.nextToken();
			std::string type = tokeniser.nextToken();
			std::string description = tokeniser.remainingTokens();   
				
			if (!createCharacter(name, sex, type, description)) {
				return;
			}

		// Take Character Command
		} else if (TakeChar == command) {
			if (myAccount)
			{
				takeCharacter(args);
			}

		// List Characters Command
		} else if (ListChars == command) {
			if (myAccount)
			{
				myAccount->refreshCharacterInfo();
			}

		// Say (In-Game chat) Command
		} else if (Say == command) {
			say(args);

/*		// Touch Command
		} else if (command==TOUCH) {
			// TODO: make this switch call the touch method
			// TODO: polish this rough check
			S_LOG_VERBOSE("Touching");
			if(!myAvatar) {
				S_LOG_WARNING("No avatar.");
				return;
			}
	
			Atlas::Objects::Operation::Touch touch;
			Atlas::Message::MapType opargs;
	
			opargs["id"] = args;
			touch->setFrom(myAvatar->getId());
			touch->setArgsAsList(Atlas::Message::ListType(1, opargs));
	
			myConn->send(touch);*/
		}	
	}
	
	bool ServerService::createCharacter(const std::string& name, const std::string& sex, const std::string& type, const std::string& description)
	{
		ConsoleBackend::getMainConsole()->pushMessage("Creating char...");
		if (myAccount)
		{
			std::string msg;
			msg = "Creating character: Name: [" + name + "], Sex: [" + sex + "], Type: [" + type + "], Desc: [" + description + "]";
			ConsoleBackend::getMainConsole()->pushMessage(msg);
			
			S_LOG_INFO("Creating character.");
			Atlas::Objects::Entity::RootEntity character;
			character->setParentsAsList(Atlas::Message::ListType(1,type));
			character->setName(name);
			character->setAttr("sex", sex);
			character->setAttr("description", description);
			try {
				myAccount->createCharacter(character);
			} 
			catch (const Eris::BaseException& except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on character creation: " << except._msg << ENDM;
				return false;
			}
			catch (const std::runtime_error& except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on character creation: " << except.what() << ENDM;
				return false;
			}
			S_LOG_INFO("Done creating character.");
			if (myAvatar) {
				GotAvatar.emit(myAvatar);
				myView = myAvatar->getView();
				if (myView) {
					GotView.emit(myView);
				}
			}
		} else {
			ConsoleBackend::getMainConsole()->pushMessage("Not logged in. Can't create char...");
		}

	
		return true;
	}
  
	void ServerService::moveToPoint(const WFMath::Point<3>& dest) {
		mServerAdapter->moveToPoint(dest);
 	}

	void ServerService::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) {
		mServerAdapter->moveInDirection(velocity, orientation);
	}


	void ServerService::moveInDirection(const WFMath::Vector<3>& velocity) {
		mServerAdapter->moveInDirection(velocity);
	}


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
	
	void ServerService::wield(Eris::Entity* entity)
   	{
		mServerAdapter->wield(entity);
   	}

	void ServerService::use(Eris::Entity* entity, WFMath::Point<3> pos)
   	{
		mServerAdapter->use(entity, pos);
   	}

	void ServerService::useStop()
	{
		mServerAdapter->useStop();
	}


	void ServerService::attack(Eris::Entity* entity)
   	{
		mServerAdapter->attack(entity);
   	}   

	void ServerService::say(const std::string &message) {
		mServerAdapter->say(message);
	}
 
} // namespace Ember
