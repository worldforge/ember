/*
 Copyright (C) 2015 erik

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
#ifndef FRAMEWORK_STACKCHECKER_H_
#define FRAMEWORK_STACKCHECKER_H_

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <csignal>

namespace Ember {

/**
 * Use this to profile frame drops.
 *
 * A thread is created in the background. At regular intervals it checks how long since the last frame started.
 * If the time exceeds the threshold set, the SIGUSR1 signal is emitted. This is handled on the main thread
 * as soon as possibly, which will in turn dump the stack.
 * After each frame the main loop then needs to call printBacktraces. If a stack was dumped this frame, it's written to log.
 */

class StackChecker {
public:
	StackChecker() = delete;

	static void start(std::chrono::milliseconds maxFrameDuration);

	static void stop();

	static void resetCounter(std::chrono::steady_clock::time_point timePoint = std::chrono::steady_clock::now());

	static void printBacktraces();

protected:
	class StackCheckerInstance;
	friend class StackCheckerInstance;

	static std::unique_ptr<StackCheckerInstance> sInstance;

	static void backtrace();

};

}

#endif /* FRAMEWORK_STACKCHECKER_H_ */
