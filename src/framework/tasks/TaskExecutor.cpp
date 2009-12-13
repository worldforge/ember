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

#include "TaskExecutor.h"
#include "TaskQueue.h"
#include "TaskExecutionContext.h"
#include "TaskUnit.h"



#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace Ember
{

namespace Tasks
{
TaskExecutor::TaskExecutor(TaskQueue& taskQueue) :
	mTaskQueue(taskQueue), mActive(true)
{
	mThread = std::auto_ptr<boost::thread>(new boost::thread(boost::bind(&TaskExecutor::run, this)));
}

TaskExecutor::~TaskExecutor()
{
}

void TaskExecutor::run()
{
	while (mActive) {
		TaskUnit* taskUnit = mTaskQueue.fetchNextTask();
		if (taskUnit) {
			TaskExecutionContext context(*this, *taskUnit);
			taskUnit->executeInBackgroundThread(context);
			mTaskQueue.addProcessedTask(taskUnit);
		}
	}
}


void TaskExecutor::setActive(bool active)
{
	mActive = active;
}

void TaskExecutor::join()
{
	mThread->join();
}

}
}
