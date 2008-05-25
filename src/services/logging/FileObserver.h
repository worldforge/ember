//
// C++ Interface: FileObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2008
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
#ifndef EMBER_LOGGINGSERVICEFILEOBSERVER_H
#define EMBER_LOGGINGSERVICEFILEOBSERVER_H

#include "Observer.h"
#include <ctime>
#include <stdarg.h>
#include <memory>

namespace Ember {

	/**
	* Predefined implementation of Observer-class handling FILE * 
	*
	* The format of messages written into a FILE * is the following:
	* 
	* timeStamp "\t" importance "\t" file "\t" line "\t" message
	*/
	class FileObserver : public Observer
	{
	public:
		FileObserver (FILE * file, LoggingService::MessageImportance filter)
		{
			setFilter (filter);
			myFile = file;
		}
		virtual ~FileObserver() {}

		virtual void onNewMessage (const std::string & message,
								const std::string & file,
								const int &line,
								const LoggingService::MessageImportance & importance,
								const time_t & timeStamp)
		{
			tm *ctm = localtime (&timeStamp);       //currentLocalTime was too long, sorry

			fprintf (myFile,
					"[%04d-%02d-%02d %02d:%02d:%02d]\t%s\t%s\t%d\t%s\n",
					ctm->tm_year, ctm->tm_mon, ctm->tm_mday,
					ctm->tm_hour, ctm->tm_min, ctm->tm_sec,
					(importance ==
					LoggingService::CRITICAL) ? "CRITICAL" : ((importance ==
												LoggingService::FAILURE) ? "FAILURE"
												: ((importance ==
													LoggingService::WARNING) ? "WARNING" :
												((importance== LoggingService::INFO) ?"INFO":"VERBOSE"))),
					file.c_str (), line, message.c_str ());
		}

		FILE *getFile ()
		{
			return myFile;
		}

	private:
		FILE * myFile;
	};



}

#endif
