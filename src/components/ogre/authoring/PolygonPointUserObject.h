//
// C++ Interface: PolygonPointUserObject
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINTUSEROBJECT_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINTUSEROBJECT_H

#include <sigc++/signal.h>

namespace EmberOgre
{

namespace Authoring
{

class PolygonPoint;

/**
 @brief An Ogre user object, connected to a PolygonPoint instance, which can be attached to any Ogre Entity.
 This allows the system to know when the user has clicked on a polygon point and then initialize a movement operation.
 Note that most logic resides in PolygonPointPickListener and PolygonPointMover.
 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 */
class PolygonPointUserObject
{
public:

	/**
	 * @brief Ctor.
	 * @param point The polygon point to which this object belongs.
	 */
	PolygonPointUserObject(PolygonPoint& point);

	/**
	 * @brief Dtor.
	 */
	virtual ~PolygonPointUserObject();

	/**
	 * @brief Gets the polygon point to which this instance is attached.
	 * @return The polygon point to which this instance is attached.
	 */
	PolygonPoint& getPoint();

	/**
	 * @brief Emitted when the position of the base point has been updated
	 */
	sigc::signal<void> EventUpdatedPosition;

	/**
	 * @brief Marks the entity as "moved"
	 */
	void markAsMoved();

	/**
	 * @brief Resets the marking of the entity to the normal material (instead of the "moved" marking)
	 */
	void resetMarking();

private:
	/**
	 * @brief The point to which this instance belongs.
	 */
	PolygonPoint& mPoint;

};

}

}

#endif
