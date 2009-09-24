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

#include "TaskQueue.h"
#include "ITask.h"
#include "ITaskExecutionListener.h"
#include "TaskExecutor.h"
#include "framework/LoggingInstance.h"

namespace Ember
{

namespace Tasks
{

TaskQueue::TaskQueue(unsigned int numberOfExecutors)
{
	S_LOG_VERBOSE("Creating task queue with " << numberOfExecutors << " executors.");
	for (unsigned int i = 0; i < numberOfExecutors; ++i) {
		TaskExecutor* executor = new TaskExecutor(*this);
		mExecutors.push_back(executor);
	}
}

TaskQueue::~TaskQueue()
{
	for (TaskExecutorStore::iterator I = mExecutors.begin(); I != mExecutors.end(); ++I) {
		delete *I;
	}
}

void TaskQueue::enqueueTask(ITask* task, ITaskExecutionListener* listener)
{
	{
		boost::mutex::scoped_lock l(mQueueMutex);

		mTaskUnits.push(TaskUnit(task, listener));
	}
	mQueueCond.notify_one();

}

TaskQueue::TaskUnit TaskQueue::fetchNextTask()
{

	boost::unique_lock<boost::mutex> lock(mQueueMutex);

	while (!mTaskUnits.size()) {
		mQueueCond.wait(lock);
	}
	TaskUnit taskUnit = mTaskUnits.front();
	mTaskUnits.pop();
	return taskUnit;
}

}

}
