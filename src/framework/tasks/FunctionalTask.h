/*
 Copyright (C) 2023 Erik Ogenvik

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

#ifndef EMBER_FUNCTIONALTASK_H
#define EMBER_FUNCTIONALTASK_H

#include <functional>
#include <optional>
#include "ITask.h"
#include "TaskQueue.h"

namespace Ember {

namespace Tasks {
template<typename T>
struct FunctionalTask : public ITask {

	std::function<T(TaskExecutionContext&)> backgroundFn;
	std::function<bool(const T&)> foregroundFn;

	std::optional<T> result = {};

	void executeTaskInBackgroundThread(TaskExecutionContext& context) override {
		result = backgroundFn(context);
	}

	bool executeTaskInMainThread() override { return foregroundFn(*result); };

	std::string getName() const override { return "FunctionalTask"; }
};

template<typename T>
void enqeueFunctionalTask(TaskQueue& queue, std::function<T(TaskExecutionContext&)> backgroundFn, std::function<bool(const T&)> foregroundFn) {
	auto task = std::unique_ptr<FunctionalTask<T>>(new FunctionalTask<T>{.foregroundFn=foregroundFn, .backgroundFn=backgroundFn});
	queue.enqueueTask(std::move(task), nullptr);
}


}
}


#endif //EMBER_FUNCTIONALTASK_H
