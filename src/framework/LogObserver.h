//
// C++ Interface: LogObserver
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
#ifndef EMBERLOGOBSERVER_H
#define EMBERLOGOBSERVER_H

#include "Log.h"

namespace Ember {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	Abstract base class (=interface) for all observers
*/
class LogObserver
{
public:

	LogObserver()
	{
		mFilter = Log::INFO;        //No filtering assumed
	}
	
	virtual ~LogObserver() {}

	/**
	* Called every time a new message arrived at the LoggingService
	*
	* @param message The message string to send.
	* @param file The source code file the message was initiated or empty if not specified.
	* @param line The source code line the message was initiated or -1 if not specified.
	* @param importance The level of importance (see MessageImportance enum)
	* @param time_t The time the message was initiated.
	*/
	virtual void onNewMessage (const std::string & message,
							const std::string & file,
							const int &line,
							const Log::MessageImportance & importance,
							const time_t & timeStamp) = 0;

	Log::MessageImportance getFilter ()
	{
		return mFilter;
	}

	void setFilter (Log::MessageImportance filter)
	{
		mFilter = filter;
	}

private:

	/**
	* A filter used by the LoggingService to determine wether the message should be send
	* to onNewMessage. This happens only if the message is at least as important as
	* the filter value.
	*/
	Log::MessageImportance mFilter;
};


}

#endif
