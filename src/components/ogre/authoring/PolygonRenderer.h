//
// C++ Interface: PolygonRenderer
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_MANIPULATIONPOLYGONRENDERER_H
#define EMBEROGRE_MANIPULATIONPOLYGONRENDERER_H

#include <wfmath/point.h>
#include <list>

namespace Ogre
{
class ManualObject;
class SceneNode;
}

namespace Ember
{
namespace OgreView
{

namespace Authoring
{

class PolygonPoint;

/**
 @brief Renders a polygon which is being edited.
 The polygon is rendered using a simple line.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class PolygonRenderer
{
public:

	/**
	 * @brief Ctor.
	 * @param polygon The polygon which should be rendered.
	 */
	PolygonRenderer(Ogre::SceneNode& sceneNode, const std::list<PolygonPoint*>& points, bool closed);

	/**
	 * @brief Dtor.
	 */
	~PolygonRenderer();

	/**
	 * @brief Updates the graphical representation of the polygon.
	 * Call this whenever the polygon has changed and you want to update the graphics.
	 */
	void update();

protected:

	/**
	 * @brief The scene node to which the movable object is attached. Owned by this instance.
	 */
	Ogre::SceneNode* mNode;

	/**
	 * @brief A list of the points that makes up the polygon.
	 */
	const std::list<PolygonPoint*>& mPoints;

	/**
	 * @brief The Ogre ManualObject instance used for generating the mesh.
	 */
	Ogre::ManualObject* mManualObject;

	/**
	 * @brief True if the polygon is closed; i.e. the last point is connected to the first.
	 */
	bool mClosed;

	/**
	 * @brief A counter used for auto generating Ogre Resource names.
	 */
	static unsigned int sCounter;
};

}

}

}

#endif
