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
TimeFrame::TimeFrame(const boost::posix_time::time_duration timeSlice) :
		mStartTime(boost::posix_time::microsec_clock::local_time()), mTimeSlice(timeSlice)
{
}

bool TimeFrame::isTimeLeft() const
{
	if ((boost::posix_time::microsec_clock::local_time() - mStartTime) > mTimeSlice) {
		return false;
	}
	return true;
}

boost::posix_time::time_duration TimeFrame::getRemainingTime() const
{
	static const boost::posix_time::time_duration zeroTime = boost::posix_time::microseconds(0);
	const boost::posix_time::time_duration remaining = getElapsedTime() - mTimeSlice;

	return std::max<boost::posix_time::time_duration>(zeroTime, -remaining);
}

boost::posix_time::time_duration TimeFrame::getElapsedTime() const
{
	return boost::posix_time::microsec_clock::local_time() - mStartTime;
}

}
