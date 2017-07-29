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


#ifndef TEMPLATENAMEDTASK_H_
#define TEMPLATENAMEDTASK_H_

#include "ITask.h"
#include <typeinfo>
#include <string>

namespace Ember
{

namespace Tasks
{

/**
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @brief An utility subclass of ITask which will provide automatic templated typeinfo discovery for the ITask::getName method.
 */
template <typename T>
class TemplateNamedTask : public ITask
{
public:

	virtual std::string getName() const
	{
		return typeid(T).name();
	}


};

}
}

#endif /* TEMPLATENAMEDTASK_H_ */
