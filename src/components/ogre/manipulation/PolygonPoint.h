//
// C++ Interface: PolygonPoint
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINT_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINT_H

#include <wfmath/point.h>
#include "PolygonPointUserObject.h"

namespace Ogre {
class SceneNode;
}

namespace EmberOgre {

namespace Manipulation {

class Polygon;
class PolygonPointMover;
/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class PolygonPoint
{
public:

	PolygonPoint(Polygon& polygon, const WFMath::Point<2>& localPosition);
	
	~PolygonPoint();
	
	Polygon& getPolygon();
	
	Ogre::SceneNode* getNode();
	
	Ogre::SceneNode* getNode() const;
	
	void translate(const WFMath::Vector<2>& translation);
	
	WFMath::Point<2> getLocalPosition();
	
	WFMath::Point<2> getDerivedPosition();
	
	void startMovement();
	
	void endMovement();
	
protected:
	static unsigned int sPointCounter;
	
	Polygon& mPolygon;
	PolygonPointUserObject mUserObject;
	Ogre::SceneNode* mNode;
	PolygonPointMover* mMover;
};

}

}

#endif
