/*
 Copyright (C) 2002  Lakin Wecker
	
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

#include "StreamLogObserver.h"
#include <sstream>
#include <thread>
#include <atomic>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace Ember {


/**
 * @brief Simple class used for giving a sequential identifier to threads.
 */
class ThreadIdentifier {
public:
	std::string id;
	static std::atomic<int> sCounter;

	ThreadIdentifier() {
		std::stringstream ss;
		ss << sCounter++;
		id = ss.str();
	}
};

std::atomic<int> ThreadIdentifier::sCounter;

unsigned long StreamLogObserver::sCurrentFrame = 0;
std::chrono::steady_clock::time_point StreamLogObserver::sCurrentFrameStartMilliseconds = std::chrono::steady_clock::now();


StreamLogObserver::StreamLogObserver(std::ostream& out)
		: myOut(out),
		  mDetailed(false),
		  mStart(std::chrono::steady_clock::now()) {
}


StreamLogObserver::~StreamLogObserver() {
	myOut.flush();
}

//----------------------------------------------------------------------
// Implemented methods from Log::Observer

/**
 * Prints out the message provided with file, line and datestamp to myOut;
 */
void StreamLogObserver::onNewMessage(const std::string& message, const std::string& file, const int& line,
									 const Log::MessageImportance& importance) {


	boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::local_time();

	myOut.fill('0');
	myOut << "[";
	myOut.width(2);
	myOut << currentTime.time_of_day().hours() << ":";
	myOut.width(2);
	myOut << currentTime.time_of_day().minutes() << ":";
	myOut.width(2);
	myOut << currentTime.time_of_day().seconds();
	if (mDetailed) {
		//We don't expect many threads to be created, so we'll use a static variable.
		static std::map<std::thread::id, ThreadIdentifier> threadIdentifiers;
		myOut << "(";
		myOut.width(8);
		auto millisecondsIntoFrame = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - sCurrentFrameStartMilliseconds);
		auto microsecondsSinceStart = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - mStart);
		auto threadId = threadIdentifiers[std::this_thread::get_id()].id;
		myOut << microsecondsSinceStart.count() << ":" << threadId << ":" << sCurrentFrame << ":";
		//Only print out how far into the frame we are if we're the main thread, since it's only there that it's worth knowing. This avoids confusion.
		if (threadId == "0") {
			myOut << millisecondsIntoFrame.count() << ")";
		} else {
			myOut << ":-)";
		}
	}
	myOut << "] ";

	if (importance == Log::MessageImportance::CRITICAL) {
		myOut << "CRITICAL";
	} else if (importance == Log::MessageImportance::FAILURE) {
		myOut << "Failure";
	} else if (importance == Log::MessageImportance::WARNING) {
		myOut << "WARNING";
	} else if (importance == Log::MessageImportance::INFO) {
		myOut << "INFO";
	} else {
		myOut << "VERBOSE";
	}

	myOut << " " << message;

#ifdef EMBER_LOG_SHOW_ORIGIN
	if(line != -1){
		myOut << " [" << file << "(" <<  line << ")]";
	}
#endif

	myOut << std::endl;

}

void StreamLogObserver::setDetailed(bool enabled) {
	if (enabled && !mDetailed) {
		Log::log("Enabling detailed logging. The values are as follows: microseconds since start: current thread id : current frame : milliseconds since start of current frame");
	}
	mDetailed = enabled;
}

}; //end namespace Ember
