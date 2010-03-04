//
// C++ Interface: TimeService
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBER_SERVICESTIMESERVICE_H
#define EMBER_SERVICESTIMESERVICE_H

#include "framework/Service.h"

//include the EmberServices here, since you always need that anyway whenever you need to access the service
#include "../EmberServices.h"

#ifdef __WIN32__
// GetTickCount()
#include <windows.h>
#else
// for gettimeofday and timeval struct
// TODO: there has to be a better more c++ ish way to do this
#include <sys/time.h>
#include <time.h>
#endif

namespace Ember {

namespace Services {

class TimeImpl;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
	@author Sean Ryan <sryan@evercrack.com>
	
	Provides time services, both for local time and server time.
*/
class Time : public Service
{
public:
    Time();

    virtual ~Time();
    
    /**
     * Starts ConfigService.  Returns status.
     *
     */
    Service::Status start(void);

    /**
     * Stops ConfigService.
     *
     * @param stop code.
     */
    void stop(int code);    
    
	/**
	 *    Gets the local time of the client machine.
	 * @param year 
	 * @param month 
	 * @param day 
	 * @param hour 
	 * @param minute 
	 * @param second 
	 */
	void getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second);
	
	/**
	 * utility method to get a string of the current time
	 */
	std::string getLocalTimeStr(void);

	/**
	 * utility that will get the epoc time in milliseconds from the local system
	 * @return long int value representing current local time in milliseconds
	 */
	long currentTimeMillis(void);
	
	/**
	 *    Gets the server time. If we for some reason can't get the server time, local time is returned.
	 * @param year 
	 * @param month 
	 * @param day 
	 * @param hour 
	 * @param minute 
	 * @param second 
	 * @return True if we successfully got the server time, else false (we then used the local time).
	 */
	bool getServerTime(int& year, int& month, int& day, int& hour, int& minute, int& second);
	
	
	
protected:

	/**
	The actual implementation.
	*/
	std::auto_ptr<TimeImpl> mImpl;

};

}

}

#endif
