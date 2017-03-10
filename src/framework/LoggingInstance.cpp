//
// C++ Implementation: LoggingInstance
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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

namespace Ember {

LoggingInstance::LoggingInstance(const std::string & file, const int line, const Log::MessageImportance importance)
: mFile(file), mLine(line), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(const Log::MessageImportance importance)
: mFile(""), mLine(-1), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(const std::string & file, const Log::MessageImportance importance)
: mFile(file), mLine(-1), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(const std::string & file, const int line)
: mFile(file), mLine(line), mImportance(Log::INFO)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(const std::string & file)
: mFile(file), mLine(-1), mImportance(Log::INFO)
{
	mMessage.reserve(256);
}

LoggingInstance::~LoggingInstance()
{
	//If we haven't sent to the service yet, do it now.
	if (!mMessage.empty()) {
		Log::sendMessage (mMessage, mFile, mLine, mImportance);
	}
}

LoggingInstance & LoggingInstance::operator<< (const std::string & stringToAdd)
{
	mMessage += stringToAdd;
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (const char* stringToAdd)
{
	mMessage += stringToAdd;
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const unsigned int uintToAdd)
{
	mMessage += std::to_string(uintToAdd);

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const int intToAdd)
{
	mMessage += std::to_string(intToAdd);

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
	mMessage += std::to_string(doubleToAdd);
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const long longToAdd)
{
	mMessage += std::to_string(longToAdd);

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (const unsigned long ulongToAdd)
{
	mMessage += std::to_string(ulongToAdd);
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (const long long longLongToAdd)
{
	mMessage += std::to_string(longLongToAdd);
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (const unsigned long long ulongLongToAdd)
{
	mMessage += std::to_string(ulongLongToAdd);
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
