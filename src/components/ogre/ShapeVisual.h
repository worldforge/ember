//
// C++ Interface: PolygonRenderer
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
#ifndef EMBER_OGREVIEW_POLYGONVISUAL_H
#define EMBER_OGREVIEW_POLYGONVISUAL_H

#include "Convert.h"

#include <OgreManualObject.h>

namespace Ogre
{
class SceneNode;
}

namespace WFMath
{

template<int>
class Segment;
}

namespace Ember
{
namespace OgreView
{

/**
 @brief Visualizes a WFMath Shape.

 The shape is rendered using a simple line.
 @author Erik Ogenvik <erik@ogenvik.org>
 */
class ShapeVisual
{
public:

	/**
	 * @brief Ctor.
	 * @param polygon The polygon which should be rendered.
	 */
	ShapeVisual(Ogre::SceneNode& baseNode, bool closed);

	/**
	 * @brief Dtor.
	 */
	~ShapeVisual();

	/**
	 * @brief Updates the graphical representation of the shape.
	 * @param shape The shape which is used to build the graphical representation.
	 */
	template<typename T>
	void update(const T& shape);

protected:

	/**
	 * @brief The base node to which we'll attach the Ogre shape.
	 */
	Ogre::SceneNode& mBaseNode;

	/**
	 * @brief True if the shape should be "closed", i.e. if the lines should wrap around if there are more than two points.
	 */
	bool mClosed;

	/**
	 * @brief The Ogre ManualObject instance used for generating the graphical representation.
	 */
	Ogre::ManualObject* mManualObject;

};

template<typename T>
inline void ShapeVisual::update(const T& shape)
{
	mManualObject->clear();

	if (shape.isValid() && shape.numCorners() > 1) {
		mManualObject->begin("/global/authoring/polygon/line", Ogre::RenderOperation::OT_LINE_STRIP);

		for (int i = 0; i < shape.numCorners(); ++i) {
			mManualObject->position(Convert::toOgre(shape.getCorner(i)));
		}

		if (mClosed) {
			//if there are more than two points we should close the shape
			if (shape.numCorners() > 2) {
				mManualObject->position(Convert::toOgre(shape.getCorner(0)));
			}
		}
		mManualObject->end();
	}

}

}

}

#endif
