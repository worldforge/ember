/*
 Copyright (C) 2002  Martin Pollard
	
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

#include "OOGChat.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"

#include <Eris/Person.h>
#include <Eris/Lobby.h>
#include <Eris/Account.h>

#include <sstream>

namespace dime {

    // List of OOGChat's console commands
    const char * const OOGChat::CMD_TALK  = "talk";
    const char * const OOGChat::CMD_EMOTE = "emote";
    const char * const OOGChat::CMD_ME    = "me";
    const char * const OOGChat::CMD_JOIN  = "join";
    const char * const OOGChat::CMD_PART  = "part";
    const char * const OOGChat::CMD_MSG   = "msg";

    OOGChat::OOGChat(Eris::Account* account) : myLobby(NULL)
    {
      // Set up the lobby object
      //HACK: to get it to work with eris 1.3
	  myLobby = new Eris::Lobby(account);

      // Specfic to lobby Callbacks setup
      myLobby->SightPerson.connect(SigC::slot(*this,&OOGChat::sightPerson));
      myLobby->PrivateTalk.connect(SigC::slot(*this,&OOGChat::privateTalk));
      //myLobby->LoggedIn.connect(SigC::slot(*this,&OOGChat::loggedIn));

      // Ordinary rooms callbacks
      myLobby->Entered.connect(SigC::slot(*this,&OOGChat::entered));
      myLobby->Speech.connect(SigC::slot(*this,&OOGChat::talk));
      myLobby->Emote.connect(SigC::slot(*this,&OOGChat::emote));
      myLobby->Appearance.connect(SigC::slot(*this,&OOGChat::appearance));
      myLobby->Disappearance.connect(SigC::slot(*this,&OOGChat::disappearance));
//      myLobby->Changed.connect(SigC::bind(SigC::slot(*this,&OOGChat::changed),myLobby));

      ConsoleBackend::getMainConsole()->registerCommand( CMD_TALK, this );
      ConsoleBackend::getMainConsole()->registerCommand( CMD_EMOTE, this );
      ConsoleBackend::getMainConsole()->registerCommand( CMD_ME, this );
      ConsoleBackend::getMainConsole()->registerCommand( CMD_JOIN, this );
      ConsoleBackend::getMainConsole()->registerCommand( CMD_PART, this );
      ConsoleBackend::getMainConsole()->registerCommand( CMD_MSG, this );
    }

    OOGChat::~OOGChat()
    {
      // TODO: Free any allocated resources here.
      // Deregister our console commands
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_TALK );
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_EMOTE );
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_ME );
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_JOIN );
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_PART );
      ConsoleBackend::getMainConsole()->deregisterCommand( CMD_MSG );
    }


  // Lobby Specific callbacks

  void OOGChat::sightPerson(Eris::Person* person)
  {
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Sighted Person name:\""<< person->getName()<<"\" id:"<<person->getAccount()<< ENDM;    
  }

  void OOGChat::privateTalk(Eris::Person* person, const std::string& msg)
  {
    std::ostringstream temp;

    temp << "PRIVMSG("<<person->getName()<<") says:"<<msg;

    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)<<temp.str()<<ENDM;

#if 0 // not new sstream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::loggedIn( const Atlas::Objects::Entity::Player& player)
  {
    // Xmp's Notes
    // Erm dunno what this function is for eris's doxygen doesn't explain
    ConsoleBackend::getMainConsole()->pushMessage("Logged In eris msg received");
  }

  void OOGChat::runCommand(const std::string &command, const std::string &args)
  {
    // %FIXME xmp,4: Don't allow talk until we have logged in
    // %FIXME xmp,4: Stop just using myLobby to allow chat to multiple rooms

    if (command==CMD_TALK)
    {
      myLobby->say(args);
      return;
    } else if (command == CMD_EMOTE || command == CMD_ME) {
      myLobby->emote(args);
      return;
    } else if (command == CMD_JOIN) {
      return;
    } else if (command == CMD_PART) {
      return;
    } else if (command == CMD_MSG) {
      return;
    }
  }

  // All Eris::Room callbacks

  void OOGChat::entered(Eris::Room *room)
  {
    std::ostringstream temp;

    temp << "Entry of "<< room->getName()<<" complete";
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << temp.str() << ENDM;
#if 0 //not new stream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::talk(Eris::Room *room, Eris::Person* person, const std::string& msg)
  {
    std::ostringstream temp;

    temp << "["<< room->getName()<<"] "<<person->getName()<<" says: "<<msg;
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << temp.str() << ENDM;
    temp<<std::ends;
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::emote(Eris::Room *room, Eris::Person* person, const std::string& msg)
  {
    std::ostringstream temp;

    temp << "["<< room->getName()<<"] "<<person->getName()<<" "<<msg;
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << temp.str() << ENDM;
#if 0 // not new sstream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::appearance(Eris::Room *room,  Eris::Person* person)
  {
    std::ostringstream temp;

    temp << person->getName() << " appears in "<< room->getName();
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << temp.str() <<ENDM;
#if 0 // not new sstream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::disappearance(Eris::Room* room,  Eris::Person* person)
  {
    std::ostringstream temp;

    temp << person->getName() << " disappears from "<< room->getName();
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE)<<temp.str()<<ENDM;
#if 0 // if not new sstream
    temp<<std::ends;
#endif
    ConsoleBackend::getMainConsole()->pushMessage(temp.str());
  }

  void OOGChat::changed(const Eris::StringSet& sset, Eris::Room *room)
  {
  }
}
