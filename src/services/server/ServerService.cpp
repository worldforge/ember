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

#include <list>
#include <algorithm>
#include <iostream>
#include <strstream>

// HINT(Tim): cannot 'using namespace Eris;' due to problem with STLport

namespace dime
{


  /* ctor */
  ServerService::ServerService() : myConn(NULL), myPlayer(NULL),
				   myWorld(NULL), myOOGChat(NULL),
				   myConnected(false)
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
  }
	
  /* dtor */
  ServerService::~ServerService()
  {
    if (myConn) delete myConn;
    if (myPlayer) delete myPlayer;
    if (myWorld) delete myWorld;
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
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on connect:" << except._msg << ENDM;
        return;
    }
    catch (...)
      {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on connect" << ENDM;
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
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got error on connect:" << except._msg << ENDM;
        return;
    }
    catch (...)
      {
        LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Got unknown error on connect" << ENDM;
        return;
      }
  }
	
  void ServerService::gotFailure(const std::string & msg)
  {
    std::ostrstream temp;

    temp << "Got Server error: " << msg;
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING)  << temp.str()<< ENDM;

    temp<<std::ends;
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }
	
  void ServerService::connected()
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Connected"<< ENDM;
    myConnected = true;

    // Set up the player object
    myPlayer=new Eris::Player(myConn);
    myPlayer->GotCharacterInfo.connect(SigC::slot(*this,&ServerService::gotCharacterInfo));
    myPlayer->GotAllCharacters.connect(SigC::slot(*this,&ServerService::gotAllCharacters));
    myPlayer->LoginFailure.connect(SigC::slot(*this,&ServerService::loginFailure));
    myPlayer->LoginSuccess.connect(SigC::slot(*this,&ServerService::loginSuccess));
    myPlayer->LogoutComplete.connect(SigC::slot(*this,&ServerService::logoutComplete));

    // Init OOGChat controller
    myOOGChat = new OOGChat;

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
    delete myPlayer;
    myPlayer=NULL;
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

  void ServerService::gotCharacterInfo(const Atlas::Objects::Entity::GameEntity &) {}

  void ServerService::gotAllCharacters() {}

  void ServerService::loginFailure(Eris::LoginFailureType, const std::string &msg) 
  {
    std::ostrstream temp;

    temp<< "Login Failure:"<<msg;
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << temp.str() << ENDM;
    temp<<std::ends;    
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void ServerService::loginSuccess(){
    myWorld = new Eris::World(myPlayer, myConn);

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Login Success."<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Login Successful");
  }

  void ServerService::logoutComplete(bool clean) {
    delete myWorld;
    myWorld = NULL;

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Logout Complete cleanness="<<clean<< ENDM;
    ConsoleBackend::getMainConsole()->pushMessage("Logged out from server");
  }

  void ServerService::runCommand(const std::string &command, const std::string &args)
  {
    if(command == CONNECT){
      // Split string into server / port pair
      Tokeniser tokeniser = Tokeniser();
      tokeniser.initTokens(args);
      std::string server = tokeniser.nextToken();
      std::string port = tokeniser.remainingTokens();
      if (port=="")
        connect(server);
      else
        connect(server, (short)atoi(port.c_str()));
    } else if(command == RECONNECT) {
      reconnect();
    } else if (command==CONNECT){
      disconnect();
    } else if (command == CREATEACC) {
      if (!myPlayer) return;
      Tokeniser tokeniser = Tokeniser();
      tokeniser.initTokens(args);
      std::string uname = tokeniser.nextToken();
      std::string password = tokeniser.nextToken();
      std::string realname = tokeniser.remainingTokens();
      myPlayer->createAccount(uname,realname,password);
    } else if (command==LOGIN) {
      if (myPlayer)
      {
        // Split string into userid / password pair
        Tokeniser tokeniser = Tokeniser();
        tokeniser.initTokens(args);
        std::string userid = tokeniser.nextToken();
        std::string password = tokeniser.remainingTokens();
        myPlayer->login(userid,password);
      }
    } else if (command==LOGOUT) {
      if (myPlayer)
      {
        myPlayer->logout();
      }
    } else if (command==CREATECHAR) {
      if (myPlayer)
      {
        //myPlayer->createCharacter();
      }
    } else if (command==TAKECHAR) {
      if (myPlayer)
      {
        //myPlayer->takeCharacter();
      }
    } else if (command==LISTCHARS) {
      if (myPlayer)
      {
        myPlayer->refreshCharacterInfo();
      }
    }
  }
} // namespace dime
