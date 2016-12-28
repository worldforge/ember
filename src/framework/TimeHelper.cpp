/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#include "TimeHelper.h"

#include <boost/date_time/c_time.hpp>

// for the stringstream
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>

namespace Ember
{

void TimeHelper::getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	// latest version of boost::posix_time seems to be broken on win32. Fallback to boost::date_time::c_time
	tm storage;
	time_t t = time(nullptr);
	tm* now = boost::date_time::c_time::localtime(&t, &storage); // Thread-safe version of localtime

	second = now->tm_sec;
	minute = now->tm_min;
	hour = now->tm_hour;
	day = now->tm_mday;
	month = now->tm_mon + 1;
	year = now->tm_year + 1900;
}

std::string TimeHelper::getLocalTimeStr()
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

long long TimeHelper::currentTimeMillis()
{
	// Determine miliseconds since epoch (1970.01.01) with C++11 chrono.
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

}

