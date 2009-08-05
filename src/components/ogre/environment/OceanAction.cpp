/*
 Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#include "OceanAction.h"
#include "OceanRepresentation.h"
#include "Environment.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/EmberOgre.h"

#include "framework/LoggingInstance.h"

namespace EmberOgre
{
namespace Environment
{
OceanAction::OceanAction(EmberEntity& entity) :
	mEntity(entity)
{
}

OceanAction::~OceanAction()
{
}

void OceanAction::activate()
{
	if (EmberEntityFactory* factory = EmberOgre::getSingleton().getEntityFactory()) {
		if (WorldEmberEntity* world = factory->getWorld()) {
			if (Environment * environment = world->getEnvironment()) {
				if (IWater* water = environment->getWater()) {
					OceanRepresentation* ocean = new OceanRepresentation(mEntity, *water);
					mEntity.setGraphicalRepresentation(ocean);
					return;
				}
			}
		}
	}
	S_LOG_WARNING("Tried to activate ocean representation, but there was no water instance available.");
}

void OceanAction::deactivate()
{
	mEntity.setGraphicalRepresentation(0);
}
}
}
