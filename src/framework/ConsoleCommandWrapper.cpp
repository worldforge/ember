//
// C++ Implementation: ConsoleCommandWrapper
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include "ConsoleCommandWrapper.h"
#include "ConsoleBackend.h"
#include "framework/LoggingInstance.h"
#include <utility>

namespace Ember {


ConsoleCommandWrapper::ConsoleCommandWrapper(std::string command, ConsoleObject* object, std::string description, bool suppressLogging)
		: mCommand(std::move(command)),
		  mDescription(std::move(description)),
		  mSuppressLogging(suppressLogging) {
	if (!mCommand.empty() && mCommand[0] == '+') {
		mInverseCommand = std::string("-") + std::string(mCommand).erase(0, 1);
	}
	if (ConsoleBackend::hasInstance()) {
		ConsoleBackend::getSingletonPtr()->registerCommand(mCommand, object, mDescription, suppressLogging);
		if (!mInverseCommand.empty()) {
			ConsoleBackend::getSingletonPtr()->registerCommand(mInverseCommand, object, std::string("Releases the command ") + mCommand, suppressLogging);
		}
	} else {
		S_LOG_WARNING("Could not register command " << mCommand << " since there was no console backend.");
	}
}

ConsoleCommandWrapper::ConsoleCommandWrapper(ConsoleBackend& consoleBackend, std::string command, ConsoleObject* object, std::string description, bool suppressLogging)
		: mCommand(std::move(command)),
		  mDescription(std::move(description)),
		  mSuppressLogging(suppressLogging) {
	if (!mCommand.empty() && mCommand[0] == '+') {
		mInverseCommand = std::string("-") + std::string(mCommand).erase(0, 1);
	}
	consoleBackend.registerCommand(mCommand, object, mDescription, suppressLogging);
	if (!mInverseCommand.empty()) {
		consoleBackend.registerCommand(mInverseCommand, object, std::string("Releases the command ") + mCommand, suppressLogging);
	}
}

ConsoleCommandWrapper::ConsoleCommandWrapper(ConsoleBackend& consoleBackend, std::string command, ConsoleCallback callback, const std::string& description, bool suppressLogging)
		: mCommand(std::move(command)),
		  mSuppressLogging(suppressLogging) {
	consoleBackend.registerCommand(mCommand, std::move(callback), description, suppressLogging);
}


ConsoleCommandWrapper::~ConsoleCommandWrapper() {
	if (ConsoleBackend::hasInstance()) {
		ConsoleBackend::getSingletonPtr()->deregisterCommand(mCommand, mSuppressLogging);
		if (!mInverseCommand.empty()) {
			ConsoleBackend::getSingletonPtr()->deregisterCommand(mInverseCommand, mSuppressLogging);
		}
	}
}


}
