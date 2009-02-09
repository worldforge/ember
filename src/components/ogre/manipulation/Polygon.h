//
// C++ Interface: Polygon
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGON_H
#define EMBEROGRE_MANIPULATIONPOLYGON_H

#include "PolygonRenderer.h"
#include <wfmath/polygon.h>

namespace Ogre {
class SceneNode;
}

namespace EmberOgre {

namespace Manipulation {



class PolygonPointUserObject;
class Polygon;
class PolygonPoint;
class IPolygonPositionProvider;
class PolygonPointPickListener;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Polygon
{
public:
friend class PolygonPoint;
typedef std::list<PolygonPoint*> PointStore;
	Polygon(Ogre::SceneNode* baseNode, IPolygonPositionProvider* positionProvider);
	
	~Polygon();
	
	WFMath::Polygon<2> getShape() const;
	
	void loadFromShape(const WFMath::Polygon<2>& shape);
	
	void clear();
	
	const PointStore& getPoints() const;
	
	PolygonPoint* appendPoint();
	
	PolygonPoint* insertPoint(size_t index);
	
	bool removePoint(PolygonPoint& point);
	
	Ogre::SceneNode* getBaseNode();
	
	IPolygonPositionProvider* getPositionProvider() const;
	
	void updateRender();

protected:

	void updateNodes();

	Ogre::SceneNode* mBaseNode;

	PointStore mPoints;
	
	IPolygonPositionProvider* mPositionProvider;
	
	PolygonRenderer mRenderer;
	
	std::auto_ptr<PolygonPointPickListener> mListener;
	
};

}

}

#endif
