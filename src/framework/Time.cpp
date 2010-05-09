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

#ifdef __WIN32__
// GetTickCount()
#include <windows.h>
#else
// for gettimeofday and timeval struct
// TODO: there has to be a better more c++ ish way to do this
#include <sys/time.h>
#include <time.h>
#endif

// for the stringstream
#include <sstream>
#include <iomanip>


namespace Ember
{

void Time::getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	/// Set some time parameters
	time_t t = time(&t);
	struct tm *t2 = localtime(&t);
	year = t2->tm_year;
	month = t2->tm_mon;
	day = t2->tm_mday;
	hour = t2->tm_hour;
	minute = t2->tm_min;
	second = t2->tm_sec;
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

/*
 * Almost precision time acquisition
 * NOTE: there is no reliable way to get time in milliseconds reliably cross platform
 */
long Time::currentTimeMillis(void)
{
	long ttime;

#ifdef __WIN32__
	// ttime set to milliseconds *since midnight*
	ttime = (long)GetTickCount();
#else
	// tv_usec is microseconds
	// tv_sec  is seconds
	// ttime is set to epoc milliseconds
	struct timeval detail_time;
	gettimeofday(&detail_time, NULL);
	ttime = (long)((detail_time.tv_usec / 1000) + (detail_time.tv_sec * 1000));
#endif
	return ttime;
}
}

#include "Time.h"

