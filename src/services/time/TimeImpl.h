//
// C++ Interface: TimeImpl
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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
#ifndef EMBER_SERVICESTIMEIMPL_H
#define EMBER_SERVICESTIMEIMPL_H

#include <memory>

namespace Eris
{
class Calendar;
class Avatar;
}

namespace Ember {

namespace Services {

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TimeImpl{
public:
    TimeImpl();

    ~TimeImpl();

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
	
	void initialize();
	
protected:
	std::auto_ptr<Eris::Calendar> mCalendar;
	
	void Server_GotAvatar(Eris::Avatar* avatar);
	void createCalendar(Eris::Avatar* avatar);

};

}

}

#endif
