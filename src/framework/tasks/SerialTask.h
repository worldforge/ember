/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef SERIALTASK_H_
#define SERIALTASK_H_

#include "ITask.h"
#include <vector>

namespace Ember
{

namespace Tasks
{

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A task which wraps two or more other tasks, which will be executed in order.
 * This is useful if you want to make sure that a certain task is executed after another task.
 */
class SerialTask : public ITask
{
public:
	typedef std::vector<ITask*> TaskStore;

	/**
	 * @brief Ctor.
	 * @param subTasks The tasks to execute, in order.
	 */
	SerialTask(const TaskStore& subTasks);

	/**
	 * @brief Ctor.
	 * This is a convenience constructor which allows you to specify the tasks directly without having to first create a vector instance.
	 * @param firstTask The first task to execute.
	 * @param secondTask The second task to execute.
	 * @param thirdTask The third task to execute.
	 * @param firstTask The fourth task to execute.
	 */
	SerialTask(ITask* firstTask, ITask* secondTask, ITask* thirdTask = 0, ITask* fourthTask = 0);
	virtual ~SerialTask();

	virtual void executeTaskInBackgroundThread(TaskExecutionContext& context);

private:
	TaskStore mSubTasks;
};

}

}

#endif /* SERIALTASK_H_ */
