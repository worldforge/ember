//
// C++ Interface: IEntityMoveBridge
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
#ifndef EMBEROGREIENTITYMOVEBRIDGE_H
#define EMBEROGREIENTITYMOVEBRIDGE_H

#include "../EmberOgrePrerequisites.h"
#include <wfmath/quaternion.h>

namespace EmberOgre {

/**
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class IEntityMoveBridge
{
public:

	virtual const WFMath::Quaternion& getOrientation() const = 0;
	virtual const WFMath::Point<3>& getPosition() const = 0;
	virtual void setPosition(const WFMath::Point<3>& position) = 0;
	virtual void move(const WFMath::Vector<3> directionVector) = 0;
	virtual void setRotation (int axis, WFMath::CoordType angle) = 0;
	virtual void setOrientation(const WFMath::Quaternion& rotation) = 0;
	virtual void yaw(WFMath::CoordType angle) = 0;
	
	virtual void finalizeMovement() = 0;
	virtual void cancelMovement() = 0;
	
};

}

#endif
