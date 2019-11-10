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

#include "TaskExecutor.h"
#include "TaskQueue.h"
#include "TaskExecutionContext.h"
#include "TaskUnit.h"
#include "framework/LoggingInstance.h"

namespace Ember {

namespace Tasks {
TaskExecutor::TaskExecutor(TaskQueue& taskQueue) :
		mTaskQueue(taskQueue),
		mActive(true),
		mThread([&]() { this->run(); }) {
}

TaskExecutor::~TaskExecutor() = default;

void TaskExecutor::run() {
#ifdef __APPLE__
	pthread_setname_np("Task Executor");
#elif !defined(_WIN32)
	pthread_setname_np(pthread_self(), "Task Executor");
#endif
	while (mActive) {
		auto taskUnit = mTaskQueue.fetchNextTask();
		//If the queue returns a null pointer, it means that the queue is being shut down, and this executor is expected to exit its main processing loop.
		if (taskUnit) {
			try {
				TaskExecutionContext context(*this, *taskUnit);
				taskUnit->executeInBackgroundThread(context);
				mTaskQueue.addProcessedTask(std::move(taskUnit));
			} catch (const std::exception& ex) {
				S_LOG_CRITICAL("Error when executing task in background." << ex);
			} catch (...) {
				S_LOG_CRITICAL("Unknown error when executing task in background.");
			}
		} else {
			break;
		}
	}
}

void TaskExecutor::setActive(bool active) {
	mActive = active;
}

void TaskExecutor::join() {
	mThread.join();
}

}
}
