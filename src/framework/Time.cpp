/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "Time.h"

// for the stringstream
#include <sstream>
#include <iomanip>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace Ember
{

void Time::getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();

	year = currentTime.date().year();
	month = currentTime.date().month();
	day = currentTime.date().day();
	hour = currentTime.time_of_day().hours();
	minute = currentTime.time_of_day().minutes();
	second = currentTime.time_of_day().seconds();
}

std::string Time::getLocalTimeStr()
{
	int year, month, day, hour, minute, second;
	std::stringstream s;

	getLocalTime(year, month, day, hour, minute, second);

	// YYYY-MM-DD HH:MM:SS
	s.fill('0');
	s << std::setw(4) << (1900 + year);
	s << "-";
	s << std::setw(2) << month;
	s << "-";
	s << std::setw(2) << day;
	s << " ";
	s << std::setw(2) << hour;
	s << ":";
	s << std::setw(2) << minute;
	s << ":";
	s << std::setw(2) << second;

	return s.str();

}

long long Time::currentTimeMillis()
{
	static boost::posix_time::ptime const epoch(boost::gregorian::date(1970, 1, 1));
	return (boost::posix_time::microsec_clock::universal_time() - epoch).total_milliseconds();
}

}

