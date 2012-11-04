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

#ifndef TIMEFRAME_H_
#define TIMEFRAME_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace Ember
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Represents a time frame of future time.
 *
 * An instance of this is to be used whenever you want to check whether a certain amount of time has elapsed.
 * The typical situation where it would be useful is when rendering a frame, and you don't want to spend too much time until the next frame.
 */
class TimeFrame
{
public:
	/**
	 * @brief Ctor.
	 * @param timeSliceMilliseconds The slice of time for this time frame.
	 */
	TimeFrame(long long timeSliceMilliseconds);

	/**
	 * @brief Checks whether there's any time left until the threshold has passed.
	 * @return True if there's any time left.
	 */
	bool isTimeLeft() const;

private:

	/**
	 * @brief Time when the task started.
	 */
	boost::posix_time::ptime mStartTime;

	/**
	 * @brief The slice of time for this time frame.
	 */
	long long mTimeSliceMicroseconds;
};

}

#endif /* TIMEFRAME_H_ */
