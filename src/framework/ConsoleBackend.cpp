/*
    Copyright (C) 2002  Martin Pollard (Xmp), Simon Goodall
    Copyright (C) 2005 Erik Hjortsberg <erik.hjortsberg@iteam.se>

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConsoleBackend.h"
#include "services/logging/LoggingService.h"
#include "Tokeniser.h"

#include <sstream>

namespace Ember {

// List of ConsoleBackend's console commands
const char * const ConsoleBackend::LIST_CONSOLE_COMMANDS = "list_commands";
const unsigned int ConsoleBackend::MAX_MESSAGES = 7;
template<> Ember::ConsoleBackend* Ember::Singleton<Ember::ConsoleBackend>::ms_Singleton = 0;


ConsoleBackend::ConsoleBackend(void) :
	mRegisteredCommands(ConsoleObjectEntryStore()),
	mConsoleMessages(std::list<std::string>()),
	mHistoryPosition(0)
{
	// Register console commands
	registerCommand(LIST_CONSOLE_COMMANDS, this);
}

ConsoleBackend::ConsoleBackend( const ConsoleBackend &source )
{
	// Use assignment operator to do the copy
	// NOTE: If you need to do custom initialization in the constructor this may not be enough.
	*this = source;
}

ConsoleBackend& ConsoleBackend::operator= ( const ConsoleBackend &source )
{
	// Copy fields from source class to this class here.

	// Return this object with new value
	return *this;
}

ConsoleBackend::~ConsoleBackend ()
{
	// TODO: Free any allocated resources here.
}

void ConsoleBackend::moveBackwards(void)
{
	if(mHistoryPosition < mHistory.size())
	{
		mHistoryPosition++;
	}
}

void ConsoleBackend::moveForwards(void)
{
	if(mHistoryPosition > 0)
	{
		mHistoryPosition--;
	}
}

const std::string& ConsoleBackend::getHistoryString()
{
	static std::string sEmpty("");
	
	if(mHistoryPosition == 0)
	{
		return sEmpty;
	}
	else
	{
		return mHistory[mHistoryPosition - 1];
	}
}


void ConsoleBackend::pushMessage(const std::string &message) {
	//only save message if onGotMessage returns true
	if (!onGotMessage(message)) {
	// If we have reached our message limit, remove the oldest message
		if (mConsoleMessages.size() >= MAX_MESSAGES)
		mConsoleMessages.erase(mConsoleMessages.begin());
		mConsoleMessages.push_back(message);
	}
}

bool ConsoleBackend::onGotMessage(const std::string &message)
{
	return GotMessage.emit(message);
}


void ConsoleBackend::registerCommand(const std::string &command, ConsoleObject *object, const std::string& description)
{
	S_LOG_INFO("Registering: " << command);
	
	ConsoleObjectEntry entry;
	entry.Object = object;
	entry.Description = description;
	// Assign the ConsoleObject to the command
	mRegisteredCommands[command] = entry;
	
	// prepare the prefix map to have fast access to commands
	for(std::string::size_type i = 1; i <= command.length(); ++i)
	{
		mPrefixes[command.substr(0, i)].insert(command);
	}
}

void ConsoleBackend::deregisterCommand(const std::string &command)
{
	S_LOG_INFO("Deregistering: " << command);

	// Delete from the map
	mRegisteredCommands.erase(mRegisteredCommands.find(command));
}

void ConsoleBackend::runCommand(const std::string &command, bool addToHistory)
{
	if (command.empty()) return; // Ignore empty string
	
	// Grab first character of command string
	char c = command.c_str()[0];
	
	// Check to see if command is a command, or a speech string
	if ((c != '/' && c != '+' && c != '-')) {
		// Its a speech string, so SAY it
		// FIXME /say is not always available!
		runCommand(std::string("/say ") + command, addToHistory);
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
	ConsoleObjectEntryStore::iterator I = mRegisteredCommands.find(cmd);
	
	// Print all commands to the console
	// pushMessage(command_string);
	
		if (addToHistory) {
			mHistory.push_front(command);
			mHistoryPosition = 0;
		}
	// If object exists, run the command
		if (I != mRegisteredCommands.end() && I->second.Object != 0) {
			I->second.Object->runCommand(cmd, args);
		}
	else { // Else print error message
		S_LOG_WARNING("Unknown command:" << command);
		pushMessage(std::string("Unknown command ") + command);
	}
}

void ConsoleBackend::runCommand(const std::string &command, const std::string &args)
{
	std::ostringstream temp;
	
	// This commands prints all currently registers commands to the Log File
	if (command == LIST_CONSOLE_COMMANDS) {
		for (ConsoleObjectEntryStore::const_iterator I = mRegisteredCommands.begin(); I != mRegisteredCommands.end(); I++) {
		// TODO - should we check to see if I->second is valid?
		temp << I->first<< " : " << I->second.Description << std::endl;
		}
	}
	
	S_LOG_VERBOSE(temp.str());
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
