/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef TIMEFRAME_H_
#define TIMEFRAME_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace Ember {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents a time frame of future time.
 *
 * An instance of this is to be used whenever you want to check whether a certain amount of time has elapsed.
 * The typical situation where it would be useful is when rendering a frame, and you don't want to spend too much time until the next frame.
 */
class TimeFrame {
public:

	/**
	 * @brief Ctor.
	 * @param timeSlice The slice of time for this time frame.
	 */
	explicit TimeFrame(boost::posix_time::time_duration timeSlice);

	/**
	 * @brief Checks whether there's any time left until the threshold has passed.
	 * @return True if there's any time left.
	 */
	bool isTimeLeft() const;

	/**
	 * @brief Returns the remaining.
	 * @return The remaining time.
	 */
	boost::posix_time::time_duration getRemainingTime() const;

	/**
	 * @brief Returns the elapsed time.
	 * @return The elapsed time.
	 */
	boost::posix_time::time_duration getElapsedTime() const;

private:

	/**
	 * @brief Time when the task started.
	 */
	const boost::posix_time::ptime mStartTime;

	const boost::posix_time::ptime mEndTime;

	/**
	 * @brief The slice of time for this time frame.
	 */
	const boost::posix_time::time_duration mTimeSlice;
};

}

#endif /* TIMEFRAME_H_ */
