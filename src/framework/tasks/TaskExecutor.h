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

#ifndef TASKEXECUTOR_H_
#define TASKEXECUTOR_H_

#include <thread>

namespace Ember
{

namespace Tasks
{

class TaskQueue;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A task executor, responsible for processing tasks.
 * Each instance of this holds a thread. It's only purpose is to ask the queue for new tasks to process. If no tasks are available it will sleep (inside of TaskQueue::fetchNextTask).
 */
class TaskExecutor
{
	friend class TaskQueue;
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~TaskExecutor();

	/**
	 * @brief Sets whether the executor is active or not.
	 * @param active Whether the executor should be active or not.
	 */
	void setActive(bool active);

	/**
	 * @brief Joins the thread the executor is using.
	 */
	void join();

protected:

	/**
	 * @brief The queue to which this executor belong.
	 */
	TaskQueue& mTaskQueue;

	/**
	 * @brief Whether the executor is active or not.
	 */
	bool mActive;

	/**
	 * @brief The thread which performs the execution.
	 */
	std::thread* mThread;

	/**
	 * @brief Ctor.
	 * During construction a new thread will be created and executed.
	 * @param taskQueue The queue to which this executor belongs.
	 */
	TaskExecutor(TaskQueue& taskQueue);

	/**
	 * @brief Main loop method.
	 */
	void run();
	//	void shutdown();
};

}
}
#endif /* TASKEXECUTOR_H_ */
