//
// C++ Implementation: TimeService
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "TimeService.h"

#include "TimeImpl.h"

#include <sstream>

namespace Ember {

namespace Services {

Time::Time()
: mImpl(new TimeImpl())
{
}


Time::~Time()
{
}

Service::Status Time::start()
{
	mImpl->initialize();
	return Service::OK;
}

void Time::stop(int code)
{

}

void Time::getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	mImpl->getLocalTime(year, month, day, hour, minute, second);
}

std::string Time::getLocalTimeStr()
{
	int year,month,day,hour,minute,second;
	std::stringstream s;
	
	mImpl->getLocalTime(year, month, day, hour, minute, second);
	
	// YYYY-MM-DD HH:MM:SS
	s << (1900+year) << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second;  
	return s.str();
		
}

bool Time::getServerTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	return mImpl->getServerTime(year, month, day, hour, minute, second);
}


}

}
