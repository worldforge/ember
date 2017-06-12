/*
 Copyright (C) 2009 Erik Ogenvik

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

#ifndef TIMEDLOG_H_
#define TIMEDLOG_H_

#include <string>
#define ENABLE_TIMED_LOG 1

#ifdef ENABLE_TIMED_LOG
#include <chrono>
#endif

namespace Ember
{

/**
 * @brief Utility class for timing tasks.
 *
 * Use this class when you need to time a specific task. The time the task took will be printed when the object is destroyed.
 * If you want to report time before this happens, use the report() methods.
 */
class TimedLog
{
public:
	/**
	 * @brief Ctor.
	 * @param logName The name of the log.
	 * @param reportStart Whether to also log the start of the task.
	 */
	TimedLog(const std::string& logName, bool reportStart = false);

	/**
	 * @brief Dtor.
	 * During destruction the time for the task will be written to the log.
	 */
	virtual ~TimedLog();

	/**
	 * @brief Reports the current elapsed time for the task.
	 */
	void report();

	/**
	 * @brief Reports the current elapsed time for the task.
	 * @param reportName The name of this report.
	 */
	void report(const std::string& reportName);

private:

#ifdef ENABLE_TIMED_LOG
	/**
	 * @brief The name of the log.
	 */
	std::string mLogName;

	/**
	 * @brief The start of the logging, in milliseconds.
	 */
	std::chrono::steady_clock::time_point mStart;

	/**
	 * @brief If report() has been called, record the last time of that.
	 */
	std::chrono::steady_clock::time_point mLastReport;

#endif
};

}

#endif /* TIMEDLOG_H_ */
