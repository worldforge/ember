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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TimedLog.h"

#include <utility>

#include "LoggingInstance.h"

namespace Ember
{
#if ENABLE_TIMED_LOG==1
TimedLog::TimedLog(std::string logName, bool reportStart) :
	mLogName(std::move(logName)), mStart(std::chrono::steady_clock::now())
{
	if (reportStart) {
		S_LOG_VERBOSE("Started task '" << mLogName << "'.");
	}
}

TimedLog::~TimedLog()
{
	auto currentTime = std::chrono::steady_clock::now();
	auto microDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mStart);
	S_LOG_VERBOSE("Ended task '" << mLogName << "' after " << microDuration.count() << " microseconds.");
}

void TimedLog::report()
{
	auto currentTime = std::chrono::steady_clock::now();
	auto microDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mStart);
	if (mLastReport.time_since_epoch().count()) {
		auto microLastReportDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mLastReport);
		S_LOG_VERBOSE("Reported on task '" << mLogName << "' after " << microDuration.count() << " microseconds, "<< microLastReportDuration.count() <<" since last reported time.");
	} else {
		S_LOG_VERBOSE("Reported on task '" << mLogName << "' after " << microDuration.count() << " microseconds.");
	}
	mLastReport = currentTime;
}
void TimedLog::report(const std::string& reportName)
{
	auto currentTime = std::chrono::steady_clock::now();
	auto microDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mStart);
	if (mLastReport.time_since_epoch().count()) {
		auto microLastReportDuration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - mLastReport);
		S_LOG_VERBOSE("Reported '" << reportName << "' on task '" << mLogName << "' after " << microDuration.count() << " microseconds, "<< microLastReportDuration.count() <<" since last reported time.");
	} else {
		S_LOG_VERBOSE("Reported '" << reportName << "' on task '" << mLogName << "' after " << microDuration.count() << " microseconds.");
	}
	mLastReport = currentTime;
}
#else
TimedLog::TimedLog(const std::string& logName, bool reportStart)
{
}

TimedLog::~TimedLog() = default;

void TimedLog::report()
{
}

void TimedLog::report(const std::string& reportName)
{
}
#endif
}
