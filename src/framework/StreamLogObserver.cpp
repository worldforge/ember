/*
 Copyright (C) 2002  Lakin Wecker
	
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details. 

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "StreamLogObserver.h"
#include <ctime>

namespace Ember {
    /**
     * Creates a new StreamLogObserver using default values.
     */
    StreamLogObserver::StreamLogObserver(std::ostream &out) 
        : myOut(out)
    {
    }

    /**
     * Copy constructor.
     */
    StreamLogObserver::StreamLogObserver( const StreamLogObserver &source )
        : myOut(source.myOut)
        {
        }

    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a StreamLogObserver instance.
     */
    StreamLogObserver::~StreamLogObserver () {}

    //----------------------------------------------------------------------
    // Implemented methods from LoggingService::Observer

    /**
     * Prints out the message provided with file, line and datestamp to myOut;
     */
    void StreamLogObserver::onNewMessage(const std::string & message, const std::string & file, const int & line, 
                                                 const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp)
    {
        tm * ctm = localtime(&timeStamp); //currentLocalTime was too long, sorry
        
        myOut.fill('0');
        myOut << "[";
        myOut.width(2);
        myOut << ctm->tm_hour << ":";
        myOut.width(2);
        myOut <<  ctm->tm_min << ":";
        myOut.width(2);			
        myOut << ctm->tm_sec << "] ";			

        if(importance == Ember::LoggingService::CRITICAL)
		{
			myOut << "CRITICAL";
		}
        else  if(importance == Ember::LoggingService::FAILURE)
		{
			myOut << "FAILURE";
		} 
        else if(importance == Ember::LoggingService::WARNING)
		{
			myOut << "WARNING";
		}
        else if(importance == Ember::LoggingService::INFO)
		{
			myOut << "INFO";
		}
        else
		{
			myOut << "VERBOSE";
		}
        
        myOut << " " << message;
        
        ///only write file and line number if we're in verbose mode (to make the log a little smaller in most cases
/*        if (getFilter() == Ember::LoggingService::VERBOSE) {
        	myOut << " [File: " << file << ", Line #:" <<  line << "]";
        }*/
        myOut << std::endl;

    }
}; //end namespace Ember
