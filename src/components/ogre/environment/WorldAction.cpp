/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "WorldAction.h"
#include "Environment.h"
#include "WorldRepresentation.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/World.h"

#include "framework/LoggingInstance.h"

namespace Ember
{
namespace OgreView
{
namespace Environment
{

WorldAction::WorldAction(EmberEntity& entity) :
		mEntity(entity)
{
}

WorldAction::~WorldAction()
{
}

void WorldAction::activate(EntityMapping::ChangeContext& context)
{
	if (EmberOgre::getSingleton().getWorld()) {
		World* world = EmberOgre::getSingleton().getWorld();
		if (Environment* environment = world->getEnvironment()) {

			WorldRepresentation* world = new WorldRepresentation(mEntity, *environment);
			mEntity.setGraphicalRepresentation(world);
			return;
		} else {
			S_LOG_WARNING("Tried to activate world representation, but there was no environment instance available.");
		}
	}
}

void WorldAction::deactivate(EntityMapping::ChangeContext& context)
{
//	mEntity.setGraphicalRepresentation(0);
}
}
}
}
