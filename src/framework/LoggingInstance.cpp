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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#include <Atlas/Codecs/Bach.h>
#include <Atlas/Objects/Encoder.h>
#include <Atlas/Objects/SmartPtr.h>

#include <sstream>
#include "LoggingInstance.h"

namespace Ember {

LoggingInstance::LoggingInstance(std::string file, int line, Log::MessageImportance importance)
: mFile(std::move(file)), mLine(line), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(Log::MessageImportance importance)
: mLine(-1), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(std::string file, Log::MessageImportance importance)
: mFile(std::move(file)), mLine(-1), mImportance(importance)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(std::string file, int line)
: mFile(std::move(file)), mLine(line), mImportance(Log::MessageImportance::INFO)
{
	mMessage.reserve(256);
}

LoggingInstance::LoggingInstance(std::string file)
: mFile(std::move(file)), mLine(-1), mImportance(Log::MessageImportance::INFO)
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

LoggingInstance & LoggingInstance::operator<< (unsigned int uintToAdd)
{
	mMessage += std::to_string(uintToAdd);

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (int intToAdd)
{
	mMessage += std::to_string(intToAdd);

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (Log::HexNumber intHexToAdd)
{
	char buffer[Log::NUMBER_BUFFER_SIZE];
	sprintf (buffer, "%x", intHexToAdd.myNumber);
	mMessage += buffer;
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (double doubleToAdd)
{
	mMessage += std::to_string(doubleToAdd);
	return *this;
}

LoggingInstance & LoggingInstance::operator<< (long longToAdd)
{
	mMessage += std::to_string(longToAdd);

	return *this;
}

LoggingInstance & LoggingInstance::operator<< (unsigned long ulongToAdd)
{
	mMessage += std::to_string(ulongToAdd);
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (long long longLongToAdd)
{
	mMessage += std::to_string(longLongToAdd);
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (unsigned long long ulongLongToAdd)
{
	mMessage += std::to_string(ulongLongToAdd);
	return *this;
}

LoggingInstance& LoggingInstance::operator<< (const std::exception& exception)
{
	mMessage += "\n\tException: " + std::string(exception.what());
	return *this;
}

LoggingInstance& LoggingInstance::operator<<(const Atlas::Objects::SmartPtr<Atlas::Objects::RootData>& obj)
{
	std::stringstream s;
	Atlas::Codecs::Bach debugCodec(s, s, *(Atlas::Bridge*)nullptr);
	Atlas::Objects::ObjectsEncoder debugEncoder(debugCodec);
	debugEncoder.streamObjectsMessage(obj);
	mMessage += s.str();
	return *this;
}

LoggingInstance& LoggingInstance::operator<<(const Atlas::Message::Element& msg)
{
	std::stringstream s;
	Atlas::Codecs::Bach debugCodec(s, s, *(Atlas::Bridge*)nullptr);
	Atlas::Message::Encoder debugEncoder(debugCodec);
	debugEncoder.streamMessageElement(msg.asMap());
	mMessage += s.str();
	return *this;
}


}
