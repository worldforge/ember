/*
 Copyright (C) 2009 Erik Hjortsberg

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

#include "TimedLog.h"

#include "services/EmberServices.h"
#include "services/time/TimeService.h"
#include "framework/LoggingInstance.h"

namespace Ember
{
#ifdef DEBUG
TimedLog::TimedLog(const std::string& logName) :
	mLogName(logName), mStartMilliseconds(0), mLastReportMilliseconds(0)
{
	mStartMilliseconds = EmberServices::getSingleton().getTimeService()->currentTimeMillis();
}

TimedLog::~TimedLog()
{
	long currentTime = EmberServices::getSingleton().getTimeService()->currentTimeMillis();
	S_LOG_VERBOSE("Ended task '" << mLogName << "' after " << currentTime - mStartMilliseconds << " milliseconds.");
}

void TimedLog::report()
{
	long currentTime = EmberServices::getSingleton().getTimeService()->currentTimeMillis();
	if (mLastReportMilliseconds) {
		S_LOG_VERBOSE("Reported on task '" << mLogName << "' after " << currentTime - mStartMilliseconds << " milliseconds, "<< currentTime - mLastReportMilliseconds <<" since last reported time.");
	} else {
		S_LOG_VERBOSE("Reported on task '" << mLogName << "' after " << currentTime - mStartMilliseconds << " milliseconds.");
	}
	mLastReportMilliseconds = currentTime;
}
void TimedLog::report(const std::string& reportName)
{
	long currentTime = EmberServices::getSingleton().getTimeService()->currentTimeMillis();
	if (mLastReportMilliseconds) {
		S_LOG_VERBOSE("Reported '" << reportName << "' on task '" << mLogName << "' after " << currentTime - mStartMilliseconds << " milliseconds, "<< currentTime - mLastReportMilliseconds <<" since last reported time.");
	} else {
		S_LOG_VERBOSE("Reported '" << reportName << "' on task '" << mLogName << "' after " << currentTime - mStartMilliseconds << " milliseconds.");
	}
	mLastReportMilliseconds = currentTime;
}
#else
TimedLog::TimedLog(const std::string& logName)
{
}

TimedLog::~TimedLog()
{
}

void TimedLog::report()
{
}

void TimedLog::report(const std::string& reportName)
{
}
#endif
}
