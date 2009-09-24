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

#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <queue>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

namespace Ember
{

namespace Tasks
{

class ITask;
class ITaskExecutionListener;
class TaskExecutor;

class TaskQueue
{
friend class TaskExecutor;
public:
	typedef std::pair<ITask*, ITaskExecutionListener*> TaskUnit;
	TaskQueue(unsigned int numberOfExecutors);
	virtual ~TaskQueue();

	void enqueueTask(ITask* task, ITaskExecutionListener* listener);

protected:
	typedef std::queue<TaskUnit> TaskUnitQueue;
	typedef std::vector<TaskExecutor*> TaskExecutorStore;

	TaskUnitQueue mTaskUnits;

	TaskExecutorStore mExecutors;

	boost::mutex mQueueMutex;
	boost::condition_variable mQueueCond;


	TaskUnit fetchNextTask();


};

}

}

#endif /* TASKQUEUE_H_ */
