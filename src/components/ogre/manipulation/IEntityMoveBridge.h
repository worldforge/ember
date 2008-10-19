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
@brief A bridge to the entity. Any instance of this is responsible for making sure that the entity is properly moved.

This acts as an abstraction for the actual entity, relieving the other components in the movement framework from altering the entity directly.
Note that this deals with world space only, which means that all orientations and positions are relative to the world, not the parent entity.

All movement that happens through this class will be on the client only, until either finalizeMovement() or cancelMovement() are called.
	@author Erik Hjortsberg <erik@katastrof.nu>
*/
class IEntityMoveBridge
{
public:
	
	virtual ~IEntityMoveBridge() {}

	/**
	 * @brief Gets the orientation of the entity, in world space.
	 * @return The orientation of the entity, in world space.
	 */
	virtual const WFMath::Quaternion& getOrientation() const = 0;
	
	/**
	 * @brief Gets the position of the entity, in world space.
	 * @return The position of the entity, in world space.
	 */
	virtual const WFMath::Point<3>& getPosition() const = 0;
	
	/**
	 * @brief Sets the new position of the entity, in world space.
	 * @param position The new position of the entity, in world space.
	 */
	virtual void setPosition(const WFMath::Point<3>& position) = 0;
	
	/**
	 * @brief Moves the entity.
	 * @param directionVector How much to translate the entity.
	 */
	virtual void move(const WFMath::Vector<3>& directionVector) = 0;
	/**
	 * @brief Sets the rotation of the entity, in world space. This allows you only to set the rotation around one of the axis aligned vectors.
	 * @param axis The axis around which to rotate. This is in WF space.
	 * @param angle The angle to rotate.
	 */
	virtual void setRotation (int axis, WFMath::CoordType angle) = 0;
	/**
	 * @brief Sets the orientation of the entity, in world space.
	 * @param rotation The new orientation, in world space.
	 */
	virtual void setOrientation(const WFMath::Quaternion& rotation) = 0;
	/**
	 * @brief Yaws the entity.
	 * Since yawing is something that the user most often wants to do we provide this as a separate method.
	 * @see setOrientation
	 * @param angle The angle to yaw.
	 */
	virtual void yaw(WFMath::CoordType angle) = 0;
	
	/**
	 * @brief Finalizes movement, and send to the server.
	 * Call this when movement is complete.
	 */
	virtual void finalizeMovement() = 0;
	
	/**
	 * @brief Cancels the movement, returning the entity to it's original place.
	 */
	virtual void cancelMovement() = 0;
	
};

}

#endif
