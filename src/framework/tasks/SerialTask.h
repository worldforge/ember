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

#ifndef SERIALTASK_H_
#define SERIALTASK_H_

#include "TemplateNamedTask.h"
#include <vector>
#include <memory>

namespace Ember {

namespace Tasks {

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A task which wraps two or more other tasks, which will be executed in order.
 * This is useful if you want to make sure that a certain task is executed after another task.
 */
class SerialTask : public TemplateNamedTask<SerialTask> {
public:
	typedef std::vector<std::unique_ptr<ITask>> TaskStore;

	/**
	 * @brief Ctor.
	 * @param subTasks The tasks to execute, in order.
	 */
	explicit SerialTask(TaskStore subTasks);

	/**
	 * @brief Ctor.
	 * This is a convenience constructor which allows you to specify the tasks directly without having to first create a vector instance.
	 * @param tasks A list of tasks.
	 */
	SerialTask(std::unique_ptr<ITask> firstTask, std::unique_ptr<ITask> secondTask, std::unique_ptr<ITask> thirdTask, std::unique_ptr<ITask> fourthTask);

	~SerialTask() override;

	void executeTaskInBackgroundThread(TaskExecutionContext& context) override;

private:
	TaskStore mSubTasks;
};

}

}

#endif /* SERIALTASK_H_ */
