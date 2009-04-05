//
// C++ Implementation: Log
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Log.h"
#include "LogObserver.h"
#include "LoggingInstance.h"
#include <algorithm>




namespace Ember {

void Log::log (const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam("", -1, INFO, message, vl);
	va_end(vl);
}


void Log::log (const char *file, const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam(file, -1, INFO, message, vl);
	va_end(vl);
}



void Log::log (const char *file, const int line, const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam(file, line, INFO, message, vl);
	va_end(vl);
}


void Log::log (const MessageImportance importance, const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam("", -1, importance, message, vl);
	va_end(vl);
}

void Log::log (const char *file, const MessageImportance importance, const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam(file, -1, importance, message, vl);
	va_end(vl);
}

void Log::log (const char *file, const int line,  const MessageImportance importance, const char *message, ...)
{
	va_list vl;
	va_start(vl, message);
	logVarParam(file, line, importance, message, vl);
	va_end(vl);
}

void Log::logVarParam (const char *file, const int line, const MessageImportance importance, const char *message, va_list argptr)
{
	char Buffer[MESSAGE_BUFFER_SIZE];
	vsprintf((char *) Buffer, message, argptr);
	sendMessage(std::string((char *) Buffer), file, line, importance);
}


LoggingInstance Log::slog (const std::string & file, const int line, const MessageImportance importance)
{
	return LoggingInstance(file, line, importance);
}

LoggingInstance Log::slog (const MessageImportance importance)
{
	return LoggingInstance(importance);
}

LoggingInstance Log::slog (const std::string & file, const MessageImportance importance)
{
	return LoggingInstance(file, importance);
}

LoggingInstance Log::slog (const std::string & file, const int line)
{
	return LoggingInstance(file, line);
}

LoggingInstance Log::slog (const std::string & file)
{
	return LoggingInstance(file);
}




void Log::addObserver(LogObserver* observer)
{
	//test on already existing observer
	if (std::find(sObserverList.begin(), sObserverList.end(), observer) == sObserverList.end()) {
		//no existing observer, add a new
		sObserverList.push_back(observer);
	}
}

int Log::removeObserver(LogObserver* observer)
{
	ObserverList::iterator I = std::find(sObserverList.begin(), sObserverList.end(), observer);
	if (I != sObserverList.end()) {
		sObserverList.erase(I);
		return 0;
	}
	return -1;
}

Log::HexNumber Log::hexNumber(const int intDecimal)
{
	HexNumber intHex;
	intHex.myNumber = intDecimal;
	return intHex;
}

void Log::sendMessage(const std::string & message, const std::string & file, const int line, const MessageImportance importance)
{
	time_t currentTime;
	time(&currentTime);

	for (ObserverList::iterator i = sObserverList.begin(); i != sObserverList.end(); i++) {
		if (static_cast<int>(importance) >= static_cast<int>((*i)->getFilter())) {
			(*i)->onNewMessage(message, file, line, importance, currentTime);
		}
	}
}

}
