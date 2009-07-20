//
// C++ Implementation: LoggingInstance
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

#include "LoggingInstance.h"
#include <sstream>
#include <cstdio>
#include <exception>

namespace Ember {

LoggingInstance::LoggingInstance(const std::string & file, const int line, const Log::MessageImportance importance)
: mFile(file), mLine(line), mImportance(importance), mMessage("")
{
}

LoggingInstance::LoggingInstance(const Log::MessageImportance importance)
: mFile(""), mLine(-1), mImportance(importance), mMessage("")
{
}

LoggingInstance::LoggingInstance(const std::string & file, const Log::MessageImportance importance)
: mFile(file), mLine(-1), mImportance(importance), mMessage("")
{
}

LoggingInstance::LoggingInstance(const std::string & file, const int line)
: mFile(file), mLine(line), mImportance(Log::INFO), mMessage("")
{
}

LoggingInstance::LoggingInstance(const std::string & file)
: mFile(file), mLine(-1), mImportance(Log::INFO), mMessage("")
{
}

LoggingInstance::~LoggingInstance()
{
	///If we haven't sent to the service yet, do it now.
	if (mMessage != "") {
		Log::sendMessage (mMessage, mFile, mLine, mImportance);
	}
}

LoggingInstance & LoggingInstance::operator<< (const std::string & stringToAdd)
{
	mMessage += stringToAdd;
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const unsigned int uintToAdd)
{
	std::stringstream ss;
	ss << uintToAdd;
	mMessage += ss.str();

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const int intToAdd)
{
	std::stringstream ss;
	ss << intToAdd;
	mMessage += ss.str();

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const Log::HexNumber & intHexToAdd)
{
	char buffer[Log::NUMBER_BUFFER_SIZE];
	sprintf (buffer, "%x", intHexToAdd.myNumber);
	mMessage += buffer;
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const double doubleToAdd)
{
	std::stringstream ss;
	ss << doubleToAdd;
	mMessage += ss.str();
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const long longToAdd)
{
	std::stringstream ss;
	ss << longToAdd;
	mMessage += ss.str();

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const unsigned long ulongToAdd)
{
	std::stringstream ss;
	ss << ulongToAdd;
	mMessage += ss.str();
	return *this;
}


void LoggingInstance::operator<< (const Log::EndMessageEnum endMessage)
{
	Log::sendMessage(mMessage, mFile, mLine, mImportance);
	mMessage = "";
}

LoggingInstance& LoggingInstance::operator<< (const std::exception& exception)
{
	mMessage += "\n\tException: " + std::string(exception.what());
	return *this;
}



}
