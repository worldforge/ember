//
// C++ Implementation: EntityCreatorMovementBridge
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "EntityCreatorMovementBridge.h"
#include "EntityCreator.h"
#include "components/ogre/manipulation/DetachedEntity.h"
#include <OgreSceneNode.h>
#include "../Convert.h"

namespace EmberOgre {

namespace Gui {

EntityCreatorMovementBridge::EntityCreatorMovementBridge(EntityCreator& creator, DetachedEntity& entity, Ogre::SceneNode* node)
: ::EmberOgre::Manipulation::EntityMoverBase(entity, node), mCreator(creator)
{
}


EntityCreatorMovementBridge::~EntityCreatorMovementBridge()
{
}


void EntityCreatorMovementBridge::finalizeMovement()
{
	mCreator.finalizeCreation();
}
void EntityCreatorMovementBridge::cancelMovement()
{
	mCreator.stopCreation();
}

}

}
