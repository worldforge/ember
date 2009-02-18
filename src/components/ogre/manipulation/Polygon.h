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
@brief A graphical, and editable, representation of a polygon.
A Polygon is a 2d shape, made up of multiple connected points.
This class can be used for represent it graphically in the world, and allow for easy editing through dragging and dropping.
To alter a point, just drag and drop it. To create a new point, drag an existing one and press and hold ctrl.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class Polygon
{
public:
friend class PolygonPoint;
typedef std::list<PolygonPoint*> PointStore;
	/**
	 * @brief Ctor.
	 * @param baseNode The base node to which the polygon will be attached. This node will get new child nodes, which will be removed when the Polygon instance is destroyed.
	 * @param positionProvider An optional position provider instance which will provide a height. Polygons are 2d, but if they are placed on something like a terrain they will need to get correct height data from somewhere.
	 */
	Polygon(Ogre::SceneNode* baseNode, IPolygonPositionProvider* positionProvider);
	
	/**
	 * @brief Dtor.
	 * All created Ogre resources will be detached and destroyed.
	 */
	~Polygon();
	
	/**
	 * @brief Gets a shape which represents the polygon as it is now.
	 * The shape returned will be created from the actual current polygon, so if it has been altered by the user the returned shape will be the altered version, not the original version.
	 * @return A shape representing the current state of the polygon.
	 */
	WFMath::Polygon<2> getShape() const;
	
	/**
	 * @brief Loads a new shape into the polygon, creating PolygonPoint instances.
	 * @param shape The shape to create a graphical representation from.
	 */
	void loadFromShape(const WFMath::Polygon<2>& shape);
	
	/**
	 * @brief Removes all points from the polygon.
	 */
	void clear();
	
	/**
	 * @brief Gets the points that make up the polygon.
	 * @return A store of the points that make up the polygon.
	 */
	const PointStore& getPoints() const;
	
	/**
	 * @brief Appends a new point to the end of the point list.
	 * @return The new appended point.
	 */
	PolygonPoint* appendPoint();
	
	/**
	 * @brief Inserts a new point at the specified index.
	 * @param index The index where the new point will be inserted.
	 * @return The newly inserted point.
	 */
	PolygonPoint* insertPoint(size_t index);
	
	/**
	 * @brief Inserts an existing point at the specified index.
	 * @param index The index where the new point will be inserted.
	 * @param point The point to insert.
	 */
	void insertPoint(size_t index, PolygonPoint& point);
	
	/**
	 * @brief Inserts a point before the specified point.
	 * @param point The point before which a point should be inserted.
	 * @return The newly inserted point, or null if the supplied point doesn't belong to the polygon.
	 */
	PolygonPoint* insertPointBefore(PolygonPoint& point);
	
	/**
	 * @brief Gets the point which is positioned before the supplied point.
	 * If the point supplied is the first point in the list, the last point will be returned.
	 * @param point The point which will be used as a reference.
	 * @return A new point, or null.
	 */
	PolygonPoint* getPointBefore(PolygonPoint& point);
	
	/**
	 * @brief Gets the point which is positioned after the supplied point.
	 * If the point supplied is the last point in the list, the first point will be returned.
	 * @param point The point which will be used as a reference.
	 * @return A new point, or null.
	 */
	PolygonPoint* getPointAfter(PolygonPoint& point);
	
	/**
	 * @brief Removes a point.
	 * @param point The point to remove.
	 * @return True if successfull, false if there was a problem or if the point doesn't belong to this polygon.
	 */
	bool removePoint(PolygonPoint& point);
	
	/**
	 * @brief Gets the base scene node, to which the polygon is attached.
	 * The base scene node isn't owned by the polygon, and shouldn't be altered.
	 * @return The base scene node.
	 */
	Ogre::SceneNode* getBaseNode();
	
	/**
	 * @brief Gets an optional position provider instance.
	 * The position provider is used to provide a height for the polygon (which otherwise is 2d).
	 * @return The optional position provider instance.
	 */
	IPolygonPositionProvider* getPositionProvider() const;
	
	/**
	 * @brief Updates the rendering of the polygon.
	 * Call this whenever the polygon has changed. This will rebuild the mesh that provides the points the binds the polygon points together.
	 */
	void updateRender();

protected:

	/**
	 * @brief The base node, to which the scene nodes that make up the points, as well as the mesh that displays the lines, are attached.
	 * This isn't owned by the polygon and should never be altered.
	 */
	Ogre::SceneNode* mBaseNode;

	/**
	 * @brief The collection of polygon point instances which represents the polygon.
	 */
	PointStore mPoints;
	
	/**
	 * @brief An optional position provider, used to provide height for the otherwise 2d polygon.
	 */
	IPolygonPositionProvider* mPositionProvider;
	
	/**
	 * @brief Responsible for rendering the mesh that binds the polygon points together.
	 */
	PolygonRenderer mRenderer;
	
	/**
	 * @brief Responsible for listening for pick events and initiating movements of the points.
	 */
	std::auto_ptr<PolygonPointPickListener> mListener;
	
};

}

}

#endif
