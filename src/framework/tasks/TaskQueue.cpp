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

#include "TaskQueue.h"
#include "ITask.h"
#include "ITaskExecutionListener.h"
#include "TaskExecutor.h"
#include "TaskUnit.h"

#include "framework/LoggingInstance.h"

#include <Eris/EventService.h>

namespace Ember {

namespace Tasks {

TaskQueue::TaskQueue(unsigned int numberOfExecutors, Eris::EventService& eventService) :
		mEventService(eventService), mActive(true), mIsQueuedOnMainThread(false) {
	S_LOG_VERBOSE("Creating task queue with " << numberOfExecutors << " executors.");
	for (unsigned int i = 0; i < numberOfExecutors; ++i) {
		TaskExecutor* executor = new TaskExecutor(*this);
		mExecutors.push_back(executor);
	}
}

TaskQueue::~TaskQueue() {
	deactivate();
}

void TaskQueue::deactivate() {
	if (mActive) {
		{
			std::unique_lock<std::mutex> l(mUnprocessedQueueMutex);
			mActive = false;
		}
		mUnprocessedQueueCond.notify_all();
		//Join all executors. Since the queue is shutting down they will all exit their main loop if there are no more tasks to process.
		for (TaskExecutorStore::iterator I = mExecutors.begin(); I != mExecutors.end(); ++I) {
			TaskExecutor* executor = *I;
			executor->join();
			delete executor;
		}

		//Finally we must process all of the tasks in our main loop. This of course requires that this instance is destroyed from the main loop.
		mEventService.processAllHandlers();

		assert(mProcessedTaskUnits.empty());
		assert(mUnprocessedTaskUnits.empty());
	}
}

bool TaskQueue::enqueueTask(ITask* task, ITaskExecutionListener* listener) {
	std::unique_lock<std::mutex> l(mUnprocessedQueueMutex);
	if (mActive) {
		mUnprocessedTaskUnits.push(new TaskUnit(task, listener));
		mUnprocessedQueueCond.notify_one();
		return true;
	} else {
		S_LOG_WARNING("Tried to enqueue the task " << task->getName() << " on a task queue which isn't active (i.e. is shutting down).");
		return false;
	}

}

TaskUnit* TaskQueue::fetchNextTask() {
	//The semantics of this method is that if a null pointer is returned the task executor is required to exit its main processing loop, since this indicates that the queue is shuttin down.
	TaskUnit* taskUnit(0);
	std::unique_lock<std::mutex> lock(mUnprocessedQueueMutex);
	if (mUnprocessedTaskUnits.empty()) {
		if (!mActive) {
			return 0;
		}
		mUnprocessedQueueCond.wait(lock);
	}
	if (!mUnprocessedTaskUnits.empty()) {
		taskUnit = mUnprocessedTaskUnits.front();
		mUnprocessedTaskUnits.pop();
	}
	return taskUnit;
}

void TaskQueue::addProcessedTask(TaskUnit* taskUnit) {
	std::unique_lock<std::mutex> lock(mProcessedQueueMutex);

	mProcessedTaskUnits.push(taskUnit);
	if (!mIsQueuedOnMainThread) {
		//Make sure that the task is handled on the main queue.
		mEventService.runOnMainThread([this] {
			processCompletedTasks();
		});
	}

}

bool TaskQueue::isActive() const {
	return mActive;
}

void TaskQueue::processCompletedTasks() {
	if (!mProcessedTaskUnits.empty()) {
		TaskUnit* taskUnit;
		{
			std::unique_lock<std::mutex> lock(mProcessedQueueMutex);
			taskUnit = mProcessedTaskUnits.front();
		}
		try {
			bool result = taskUnit->executeInMainThread();
			if (result) {
				try {
					delete taskUnit;
				} catch (const std::exception& ex) {
					S_LOG_FAILURE("Error when deleting task in main thread." << ex);
				} catch (...) {
					S_LOG_FAILURE("Unknown error when deleting task in main thread.");
				}
				{
					std::unique_lock<std::mutex> lock(mProcessedQueueMutex);
					mProcessedTaskUnits.pop();
				}
			}

		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when executing task in main thread." << ex);
		} catch (...) {
			S_LOG_FAILURE("Unknown error when executing task in main thread.");
		}

		{
			std::unique_lock<std::mutex> lock(mProcessedQueueMutex);
			if (!mProcessedTaskUnits.empty()) {
				mEventService.runOnMainThread([this] {
					processCompletedTasks();
				});
			} else {
				mIsQueuedOnMainThread = false;
			}
		}

	}
}

}

}
