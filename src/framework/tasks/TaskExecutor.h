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

#ifndef TASKEXECUTOR_H_
#define TASKEXECUTOR_H_

#include <memory>

namespace boost
{
class thread;
}

namespace Ember
{

namespace Tasks
{

class TaskQueue;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief A task executor, responsible for processing tasks.
 * Each instance of this holds a thread. It's only purpose is to ask the queue for new tasks to process. If no tasks are available it will sleep (inside of TaskQueue::fetchNextTask).
 */
class TaskExecutor
{
	friend class TaskQueue;
public:
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
	std::auto_ptr<boost::thread> mThread;

	/**
	 * @brief Ctor.
	 * During construction a new thread will be created and executed.
	 * @param taskQueue The queue to which this executor belongs.
	 */
	TaskExecutor(TaskQueue& taskQueue);

	/**
	 * @brief Dtor.
	 */
	virtual ~TaskExecutor();

	/**
	 * @brief Main loop method.
	 */
	void run();
	//	void shutdown();
};

}
}
#endif /* TASKEXECUTOR_H_ */
