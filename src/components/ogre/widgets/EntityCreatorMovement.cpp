//
// C++ Implementation: EntityCreatorMovement
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityCreatorMovement.h"
#include "EntityCreatorMovementBridge.h"

namespace Ember
{
namespace OgreView
{

namespace Gui
{

EntityCreatorMovement::EntityCreatorMovement(EntityCreatorCreationInstance& creationInstance, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node)
: mMoveAdapter(camera)
{
	// When the point is moved, an instance of this will be created and the movement handled by it.
	// Note that ownership will be transferred to the adapter, so we shouldn't delete it ourselves.
	mMovementBridge = std::make_shared<EntityCreatorMovementBridge>(creationInstance, entity, node);
	mMoveAdapter.attachToBridge(mMovementBridge);
	mMoveAdapter.update();
}

EntityCreatorMovement::~EntityCreatorMovement()
{
	mMoveAdapter.detach();
}

const std::shared_ptr<EntityCreatorMovementBridge>& EntityCreatorMovement::getBridge() const {
	return mMovementBridge;
}

}

}
}
