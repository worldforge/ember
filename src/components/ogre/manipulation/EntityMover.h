//
// C++ Interface: EntityMover
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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
#include "IEntityMoveBridge.h"
#include <wfmath/point.h>

namespace EmberOgre {

class EmberEntity;

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
	
	@brief An implementation of IEntityMoveBridge which allows for EmberEntity instances to be moved.
*/
class EntityMover : public IEntityMoveBridge
{
public:

	EntityMover(EmberEntity* entity);
	virtual ~EntityMover() {}

	virtual const WFMath::Quaternion& getOrientation() const;
	virtual const WFMath::Point<3>& getPosition() const;
	virtual void setPosition(const WFMath::Point<3>& position);
	virtual void move(const WFMath::Vector<3>& directionVector);
	virtual void setRotation (int axis, WFMath::CoordType angle);
	virtual void setOrientation(const WFMath::Quaternion& rotation);
	virtual void yaw(WFMath::CoordType angle);
	
	virtual void finalizeMovement();
	virtual void cancelMovement();

private:

	EmberEntity* mEntity;
	mutable WFMath::Quaternion mOrientation;
	mutable WFMath::Point<3> mPosition;
};

}

#endif
