//
// C++ Implementation: TasksProperties
//
// Description:
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2009
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TasksProperties.h"

#include "../EmberOgre.h"
#include "../Avatar.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Objects/Anonymous.h>

#include <Eris/Account.h>
#include <Eris/TypeInfo.h>
#include <Eris/Connection.h>

#include "../EmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include <iostream>

using namespace Ember;
namespace EmberOgre {

namespace Gui {

TasksProperties::TasksProperties()
{
}

TasksProperties::~TasksProperties()
{
}

void TasksProperties::doSomething()
{
	std::cout << "Did something\n";

	Atlas::Objects::Operation::Set s;

	Atlas::Message::MapType task_args;
	task_args["name"] = "twist";
	task_args["param"] = "value";

	Atlas::Message::ListType list;
	list.push_back(task_args);

	Atlas::Objects::Entity::Anonymous obj;
	obj->setId(EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getId());
	obj->setObjtype("obj");
	obj->setAttr("tasks", list);

	s->setArgs1(obj);
	s->setFrom(Ember::EmberServices::getSingleton().getServerService()->getAccount()->getId());

	Ember::EmberServices::getSingleton().getServerService()->getConnection()->send(s);

}

}
}
