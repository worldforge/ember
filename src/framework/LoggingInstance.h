//
// C++ Interface: LoggingInstance
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
#ifndef EMBERLOGGINGINSTANCE_H
#define EMBERLOGGINGINSTANCE_H

#include "Log.h"

#define S_LOG_VERBOSE(message) (Ember::LoggingInstance(__FILE__, __LINE__, Ember::Log::MessageImportance::VERBOSE) << message)
#define S_LOG_INFO(message) (Ember::LoggingInstance(__FILE__, __LINE__, Ember::Log::MessageImportance::INFO) << message)
#define S_LOG_WARNING(message) (Ember::LoggingInstance(__FILE__, __LINE__, Ember::Log::MessageImportance::WARNING) << message)
#define S_LOG_FAILURE(message) (Ember::LoggingInstance(__FILE__, __LINE__, Ember::Log::MessageImportance::FAILURE) << message)
#define S_LOG_CRITICAL(message) (Ember::LoggingInstance(__FILE__, __LINE__, Ember::Log::MessageImportance::CRITICAL) << message)

namespace Atlas {
namespace Message {
class Element;
}
namespace Objects {
template<class T>
class SmartPtr;

class RootData;
}
}

namespace Ember {

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class LoggingInstance {
public:
	friend class Log;

	LoggingInstance(std::string  file, int line, Log::MessageImportance importance);

	explicit LoggingInstance(Log::MessageImportance importance);

	LoggingInstance(std::string  file, Log::MessageImportance importance);

	LoggingInstance(std::string  file, int line);

	explicit LoggingInstance(std::string  file);


	LoggingInstance& operator<<(const std::string& stringToAdd);

	LoggingInstance& operator<<(const char* stringToAdd);

	LoggingInstance& operator<<(int intToAdd);

	LoggingInstance& operator<<(unsigned int uintToAdd);

	LoggingInstance& operator<<(long longToAdd);

	LoggingInstance& operator<<(unsigned long ulongToAdd);

	LoggingInstance& operator<<(long long longLongToAdd);

	LoggingInstance& operator<<(unsigned long long ulongLongToAdd);

	LoggingInstance& operator<<(Log::HexNumber intHexToAdd);

	LoggingInstance& operator<<(double doubleToAdd);

	LoggingInstance& operator<<(const std::exception& exception);

	LoggingInstance& operator<<(const Atlas::Objects::SmartPtr<Atlas::Objects::RootData>& obj);

	LoggingInstance& operator<<(const Atlas::Message::Element& msg);

	/**
	 * @brief At destruction the message will be written, if not already done.
	 */
	~LoggingInstance();

private:

	/**
	 * @brief This is private to prevent copying.
	 * @param l
	 */
	LoggingInstance(const LoggingInstance& l);


	/**
	* currently set source file (option; used by << streaming only)
	* An empty string indicates that no file option was set.
	*/
	std::string mFile;

	/**
	* currently set source code line (option; used by << streaming only)
	* -1 indicates that no line option was set.
	*/
	int mLine;


	/**
	* currently set importance (option; used by << streaming only)
	* The default value is INFO.
	*/
	Log::MessageImportance mImportance;

	/**
	currently given part of the message string (used by << streaming only)
	*/
	std::string mMessage;

};

}

#endif
