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

#include "TaskUnit.h"
#include "ITask.h"
#include "ITaskExecutionListener.h"

#include "framework/Exception.h"

namespace Ember
{

namespace Tasks
{

TaskUnit::TaskUnit(ITask* task, ITaskExecutionListener* listener) :
	mTask(task), mListener(listener)
{

}

TaskUnit::~TaskUnit()
{
	for (SubtasksStore::iterator I = mSubtasks.begin(); I != mSubtasks.end(); ++I) {
		delete *I;
	}
	delete mTask;
}

ITask* TaskUnit::getTask() const
{
	return mTask;
}

ITaskExecutionListener* TaskUnit::getListener() const
{
	return mListener;
}

TaskUnit* TaskUnit::addSubtask(ITask* task)
{
	TaskUnit* taskUnit = new TaskUnit(task, 0);
	mSubtasks.push_back(taskUnit);
	return taskUnit;
}

const TaskUnit::SubtasksStore& TaskUnit::getSubtasks() const
{
	return mSubtasks;
}

void TaskUnit::executeInBackgroundThread(TaskExecutionContext& context)
{
	try {
		if (mListener) {
			mListener->executionStarted();
		}
		mTask->executeTaskInBackgroundThread(context);
		if (mListener) {
			mListener->executionEnded();
		}
	} catch (const std::exception& ex) {
		if (mListener) {
			//TODO: wrap the original error somehow
			mListener->executionError(Exception("Error when executing task."));
		}
	} catch (...) {
		if (mListener) {
			mListener->executionError(Exception("Error when executing task."));
		}
	}

}

void TaskUnit::executeInMainThread()
{
	//First execute all subtasks
	for (SubtasksStore::const_iterator I = mSubtasks.begin(); I != mSubtasks.end(); ++I) {
		(*I)->executeInMainThread();
	}
	mTask->executeTaskInMainThread();
}

}

}
