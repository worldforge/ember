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

#include "TaskExecutionContext.h"
#include "TaskExecutor.h"
#include "TaskUnit.h"
#include "ITask.h"

namespace Ember {
namespace Tasks {
TaskExecutionContext::TaskExecutionContext(TaskExecutor& executor, TaskUnit& taskUnit) :
		mExecutor(executor),
		mTaskUnit(taskUnit) {
}

TaskExecutionContext::~TaskExecutionContext() = default;

const TaskExecutor& TaskExecutionContext::getExecutor() const {
	return mExecutor;
}

void TaskExecutionContext::executeTask(std::unique_ptr<ITask> task, ITaskExecutionListener* listener) {
	TaskUnit* taskUnit = mTaskUnit.addSubtask(std::move(task), listener);
	TaskExecutionContext subtaskContext(mExecutor, *taskUnit);
	taskUnit->executeInBackgroundThread(subtaskContext);
}

void TaskExecutionContext::executeTasks(std::vector<std::unique_ptr<ITask>> tasks) {
	for (auto& task : tasks) {
		executeTask(std::move(task));
	}
}

}
}
