/*
    Copyright (C) 2002  Martin Pollard (Xmp), Simon Goodall
    Copyright (C) 2005 Erik Ogenvik <erik@ogenvik.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// Rewritten for Ember by Martin Pollard (Xmp)

// Some code originally written for Sear by Simon Goodall, University of Southampton
// Original Copyright (C) 2001 - 2002

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ConsoleBackend.h"

#include "framework/LoggingInstance.h"
#include "Tokeniser.h"
#include "CommandHistory.h"
#include <memory>
#include <sstream>

namespace Ember {

// List of ConsoleBackend's console commands
const char* const ConsoleBackend::LIST_CONSOLE_COMMANDS = "list_commands";
const unsigned int ConsoleBackend::MAX_MESSAGES = 7;


ConsoleBackend::ConsoleBackend()
		: mCommandHistory(std::make_unique<CommandHistory>()) {
	// Register console commands
	registerCommand(LIST_CONSOLE_COMMANDS, this);
}

ConsoleBackend::~ConsoleBackend() = default;

CommandHistory& ConsoleBackend::getHistory() {
	return *mCommandHistory;
}


void ConsoleBackend::pushMessage(const std::string& message, const std::string& tag) {
	//only save message if onGotMessage returns true
	if (!onGotMessage(message, tag)) {
		// If we have reached our message limit, remove the oldest message
		if (mConsoleMessages.size() >= MAX_MESSAGES) {
			mConsoleMessages.erase(mConsoleMessages.begin());
		}
		mConsoleMessages.push_back(message);
	}
}

bool ConsoleBackend::onGotMessage(const std::string& message, const std::string& tag) {
	return GotMessage.emit(message, tag);
}


void ConsoleBackend::registerCommand(const std::string& command, ConsoleCallback callback, const std::string& description, bool suppressLogging) {
	if (!suppressLogging) {
		S_LOG_INFO("Registering: " << command);
	}

	if (mRegisteredCommands.find(command) != mRegisteredCommands.end()) {
		S_LOG_WARNING("The command '" + command + "' already has been registered.");
	} else {
		ConsoleObjectEntry entry;
		entry.Object = std::move(callback);
		entry.Description = description;
		// Assign the ConsoleObject to the command
		mRegisteredCommands[command] = entry;

		// prepare the prefix map to have fast access to commands
		for (std::string::size_type i = 1; i <= command.length(); ++i) {
			mPrefixes[command.substr(0, i)].insert(command);
		}
	}
}

void ConsoleBackend::registerCommand(const std::string& command, ConsoleObject* callback, const std::string& description, bool suppressLogging) {
	registerCommand(command, [callback](const std::string& command, const std::string& args) { callback->runCommand(command, args); }, description, suppressLogging);
}


void ConsoleBackend::deregisterCommand(const std::string& command, bool suppressLogging) {
	if (!suppressLogging) {
		S_LOG_INFO("Deregistering: " << command);
	}

	auto I = mRegisteredCommands.find(command);
	if (I == mRegisteredCommands.end()) {
		S_LOG_WARNING("Trying to deregister command '" << command << "' which isn't registered.");
	} else {
		// Delete from the map
		mRegisteredCommands.erase(I);

		for (std::string::size_type i = 1; i <= command.length(); ++i) {
			std::string prefix = command.substr(0, i);
			auto entryI = mPrefixes.find(prefix);
			if (entryI != mPrefixes.end()) {
				entryI->second.erase(command);
				if (entryI->second.empty()) {
					mPrefixes.erase(entryI);
				}
			}
		}
	}
}

void ConsoleBackend::runCommand(const std::string& command, bool addToHistory) {
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
	std::string command_string = (c == '/') ? command.substr(1) : command;

	// Split string into command / arguments pair
	Tokeniser tokeniser = Tokeniser();
	tokeniser.initTokens(command_string);
	std::string cmd = tokeniser.nextToken();
	std::string args = tokeniser.remainingTokens();

	//Grab object registered to the command
	auto I = mRegisteredCommands.find(cmd);

	// Print all commands to the console
	// pushMessage(command_string);

	if (addToHistory) {
		mCommandHistory->addToHistory(command);
	}
	// If object exists, run the command
	if (I != mRegisteredCommands.end() && I->second.Object != nullptr) {
		I->second.Object(cmd, args);
	} else { // Else print error message
		S_LOG_WARNING("Unknown command:" << command);
		pushMessage(std::string("Unknown command ") + command, "error");
	}
}

void ConsoleBackend::runCommand(const std::string& command, const std::string& args) {
	std::ostringstream temp;

	// This commands prints all currently registered commands to the Log File
	if (command == LIST_CONSOLE_COMMANDS) {
		for (const auto& registeredCommand : mRegisteredCommands) {
			// TODO - should we check to see if I->second is valid?
			temp << registeredCommand.first << " : " << registeredCommand.second.Description << std::endl;
		}
	}

	S_LOG_VERBOSE(temp.str());
	temp << std::ends;

	pushMessage(temp.str());
}

const std::set<std::string>& ConsoleBackend::getPrefixes(const std::string& prefix) const {
	static std::set<std::string> empty;
	auto iPrefixes(mPrefixes.find(prefix));

	if (iPrefixes != mPrefixes.end()) {
		return iPrefixes->second;
	}

	return empty;
}

} // namespace Ember
