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

class TaskExecutor
{
public:
	TaskExecutor(TaskQueue& taskQueue);
	virtual ~TaskExecutor();

	void shutdown();

protected:

	TaskQueue& mTaskQueue;
	bool mActive;
	std::auto_ptr<boost::thread> mThread;

	void run();
};

}
}
#endif /* TASKEXECUTOR_H_ */
