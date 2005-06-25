/*
    Copyright (C) 2002  Martin Pollard (Xmp), Simon Goodall

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

// Rewritten for Ember by Martin Pollard (Xmp)

// Some code originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002

#include "ConsoleBackend.h"
#include "services/logging/LoggingService.h"

#include <sstream>

namespace Ember {

// List of ConsoleBackend's console commands
const char * const ConsoleBackend::LIST_CONSOLE_COMMANDS = "list_commands";
const unsigned int ConsoleBackend::MAX_MESSAGES = 7;

Ember::ConsoleBackend *Ember::ConsoleBackend::theMainConsole = NULL;

void ConsoleBackend::pushMessage(const std::string &message) {
  //only save message if onGotMessage returns true
  if (!onGotMessage(message)) {
  // If we have reached our message limit, remove the oldest message
	if (myConsoleMessages.size() >= MAX_MESSAGES)
	myConsoleMessages.erase(myConsoleMessages.begin());
	myConsoleMessages.push_back(message);
  }
}

bool ConsoleBackend::onGotMessage(const std::string &message)
{
	return GotMessage.emit(message);
}


void ConsoleBackend::registerCommand(const std::string &command, ConsoleObject *object)
{
LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Registering: " << command << ENDM;

// Assign the ConsoleObject to the command
myRegisteredCommands[command] = object;
	
	// prepare the prefix map to have fast access to commands
	for(std::string::size_type i = 1; i <= command.length(); ++i)
	{
		mPrefixes[command.substr(0, i)].insert(command);
	}
}

void ConsoleBackend::deregisterCommand(const std::string &command)
{
  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Deregistering: " << command << ENDM;

  // Delete from the map
  myRegisteredCommands.erase(myRegisteredCommands.find(command));
}

void ConsoleBackend::runCommand(const std::string &command)
{
  if (command.empty()) return; // Ignore empty string

  // Grab first character of command string
  char c = command.c_str()[0];

  // Check to see if command is a command, or a speech string
  if ((c != '/' && c != '+' && c != '-')) {
    // Its a speech string, so SAY it
    // FIXME /say is not always available!
    runCommand(std::string("/say ") + command);
    return; 
  }

  // If command has a leading /, remove it
  std::string command_string = (c == '/')? command.substr(1) : command;

  // Split string into command / arguments pair
  Tokeniser tokeniser = Tokeniser();
  tokeniser.initTokens(command_string);
  std::string cmd = tokeniser.nextToken();
  std::string args = tokeniser.remainingTokens();

  //Grab object registered to the command
  ConsoleObject* con_obj = myRegisteredCommands[cmd];

  // Print all commands to the console
  // pushMessage(command_string);

	mHistory.push_front(command);
	mHistoryPosition = 0;
  // If object exists, run the command
	if(con_obj != 0)
		con_obj->runCommand(cmd, args);
  else { // Else print error message
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::WARNING) << "Unknown command:"<<command<< ENDM;
    pushMessage("Unknown command");
  }
}

void ConsoleBackend::runCommand(const std::string &command, const std::string &args)
{
  std::ostringstream temp;

  // This commands prints all currently registers commands to the Log File
  if (command == LIST_CONSOLE_COMMANDS) {
    for (std::map<std::string, ConsoleObject*>::const_iterator I = myRegisteredCommands.begin(); I != myRegisteredCommands.end(); I++) {
      // TODO - should we check to see if I->second is valid?
      temp << I->first<< " ";
    }
  }

  LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << temp.str() << ENDM;
  temp<< std::ends;

  pushMessage(temp.str());
}

const std::set< std::string > & ConsoleBackend::getPrefixes(const std::string & prefix) const
{
	static std::set< std::string > empty;
	std::map< std::string, std::set< std::string > >::const_iterator iPrefixes(mPrefixes.find(prefix));
	
	if(iPrefixes != mPrefixes.end())
	{
		return iPrefixes->second;
	}
		
	return empty;
}

void ConsoleBackend::changeHistory(size_t stHistoryIndex, const std::string & sCommand)
{
	if(stHistoryIndex < mHistory.size())
	{
		mHistory[stHistoryIndex - 1] = sCommand;
	}
}

} // namespace Ember
