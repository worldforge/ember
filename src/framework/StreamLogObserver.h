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

#ifndef STREAMLOGOBSERVER_H
#define STREAMLOGOBSERVER_H

// Included headers from the current project
#include "services/logging/LoggingService.h"
#include "services/logging/Observer.h"

// Included custom library headers

// Included system headers
#include <iostream>

namespace Ember {

/**
 * Log Observer which logs stuff to the stream provided on creation.
 *
 * This log observer takes an arbitrary ostream at it's creation
 * and logs all messages sent to it to this stream.
 * 
 * LoggingService *logging = EmberServices::getSingletonPtr()->getLoggingService();
 * //Create log observer that prints everything to cerr
 * StreamLogObserver* obs = new StreamLogObserver(std::cerr);
 * obs->setFilter(LoggingService::VERBOSE);
 * logging->addObserver(obs);
 *
 * @author Lakin Wecker aka nikal
 *
 * @see LoggingService::Observer
 */

class StreamLogObserver : public Ember::Observer
{

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        std::ostream &myOut;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new StreamLogObserver using default values.
     */
     StreamLogObserver(std::ostream &out);

    /**
     * Copy constructor.
     */
    StreamLogObserver( const StreamLogObserver &source );




    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a StreamLogObserver instance.
     */
    virtual ~StreamLogObserver ();

    //----------------------------------------------------------------------
    // Implmented methods from LogginService::Observer

    /**
     * Prints out the message provided with file, line and datestamp to myOut;
     */
    virtual void onNewMessage(const std::string & message, const std::string & file, const int & line, 
                                  const Ember::LoggingService::MessageImportance & importance, const time_t & timeStamp);

    //----------------------------------------------------------------------
    // Disable Assignment operator
    private:
    /**
     * Disabled Assignment operator.
     */
    StreamLogObserver &operator= ( const StreamLogObserver &source );

}; // End of StreamLogObserver

} // End of Ember namespace

#endif
