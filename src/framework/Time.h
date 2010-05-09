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

#ifndef EMBER_TIME_H_
#define EMBER_TIME_H_

#include <string>

namespace Ember
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Provides basic time services.
 */
class Time
{
public:
	/**
	 * @brief Gets the local time of the client machine.
	 *
	 * @param year The year.
	 * @param month The month.
	 * @param day The day.
	 * @param hour The hour.
	 * @param minute The minute.
	 * @param second The second.
	 */
	static void getLocalTime(int& year, int& month, int& day, int& hour, int& minute, int& second);

	/**
	 * @brief Utility method to get a string of the current time.
	 *
	 * @returns The current local time in an easy to read string.
	 */
	static std::string getLocalTimeStr(void);

	/**
	 * @brief Gets the epoch time in milliseconds from the local system
	 *
	 * @return The current local time in milliseconds.
	 */
	static long currentTimeMillis(void);

};

}

#endif /* EMBER_TIME_H_ */
