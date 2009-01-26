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

#include <wfmath/point.h>
#include <OgreUserDefinedObject.h>
#include <sigc++/signal.h>

namespace Ogre {
class SceneNode;
}

namespace EmberOgre {

namespace Manipulation {

class IPolygonPositionProvider
{
public:

virtual float getHeightForPosition(const WFMath::Point<2>& localPosition) = 0;

};

class PolygonPointUserObject;
class Polygon;
class PolygonPoint;

class PolygonPointUserObject : public Ogre::UserDefinedObject
{
public: 
	/**
	The type of UserDefinedObject
	*/
	static const std::string s_TypeName;
	
	
	PolygonPointUserObject(PolygonPoint& point);
	
	virtual ~PolygonPointUserObject();
	
	/**
	 *    Overloaded method for getting the type name of this instance.
	 * @param  
	 * @return 
	 */
	virtual const Ogre::String & getTypeName (void) const;
	
	
	PolygonPoint& getPoint();
	

	
// 	void translate(Ogre::Real verticalMovement);
	
	/**
	Emitted when the position of the base point has been updated
	*/
	sigc::signal<void> EventUpdatedPosition;
	
	/**
	 *    Marks the entity as "moved"
	 */
	void markAsMoved();

	/**
	 *    Resets the marking of the entity to the normal material (instead of the "moved" marking)
	 */
	void resetMarking();
	
	
private:
	PolygonPoint& mPoint;

};


class PolygonPoint
{
public:

	PolygonPoint(Polygon& polygon, const WFMath::Point<2>& localPosition);
	
	~PolygonPoint();
	
	Polygon& getPolygon();
	
	Ogre::SceneNode* getNode();
	
	void translate(const WFMath::Vector<2>& translation);
	
	WFMath::Point<2> getLocalPosition();
	
	WFMath::Point<2> getDerivedPosition();
	
protected:
	static unsigned int sPointCounter;
	
	Polygon& mPolygon;
	PolygonPointUserObject mUserObject;
	Ogre::SceneNode* mNode;
};




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
    
protected:



	void updateNodes();

	Ogre::SceneNode* mBaseNode;

	PointStore mPoints;
	
	IPolygonPositionProvider* mPositionProvider;
	
};

}

}

#endif
