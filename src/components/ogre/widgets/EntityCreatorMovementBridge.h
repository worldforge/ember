//
// C++ Interface: EntityCreatorMovementBridge
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
#ifndef EMBEROGRE_GUIENTITYCREATORMOVEMENTBRIDGE_H
#define EMBEROGRE_GUIENTITYCREATORMOVEMENTBRIDGE_H

#include "components/ogre/authoring/IMovementBridge.h"
#include "components/ogre/authoring/EntityMoverBase.h"
#include <wfmath/point.h>

namespace Ogre
{
class SceneNode;
}
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

/**
 @brief A movement bridge which knows how to move a DetachedEntity instance, which is a temporary preview entity used by the EntityCreator.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class EntityCreatorMovementBridge: public Authoring::EntityMoverBase
{
public:
	/**
	 * @brief Ctor.
	 * @param entityCreator The owning entity creator instance.
	 * @param entity The entity which should be moved.
	 * @param node The node to which the entity belongs.
	 */
	EntityCreatorMovementBridge(EntityCreatorCreationInstance& creationInstance, Authoring::DetachedEntity* entity, Ogre::SceneNode* node);

	/**
	 * @brief Dtor.
	 */
	~EntityCreatorMovementBridge() override = default;

	void finalizeMovement() override;

	void cancelMovement() override;

private:

	/**
	 * @brief The entity creator which owns the entity.
	 */
	EntityCreatorCreationInstance& mCreationInstance;

};

}

}

}

#endif
