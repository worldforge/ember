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

#ifndef TASKUNIT_H_
#define TASKUNIT_H_

#include <vector>

namespace Ember
{

namespace Tasks
{

class ITask;
class ITaskExecutionListener;
class TaskExecutionContext;
/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief Represents both a task, its subtasks, and a listener.
 *
 * An instance of this contains the main task to perform, as well as a listener.
 * It also contains any subtasks which might have been added during background thread execution.
 *
 * When the task is executed in the main thread this class will make sure to first execute any subtasks in the main thread before executing the main task.
 */
class TaskUnit
{
public:

	/**
	 * @brief A store of task units.
	 */
	typedef std::vector<TaskUnit*> SubtasksStore;

	/**
	 * @brief Ctor.
	 * @param task The main task. This will be owned by the unit.
	 * @param listener An optional listener. This won't be owned by the unit.
	 */
	TaskUnit(ITask* task, ITaskExecutionListener* listener = 0);

	/**
	 * @brief Dtor.
	 * Note that any subtasks will also be deleted when the task unit is deleted.
	 */
	virtual ~TaskUnit();

	/**
	 * @brief Adds a subtask.
	 * The effect of this is primarily that the subtask will be executed in the main thread before the main task is.
	 * Ownership is also transferred to this instance (or actually the TaskUnit instance which will be created).
	 * @param task The sub task.
	 * @param listener An optional listener. This won't be owned by the unit.
	 * @returns The new task unit instance which envelopes the subtask.
	 */
	TaskUnit* addSubtask(ITask* task, ITaskExecutionListener* listener = 0);

	/**
	 * @brief Gets all subtasks.
	 */
	const SubtasksStore& getSubtasks() const;

	/**
	 * @brief Executes the task in a background thread.
	 * Only call this from a background thread.
	 * @param The execution context.
	 */
	void executeInBackgroundThread(TaskExecutionContext& context);

	/**
	 * @brief Executes the main task, and any subtasks before that, in the main thread.
	 * Only call this from the main thread.
	 */
	bool executeInMainThread();

private:

	/**
	 * @brief The main task.
	 */
	ITask* mTask;

	/**
	 * @brief An optional task listener.
	 */
	ITaskExecutionListener* mListener;

	/**
	 * @brief A list of subtasks.
	 * These tasks are owned by this instance.
	 * When the executeInMainThread() method is called these subtasks will be executed before the main task is.
	 */
	SubtasksStore mSubtasks;
};

}

}

#endif /* TASKUNIT_H_ */
