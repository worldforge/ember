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

#include "ServerService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

#include <sigc++/object_slot.h>
#include <sigc++/object.h>
#include <Eris/Connection.h>
#include <Eris/Person.h>
#include <Eris/Avatar.h>
#include <Eris/Entity.h>
#include <Eris/Exceptions.h>

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/Element.h>


#include <list>
#include <algorithm>
#include <iostream>
#include <sstream>


// HINT(Tim): cannot 'using namespace Eris;' due to problem with STLport

namespace dime
{

	// List of ServerService's console commands
	const char * const ServerService::CONNECT    = "connect";
	const char * const ServerService::RECONNECT  = "reconnect";
	const char * const ServerService::DISCONNECT = "disconnect";
	const char * const ServerService::CREATEACC  = "create";
	const char * const ServerService::LOGIN      = "login";
	const char * const ServerService::LOGOUT     = "logout";
	const char * const ServerService::CREATECHAR = "add";
	const char * const ServerService::TAKECHAR   = "take";
	const char * const ServerService::LISTCHARS  = "list";
	const char * const ServerService::SAY        = "say";
	const char * const ServerService::TOUCH      = "touch";

  /* ctor */
  ServerService::ServerService() : myConn(NULL), myAccount(NULL),
				   myView(NULL), myOOGChat(NULL),
				   myConnected(false), myAvatar(NULL)
  {
    setName("Server Service");
    setDescription("Service for Server session");

    // Create new instance of myConn the constructor sets the
    // singleton instance up.  Do _not_ use Connection::Instance()
    // this does not create a new connection.
    // We are connected without debuging enabled thus the false
    myConn = new Eris::Connection("dime",false);

    // Bind signals
    myConn->Failure.connect(SigC::slot(*this, &ServerService::gotFailure));
    myConn->Connected.connect(SigC::slot(*this, &ServerService::connected));
    myConn->Disconnected.connect(SigC::slot(*this, &ServerService::disconnected));
    myConn->Disconnecting.connect(SigC::slot(*this, &ServerService::disconnecting));
    myConn->StatusChanged.connect(SigC::slot(*this, &ServerService::statusChanged));
    myConn->Timeout.connect(SigC::slot(*this, &ServerService::timeout));

	ConsoleBackend::getMainConsole()->registerCommand(CONNECT,this);
	ConsoleBackend::getMainConsole()->registerCommand(RECONNECT,this);
	ConsoleBackend::getMainConsole()->registerCommand(DISCONNECT,this);
	ConsoleBackend::getMainConsole()->registerCommand(CREATEACC,this);
	ConsoleBackend::getMainConsole()->registerCommand(LOGIN,this);
	ConsoleBackend::getMainConsole()->registerCommand(LOGOUT,this);
	ConsoleBackend::getMainConsole()->registerCommand(CREATECHAR,this);
	ConsoleBackend::getMainConsole()->registerCommand(LISTCHARS,this);
	ConsoleBackend::getMainConsole()->registerCommand(TAKECHAR,this);
	ConsoleBackend::getMainConsole()->registerCommand(SAY,this);
	ConsoleBackend::getMainConsole()->registerCommand(TOUCH,this);

  }

  /* dtor */
  ServerService::~ServerService()
  {
    if (myConn) delete myConn;
    if (myAccount) delete myAccount;
    if (myView) delete myView;
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

    myConn->disconnect();
    myConnected = false;
  }

  /* Interface method for connecting to host */
  bool ServerService::connect(const std::string& host, short port)
  {
    myHost = host;
    myPort = port;
    try {
      // If the connection fails here an exception is thrown
      myConn->connect( myHost, myPort );
    }
    catch (Eris::BaseException except)
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

  void ServerService::reconnect()
  {
    if (!myConn) return;
    try {
        myConn->reconnect();
      }
    catch (Eris::BaseException except)
    {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on reconnect:" << except._msg << ENDM;
        return;
    }
    catch (...)
      {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on reconnect" << ENDM;
        return;
      }
  }

  void ServerService::disconnect()
  {
    if (!myConn) return;
    try {
      myConn->disconnect();
      }
    catch (Eris::BaseException except)
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
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING)  << temp.str()<< ENDM;

#if 0 // not new sstream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }
	
  void ServerService::connected()
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Connected"<< ENDM;
    myConnected = true;
    GotConnection.emit(myConn);

    // Set up the player object
    myAccount=new Eris::Account(myConn);
    myAccount->GotCharacterInfo.connect(SigC::slot(*this,&ServerService::gotCharacterInfo));
    myAccount->GotAllCharacters.connect(SigC::slot(*this,&ServerService::gotAllCharacters));
    myAccount->LoginFailure.connect(SigC::slot(*this,&ServerService::loginFailure));
    myAccount->LoginSuccess.connect(SigC::slot(*this,&ServerService::loginSuccess));
    myAccount->LogoutComplete.connect(SigC::slot(*this,&ServerService::logoutComplete));

	GotAccount.emit(myAccount);
    // Init OOGChat controller
    myOOGChat = new OOGChat(myAccount);
    

    ConsoleBackend::getMainConsole()->pushMessage("Connected to Server");
  }

  bool ServerService::disconnecting()
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnecting"<< ENDM;
    return true;
  }

  void ServerService::disconnected()
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Disconnected"<< ENDM;

    // NULL out OOGChat & player so noone gets tempted to play with an unconnected lobby/player
    delete myAccount;
    myAccount=NULL;
    delete myOOGChat;
    myOOGChat = NULL;

    ConsoleBackend::getMainConsole()->pushMessage("Disconnected from server.");
  }

  void ServerService::statusChanged(Eris::BaseConnection::Status status)
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Status Changed to: "<<status<<ENDM;
  }

  void ServerService::timeout(Eris::BaseConnection::Status status)
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Connection Timed Out"<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Connection to server timed out");
  }

void ServerService::gotCharacterInfo(const Atlas::Objects::Entity::GameEntity & info)
{
	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Character Info"<< ENDM;
	ConsoleBackend::getMainConsole()->pushMessage("Got character info");
	
	GotCharacterInfo.emit(info);
}

  void ServerService::gotAllCharacters()
  {
	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got All Characters"<< ENDM;
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
#if 0 // not new sstream
    temp<<std::ends;    
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
	LoginFailure.emit(myAccount);
  }

  void ServerService::loginSuccess(){
    //myView = new Eris::View(myAccount, myConn);

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Login Success."<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Login Successful");
	LoginSuccess.emit(myAccount);
  }
  
  void ServerService::takeCharacter(const std::string &id){
		myAvatar = myAccount->takeCharacter(id);
		if (myAvatar) {
			GotAvatar.emit(myAvatar);
			myView = myAvatar->getView();
			GotView.emit(myView);
		}
	}


  void ServerService::logoutComplete(bool clean) {
    delete myView;
    myView = NULL;

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Logout Complete cleanness="<<clean<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Logged out from server");
  }

	void ServerService::runCommand(const std::string &command, const std::string &args)
	{
		// Connect command
		if(command == CONNECT){
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
			ConsoleBackend::getMainConsole()->pushMessage("Connected.");

		// Reconnect command
		} else if(command == RECONNECT) {
			ConsoleBackend::getMainConsole()->pushMessage("Reconnecting...");
			reconnect();

		// Disonnect command
		} else if (command==DISCONNECT){
			disconnect();

		// Create Account command
		} else if (command == CREATEACC) {	
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
			catch (Eris::BaseException except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on account creation: " << except._msg << ENDM;
				return;
			}
			catch (std::runtime_error except)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on account creation: " << except.what() << ENDM;
				return;
			}      
			
		// Login command
		} else if (command==LOGIN) {
	
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
		} else if (command==LOGOUT) {
			ConsoleBackend::getMainConsole()->pushMessage("Loggin out...");
			if (myAccount)
			{
				myAccount->logout();
			}
		
		// Create Character command
		} else if (command==CREATECHAR) {
			ConsoleBackend::getMainConsole()->pushMessage("Creating char...");
			if (myAccount)
			{
				// Split string into name/type/sex/description
				Tokeniser tokeniser = Tokeniser();
				tokeniser.initTokens(args);
				std::string name = tokeniser.nextToken();
				std::string sex  = tokeniser.nextToken();
				std::string type = tokeniser.nextToken();
				std::string description = tokeniser.remainingTokens();     
				
				std::string msg;
				msg = "Creating character: Name: [" + name + "], Sex: [" + sex + "], Type: [" + type + "], Desc: [" + description + "]";
				ConsoleBackend::getMainConsole()->pushMessage(msg);
				
				fprintf(stderr, "TRACE - CREATING CHARACTER - SERVERSERVICE\n");
				Atlas::Objects::Entity::GameEntity character;
				character->setParentsAsList(Atlas::Message::ListType(1,type));
				character->setName(name);
				character->setAttr("sex", sex);
				character->setAttr("description", description);
				fprintf(stderr, "TRACE - ATTRs SET - GONNA CREATE THE CHAR\n");
				try {
					myAvatar = myAccount->createCharacter(character);
					myView = myAvatar->getView();
				} 
				catch (Eris::BaseException except)
				{
					LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on character creation: " << except._msg << ENDM;
					return;
				}
				catch (std::runtime_error except)
				{
					LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on character creation: " << except.what() << ENDM;
					return;
				}
				fprintf(stderr, "TRACE - DONE\n");
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

		// Take Character Command
		} else if (command==TAKECHAR) {
			if (myAccount)
			{
				takeCharacter(args);
			}

		// List Characters Command
		} else if (command==LISTCHARS) {
			if (myAccount)
			{
				myAccount->refreshCharacterInfo();
			}

		// Say (In-Game chat) Command
		} else if (command==SAY) {
			say(args);

		// Touch Command
		} else if (command==TOUCH) {
			// TODO: make this switch call the touch method
			// TODO: polish this rough check
			fprintf(stderr, "TRACE - TOUCHING\n");
			if(!myAvatar) {
				fprintf(stderr, "TRACE - NO AVATAR\n");
				return;
			}
	
			Atlas::Objects::Operation::Touch touch;
			Atlas::Message::MapType opargs;
	
			opargs["id"] = args;
			touch->setFrom(myAvatar->getId());
			touch->setArgsAsList(Atlas::Message::ListType(1, opargs));
	
			myConn->send(touch);
		}	
	}
  
   	void ServerService::moveToPoint(const WFMath::Point<3>& dest) {
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->moveToPoint(dest);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on moving: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on moving: " << except.what() << ENDM;
 			return;
 		}

 	}

   	void ServerService::moveInDirection(const WFMath::Vector<3>& velocity, const WFMath::Quaternion& orientation) {
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		if(myAvatar->getEntity()->getLocation() == 0) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Avatar in limbo." << ENDM;
 			return;
 		}
 		try {
 			myAvatar->moveInDirection(velocity, orientation);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on moving: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on moving: " << except.what() << ENDM;
 			return;
 		}
 	}


   	void ServerService::moveInDirection(const WFMath::Vector<3>& velocity) {
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->moveInDirection(velocity);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on moving: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on moving: " << except.what() << ENDM;
 			return;
 		}
 	}


   	void ServerService::touch(Eris::Entity* entity) 
   	{
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->touch(entity);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on touching: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on touching: " << except.what() << ENDM;
 			return;
 		}
   	}   		
   	
	void ServerService::drop(Eris::Entity* entity, const WFMath::Vector<3>& offset) 
   	{
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->drop(entity, offset);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on dropping: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on dropping: " << except.what() << ENDM;
 			return;
 		}
   	}   		
	
	void ServerService::place(Eris::Entity* entity, Eris::Entity* target)
   	{
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->place(entity, target);
 		}
 		catch (Eris::BaseException except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on dropping: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on dropping: " << except.what() << ENDM;
 			return;
 		}
   	}   		

   		
	void ServerService::say(const std::string &message) {
 		if(!myAvatar) {
 			// TODO: redesign so that this doesn't happen
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "No Avatar when saying" << ENDM;
 			return;
 		}
 		try {
 			myAvatar->say(message);
			
			std::string msg;
			msg = "Saying: [" + message + "]. ";
			ConsoleBackend::getMainConsole()->pushMessage(msg);
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << msg << ENDM;
 		}
 		catch (Eris::BaseException except)
 		{
			/// _key_map.insert(InputKeyMap::value_type(SDLK_SLASH, KC_SLASH)); at SDLInput ctor
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got Eris error on say: " << except._msg << ENDM;
 			return;
 		}
 		catch (std::runtime_error except)
 		{
 			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on say: " << except.what() << ENDM;
 			return;
 		}	
	}
 
} // namespace dime
