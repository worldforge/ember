//
// C++ Interface: EntityMover
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2006
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
#ifndef EMBEROGREENTITYMOVER_H
#define EMBEROGREENTITYMOVER_H

#include "../EmberOgrePrerequisites.h"
#include "EntityMoverBase.h"
#include <wfmath/point.h>

namespace EmberOgre {
namespace Manipulation {
class SnapToMovement;
}

class SceneNodeAttachment;
class EntityMoveManager;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

	@brief An implementation of IEntityMoveBridge which allows for EmberEntity instances to be moved.
*/
class EntityMover : public Manipulation::EntityMoverBase
{
public:

	EntityMover(SceneNodeAttachment& sceneNodeAttachment, EntityMoveManager& manager);
	virtual ~EntityMover() {}

	virtual void finalizeMovement();
	virtual void cancelMovement();

protected:

	virtual void newEntityPosition(const Ogre::Vector3& position);


private:

	SceneNodeAttachment& mSceneNodeAttachment;
	EntityMoveManager& mManager;

};

}

#endif
