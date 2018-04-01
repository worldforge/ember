//
// C++ Interface: EntityCreatorMovement
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
#ifndef EMBEROGRE_GUIENTITYCREATORMOVEMENT_H
#define EMBEROGRE_GUIENTITYCREATORMOVEMENT_H

#include "components/ogre/authoring/MovementAdapter.h"

namespace Ember
{
namespace OgreView
{
namespace Authoring
{
class DetachedEntity;
}
namespace Gui
{
class EntityCreatorCreationInstance;
class EntityCreatorMovementBridge;
/**
 @brief A movement of a DetachedEntity, belonging to the EntityCreator.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityCreatorMovement
{
public:
	/**
	 * @brief Ctor.
	 * @param entityCreator The owning entity creator instance.
	 * @param camera The main camera in the world.
	 * @param entity The entity which should be moved.
	 * @param node The node to which the entity belongs.
	 */
	EntityCreatorMovement(EntityCreatorCreationInstance& creationInstance, const Camera::MainCamera& camera, Authoring::DetachedEntity& entity, Ogre::SceneNode* node);

	/**
	 * @brief Dtor.
	 */
	~EntityCreatorMovement();

	const std::shared_ptr<EntityCreatorMovementBridge>& getBridge() const;

protected:


	std::shared_ptr<EntityCreatorMovementBridge> mMovementBridge;
	/**
	 * @brief Main adapter which will intercept mouse and keyboard input to allow for movement of this point.
	 */
	Authoring::MovementAdapter mMoveAdapter;
};

}

}

}

#endif
