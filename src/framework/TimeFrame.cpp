/*
 Copyright (C) 2011 Erik Hjortsberg

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

#include "TimeFrame.h"

namespace Ember
{
TimeFrame::TimeFrame(MicrosecondType timeSliceMicroseconds) :
		mStartTime(boost::posix_time::microsec_clock::local_time()), mTimeSliceMicroseconds(timeSliceMicroseconds)
{
}

bool TimeFrame::isTimeLeft() const
{
	if ((boost::posix_time::microsec_clock::local_time() - mStartTime).total_microseconds() > mTimeSliceMicroseconds) {
		return false;
	}
	return true;
}

TimeFrame::MicrosecondType TimeFrame::getRemainingTimeInMicroseconds() const
{
	MicrosecondType remaining = getElapsedTimeInMicroseconds() - mTimeSliceMicroseconds;
	return std::max<long long>(0, -remaining);
}

TimeFrame::MicrosecondType TimeFrame::getElapsedTimeInMicroseconds() const
{
	return (boost::posix_time::microsec_clock::local_time() - mStartTime).total_microseconds();
}


}
