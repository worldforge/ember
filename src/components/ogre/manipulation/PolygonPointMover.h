//
// C++ Interface: PolygonPointMover
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINTMOVER_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINTMOVER_H

// #include "../EmberOgrePrerequisites.h"
#include "IMovementBridge.h"
#include "services/input/Input.h"
#include <wfmath/point.h>
#include <sigc++/trackable.h>

namespace EmberOgre {

namespace Manipulation {

class PolygonPoint;

/**
@brief Responsible for handling movement of a PolygonPoint instance.
The movement happens through a graphical interface in Ogre. Basically a sphere is added to the world, which can then moved with the mouse.
If a point is moved and ctrl is pressed a new point will be created.

@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class PolygonPointMover : public IMovementBridge, public sigc::trackable
{
public:

	/**
	 * @brief Ctor.
	 * @param point The point for which movement should occur.
	 */
	PolygonPointMover(PolygonPoint& point);
	
	/**
	 * @brief Dtor.
	 */
	virtual ~PolygonPointMover();

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

	/**
	 * @brief The polygon point to which this movement belongs.
	 */
	PolygonPoint& mPoint;
	
	/**
	 * @brief The current position of the polygon point.
	 */
	mutable WFMath::Point<3> mPosition;
	
	/**
	 * @brief Stores a newly created point.
	 */
	PolygonPoint* mNewPoint;
	
	/**
	 * @brief Gets the active point, which is either the existing point or a new one if any such exists.
	 * @see mNewPoint
	 * @return Either the existing point, or a new point if any such exists.
	 */
	PolygonPoint* getActivePoint() const;
	
	/**
	 * @brief The initial position of the point, when the movement started.
	 */
	WFMath::Point<2> mInitialPosition;
	
	void input_KeyPressed(const SDL_keysym& key, Ember::Input::InputMode mode);
	void input_KeyReleased(const SDL_keysym& key, Ember::Input::InputMode mode);
	
	/**
	 * @brief Switch to new point mode.
	 * This involves creating a new point, positioning that at the current position of the moved point, and reverting the existing point back to the original position.
	 * If new point mode already is selected nothing will happen.
	 */
	void switchToNewPointMode();
	
	/**
	 * @brief Switches to existing point mode. This is the default.
	 * This involves removing the new point and placing the existing point on the mouse position.
	 * If existing point mode already is selected nothing will happen.
	 */
	void switchToExistingPointMode();
	
	
};

}

}

#endif
