//
// C++ Interface: PolygonPointPickListener
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINTPICKLISTENER_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINTPICKLISTENER_H

#include "../IWorldPickListener.h"

namespace EmberOgre {

namespace Manipulation {

class PolygonPointUserObject;
class Polygon;

/**
@brief A listener which knows how to handle PolygonPoints.
The Polygon class allows for easy editing of polygon through clicking and dragging of the polygon points.
This listener class will take care of recognizing mouse clicking and initializing movements of the point.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class PolygonPointPickListener : public IWorldPickListener
{
public:
	/**
	 * @brief Ctor.
	 * @param polygon The polygon to which this listener belongs to.
	 */
	PolygonPointPickListener(Polygon& polygon);
	/**
	 * @brief Dtor.
	 */
	virtual ~PolygonPointPickListener();
	
	virtual void processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs);
	
	virtual void initializePickingContext();

	virtual void endPickingContext(const MousePickerArgs& mousePickerArgs);


private:
	/**
	 * @brief The polygon to which this listener belongs to.
	 */
	Polygon& mPolygon;
	
	/**
	 * @brief The picked user object, if any has been picked.
	 */
	PolygonPointUserObject* mPickedUserObject;
};

}

}

#endif
