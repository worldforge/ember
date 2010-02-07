//
// C++ Implementation: TimeImpl
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TimeImpl.h"
#include "../EmberServices.h"
#include "../server/ServerService.h"
#include "framework/LoggingInstance.h"
#include <time.h>

#include <Eris/Calendar.h>

namespace Ember {

namespace Services {

TimeImpl::TimeImpl()
: mCalendar(0)
{
}


TimeImpl::~TimeImpl()
{
}

void TimeImpl::initialize()
{
	///we either already have the avatar, or we need to wait until it's been created
	Eris::Avatar* avatar(0);
	if ((avatar = EmberServices::getSingleton().getServerService()->getAvatar())) {
		createCalendar(avatar);
	} else {
		EmberServices::getSingleton().getServerService()->GotAvatar.connect(sigc::mem_fun(*this, &TimeImpl::Server_GotAvatar));
	}
}


void TimeImpl::getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	/// Set some time parameters
	time_t t = time (&t);
	struct tm *t2 = localtime (&t);
	year = t2->tm_year;
	month = t2->tm_mon;
	day = t2->tm_mday;
	hour = t2->tm_hour;
	minute = t2->tm_min;
	second = t2->tm_sec;
}

bool TimeImpl::getServerTime(int& year, int& month, int& day, int& hour, int& minute, int& second)
{
	///try to get the server time, and if not possible fall back to the local time
	if (mCalendar.get()) {
		Eris::DateTime dt = mCalendar->now();
		if (dt.valid()) {
			year = dt.year();
			month = dt.month();
			day = dt.dayOfMonth();
			hour = dt.hours();
			minute = dt.minutes();
			second = dt.seconds();
			return true;
		}
	}
	getLocalTime(year, month, day, hour, minute, second);
	return false;
}

void TimeImpl::Server_GotAvatar(Eris::Avatar* avatar)
{
	createCalendar(avatar);
}

void TimeImpl::createCalendar(Eris::Avatar* avatar)
{
	try {
		mCalendar = std::auto_ptr<Eris::Calendar>(new Eris::Calendar(avatar));
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating calendar object." << ex);
		mCalendar.release();
	}
}

}

}
