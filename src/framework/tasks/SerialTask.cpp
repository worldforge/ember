/*
 Copyright (C) 2010 Erik Ogenvik <erik@ogenvik.org>

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

#include "SerialTask.h"

#include <utility>
#include "TaskExecutionContext.h"

namespace Ember {

namespace Tasks {

SerialTask::SerialTask(TaskStore subTasks)
		: mSubTasks(std::move(subTasks)) {
}

SerialTask::SerialTask(std::unique_ptr<ITask> firstTask, std::unique_ptr<ITask> secondTask, std::unique_ptr<ITask> thirdTask, std::unique_ptr<ITask> fourthTask)
{
	if (firstTask) {
		mSubTasks.push_back(std::move(firstTask));
	}
	if (secondTask) {
		mSubTasks.push_back(std::move(secondTask));
	}
	if (thirdTask) {
		mSubTasks.push_back(std::move(thirdTask));
	}
	if (fourthTask) {
		mSubTasks.push_back(std::move(fourthTask));
	}
}

SerialTask::~SerialTask() = default;

void SerialTask::executeTaskInBackgroundThread(TaskExecutionContext& context) {
	context.executeTasks(std::move(mSubTasks));
}

}

}
