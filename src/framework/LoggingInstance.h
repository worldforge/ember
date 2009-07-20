//
// C++ Interface: LoggingInstance
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
#ifndef EMBERLOGGINGINSTANCE_H
#define EMBERLOGGINGINSTANCE_H

#include "Log.h"

#define S_LOG_VERBOSE(message) (Ember::Log::slog(__FILE__, __LINE__, Ember::Log::VERBOSE) << message << ENDM)
#define S_LOG_INFO(message) (Ember::Log::slog(__FILE__, __LINE__, Ember::Log::INFO) << message << ENDM)
#define S_LOG_WARNING(message) (Ember::Log::slog(__FILE__, __LINE__, Ember::Log::WARNING) << message << ENDM)
#define S_LOG_FAILURE(message) (Ember::Log::slog(__FILE__, __LINE__, Ember::Log::FAILURE) << message << ENDM)
#define S_LOG_CRITICAL(message) (Ember::Log::slog(__FILE__, __LINE__, Ember::Log::CRITICAL) << message << ENDM)

namespace Ember {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class LoggingInstance{
public:
	friend class Log;

	LoggingInstance(const std::string & file, const int line, const Log::MessageImportance importance);

	LoggingInstance(const Log::MessageImportance importance);

	LoggingInstance(const std::string & file, const Log::MessageImportance importance);

	LoggingInstance(const std::string & file, const int line);

	LoggingInstance(const std::string & file);


	LoggingInstance& operator<< (const std::string & stringToAdd);

	LoggingInstance& operator<< (const int intToAdd);

	LoggingInstance& operator<< (const unsigned int uintToAdd);

	LoggingInstance& operator<< (const long longToAdd);

	LoggingInstance& operator<< (const unsigned long ulongToAdd);

	LoggingInstance& operator<< (const Log::HexNumber & intHexToAdd);

	LoggingInstance& operator<< (const double doubleToAdd);

	LoggingInstance& operator<< (const std::exception& exception);

	/**
	 * @brief At destruction the message will be written, if not already done.
	 */
	~LoggingInstance();

	/**
	* By streaming in END_MESSAGE (equally to ENDM-macro) you finish the message and start
	* sending it.
	*/
	void operator<< (const Log::EndMessageEnum endMessage);

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
