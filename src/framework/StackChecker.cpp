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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "StackChecker.h"
#include "LoggingInstance.h"

#ifdef UNWIND_ENABLED

#define UNW_LOCAL_ONLY

#include <libunwind.h>
#include <cxxabi.h>

#include <memory>

#endif

namespace Ember {

std::unique_ptr<StackChecker::StackCheckerInstance> StackChecker::sInstance;

#ifdef UNWIND_ENABLED

struct StackLine {
	unw_word_t pc;
	unw_word_t offset;
	char sym[256];
};

struct StackEntry {
	long milliseconds;
	std::array<StackLine, 40> stack;
	size_t stackSize;
};


class StackChecker::StackCheckerInstance {
public:
	StackEntry stackEntry{};

	std::thread pollingThread;
	std::atomic_bool hasReportedThisFrame;
	std::chrono::steady_clock::time_point resetTime{};
	std::chrono::milliseconds lastFrameMilliseconds{};
	std::chrono::milliseconds lastReportDuration{};
	std::mutex mutex;
	std::shared_ptr<bool> activeMarker;

	sighandler_t oldSignalHandler = SIG_ERR;


	void backtrace() {

		hasReportedThisFrame = true;
		std::lock_guard<std::mutex> lock(mutex);

		unw_cursor_t cursor{};
		unw_context_t context{};

		// Initialize cursor to current frame for local unwinding.
		unw_getcontext(&context);
		unw_init_local(&cursor, &context);

		// Unwind frames one by one, going up the frame stack.

		for (int i = 0; i < 3; ++i) {
			unw_step(&cursor);
		}

		size_t i = 0;
		while (unw_step(&cursor) > 0) {
			if (i == stackEntry.stack.max_size()) {
				break;
			}

			StackLine& stackLine = stackEntry.stack[i];

			unw_get_reg(&cursor, UNW_REG_IP, &stackLine.pc);
			if (stackLine.pc == 0) {
				break;
			}
			unw_get_proc_name(&cursor, stackLine.sym, sizeof(stackLine.sym), &stackLine.offset);
			i++;
		}
		stackEntry.stackSize = i;
		auto duration = std::chrono::steady_clock::now() - resetTime;
		lastFrameMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}


	void installSignalHandler() {
		auto existingHandler = std::signal(SIGUSR1, [](int signum) {
			StackChecker::sInstance->backtrace();
		});
		if (existingHandler != SIG_ERR) {
			oldSignalHandler = existingHandler;
		}
	}

	void resetCounter(std::chrono::steady_clock::time_point timePoint) {
		resetTime = timePoint;
		lastFrameMilliseconds = std::chrono::milliseconds(0);
		hasReportedThisFrame = false;
	}

	void printBacktraces() {
		if (hasReportedThisFrame) {
			std::lock_guard<std::mutex> lock(mutex);
			std::printf("Last frame took %li milliseconds, reported this trace after %li milliseconds:\n",
						lastFrameMilliseconds.count(),
						lastReportDuration.count());

			for (size_t j = 0; j < stackEntry.stackSize; ++j) {
				StackLine& stackLine = stackEntry.stack[j];
				std::printf("0x%lx:", stackLine.pc);

				char* nameptr = stackLine.sym;
				int status;
				char* demangled = abi::__cxa_demangle(stackLine.sym, nullptr, nullptr, &status);
				if (status == 0) {
					nameptr = demangled;
				}
				std::printf(" (%s+0x%lx)\n", nameptr, stackLine.offset);
				std::free(demangled);
			}
		}
	}

	explicit StackCheckerInstance(std::chrono::milliseconds maxFrameDuration)
			: hasReportedThisFrame(false),
			  activeMarker(std::make_shared<bool>(true)) {
		//Store the main thread id.
		auto nativeThread = pthread_self();
		installSignalHandler();
		pollingThread = std::thread([this, maxFrameDuration, nativeThread]() {
			while (true) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				if (!hasReportedThisFrame) {
					std::lock_guard<std::mutex> lock(mutex);
					if (!*activeMarker) {
						return;
					}
					if ((std::chrono::steady_clock::now() - resetTime) > maxFrameDuration) {
						lastReportDuration = std::chrono::duration_cast<decltype(lastReportDuration)>(std::chrono::steady_clock::now() - resetTime);

						//Send a signal to the main thread.
						pthread_kill(nativeThread, SIGUSR1);
					}
				}
			}

		});
	}

	~StackCheckerInstance() {
		{
			std::lock_guard<std::mutex> lock(mutex);
			*activeMarker = false;
		}
		pollingThread.join();
		if (oldSignalHandler != SIG_ERR) {
			std::signal(SIGUSR1, oldSignalHandler);
		}
	}

};

#else

class StackChecker::StackCheckerInstance {
public:


	void backtrace() {
	}


	void resetCounter(std::chrono::steady_clock::time_point timePoint) {
	}

	void printBacktraces() {
	}

	explicit StackCheckerInstance(std::chrono::milliseconds maxFrameDuration) {
		S_LOG_FAILURE("The libunwind library isn't available for this build, so the slow frame stack print feature isn't available.");
	}


};

#endif

void StackChecker::resetCounter(std::chrono::steady_clock::time_point timePoint) {
	if (sInstance) {
		sInstance->resetCounter(timePoint);
	}
}

void StackChecker::printBacktraces() {
	if (sInstance) {
		sInstance->printBacktraces();
	}
}


void StackChecker::start(std::chrono::milliseconds maxFrameDuration) {
	stop();
	sInstance = std::make_unique<StackCheckerInstance>(maxFrameDuration);
}

void StackChecker::stop() {
	if (sInstance) {
		sInstance.reset();
	}
}

void StackChecker::backtrace() {
	if (sInstance) {
		sInstance->backtrace();
	}
}

}
