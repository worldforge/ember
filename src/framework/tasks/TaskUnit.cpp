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

#include "TaskUnit.h"
#include "ITask.h"
#include "ITaskExecutionListener.h"

#define LOG_TASKS


#ifdef LOG_TASKS

#include "framework/TimedLog.h"

#endif

namespace Ember {

namespace Tasks {

TaskUnit::TaskUnit(std::unique_ptr<ITask> task, ITaskExecutionListener* listener) :
		mTask(std::move(task)),
		mListener(listener) {

}

TaskUnit::~TaskUnit() = default;

TaskUnit* TaskUnit::addSubtask(std::unique_ptr<ITask> task, ITaskExecutionListener* listener) {
	auto taskUnit = std::make_unique<TaskUnit>(std::move(task), listener);
	mSubtasks.emplace_back(std::move(taskUnit));
	return mSubtasks.back().get();
}

const TaskUnit::SubtasksStore& TaskUnit::getSubtasks() const {
	return mSubtasks;
}

void TaskUnit::executeInBackgroundThread(TaskExecutionContext& context) {
#ifdef LOG_TASKS
	TimedLog timedLog(mTask->getName() + ": background");
#endif

	try {
		if (mListener) {
			mListener->executionStarted();
		}
		mTask->executeTaskInBackgroundThread(context);
		if (mListener) {
			mListener->executionEnded();
		}
	} catch (const std::exception& ex) {
		if (mListener) {
			//TODO: wrap the original error somehow
			mListener->executionError(Exception("Error when executing task."));
		}
	} catch (...) {
		if (mListener) {
			mListener->executionError(Exception("Error when executing task."));
		}
	}

}

bool TaskUnit::executeInMainThread() {
#ifdef LOG_TASKS
	TimedLog timedLog(mTask->getName() + ": foreground");
	timedLog.report();
#endif
	//First execute all subtasks
	if (!mSubtasks.empty()) {
		auto& subTask = mSubtasks.front();
		bool result = subTask->executeInMainThread();
		if (result) {
			mSubtasks.erase(mSubtasks.begin());
		}
		return false;
	}
	return mTask->executeTaskInMainThread();
}

}

}
