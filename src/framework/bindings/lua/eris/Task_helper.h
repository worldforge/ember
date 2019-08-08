/*
 Copyright (C) 2019 Erik Ogenvik

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

#ifndef EMBER_TASK_HELPER_H
#define EMBER_TASK_HELPER_H
#include <Eris/Task.h>

int _Task_usages_size(Eris::Task* task) {
	return task->getUsages().size();
}

std::string _Task_usage_name(Eris::Task* task, int index) {
	if (index < task->getUsages().size()) {
		return task->getUsages()[index].name;
	}
	return "";
}

#endif //EMBER_TASK_HELPER_H
