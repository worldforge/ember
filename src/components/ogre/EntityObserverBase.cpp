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

#include "EntityObserverBase.h"
#include <Eris/Entity.h>

namespace EmberOgre
{

EntityObserverBase::EntityObserverBase(Eris::Entity& entity, bool deleteOurselvesWhenDone) :
	mDeleteOurselvesWhenDone(deleteOurselvesWhenDone)
{
	entity.BeingDeleted.connect(sigc::mem_fun(*this, &EntityObserverBase::entity_BeingDeleted));
}

EntityObserverBase::~EntityObserverBase()
{
}

void EntityObserverBase::entity_BeingDeleted()
{
	cleanup();
	if (mDeleteOurselvesWhenDone) {
		deleteOurselves();
	}
}

void EntityObserverBase::cleanup()
{
}

void EntityObserverBase::deleteOurselves()
{
	delete this;
}
}
