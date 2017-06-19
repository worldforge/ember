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

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include "framework/TimeFrame.h"

#include <queue>

#include <condition_variable>
#include <mutex>

namespace Eris
{
class EventService;
}

namespace Ember
{

/**
 * @brief Namespace for tasks, which is a mechanism for better separate and field data for threading purposes.
 */
namespace Tasks
{

class ITask;
class ITaskExecutionListener;
class TaskExecutor;
class TaskUnit;

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief A task queue, which allows for queuing of tasks, which will be handled by a number of task executors.
 *
 * This is the main entry into the task framework. Each instance of this represents a queue onto which tasks can be added.
 *
 * Create an instance of this in your main thread, and then call pollProcessedTasks() from the same thread at a regular interval.
 * You must also make sure that you delete this instance in the main thread.
 */
class TaskQueue
{
	friend class TaskExecutor;
public:

	/**
	 * @brief Ctor.
	 * @param numberOfExecutors The number of concurrent task executors to use.
	 */
	TaskQueue(unsigned int numberOfExecutors, Eris::EventService& eventService);

	/**
	 * @brief Dtor.
	 */
	virtual ~TaskQueue();

	/**
	 * @brief Adds a task to the queue.
	 * Ownership of the task will be transferred to this queue. Ownership of the optional listener will not be transferred however.
	 * @note If the queue is being shut down, the task will not be queued and a warning will be written to the log.
	 * @param task The task to add. Note that ownership will be transferred.
	 * @param listener An optional listener. Note that ownership won't be transferred.
	 * @return False if the task couldn't be enqueued, probably because the task queue is inactive.
	 */
	bool enqueueTask(ITask* task, ITaskExecutionListener* listener = 0);

	/**
	 * @brief Deactivates the queue.
	 *
	 * Calling this has two effects.
	 * Firstly, no more tasks can be enqueued on the queue.
	 * Secondly, all currently running tasks are run to their completion.
	 * The latter means that a call to this is blocking until all tasks are done.
	 */
	void deactivate();

	/**
	 * @brief Returns true if the queue is active.
	 * @return True if queue is active.
	 */
	bool isActive() const;

protected:

	/**
	 * @brief A queue of task units.
	 */
	typedef std::queue<TaskUnit*> TaskUnitQueue;

	/**
	 * @brief A store of executors.
	 */
	typedef std::vector<TaskExecutor*> TaskExecutorStore;

	Eris::EventService& mEventService;

	/**
	 * @brief A collection of unprocessed task units, which is a tuple of a task and a listener.
	 */
	TaskUnitQueue mUnprocessedTaskUnits;

	/**
	 * @brief A collection of processed task units. These will need to be executed in the main thread before they can be deleted.
	 * @see pollProcessedTasks()
	 */
	TaskUnitQueue mProcessedTaskUnits;

	/**
	 * @brief The executors used by the queue.
	 */
	TaskExecutorStore mExecutors;

	/**
	 * @brief A mutex used whenever the unprocessed queue is accessed.
	 */
	std::mutex mUnprocessedQueueMutex;

	std::mutex mProcessedQueueMutex;

	/**
	 * @brief A condition variable used for letting threads sleep while waiting for new tasks.
	 */
	std::condition_variable mUnprocessedQueueCond;

	/**
	 * @brief Whether this queue is active or not.
	 * Setting this to true will make two things happen:
	 * 1) No more tasks can be enqueued.
	 * 2) If there are not more tasks to process, fetchNextTask() will return a null pointer, which tells the executor to exit it's processing loop.
	 * This is therefore only set to false if the queue is being shut down.
	 */
	bool mActive;

	bool mIsQueuedOnMainThread;

	/**
	 * @brief Gets the next task to process.
	 * @note This is normally only called by a TaskExecutor.
	 * Calling this while there's no current tasks will result in the current thread being put on hold until a new task is enqueued.
	 * @returns A pointer to a task unit, or a null pointer if the executor is expected to exit its processing loop (i.e. when the queue is being shut down).
	 */
	TaskUnit* fetchNextTask();

	/**
	 * @brief Adds a processed task back to the queue, to be handled in the main thread and then deleted.
	 * @param taskUnit The processed task unit.
	 */
	void addProcessedTask(TaskUnit* taskUnit);

	void processCompletedTasks();

};

}

}

#endif /* TASKQUEUE_H_ */
