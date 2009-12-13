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

#ifndef TASKEXECUTIONCONTEXT_H_
#define TASKEXECUTIONCONTEXT_H_

#include <vector>

namespace Ember
{
namespace Tasks
{

class TaskExecutor;
class ITask;
class TaskUnit;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief The context where a task is executed in.
 *
 * This envelopes the context in where a task is executed, and provides information about it.
 */
class TaskExecutionContext
{
public:

	TaskExecutionContext(TaskExecutor& executor, TaskUnit& taskUnit);
	virtual ~TaskExecutionContext();

	/**
	 * @brief Gets the executor which performs the execution of the task.
	 * @returns The task executor.
	 */
	const TaskExecutor& getExecutor() const;

	/**
	 * @brief Executes a subtask.
	 * After the subtask has been executed in the background thread, the task framework will make sure that it is executed in the main thread before the main task is executed.
	 * @note This should only be called from a background thread, i.e. while the task is being executed. Under normal circumstances this shouldn't be a problem however as an instance of this class is only available when a task is being executed in a background thread.
	 * @param task A task which will be executed.
	 */
	void executeTask(ITask* task);

	/**
	 * @brief Executes a series of subtasks.
	 * After the subtasks have been executed in the background thread, the task framework will make sure that they are executed in the main thread before the main task is executed.
	 * @note This should only be called from a background thread, i.e. while the task is being executed. Under normal circumstances this shouldn't be a problem however as an instance of this class is only available when a task is being executed in a background thread.
	 * @param tasks A list of tasks which will be executed.
	 */
	void executeTasks(std::vector<ITask*> tasks);


private:

	TaskExecutor& mExecutor;

	TaskUnit& mTaskUnit;
};
}
}

#endif /* TASKEXECUTIONCONTEXT_H_ */
